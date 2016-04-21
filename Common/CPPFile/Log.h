//
//  Log.h
//  USNIT
//
//  Created by dyno on 4/14/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#ifndef Log_h
#define Log_h

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_CONSOLE 0x01
#define LOG_FILE    0x02
#define LOG_NET     0x04

#define LOG_VERBOSE 1
#define LOG_DEBUG   2
#define LOG_INFO    3
#define LOG_WARN    4
#define LOG_ERROR   5
#define LOG_FATAL   6

void g_loginit(int outputs, const char* param);
void g_logout(int output, int lev, const char *const func, const char *const file, const unsigned line, const char* fmt, ...);
void g_logouts(int output, int lev, const char* msg);
void g_loglev(int lev);

#define G_LOG(output, lev, ...) g_logout(output, lev, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define G_LOG_C(lev, ...) g_logout(LOG_CONSOLE, lev, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)

#define G_LOG_F(lev, ...) g_logout(LOG_FILE, lev, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
#define G_LOG_FC(lev, ...) g_logout(LOG_FILE|LOG_CONSOLE, lev, __FUNCTION__, __FILE__, __LINE__, __VA_ARGS__)
    

#ifdef __cplusplus
}   // end of extern "C"
#endif

#endif /* Log_h */
