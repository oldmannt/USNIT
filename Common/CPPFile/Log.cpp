//
//  Log.cpp
//  USNIT
//
//  Created by dyno on 4/14/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#include <stdio.h>
#include "Interact.h"
#include "Log.h"

#include "zf_log.h"

#define ZF_LOG_USE_NSLOG

class ConsoleLogger:public ILogger {
public:
    virtual bool init(const char* param){
        return true;
    }
    virtual int getTarget() const { return LOG_CONSOLE;}
    virtual void logout(const char* log){
        InteractLog(log);
    }
};

class FileLogger:public ILogger {
public:
    FileLogger(){}
    virtual ~FileLogger(){}
    virtual bool init(const char* param){
        return true;
    }
    virtual int getTarget() const {return LOG_FILE;}
    virtual void logout(const char* log){
        
    }
};

class CLogMgr:public ILogMgr {
public:
    CLogMgr();
    virtual ~CLogMgr();
    virtual bool addLoger(int target, ILogger* logger);
    virtual void logout(int targets, int lev, const char* fmt, ...);
};