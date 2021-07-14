#include<iostream>
#include <Python.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <vector>
#include <time.h>

using namespace std;


namespace svcdash{
	class Combine{
		public:
			int combine(string outFile,string initFile,vector<string>& segFile);
			string merge(string initFile,vector<string>& segFile);
	};
}

//取出segFile中的内容，把SVC中不同层结合在一起（baseLayer && enhanceLayer)
