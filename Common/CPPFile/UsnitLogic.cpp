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
#include "Log.h"


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
    return CUsnitLogic::Instance().GetResult(type);
}

CUsnitLogic::CUsnitLogic(){
}

CUsnitLogic::~CUsnitLogic(){
}

void CUsnitLogic::read_type_set(Json::Value* array, SETI* set)  const{
    if (!array || !set)
        return;
    
    if (array->type() == Json::ValueType::arrayValue){
        for (int i=0; i<array->size(); ++i) {
            set->insert((*array)[i].asInt());
        }
    }
}
bool CUsnitLogic::init(const char* conf_str, int lang){
    G_LOG_FC(LOG_INFO,"CUsnitLogic::init lang:%d", lang);

    m_langData.setLang(lang);
    m_usnitData.mapOutputs = *m_langData.pmap;

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
        
        read_type_set(&m_conf["long_type_set"], &m_usnitData.setLong);
        read_type_set(&m_conf["mass_type_set"], &m_usnitData.setMass);
        read_type_set(&m_conf["square_type_set"], &m_usnitData.setSquare);
        read_type_set(&m_conf["volume_type_set"], &m_usnitData.setVolume);
        
        int lang_count = m_conf["lang"].size();

        for (int i = 0; i< lang_count; ++i){
            Json::Value lang = m_conf["lang"][i];
            MAP_ISTR* map_istr = &m_langData.ch;
            std::string str = "";
            if (lang["lang"].type() == Json::ValueType::stringValue)
                str = lang["lang"].asString();
            
            if ( str == "eng" ) map_istr = &m_langData.eng;
            else if (str == "ch") map_istr = &m_langData.ch;
            else{
                // err language
                G_LOG_FC(LOG_ERROR, "read json err lang:%s", str.c_str());
                break;
            }

            Json::Value::Members mem = lang.getMemberNames();
            Json::Value::Members::iterator it(mem.begin());
            for (; it!=mem.end(); ++it){
                std::string strKey = *it;
                
                if (strKey=="lang") {
                    continue;
                }
                
                int key = atoi(strKey.c_str());
                std::string strValue = lang[strKey].asString();
                map_istr->insert(std::make_pair(key,strValue));
            }
            
        }
    } // end if
    else {
        std::string err = reader.getFormatedErrorMessages();
        G_LOG_FC(LOG_ERROR, "read err:%s json:%s", err.c_str(), conf_str);
    }
    
    // print all lang words
    /*
    MAP_ISTR::iterator it = m_langData.ch.begin();
    for (; it!=m_langData.ch.end(); ++it) {
        G_LOG_C(LOG_INFO, "lang ch %d:%s", it->first, it->second.c_str());
    }
    
    it = m_langData.eng.begin();
    for (; it!=m_langData.eng.end(); ++it) {
        G_LOG_C(LOG_INFO, "lang eng %d:%s", it->first, it->second.c_str());
    }
    */
    
    return true;
}

bool CUsnitLogic::setLongType(int type){
    if (type!=m_usnitData.nLongType &&
        m_usnitData.setLong.find(type)!=m_usnitData.setLong.end()){
        m_usnitData.nLongType = type;
        m_conf["long_type"]=type;
        this->updateResult();
        return true;
    }

    return false;
}

bool CUsnitLogic::setMassType(int type){
    if (type!=m_usnitData.nMassType &&
        m_usnitData.setMass.find(type)!=m_usnitData.setMass.end()){
        m_usnitData.nMassType = type;
        m_conf["mass_type"]=type;
        this->updateResult();
        return true;
    }
    
    return false;
}

bool CUsnitLogic::setSquareType(int type){
    if (type!=m_usnitData.nSquareType &&
        m_usnitData.setSquare.find(type)!=m_usnitData.setSquare.end()){
        m_usnitData.nSquareType = type;
        m_conf["square_type"]=type;
        this->updateResult();
        return true;
    }
    
    return false;
}

bool CUsnitLogic::setVolumeType(int type){
    if (type!=m_usnitData.nVolumeType &&
        m_usnitData.setVolume.find(type)!=m_usnitData.setVolume.end()){
        m_usnitData.nVolumeType = type;
        m_conf["volume_type"]=type;
        this->updateResult();
        return true;
    }
    
    return false;
}

void CUsnitLogic::setType(int type){
    if (this->setLongType(type))
        return;
    if (this->setMassType(type))
        return;
    if (this->setSquareType(type))
        return;
    if (this->setVolumeType(type))
        return;
}

bool CUsnitLogic::setInput(float value){
    
    m_usnitData.fInput = value;
    this->updateResult();
    return true;
}

void CUsnitLogic::updateResult(){
    char buf[64]={0};
    for (int i=TYPE_METER; i<TYPE_MAX; ++i) {
        sprintf(buf, "%.04f%s", this->transforValue(i, m_usnitData.fInput), m_langData.getWords(i));
        m_usnitData.mapOutputs[i] = buf;
        //G_LOG_FC(LOG_INFO, "setInput output:%s", buf);
    }
}

const char* CUsnitLogic::GetResult(int type) {
    return m_usnitData.mapOutputs[type].c_str();
}

float CUsnitLogic::transforValue( int type, float value) const{
    switch (type) {
        case TYPE_METER:
            return this->getMeter(value);
        case TYPE_CMETER:
            return this->getCMeter(value);
        case TYPE_KMETER:
            return this->getKMeter(value);
        case TYPE_FEET:
            return this->getFeet(value);
        case TYPE_INCH:
            return this->getInch(value);
        case TYPE_MILE:
            return this->getMile(value);
        case TYPE_YARD:
            return this->getYard(value);
        case TYPE_LITRE:
            return this->getLitre(value);
        case TYPE_MLITRE:
            return this->getMLitre(value);
        case TYPE_GAL:
            return this->getGal(value);
        case TYPE_GRAM:
            return this->getGram(value);
        case TYPE_KGRAM:
            return this->getKGram(value);
        case TYPE_POUND:
            return this->getPound(value);
        case TYPE_OZ:
            return this->getOz(value);
        case TYPE_SQM:
            return this->getSQmeter(value);
        case TYPE_SQCM:
            return this->getSQcmeter(value);
        case TYPE_SQF:
            return this->getSQfeet(value);
        case TYPE_CELSI:
            return this->getCentigrand(value);
        case TYPE_FAHRE:
            return this->getFahrenhat(value);
        case TYPE_SQINCH:
            return this->getSQinch(value);
        case TYPE_MAX:
        default:
            break;
    }
    
    G_LOG_FC(LOG_ERROR, "err unit type");
    return 0.0f;
}

float CUsnitLogic::getMeter(float value) const {
    float metric = value;
    switch (m_usnitData.nLongType) {
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
    return this->getFeet(value)*12;
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
    return this->getLitre(value)*1000.0f;
}

float CUsnitLogic::getGal(float value) const {
    return this->getLitre(value)*0.2641721f;
}

float CUsnitLogic::getGram(float value) const {
    float gram = value;
    switch (m_usnitData.nMassType) {
        case TYPE_KGRAM:
            gram *= 1000.0f;
            break;
        case TYPE_POUND:
            gram *= 453.59237f;
            break;
        case TYPE_OZ:
            gram *= 28.3495231f;
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
        case TYPE_SQINCH:
            sqmeter = value/1550.0031f;
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

float CUsnitLogic::getSQinch(float value) const {
    return this->getSQmeter(value)*1550.0031f;
}
