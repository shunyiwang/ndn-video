#include "parse.h"

void Parse::GetSVCSegmentURLs(const char* url, const char* layer_id,mpdManager& mpdmanager){
  cerr << "Opening " << url << endl;
  // do nothing
  dash::mpd::IMPD *mpd;
  dash::IDASHManager *manager;
  manager = CreateDashManager();
  mpd = manager->Open ((char*)url);

  std::vector<std::string> segmentList;
  std::vector<mpdConception> conceptionList;


  if (mpd == NULL){
      cerr << "Error opening mpd..." << endl;
      //return segmentList;
  }

  cerr << "Successfully opened MPD!" << endl;

  std::vector<std::string> locs = mpd->GetLocations();

  cerr << "Received " << locs.size() << " locations: " << endl;
  mpdmanager.setLocationSize(locs.size());

  for (int i = 0; i < locs.size (); i++){
      cerr << locs[i] << endl;
  }


  cerr << mpd->GetMPDPathBaseUrl ()->GetUrl () << endl;

  std::vector<dash::mpd::IPeriod *> periods = mpd->GetPeriods ();

  if(periods.size() == 0)
    {
      cerr << "No periods found in MPD..." << endl;
      //return segmentList;
    }

  cerr << "Found " << periods.size() << " periods in MPD" << endl;
  mpdmanager.setPeriodSize(periods.size());


  dash::mpd::IPeriod* per = periods.at(0);
  std::vector<dash::mpd::IBaseUrl*> baseUrls = per->GetBaseURLs();



  std::string base_url = "";

  if (baseUrls.size() > 0)
  {
    cerr << "Received " << baseUrls.size() << " base URLs: " << endl;

    for (int i = 0; i < baseUrls.size (); i++)
    {
      cerr << baseUrls[i] << endl;
    }
    } else {

      base_url = mpd->GetBaseUrls ().at(0)->GetUrl();
    }
    cerr << "BASE URL:" << base_url << endl;
    mpdmanager.setBaseUrl(base_url);


    // get adaptation sets
    std::vector<dash::mpd::IAdaptationSet*> sets = per->GetAdaptationSets ();
    dash::mpd::IAdaptationSet* set = sets.at (0); //Todo deal with different sets

    cerr << "Sets.size() = " << sets.size() << endl; 
    mpdmanager.setAdaptationsetSize(sets.size());

    //std::string initSegment = base_url + set->GetSegmentBase ()->GetInitialization ()->GetSourceURL ();
    std::string initSegment = set->GetSegmentBase ()->GetInitialization ()->GetSourceURL ();


    cerr << "Download Init Segment:" << initSegment << endl;
    mpdmanager.setInitSegment(initSegment);

    std::vector<dash::mpd::IRepresentation*> reps = set->GetRepresentation ();

    int width, height;
    dash::mpd::IRepresentation* rep;



  //segmentList.push_back (initSegment);

  // iterate over all representations
  for(size_t j = 0; j < reps.size(); j++)
  {
    mpdConception conception;
    cerr << "--------------------" << endl;
    rep = reps.at(j);
    
    width = rep->GetWidth();
    conception.width=width;
    height = rep->GetHeight();
    conception.height=height;

    std::vector<dash::mpd::ISegmentURL*> segmentUrls = rep->GetSegmentList ()->GetSegmentURLs();
    cerr << "Representation ID: " << rep->GetId () << endl;
    conception.id=rep->GetId();
    std::vector<std::string> dependencies = rep->GetDependencyId ();

    if (dependencies.size () > 0)
    {
      cerr << "Dependency ID: " << dependencies.at(0) << endl;
    }

    std::vector<std::string> codecs = rep->GetCodecs ();

    if (codecs.size() > 0)
    {
      cerr << "Codec: " << codecs.at(0) << endl;
      conception.codec=codecs.at(0);
    }


    cerr << "WxH: " << width << "x" << height << ", BW:" << rep->GetBandwidth () << endl;
    conception.bandwidth=rep->GetBandwidth();
    cerr << "Segment Duration (Frames): " << rep->GetSegmentList()->GetDuration() << endl;
    conception.segmentDuration=rep->GetSegmentList()->GetDuration();

    cerr << "FramesPerSecond: " << rep->GetFrameRate () << endl;
    conception.frameRate=rep->GetFrameRate();

    cerr << "Number of segments: " << segmentUrls.size () << endl;
    conception.totalSegments=segmentUrls.size();
      
    if (layer_id == NULL || (layer_id != NULL && rep->GetId().compare(layer_id) == 0))
    {
	for (int k = 0; k < segmentUrls.size(); k++)
	{

	    // download
	    //string newURL = base_url;
        string newURL="";
	    newURL += segmentUrls.at(k)->GetMediaURI();

	    if (k >= segmentList.size())
	    {
	      segmentList.push_back (newURL);
	    }
	    else
	     {
	      segmentList[k] += "," + newURL;
	      }

	   // cerr << "Download " << newURL << endl;
	}
    } else {
   		cerr << "Skipping this layer..." << endl;
    }
    
    conceptionList.push_back(conception);

  }

    cerr << "--------------------" << endl;
    mpdmanager.setSegments(segmentList);
    mpdmanager.setMpdConceptions(conceptionList);

  //for(int k=0;k<segmentList.size();k++){
  //        cerr<<segmentList[k]<<endl;
  //}
  //return segmentList;
}


void Parse::ParseMPD(string url,mpdManager& mpdmanager){
	string urlPath="./download/"+url;
	GetSVCSegmentURLs(urlPath.c_str(),NULL,mpdmanager);
}

/*
  int main(){
    const char* url="TOS-I-360p.mpd";
    mpdManager test;
    GetSVCSegmentURLs(url,NULL,test);
    cerr<<test.getLocationSize()<<endl;
    cerr<<test.getPeriodSize()<<endl;
    cerr<<test.getAdaptationsetSize()<<endl;
    cerr<<test.getBaseUrl()<<endl;
    cerr<<test.getInitSegment()<<endl;
    vector<string> segment;
    test.getSegment(13,segment);
    for(int i=0;i<segment.size();i++){
        cerr<<segment[i]<<endl;
    }
    mpdConception mc=test.getMpdConception(0);
    cerr<<mc.id<<"-"<<mc.bandwidth<<"-"<<mc.frameRate<<endl;

    return 0;
  }*/

