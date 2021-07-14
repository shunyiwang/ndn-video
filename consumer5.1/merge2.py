#!/usr/bin/env python
# coding=utf-8


import os
import struct

def step1(output,initSegment,layer=[]):
    print "***OutputStream = ",output
    print "***InitSegment =",initSegment
    print "***Number of  layers = ",len(layer)
    for i in range(len(layer)):
        print "***LayerGroup" + str(i+1) + ": [" + str(layer[i]) + " ]"


sep = struct.pack("BBBB", 0, 0, 0, 1)
 
def countNalus(inFileName, type=20):
    stream = []
    cnt = 0
    with open(".//download//"+inFileName, 'rb') as fpIn:
        stream = fpIn.read().split(sep)[1:]
    for i in range(len(stream)):
        n = stream[i]
        hdr = struct.unpack_from("BBBB", n)
        naluType = hdr[0] & 0x1f
        if naluType == type:
            cnt = cnt + 1
    return cnt

def mux(Out, initSegment,files):
    temporalScalability = False
    step1(Out,initSegment,files)
    #initSegment
    initSegmentContent = None

    if initSegment is None or initSegment == "NULL":
        initSegment = None
    else:
        #print "InitSegment = ", initSegment
        # read initsegment
        fpInit = open(".//download//"+initSegment, 'rb')
        initSegmentContent = fpInit.read()
        fpInit.close()

    layerList = {}

    sepNaluType = 6  # type 6 = new AU


    for j in range(len(files)):
        curFile = files[j]

        naluCount = 0
        if j == 0:
            #print "Processing " + curFile + " as BaseLayer with TID=" + str(j)
            naluCount = countNalus(curFile, 6) # count Nalu Type 6 = new Access Unit
            if naluCount == 0:
                #print curFile, "does not have AU delimiters. Trying NaluType 14 instead."
                naluCount = countNalus(curFile, 14)  # count Access Units in Base Layer (NaluType 14)
                sepNaluType = 14
        else:
            #print "Processing " + curFile + " as Enhancement Layer " + str(j) + ",  with TID=" + str(j)
            naluCount = countNalus(curFile, 20)  # count NaluType 20 (= new SVC frame data)
        #print naluCount

        layerList[j] = {'Filename': curFile, 'naluCount': naluCount}


    # open output file
    fpOut = open(".//merge//"+Out, 'wb')


    # write initSegmentContent
    if initSegmentContent is not None:

        fpOut.write(initSegmentContent)
        #print "Writing initsegment..."


    #print files


    
    #numTemporalLayers = len(layerList)
    naluStreams = {}
    positions = {}
    nLayers = len(layerList)

    #print "Step 1: Reading all input files into memory..."
    for j in range(nLayers):
        # init naluStreams and positions
        #naluStreams = {}
        #positions = {}

        fileList = layerList[j]
    #nLayers = len(fileList)
    #for i in range(nLayers):
        with open(".//download//"+fileList['Filename'], 'rb') as fpIn:
            #print "Parsing ", fileList['Filename']
            naluStreams[j] = fpIn.read().split(sep)[1:]
            positions[j] = 0
    #print "------/////////------"
    active = True

    # sum up base layer AU count
    #baseLayerAUCount = {}
    #for j in range(numTemporalGroups):
    #    baseLayerAUCount[j] = layerList[j][0]['naluCount']
    baseLayerAUCount = layerList[0]['naluCount']

    frm = 0
    nal_ref_idc = 0
    cur_temp_layer = 0



    while active:
        active = False

        # decode the frame for all layers
        if temporalScalability:
            #print "Current temporal layer: ", cur_temp_layer
        # end if
        for i in range(nLayers):
            fileList = layerList
            # calculate nalu per AU (relevant for enhancement layers)
            naluPerAU = fileList[i]['naluCount'] / baseLayerAUCount
            #print "naluPerAU=", naluPerAU
            eos, found = False, False
            
            first = True

            probeForTemporalUpswitch = False
            
            if i == 0: # base layer
                # read until we find NALU type 14 (= new AU); if 14 found --> possible upswitch coming
                #print "Reading ", fileList[i]['Filename'] , " in Base Layer Mode (len=", len(naluStreams[i]), "): Frame " + str(frm)
                cnt = 0
                while (not eos) and (not found):
                    pos = positions[i]
                    
                    if pos >= len(naluStreams[i]):
                        eos = True
                        #if cur_temp_layer == 0 and numTemporalGroups > 1:
                        #    probeForTemporalUpswitch = True
                        # end if
                    else:
                        n = naluStreams[i][pos]
                        if (len(n) > 0): # NALU long enough
                            nal_ref_idc = struct.unpack_from("B", n)[0] >> 5 # get the leftmost 3 bit
                            naluType = struct.unpack_from("B", n)[0] & 0x1f # get the rightmost 5 bit
                            
                            #print "pos=", pos, ", naluType=", naluType, ", nal_ref_idc=", nal_ref_idc, ", len=", len(n)

                            if naluType == sepNaluType:  # make sure to skip first nalutype 14
                                if not first and nLayers > 1:
                                    found = True
                                    active = True
                            # end if
                            first = False

                        if naluType != 14:
                            cnt += 1

                        if not found:  # write as long as we did not find another nalu type 14
                            fpOut.write(sep+n)
                            positions[i] +=  1
                            if nLayers == 1 and naluType == 1 and (nal_ref_idc == 2 or (nal_ref_idc == 0 and cnt >= naluPerAU)):
                                #print "Info: Possible temporal layer upswitch coming. Probing..."
                                active = True
                                found = True
                                probeForTemporalUpswitch = True
                        else:
                            #print "Info: Possible layer upswitch coming, skipping last NALU (type 14) for now, and continuing with EL 1"
                        # end if not found
                    # end if Check for EOS
                # end while not eos and not found
                frm += 1
                            
            else: # EL
                # read naluPerAU nalus
                #print "Reading ", fileList[i]['Filename'] , " in Enhancement Layer Mode (naluPerAU=", naluPerAU, ",len=", len(naluStreams[i]), ")"
                cnt = 0
                while (not eos) and cnt < naluPerAU: # copy NALUs until we find type 1, 5, or 20.
                    pos = positions[i]
                    #print "cnt=", cnt, ", pos=", pos
                    
                    if pos >= len(naluStreams[i]):
                        #print "end of stream cur_temp_layer=", cur_temp_layer
                        eos = True
                        active = False
                    else:
                        n = naluStreams[i][pos]
                        cnt += 1
                        
                        nal_ref_idc = struct.unpack_from("B", n)[0] >> 5 # get the leftmost 3 bit
                        naluType = struct.unpack_from("B", n)[0] & 0x1f # get the rightmost 5 bit
                            
                        #print "cnt=", cnt,", pos=", pos, ", naluType=", naluType, ", nal_ref_idc=", nal_ref_idc, ", len=", len(n)
                        
                        fpOut.write(sep+n)
                        positions[i]+= 1
                        active = True
                    # end if
                # end while
            # end if EL
            # now check for temporal scalability:
            # if last layer AND naluType == 20 and nal_ref_idc == 2 --> next temporal layer
            #print "Temporal=", temporalScalability, ", i=", i, ", nLayers-1=", nLayers-1
#            if probeForTemporalUpswitch or (temporalScalability and i == nLayers-1 and ((naluType == 20 and nal_ref_idc == 2) or (nLayers == 1 and naluType == 1 and nal_ref_idc == 2))) :
                # go to next temporal layer
#                print "Info: Temporal Upswitch in Progress..."
#                probeForTemporalUpswitch = False

#                cur_temp_layer += 1
#                cur_temp_layer = cur_temp_layer % numTemporalLayers

#                if cur_temp_layer != 0:
 #                   eos = False
#                    active = True

#                print "New temporal layer=", cur_temp_layer, ", active=", active
            # end if
        # end for (decode current frame)
        # now, start loop at beginning again

    # end while active
# end function mux
    
    fpOut.close()

if __name__ == '__main__':
    a = "test.264"
    b = "bus_svc.init.svc"
    c = ["bus_svc.seg0-L0.svc"]
    mux(a,b,c)
