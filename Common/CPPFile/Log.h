//
//  Log.h
//  USNIT
//
//  Created by dyno on 4/14/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#ifndef Log_h
#define Log_h

#define LOG_CONSOLE 0x01
#define LOG_FILE    0x02
#define LOG_NET     0x04

class ILogger {
    virtual bool init(const char* param) = 0;
    virtual int getTarget() const = 0;
    virtual void logout(const char* log) = 0;
};

class ILogMgr {
    virtual bool addLoger(int target, ILogger* logger) = 0;
    virtual void logout(int targets, int lev, const char* fmt, ...)=0;
};

#endif /* Log_h */
