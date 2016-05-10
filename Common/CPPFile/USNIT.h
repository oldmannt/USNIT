//
//  USNIT.h
//  USNIT
//
//  Created by dyno on 4/8/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#ifndef USNIT_h
#define USNIT_h


#ifdef __cplusplus
extern "C" {
#endif
    
#define SIZE_RESULT 128
    
#define TYPE_METER      0
#define TYPE_CMETER     1
#define TYPE_KMETER     2
#define TYPE_FEET       3
#define TYPE_INCH       4
#define TYPE_MILE       5
#define TYPE_YARD       6
#define TYPE_LITRE      7
#define TYPE_MLITRE     8
#define TYPE_GAL        9
#define TYPE_GRAM       10
#define TYPE_KGRAM      11
#define TYPE_POUND      12
#define TYPE_OZ         13
#define TYPE_SQM        14
#define TYPE_SQCM       15
#define TYPE_SQF        16
#define TYPE_CELSI      17
#define TYPE_FAHRE      18
#define TYPE_SQINCH     19
#define TYPE_DOLLAR     20
#define TYPE_RMB        21
#define TYPE_RATE       22
#define TYPE_MAX        23

#define TYPE_MAX_UNIT   TYPE_RMB
    
#define CB_DOLLAR_RT    1
#define CB_RMB_RT       2
#define CB_RATEINFO     3
    
#define LANG_CH     1
#define LANG_ENG    2
    
    typedef int (*my_cb_t)(int type, const void* data);

    int     UsnitInit(const char* conf_str, int lang, my_cb_t cb_func);
/*
    int     UsnitSetLongType(int metric_type, int us_type);
    int     UsnitSetMassType(int metric_type, int us_type);
    int     UsnitSetSquareType(int metric_type, int us_type);
    int     UsnitSetVolumeType(int metric_type, int us_type);
*/
//  int     UsnitSetLongType(int type);
//  int     UsnitSetMassType(int type);
//  int     UsnitSetSquareType(int type);
//  int     UsnitSetVolumeType(int type);
    
    void     UsnitSetType(int type);

    int     UsnitSetInput(float value);
    const char* UsnitGetResult(int type);
    const char* UsnitGetUnitName(int type);
    
    void    async_loop();
    
    void    C_LOG(int output, int lev, const char* fmt);
    
#ifdef __cplusplus
}   // end of extern "C"
#endif


#endif /* USNIT_h */
