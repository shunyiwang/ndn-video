#include "dash-consumer.h"

namespace ndn{
    namespace ndndash{

NdnDashConsumer :: NdnDashConsumer()
  {
    m_nSegNumber = 0;
    pos=0;
    memset(m_SegmentBuffer,0,1024*256);
  }
  
  size_t 
NdnDashConsumer :: getSegmentNumber()
  {
    return m_nSegNumber;
  }

size_t 
NdnDashConsumer :: setSegmentNumber(size_t num)
  {
    m_nSegNumber=num;
  }

  void 
NdnDashConsumer :: getRequestNumber(std::string segmentChunkName)
  {
    std::string segName = "/ustc/ndnDash/requestNumber/";
    segName += segmentChunkName;

    Interest interest(Name(segName.c_str()));
    interest.setInterestLifetime(time::milliseconds(1000));
    interest.setMustBeFresh(true);

    m_face.expressInterest(interest,
                           bind(&NdnDashConsumer::onRequestNumberArrive, this,  _1, _2),
                           bind(&NdnDashConsumer::onRequestNumberTimeout, this, _1));

    // processEvents will block until the requested data received or timeout occurs
    m_face.processEvents();

  } 
  
  void
NdnDashConsumer :: getRequestData(std::string segmentChunkName, size_t segIndex)
  {
    std::string segName = "/ustc/ndnDash/requestData/";
    segName += segmentChunkName;
    std::ostringstream os;
    os << segIndex;
    segName += "/";
    segName += os.str();

    Interest interest(Name(segName.c_str()));
    interest.setInterestLifetime(time::milliseconds(1000));
    interest.setMustBeFresh(true);

    m_face.expressInterest(interest,
                           bind(&NdnDashConsumer::onRequestDataArrive, this,  _1, _2),
                           bind(&NdnDashConsumer::onRequsetDataTimeout, this, _1));

    // processEvents will block until the requested data received or timeout occurs
    m_face.processEvents();
  }

  void
NdnDashConsumer :: onRequestNumberArrive(const Interest& interest, const Data& data)
  {
    memcpy(&m_nSegNumber, data.getContent().value(), data.getContent().value_size());
    //std::cout << data << "segment number: " << m_nSegNumber << std::endl;
  }  
 
  void
NdnDashConsumer :: onRequestDataArrive(const Interest& interest, const Data& data)
  {
    
    //std::cout <<"**********************************"<<std::endl;
    //std::cout<< data << std::endl; 
    //std::cout <<"**********************************"<<std::endl;

    memcpy(m_SegmentBuffer+pos,data.getContent().value(),data.getContent().value_size());
    pos+=data.getContent().value_size();
  }

  void
NdnDashConsumer :: onRequestNumberTimeout(const Interest& interest)
  {
    //std::cout << "Request Number Timeout " << interest << std::endl;
    m_nSegNumber = 0;
  }
 
  void
NdnDashConsumer :: onRequsetDataTimeout(const Interest& interest)
  {
    //std::cout << "Request Data Timeout " << interest << std::endl;
  }


/*将获取的视频块写入相应的文件夹中*/
void NdnDashConsumer :: Buffer(std::string filename){
    std::string path="./download/"+filename;
    std::ofstream outfile(path,std::ios::binary | std::ios::trunc | std::ios::out);
    if (outfile){
        outfile.write((char *)m_SegmentBuffer,pos);
        outfile.close();
        //std::cout<<"<<<<<<<"<<pos<<"bytes have written"<<">>>>>>>"<<std::endl;
        }
    else{
        //std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<std::endl;
        std::cout<<"NdnDashConsumer :: Buffer : cannot open file"<<std::endl;
        //std::cout<<pos<<std::endl;
        //std::cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"<<std::endl;
    }
}

int NdnDashConsumer :: getSegmentSize(){
    return pos;
}
}
}
