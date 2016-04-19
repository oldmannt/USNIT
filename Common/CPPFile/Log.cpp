//
//  Log.cpp
//  USNIT
//
//  Created by dyno on 4/14/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#include <stdio.h>
#include <list>
#include "Interact.h"
#include "Log.h"

#include "zf_log/zf_log.h"

#define ZF_LOG_USE_NSLOG
#define ZF_LOG_LEVEL ZF_LOG_INFO



class ILogger {
public:
    virtual bool init(const char* param) = 0;
    virtual int getOutput() const = 0;
    virtual void zf_log(const zf_log_message* msg, void* arg) = 0;
};

class ILogMgr {
public:
    virtual void logout(int output, int lev, const char* fmt, ...)=0;
    virtual void setLev(int lev) = 0;
    virtual bool addLogger(int output, const char* param) = 0;
};

class ConsoleLogger:public ILogger {
public:
    ConsoleLogger():m_zf_console_output_cb(0){}
    virtual ~ConsoleLogger(){}
    virtual bool init(const char* param){
        m_zf_console_output_cb = _zf_log_global_output.callback;
        return true;
    }
    virtual int getOutput() const { return LOG_CONSOLE;}
    virtual void log(const char* log, int lev){
        
    }
    virtual void zf_log(const zf_log_message* msg, void* arg){
        if (m_zf_console_output_cb){
            (*m_zf_console_output_cb)(msg, arg);
        }
    }
private:
    zf_log_output_cb m_zf_console_output_cb;
    
};

class FileLogger:public ILogger {
public:
    FileLogger():logfile(0){}
    virtual ~FileLogger(){
        fclose(logfile);
    }
    virtual bool init(const char* param){
        logfile = fopen(param, "a");
        if (!logfile) {
            
        }
        return true;
    }
    virtual int getOutput() const {return LOG_FILE;}
    virtual void zf_log(const zf_log_message* msg, void* arg){
        *msg->p = '\n';
        fwrite(msg->buf, msg->p - msg->buf + 1, 1, logfile);
        fflush(logfile);
    }
private:
    FILE* logfile;
};

static void fz_log_callback(const zf_log_message *msg, void* arg);
class CLogMgr:public ILogMgr {
public:
    static CLogMgr& Instance() {
        static CLogMgr instance;
        return instance;
    }
    
    CLogMgr():m_outputs(0){
        this->addLogger(LOG_CONSOLE, "");
        zf_log_set_output_v(ZF_LOG_PUT_STD, &this->m_outputs, fz_log_callback);
    }
    virtual ~CLogMgr(){}
    
    virtual bool addLogger(int outputs, const char* param) {
        
        if (outputs&LOG_CONSOLE){
            ConsoleLogger* console = new ConsoleLogger();
            console->init(param);
            m_lstLogger.push_back(console);
        }
        else if (outputs&LOG_FILE){
            FileLogger* filelog = new FileLogger();
            filelog->init(param);
            m_lstLogger.push_back(filelog);
        }
        else{
            return false;
        }
        
         m_outputs|=outputs;
        return true;
    }
    
    virtual void setLev(int lev){
        zf_log_set_output_level(lev);
    }
    
    virtual void logout(int outputs, int lev, const char* fmt, ...) {
        m_outputs = outputs;
        m_lev = lev;
        char buf[512] = {0};
        va_list va;
        va_start(va, fmt);
        vsnprintf(buf,sizeof(buf),fmt,va);
        va_end(va);
        
        switch (lev) {
            case LOG_VERBOSE:
                ZF_LOGI("%s", buf);
                break;
            case LOG_DEBUG:
                ZF_LOGD("%s", buf);
                break;
            case LOG_INFO:
                ZF_LOGI("%s", buf);
                break;
            case LOG_WARN:
                ZF_LOGD("%s", buf);
                break;
            case LOG_ERROR:
                ZF_LOGE("%s", buf);
                break;
            case LOG_FATAL:
                ZF_LOGF("%s", buf);
                break;
                
            default:
                break;
        }
    }
    
    void zf_log(const zf_log_message *msg, void* arg){
        LST_LOGGER::iterator it(m_lstLogger.begin());
        for (; it!=m_lstLogger.end(); ++it) {
            ILogger* logger = *it;
            if (!logger)
                continue;
            if (logger->getOutput()&m_outputs){
                logger->zf_log(msg,arg);
            }
        }
    }
    
private:
    int m_outputs;
    int m_lev;
    typedef std::list<ILogger*> LST_LOGGER;
    LST_LOGGER m_lstLogger;
};

ILogMgr* gLogMgr(){
    return &CLogMgr::Instance();
}

struct G_LOG_INFO {
    int outputs;
    FILE* file;
    char file_path[512];
    zf_log_output_cb console_cb;
    zf_log_output_cb file_cb;
};

G_LOG_INFO g_log_info;
static void zf_file_log(const zf_log_message* msg, void* arg){
    if (!g_log_info.file){
        g_logout(LOG_CONSOLE, LOG_ERROR, "no file handle for log");
        return;
    }
    *msg->p = '\n';
    fwrite(msg->buf, msg->p - msg->buf + 1, 1, g_log_info.file);
    fflush(g_log_info.file);
}

static void fz_log_callback(const zf_log_message *msg, void* arg){
    // CLogMgr::Instance().zf_log(msg, arg);
    
    if (!msg || !arg)
        return;
    
    
}

void g_addlog(int outputs, const char* param){
    if (outputs&LOG_CONSOLE){
        g_log_info.console_cb = _zf_log_global_output.callback;
    }
    else if(outputs&LOG_FILE){
        
        g_log_info.file = fopen(param, "a");
        if (!g_log_info.file){
            g_logout(LOG_CONSOLE, LOG_ERROR, "fopne file log failed:%s", param);
        }
        g_log_info.file_cb = zf_file_log;
        strncpy(g_log_info.file_path, param, sizeof(g_log_info.file_path));
    }
    
    g_log_info.outputs |= outputs;

}

void g_loginit(int outputs, const char* param){
    memset(&g_log_info, sizeof(g_log_info),0);
    zf_log_set_output_v(ZF_LOG_PUT_STD, &g_log_info, fz_log_callback);
    
}

void g_logouts(int output, int lev, const char* msg) {
    switch (lev) {
        case LOG_VERBOSE:
            ZF_LOGI("%s", msg);
            break;
        case LOG_DEBUG:
            ZF_LOGD("%s", msg);
            break;
        case LOG_INFO:
            ZF_LOGI("%s", msg);
            break;
        case LOG_WARN:
            ZF_LOGD("%s", msg);
            break;
        case LOG_ERROR:
            ZF_LOGE("%s", msg);
            break;
        case LOG_FATAL:
            ZF_LOGF("%s", msg);
            break;
            
        default:
            break;
    }

}

void g_logout(int output, int lev, const char* fmt, ...) {
    
    char buf[512] = {0};
    va_list va;
    va_start(va, fmt);
    vsnprintf(buf,sizeof(buf),fmt,va);
    va_end(va);
    g_logouts(output, lev, buf);
}
void g_loglev(int lev) {
    zf_log_set_output_level(lev);
}

