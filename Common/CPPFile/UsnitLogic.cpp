//
//  UsnitLogic.cpp
//  USNIT
//
//  Created by dyno on 4/8/16.
//  Copyright © 2016 dyno. All rights reserved.
//
#include <fstream>

#include "USNIT.h"
#include "UsnitLogic.hpp"
#include "json/json.h"
#include "Log.h"
#include "HttpRequest.hpp"


CUsnitLogic::CUsnitLogic():m_observerResult(0){
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

bool CUsnitLogic::init(const char* conf_path, int lang, my_cb_t cb_func){
    G_LOG_FC(LOG_INFO,"CUsnitLogic::init lang:%d", lang);
    
    m_langData.setLang(lang);
    m_usnitData.mapOutputs = *m_langData.pmap;
    
    std::ifstream ifs(conf_path);
    if (!ifs.is_open()){
        G_LOG_FC(LOG_ERROR, "open conf_path failed");
    }

    Json::Reader reader;
    if (reader.parse(ifs, m_conf)) {
        m_conf_file = conf_path;
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
        
        m_usnitData.fAsk = m_conf["exchange_ask"].asFloat();
        m_usnitData.fBid = m_conf["exchange_bid"].asFloat();
        m_usnitData.fRate = m_conf["exchange_rate"].asFloat();
        m_usnitData.strRateUrl = m_conf["exchange_url"].asString();
        
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
        G_LOG_FC(LOG_ERROR, "read err:%s json:%s", err.c_str(), conf_path);
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
    
    m_observerResult = cb_func;
    HttpRequest* request = new HttpRequest();
    int rt =request->setRequest(m_usnitData.strRateUrl,
                                std::bind(&CUsnitLogic::HttpRequestCallback, this,std::placeholders::_1));
    if (rt != HttpRequest::REQUEST_OK){
        G_LOG_FC(LOG_ERROR, "http query rate err");
    }
    request->start();

    return true;
}

bool CUsnitLogic::parseRateJson(const char* str_json, CUsnitLogic::UsnitData& data) const{
    if (!str_json){
        G_LOG_FC(LOG_ERROR, "CUsnitLogic::parseRateJson str_json null");
        return false;
    }
    
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(str_json, root)){
        std::string err = reader.getFormatedErrorMessages();
        G_LOG_FC(LOG_ERROR, "read rate err:%s json:%s", err.c_str(), str_json);
        return false;
    }
    
    try {
        Json::Value rate_json = root["query"]["results"]["rate"];
        data.fAsk = atof(rate_json["Ask"].asString().c_str());
        data.fBid = atof(rate_json["Bid"].asString().c_str());
        data.fRate = atof(rate_json["Rate"].asString().c_str());
        data.bRateReady = true;

    } catch (...) {
        G_LOG_FC(LOG_ERROR, "read rate catchs ask:%f bid:%f rate:%f",
                 data.fAsk, data.fBid, data.fRate);
    }
    
    return true;
}

void CUsnitLogic::saveConf(){
    m_conf["exchange_ask"] = m_usnitData.fAsk;
    m_conf["exchange_bid"] = m_usnitData.fBid;
    m_conf["exchange_rate"] = m_usnitData.fRate;
    
    Json::FastWriter writer;
    std::string str = writer.write(m_conf);
    std::ofstream ofs;
    ofs.open(m_conf_file);
    if (!ofs.is_open()){
        G_LOG_FC(LOG_ERROR, "open conf file failed: %s", m_conf_file.c_str());
        return;
    }
    ofs << str;
    ofs.close();
}

void CUsnitLogic::HttpRequestCallback(HttpRequest* request){
    if (!request){
        G_LOG_FC(LOG_ERROR,"result null");
        return;
    }
    const HttpRequest::HTTP_RESULT& result = request->getResult();
    G_LOG_FC(LOG_INFO, "%s, data:%s", result.result?"sucess":"fail", result.content.c_str());
    /*{
        "query":{
            "count":1,
            "created":"2016-04-30T01:35:24Z",
            "lang":"zh-TW",
            "results":{
                "rate":{
                    "id":"USDCNY",
                    "Name":"USD/CNY",
                    "Rate":"6.4737",
                    "Date":"4/30/2016",
                    "Time":"2:35am",
                    "Ask":"6.4748",
                    "Bid":"6.4737"
                }
            }
        }
     }  */
    if (result.result) {
        if (this->parseRateJson(result.content.c_str(), m_usnitData)){
            
            MAP_ISTR::iterator it(m_langData.ch.find(TYPE_RATE));
            if (it!=m_langData.ch.end() && it->second.find("%")!=std::string::npos) {
                char buf[64] = {0};
                sprintf(buf, it->second.c_str(), m_usnitData.fAsk, m_usnitData.fBid);
                it->second = buf;
                G_LOG_FC(LOG_INFO, "set ch.TYPE_RATE:%s", it->second.c_str());
                
                it = m_langData.eng.find(TYPE_RATE);
                if (it!=m_langData.eng.end() && it->second.find("%")!=std::string::npos){
                    sprintf(buf, it->second.c_str(), m_usnitData.fAsk, m_usnitData.fBid);
                    it->second = buf;
                    G_LOG_FC(LOG_INFO, "set eng.TYPE_RATE:%s", buf);
                }
            }
            
            this->saveConf();
        }
    }
    
    LSTI::iterator iit(m_usnitData.lstDelayResut.begin());
    if (iit!=m_usnitData.lstDelayResut.end()){
        this->updateResult();
    }
    for (; iit!=m_usnitData.lstDelayResut.end(); ) {
        std::string result;
        int cb_type = 0;
        switch (*iit) {
            case TYPE_RATE:
                result = m_langData.getWords(TYPE_RATE);
                cb_type = CB_RATEINFO;
                break;
            case TYPE_DOLLAR:
                cb_type = CB_DOLLAR_RT;
                result = this->getResult(*iit);
                break;
            case TYPE_RMB:
                cb_type = CB_RMB_RT;
                result = this->getResult(*iit);
                break;
            default:
                ++iit;
                continue;
        }
        if (m_observerResult){
            m_observerResult(cb_type, result.c_str());
        }
        iit = m_usnitData.lstDelayResut.erase(iit);
    }
    
    delete request;
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
    for (int i=TYPE_METER; i<=TYPE_MAX_UNIT; ++i) {
        sprintf(buf, "%.06f", this->transforValue(i, m_usnitData.fInput)/*, m_langData.getWords(i)*/);
        m_usnitData.mapOutputs[i] = buf;
        //G_LOG_FC(LOG_INFO, "setInput output:%s", buf);
    }
}

const char* CUsnitLogic::getResult(int type) {
    if(!m_usnitData.bRateReady && (type == TYPE_RMB || type == TYPE_DOLLAR)){
        m_usnitData.lstDelayResut.push_back(type);
    }
    return m_usnitData.mapOutputs[type].c_str();
}

const char* CUsnitLogic::getUnitName(int type){
    if(!m_usnitData.bRateReady && (type == TYPE_RATE)){
        m_usnitData.lstDelayResut.push_back(type);
    }
    return m_langData.getWords(type);
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
        case TYPE_DOLLAR:
            return this->getDollar(value);
        case TYPE_RMB:
            return this->getRmb(value);
        case TYPE_RATE:
        case TYPE_MAX:
        default:
            break;
    }
    
    G_LOG_FC(LOG_ERROR, "err unit type: %d", type);
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

float CUsnitLogic::getDollar(float value) const{
    if (m_usnitData.bRateReady){
        return value/m_usnitData.fBid;
    }
    return 0.0f;
}

float CUsnitLogic::getRmb(float value) const{
    if (m_usnitData.bRateReady){
        return value*m_usnitData.fAsk;
    }
    return 0.0f;
}