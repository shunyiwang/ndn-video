#include <iostream>
#include <libdash.h>
#include <string>
#include <mpdManager.h>

using namespace svcdash;
using namespace std;
using namespace dash;
using namespace dash::network;

namespace svcdash{
	class Parse{
		public:
			void GetSVCSegmentURLs(const char* url, const char* layer_id,mpdManager& mpdmanager);
			void ParseMPD(string url,mpdManager& mpdmanager);
	};
}

