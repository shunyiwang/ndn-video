#include <ndn-cxx/face.hpp>
#include <ndn-cxx/name.hpp>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>
#include <fstream>
#include <string.h>
#include <ndn-cxx/security/key-chain.hpp>


#define MAX_SEG_SIZE                    (4*1024*1024)
#define PACKET_SEG_SIZE                 (4*1024)

//#include "parse.h"
/*dash-consumer solve the transition by nfd */

namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
namespace ndndash {

class NdnDashConsumer : noncopyable
{
public:  
  NdnDashConsumer();
  
  size_t getSegmentNumber(); //

  size_t setSegmentNumber(size_t num);
  
  void 
  getRequestNumber(std::string segmentChunkName); //function:get packet number of the segment
  
  void
  getRequestData(std::string segmentChunkName, size_t segIndex); //function:get segment 

  //save data
  void Buffer(std::string filename);
  
  int getSegmentSize();
private:
   
  void
  onRequestNumberArrive(const Interest& interest, const Data& data); // function:do what on requested number arriving
 
  void
  onRequestDataArrive(const Interest& interest, const Data& data); // function:do what on requested data arriving

  void
  onRequestNumberTimeout(const Interest& interest); //function:timeout(requested number)
 
  void
  onRequsetDataTimeout(const Interest& interest); //function:timeout(requested data)



private:
  size_t m_nSegNumber;
  Face m_face;
  uint8_t m_SegmentBuffer[MAX_SEG_SIZE];
  uint32_t pos;
};

} // namespace examples
} // namespace ndn

