/*************************************************************************
	> File Name: SimpleLog.cpp
	> Author: 
	> Mail: 
	> Created Time: 2017年03月10日 星期五 15时55分47秒
 ************************************************************************/

#include "SimpleLog.h"

using namespace svcdash;


string SimpleLog::get_date()
{
    time_t t = time(0);    
    struct tm *now = localtime(&t);
    stringstream ss;
    ss<<now->tm_year+1900<<now->tm_mon+1<<now->tm_mday;
    return ss.str();
}


/*
string SimpleLog::get_time()
{
    time_t t = time(0);
    struct tm* now = localtime(&t);
    stringstream ss;
    //ss<<get_date()<<' ';
    ss<<now->tm_hour<<':'
        <<now->tm_min<<':'
        <<now->tm_sec;
    cout<<ss.str()<<endl;
    return ss.str();
}*/

string SimpleLog::get_time()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    
    
    stringstream ss;
    ss<<tv.tv_sec<<'.'<<tv.tv_usec/1000;
    //cout<<ss.str()<<endl;
    return ss.str();
}

int SimpleLog::to_string(stringstream& ss, int &a , int &b)
{
    ss<<"       ";
    ss<<a;
    ss<<"       ";
    ss<<b;
    return 0;
}

int SimpleLog::to_string(stringstream& ss, int &a , double &b)
{
    ss<<"       ";
    ss<<a;
    ss<<"       ";
    ss<<b;
    return 0;
}

int SimpleLog::to_string(stringstream& ss, int &a , double &b,int &c,int &d)
{
    ss<<"       ";
    ss<<a;
    ss<<"       ";
    ss<<b;
    ss<<"       ";
    ss<<c;
    ss<<"       ";
    ss<<d;
    return 0;
}


int SimpleLog::Log(int a,int b)
{
    stringstream ss;    
    to_string(ss,a,b);
    fstream log_file;
    log_file.open(logPathForPlay+get_date()+".txt",ios::out|ios::app);
    log_file<<"["<<get_time()<<"]"<<ss.str()<<std::endl;
    log_file.close();
    //cerr<<ss.str()<<endl;
    return 0;
}

int SimpleLog::Log(int a,double b)
{
    stringstream ss;    
    to_string(ss,a,b);
    fstream log_file;
    log_file.open(logPathForDownload+get_date()+".txt",ios::out|ios::app);
    log_file<<get_time()<<ss.str()<<std::endl;
    log_file.close();
    //cerr<<ss.str()<<endl;
    return 0;
}

int SimpleLog::Log(int a,double b,int c,int d)
{
    stringstream ss;    
    to_string(ss,a,b,c,d);
    fstream log_file;
    log_file.open(logPathForDownState+get_date()+".txt",ios::out|ios::app);
    log_file<<"["<<get_time()<<"]"<<ss.str()<<std::endl;
    log_file.close();
    //cerr<<ss.str()<<endl;
    return 0;
}

int SimpleLog::Log_1(int a,double b)
{
    stringstream ss;    
    to_string(ss,a,b);
    fstream log_file;
    log_file.open(logPathForBuffer+get_date()+".txt",ios::out|ios::app);
    log_file<<"["<<get_time()<<"]"<<ss.str()<<std::endl;
    log_file.close();
    //cerr<<ss.str()<<endl;
    return 0;
}


/*
int main(){
	SimpleLog sl;
	sl.Log(1,2);
	return 0;
}*/

