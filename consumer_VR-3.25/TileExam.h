/*************************************************************************
	> File Name: downparsecombine.h
	> Author: 
	> Mail: 
	> Created Time: 2017年03月08日 星期三 10时53分57秒
 ************************************************************************/

#ifndef _DOWNPARSECOMBINE_H
#define _DOWNPARSECOMBINE_H
#endif

#include <thread>
#include <mutex>
#include <queue>
#include <unistd.h>
#include "downloadManager.h"
#include "parse.h"
#include "combine.h"
#include "Segment.h"
#include "decode.h"
#include "SimpleLog.h"
#include "fovScheduler.h"

#define NUM 4

using namespace std;
using namespace svcdash;
