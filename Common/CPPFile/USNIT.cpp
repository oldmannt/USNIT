//
//  USNIT.c
//  USNIT
//
//  Created by dyno on 5/4/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#include <stdio.h>
#include "USNIT.h"
#include "UsnitLogic.hpp"
#include "uv.h"

void    async_loop(){
    uv_run(uv_default_loop(), UV_RUN_NOWAIT);
}

int     UsnitInit(const char* conf_str, int lang, my_cb_t cb_func) {
    if (CUsnitLogic::Instance().init(conf_str, lang, cb_func))
        return 0;
    else
        return -1;
}

int     UsnitSetLongType(int type){
    return CUsnitLogic::Instance().setLongType(type)?0:-1;
}

int     UsnitSetMassType(int type){
    return CUsnitLogic::Instance().setMassType(type)?0:-1;
}

int     UsnitSetSquareType(int type){
    return CUsnitLogic::Instance().setSquareType(type)?0:-1;
}

int     UsnitSetVolumeType(int type){
    return CUsnitLogic::Instance().setVolumeType(type)?0:-1;
}

void     UsnitSetType(int type) {
    CUsnitLogic::Instance().setType(type);
}

int     UsnitSetInput(float value) {
    if (CUsnitLogic::Instance().setInput(value))
        return 0;
    else
        return -1;
}

const char*   UsnitGetResult(int type) {
    return CUsnitLogic::Instance().getResult(type);
}

const char* UsnitGetUnitName(int type){
    return CUsnitLogic::Instance().getUnitName(type);
}
