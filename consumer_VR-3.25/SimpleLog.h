/*************************************************************************
	> File Name: SimpleLog.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月10日 星期五 15时55分38秒
 ************************************************************************/

#ifndef _SIMPLELOG_H
#define _SIMPLELOG_H

#include <ctime>
#include <sys/time.h>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


using namespace std;

//string LOG_PATH="./log/log.txt";

namespace svcdash{
	class SimpleLog{
		public:
			int Log(int a,int b);
			int Log(int a,double b);
			int Log(int a,double b,int c,int d);
			int Log_1(int a,double b);
		private:
			string get_date();
			string get_time();
			int to_string(stringstream& ss, int &a , int &b);
			int to_string(stringstream& ss, int &a , double &b);
			int to_string(stringstream& ss, int &a , double &b,int &c,int &d);
			string logPathForPlay="./log/logForPlay";
			string logPathForDownload="./log/logForDownload";
			string logPathForBuffer="./log/logForBuffer";
			string logPathForDownState="./log/logForDownState";
	};
}


#endif
