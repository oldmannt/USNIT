//
//  UsnitLogic.cpp
//  USNIT
//
//  Created by dyno on 4/8/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#include "USNIT.h"
#include "UsnitLogic.hpp"
#include "json/json.h"


int     UsnitInit(const char* conf_str, int lang) {
    if (CUsnitLogic::Instance().init(conf_str, lang))
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

int     UsnitSetInput(float value) {
    if (CUsnitLogic::Instance().setInput(value))
        return 0;
    else
        return -1;
}

const char*   UsnitGetResult(int type) {
    return CUsnitLogic::Instance().GetResult(type);
}

CUsnitLogic::CUsnitLogic(){
}

CUsnitLogic::~CUsnitLogic(){
}

bool CUsnitLogic::init(const char* conf_str, int lang){
    printf("CUsnitLogic::init");
    if (m_conf.isInt())
        return false;

    switch (lang) {
        case LANG_CH:
            m_langData.cur = m_langData.ch;
            break;
        case LANG_ENG:
            m_langData.cur = m_langData.eng;
            break;
        default:
            break;
    }

    Json::Reader reader;
    if (reader.parse(conf_str, m_conf)) {
        std::string code;
        
        m_usnitData.nLongMetricType = m_conf["long_type"].asInt();
        m_usnitData.nLongUSType = m_conf["long_us_type"].asInt();
        m_usnitData.nMassMetricType = m_conf["mass_tmetric_ype"].asInt();
        m_usnitData.nMassUSType = m_conf["mass_us_type"].asInt();
        m_usnitData.nSquareMetricType = m_conf["square_type"].asInt();
        m_usnitData.nSquareUSType = m_conf["square_us_type"].asInt();
        m_usnitData.nStartCounter = m_conf["start_counter"].asInt();
        
        m_usnitData.nLongType = m_conf["long_type"].asInt();
        m_usnitData.nMassType = m_conf["mass_type"].asInt();
        m_usnitData.nSquareType = m_conf["square_type"].asInt();
        m_usnitData.nVolumeType = m_conf["volume_type"].asInt();
        
        int lang_count = m_conf["lang"].size();

        for (int i = 0; i< lang_count; ++i){
            Json::Value lang = m_conf["lang"][i];
            MAP_ISTR& map_istr = m_langData.ch;
            std::string str = "";
            if (lang.type() == Json::ValueType::stringValue)
                str = lang["lang"].asString();
            
            if ( str == "eng" ) map_istr = m_langData.eng;
            else if (str == "") {
                // err language
                break;
            }

            int words_size = lang.size();
            for (int j=0; j<words_size; ++j){
                if (lang[j].type() != Json::ValueType::stringValue) {
                    // language "str" j not string
                    continue;
                }
                map_istr.insert(std::make_pair(j,lang[j].asString()));
            }
        }
    } // end if
    else {
        std::string err = reader.getFormatedErrorMessages();
        err = err;
    }
    
    return true;
}

bool CUsnitLogic::setLongType(int type){
    if (type!=m_usnitData.nLongType && type>=TYPE_METER && type<=TYPE_YARD){
        m_usnitData.nLongType = type;
        m_conf["long_type"]=type;
    }

    return true;
}

bool CUsnitLogic::setMassType(int type){
    if (type!=m_usnitData.nMassType && type>=TYPE_GRAM && type<=TYPE_OZ){
        m_usnitData.nMassType = type;
        m_conf["mass_type"]=type;
    }
    
    return true;
}

bool CUsnitLogic::setSquareType(int type){
    if (type!=m_usnitData.nSquareType && type>=TYPE_SQM && type<=TYPE_SQF){
        m_usnitData.nSquareType = type;
        m_conf["square_type"]=type;
    }
    
    return true;
}

bool CUsnitLogic::setVolumeType(int type){
    if (type!=m_usnitData.nVolumeType && type>=TYPE_LITRE && type<=TYPE_MLITRE){
        m_usnitData.nVolumeType = type;
        m_conf["volume_type"]=type;
    }
    
    return true;
}

bool CUsnitLogic::setInput(float value){
    
    char buf[64]={0};
    sprintf(buf, "%.04f%s", this->getMeter(value), m_langData.cur[TYPE_METER].c_str());
    m_usnitData.strMeter = buf;
    
    sprintf(buf, "%.04f%s", this->getCMeter(value), m_langData.cur[TYPE_CMETER].c_str());
    m_usnitData.strCMeter = buf;
    
    sprintf(buf, "%.04f%s", this->getKMeter(value), m_langData.cur[TYPE_KMETER].c_str());
    m_usnitData.strKMeter = buf;
    
    sprintf(buf, "%.04f%s", this->getFeet(value), m_langData.cur[TYPE_FEET].c_str());
    m_usnitData.strFeet = buf;
    
    sprintf(buf, "%.04f%s", this->getInch(value), m_langData.cur[TYPE_INCH].c_str());
    m_usnitData.strInch = buf;
    
    sprintf(buf, "%.04f%s", this->getMile(value), m_langData.cur[TYPE_MILE].c_str());
    m_usnitData.strMile = buf;
    
    sprintf(buf, "%.04f%s", this->getYard(value), m_langData.cur[TYPE_YARD].c_str());
    m_usnitData.strYard = buf;
    
    sprintf(buf, "%.04f%s", this->getLitre(value), m_langData.cur[TYPE_LITRE].c_str());
    m_usnitData.strLitre = buf;
    
    sprintf(buf, "%.04f%s", this->getMLitre(value), m_langData.cur[TYPE_MLITRE].c_str());
    m_usnitData.strMLitre = buf;
    
    sprintf(buf, "%.04f%s", this->getGal(value), m_langData.cur[TYPE_GAL].c_str());
    m_usnitData.strGal = buf;
    
    sprintf(buf, "%.04f%s", this->getGram(value), m_langData.cur[TYPE_GRAM].c_str());
    m_usnitData.strGram = buf;
    
    sprintf(buf, "%.04f%s", this->getKGram(value), m_langData.cur[TYPE_KGRAM].c_str());
    m_usnitData.strKGram = buf;
    
    sprintf(buf, "%.04f%s", this->getPound(value), m_langData.cur[TYPE_POUND].c_str());
    m_usnitData.strPound = buf;
    
    sprintf(buf, "%.04f%s", this->getOz(value), m_langData.cur[TYPE_OZ].c_str());
    m_usnitData.strOz = buf;
    
    sprintf(buf, "%.04f%s", this->getSQmeter(value), m_langData.cur[TYPE_SQM].c_str());
    m_usnitData.strSQmeter = buf;
    
    sprintf(buf, "%.04f%s", this->getSQcmeter(value), m_langData.cur[TYPE_SQCM].c_str());
    m_usnitData.strSQcmeter = buf;
    
    sprintf(buf, "%.04f%s", this->getSQfeet(value), m_langData.cur[TYPE_SQF].c_str());
    m_usnitData.strSQfeet = buf;
    
    sprintf(buf, "%.04f%s", this->getCentigrand(value), m_langData.cur[TYPE_CENTI].c_str());
    m_usnitData.strCentigrand = buf;
    
    sprintf(buf, "%.04f%s", this->getFahrenhat(value), m_langData.cur[TYPE_FAHRE].c_str());
    m_usnitData.strFahrenhat = buf;
    
    return true;
}

const char* CUsnitLogic::GetResult(int type) {
    if (type >= m_langData.cur.size()){
        return "";
    }
    return m_langData.cur[TYPE_GAL].c_str();
}

float CUsnitLogic::getMeter(float value) const {
    float metric = value;
    switch (m_usnitData.nLongUSType) {
        case TYPE_CMETER:
            metric *= 0.01f;
            break;
        case TYPE_KMETER:
            metric *= 1000.0f;
            break;
        case TYPE_FEET:
            metric = value*0.3048f;
            break;
        case TYPE_INCH:
            metric = value*0.0254f;
            break;
        case TYPE_MILE:
            metric = value*1609.344f;
            break;
        case TYPE_YARD:
            metric = value*0.9144f;
            break;
        case TYPE_METER:
        default:
            metric = value*0.3048f;
            break;
    }
    return metric;
}

float CUsnitLogic::getCMeter(float value) const {
    return this->getMeter(value)*100.0f;
}

float CUsnitLogic::getKMeter(float value) const {
    return this->getMeter(value)/1000.0f;
}

float CUsnitLogic::getFeet(float value) const {
    return this->getMeter(value)*3.2808399f;
}

float CUsnitLogic::getInch(float value) const {
    return this->getFeet(value)*0.0833333f;
}

float CUsnitLogic::getMile(float value) const {
    return this->getFeet(value)*0.0001894f;
}

float CUsnitLogic::getYard(float value) const {
    return this->getFeet(value)*0.3333333f;
}

float CUsnitLogic::getLitre(float value) const {
    float metric = value;
    switch (m_usnitData.nVolumeType) {
        case TYPE_MLITRE:
            metric *= 0.001f;
            break;
        case TYPE_GAL:
            metric = value*3.7854118f;
            break;
        case TYPE_LITRE:
        default:
            break;
    }
    return metric;
}

float CUsnitLogic::getMLitre(float value) const {
    return this->getMeter(value)*1000.0f;
}

float CUsnitLogic::getGal(float value) const {
    return this->getMeter(value)*0.2641721f;
}

float CUsnitLogic::getGram(float value) const {
    float gram = value;
    switch (m_usnitData.nVolumeType) {
        case TYPE_KGRAM:
            gram *= 0.001f;
            break;
        case TYPE_POUND:
            gram *= 453.59237f;
            break;
        case TYPE_GRAM:
        default:
            break;
    }
    return gram;
}

float CUsnitLogic::getKGram(float value) const {
    return this->getGram(value)*0.001f;
}

float CUsnitLogic::getPound(float value) const {
    return this->getGram(value)*0.0022046f;
}

float CUsnitLogic::getOz(float value) const {
    return this->getGram(value)*0.035274f;
}

float CUsnitLogic::getSQmeter(float value) const {
    float sqmeter = value;
    switch (m_usnitData.nSquareType) {
        case TYPE_SQCM:
            sqmeter = value*0.00001f;
            break;
        case TYPE_SQF:
            sqmeter = value*0.092903f;
            break;
        case TYPE_SQM:
        default:
            break;
    }
    
    return sqmeter;
}

float CUsnitLogic::getSQcmeter(float value) const {
    return this->getSQmeter(value)*10000.0f;
}

float CUsnitLogic::getSQfeet(float value) const {
    return this->getSQmeter(value)*10.7639104f;
}

float CUsnitLogic::getCentigrand(float value) const {
    return 5.0f/9.0f*(value-32.0f);
}

float CUsnitLogic::getFahrenhat(float value) const {
    return 9.0f/5.0f*value + 32.0f;
}
