//
//  UsnitLogic.cpp
//  USNIT
//
//  Created by dyno on 4/8/16.
//  Copyright © 2016 dyno. All rights reserved.
//
#include <fstream>


#include "UsnitLogic.hpp"
#include "ILog.h"

#include "ui_manager_gen.hpp"
#include "view_gen.hpp"
#include "view_type.hpp"
#include "view_frame.hpp"

#include "UILogic.hpp"

using namespace usnit;

std::shared_ptr<UsnitGen> UsnitGen::instance(){
    return CUsnitLogic::instance();
}

std::shared_ptr<CUsnitLogic> CUsnitLogic::instance(){
    if (nullptr == s_instance){
        s_instance = std::make_shared<CUsnitLogic>();
    }
    return s_instance;
}

std::shared_ptr<CUsnitLogic> CUsnitLogic::s_instance = nullptr;
CUsnitLogic::CUsnitLogic():m_observerResult(nullptr), m_http_request(nullptr),
m_uilogic(std::make_shared<UILogic>()){
}

CUsnitLogic::~CUsnitLogic(){
    m_uilogic = nullptr;
    m_http_request = nullptr;
}

void CUsnitLogic::read_type_set(Json::Value* array, SET_UNIT_TYPE* set)  const{
    if (!array || !set)
        return;
    
    if (array->type() == Json::ValueType::arrayValue){
        for (int i=0; i<array->size(); ++i) {
            set->insert(static_cast<UsnitType>((*array)[i].asInt()));
        }
    }
}

bool CUsnitLogic::initialize(const std::string & conf_path, LangType lang, const std::shared_ptr<usnit::UsnitEventGen> & callback){
    G_LOG_FC(LOG_INFO,"CUsnitLogic::init lang:%d", lang);
    
    m_langData.setLang(static_cast<LangType>(lang));
    m_usnitData.mapOutputs = *m_langData.pmap;
    
    std::ifstream ifs(conf_path);
    if (!ifs.is_open()){
        G_LOG_FC(LOG_ERROR, "open conf_path failed");
    }

    Json::Reader reader;
    if (reader.parse(ifs, m_conf)) {
        m_conf_file = conf_path;
        std::string code;
        
        m_usnitData.nStartCounter = m_conf["start_counter"].asInt();
        
        m_usnitData.nLongType = static_cast<UsnitType>(m_conf["long_type"].asInt());
        m_usnitData.nMassType = static_cast<UsnitType>(m_conf["mass_type"].asInt());
        m_usnitData.nSquareType = static_cast<UsnitType>(m_conf["square_type"].asInt());
        m_usnitData.nVolumeType = static_cast<UsnitType>(m_conf["volume_type"].asInt());
        
        m_usnitData.fAsk = m_conf["exchange_ask"].asFloat();
        m_usnitData.fBid = m_conf["exchange_bid"].asFloat();
        m_usnitData.fRate = m_conf["exchange_rate"].asFloat();
        m_usnitData.strRateUrl = m_conf["exchange_url"].asString();
        m_usnitData.strDate = m_conf["exchange_date"].asString();
        
        read_type_set(&m_conf["long_type_set"], &m_usnitData.setLong);
        read_type_set(&m_conf["mass_type_set"], &m_usnitData.setMass);
        read_type_set(&m_conf["square_type_set"], &m_usnitData.setSquare);
        read_type_set(&m_conf["volume_type_set"], &m_usnitData.setVolume);
        
        int lang_count = m_conf["lang"].size();

        for (int i = 0; i< lang_count; ++i){
            Json::Value lang = m_conf["lang"][i];
            MAP_UNIT_NAME* map_istr = &m_langData.ch;
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
                
                UsnitType key = static_cast<UsnitType>(atoi(strKey.c_str()));
                std::string strValue = lang[strKey].asString();
                map_istr->insert(std::make_pair(key,strValue));
            }
            
        }
        this->formatRate();
    } // end if
    else {
        std::string err = reader.getFormatedErrorMessages();
        G_LOG_FC(LOG_ERROR, "read err:%s json:%s", err.c_str(), conf_path.c_str());
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
    
    m_observerResult = callback;
    m_http_request = gearsbox::IHttpRequest::create();
    int rt =m_http_request->setRequest(m_usnitData.strRateUrl,
                                std::bind(&CUsnitLogic::HttpRequestCallback, this,std::placeholders::_1));
    if (rt != gearsbox::REQUEST_OK){
        G_LOG_FC(LOG_ERROR, "http query rate err");
    }
    m_http_request->start();

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
        
        data.strDate = rate_json["Date"].asString();

    } catch (...) {
        G_LOG_FC(LOG_ERROR, "read rate catchs ask:%f bid:%f rate:%f",
                 data.fAsk, data.fBid, data.fRate);
    }
    
    return true;
}

bool CUsnitLogic::saveConf(){
    m_conf["exchange_ask"] = m_usnitData.fAsk;
    m_conf["exchange_bid"] = m_usnitData.fBid;
    m_conf["exchange_rate"] = m_usnitData.fRate;
    
    Json::FastWriter writer;
    std::string str = writer.write(m_conf);
    std::ofstream ofs;
    ofs.open(m_conf_file);
    if (!ofs.is_open()){
        G_LOG_FC(LOG_ERROR, "open conf file failed: %s", m_conf_file.c_str());
        return false;
    }
    ofs << str;
    ofs.close();
    return true;
}

void CUsnitLogic::formatRate(){
    char buf[64] = {0};
    MAP_UNIT_NAME::iterator it(m_langData.ch.find(UsnitType::TYPE_RATE));
    if (it!=m_langData.ch.end() /*&& it->second.find("%")!=std::string::npos*/) {
        const char* format = "%s的离岸汇率,汇卖价:%.04f,汇买价:%.04f";
        sprintf(buf, format, m_usnitData.strDate.c_str(), m_usnitData.fAsk, m_usnitData.fBid);
        it->second = buf;
        G_LOG_FC(LOG_INFO, "set ch.TYPE_RATE:%s", it->second.c_str());
    }
    
    it = m_langData.eng.find(UsnitType::TYPE_RATE);
    if (it!=m_langData.eng.end() /*&& it->second.find("%")!=std::string::npos*/){
        const char* format = "From Yahoo:%s Buying Rate:%.04f Selling Rate:%.04f";
        sprintf(buf, format, m_usnitData.strDate.c_str(),m_usnitData.fAsk, m_usnitData.fBid);
        it->second = buf;
        G_LOG_FC(LOG_INFO, "set eng.TYPE_RATE:%s", buf);
    }

}

void CUsnitLogic::HttpRequestCallback(std::shared_ptr<gearsbox::IHttpRequest> request){
    if (!request){
        G_LOG_FC(LOG_ERROR,"result null");
        return;
    }
    const gearsbox::HTTP_RESULT& result = request->getResult();
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
            this->formatRate();
            this->saveConf();
        }
    }
    
    LST_UNIT_TYPE::iterator iit(m_usnitData.lstDelayResut.begin());
    if (iit!=m_usnitData.lstDelayResut.end()){
        this->updateResult();
    }
    for (; iit!=m_usnitData.lstDelayResut.end(); ) {
        std::string result;
        UsnitEventType cb_type = UsnitEventType::CB_NONE;
        UsnitType unit_type = static_cast<UsnitType>(*iit);
        switch (unit_type) {
            case UsnitType::TYPE_RATE:
                result = m_langData.getWords(UsnitType::TYPE_RATE);
                cb_type = UsnitEventType::CB_RATEINFO;
                break;
            case UsnitType::TYPE_DOLLAR:
                cb_type = UsnitEventType::CB_DOLLAR_RT;
                result = this->getResult(unit_type);
                break;
            case UsnitType::TYPE_RMB:
                cb_type = UsnitEventType::CB_RMB_RT;
                result = this->getResult(unit_type);
                break;
            default:
                ++iit;
                continue;
        }
        if (m_observerResult){
            m_observerResult->callback(cb_type, result.c_str());
        }
        iit = m_usnitData.lstDelayResut.erase(iit);
    }
    
    request = nullptr;
}

bool CUsnitLogic::setLongType(UsnitType type){
    if (type!=m_usnitData.nLongType &&
        m_usnitData.setLong.find(type)!=m_usnitData.setLong.end()){
        m_usnitData.nLongType = type;
        m_conf["long_type"]=static_cast<int>(type);
        this->updateResult();
        return true;
    }

    return false;
}

bool CUsnitLogic::setMassType(UsnitType type){
    if (type!=m_usnitData.nMassType &&
        m_usnitData.setMass.find(type)!=m_usnitData.setMass.end()){
        m_usnitData.nMassType = type;
        m_conf["mass_type"]=static_cast<int>(type);
        this->updateResult();
        return true;
    }
    
    return false;
}

bool CUsnitLogic::setSquareType(UsnitType type){
    if (type!=m_usnitData.nSquareType &&
        m_usnitData.setSquare.find(type)!=m_usnitData.setSquare.end()){
        m_usnitData.nSquareType = type;
        m_conf["square_type"]=static_cast<int>(type);
        this->updateResult();
        return true;
    }
    
    return false;
}

bool CUsnitLogic::setVolumeType(UsnitType type){
    if (type!=m_usnitData.nVolumeType &&
        m_usnitData.setVolume.find(type)!=m_usnitData.setVolume.end()){
        m_usnitData.nVolumeType = type;
        m_conf["volume_type"]=static_cast<int>(type);
        this->updateResult();
        return true;
    }
    
    return false;
}

void CUsnitLogic::setType(UsnitType type){
    m_usnitData.nType = type;
    if (this->setLongType(type))
        return;
    if (this->setMassType(type))
        return;
    if (this->setSquareType(type))
        return;
    if (this->setVolumeType(type))
        return;
}

void CUsnitLogic::setInput(float value){
    
    m_usnitData.fInput = value;
    this->updateResult();
}

void CUsnitLogic::updateResult(){
    char buf[64]={0};
    for (int i = (int)UsnitType::TYPE_METER; i<=(int)UsnitType::TYPE_RMB; ++i) {
        UsnitType type = static_cast<UsnitType>(i);
        sprintf(buf, "%.06f", this->transforValue(type, m_usnitData.fInput)/*, m_langData.getWords(i)*/);
        m_usnitData.mapOutputs[type] = buf;
        //G_LOG_FC(LOG_INFO, "setInput output:%s", buf);
    }
}

std::string CUsnitLogic::getResult(UsnitType type) {
    if(!m_usnitData.bRateReady && (type == UsnitType::TYPE_RMB || type == UsnitType::TYPE_DOLLAR)){
        m_usnitData.lstDelayResut.push_back(type);
    }
    return m_usnitData.mapOutputs[type].c_str();
}

std::string CUsnitLogic::getUnitName(UsnitType type){
    if(!m_usnitData.bRateReady && (type == UsnitType::TYPE_RATE)){
        m_usnitData.lstDelayResut.push_back(type);
    }
    return m_langData.getWords(type);
}

float CUsnitLogic::transforValue( UsnitType type, float value) const{
    switch (type) {
        case UsnitType::TYPE_METER:
            return this->getMeter(value);
        case UsnitType::TYPE_CMETER:
            return this->getCMeter(value);
        case UsnitType::TYPE_KMETER:
            return this->getKMeter(value);
        case UsnitType::TYPE_FEET:
            return this->getFeet(value);
        case UsnitType::TYPE_INCH:
            return this->getInch(value);
        case UsnitType::TYPE_MILE:
            return this->getMile(value);
        case UsnitType::TYPE_YARD:
            return this->getYard(value);
        case UsnitType::TYPE_LITRE:
            return this->getLitre(value);
        case UsnitType::TYPE_MLITRE:
            return this->getMLitre(value);
        case UsnitType::TYPE_GAL:
            return this->getGal(value);
        case UsnitType::TYPE_GRAM:
            return this->getGram(value);
        case UsnitType::TYPE_KGRAM:
            return this->getKGram(value);
        case UsnitType::TYPE_POUND:
            return this->getPound(value);
        case UsnitType::TYPE_OZ:
            return this->getOz(value);
        case UsnitType::TYPE_SQM:
            return this->getSQmeter(value);
        case UsnitType::TYPE_SQCM:
            return this->getSQcmeter(value);
        case UsnitType::TYPE_SQF:
            return this->getSQfeet(value);
        case UsnitType::TYPE_CELSI:
            return this->getCentigrand(value);
        case UsnitType::TYPE_FAHRE:
            return this->getFahrenhat(value);
        case UsnitType::TYPE_SQINCH:
            return this->getSQinch(value);
        case UsnitType::TYPE_DOLLAR:
            return this->getDollar(value);
        case UsnitType::TYPE_RMB:
            return this->getRmb(value);
        case UsnitType::TYPE_RATE:
        case UsnitType::TYPE_MAX:
        default:
            break;
    }
    
    G_LOG_FC(LOG_ERROR, "err unit type: %d", type);
    return 0.0f;
}

float CUsnitLogic::getMeter(float value) const {
    float metric = value;
    switch (m_usnitData.nLongType) {
        case UsnitType::TYPE_CMETER:
            metric *= 0.01f;
            break;
        case UsnitType::TYPE_KMETER:
            metric *= 1000.0f;
            break;
        case UsnitType::TYPE_FEET:
            metric = value*0.3048f;
            break;
        case UsnitType::TYPE_INCH:
            metric = value*0.0254f;
            break;
        case UsnitType::TYPE_MILE:
            metric = value*1609.344f;
            break;
        case UsnitType::TYPE_YARD:
            metric = value*0.9144f;
            break;
        case UsnitType::TYPE_METER:
        default:
            break;
    }
    return metric;
}

float CUsnitLogic::getCMeter(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_CMETER)
        return value;
    return this->getMeter(value)*100.0f;
}

float CUsnitLogic::getKMeter(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_KMETER)
        return value;
    return this->getMeter(value)/1000.0f;
}

float CUsnitLogic::getFeet(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_FEET)
        return value;
    return this->getMeter(value)*3.2808399f;
}

float CUsnitLogic::getInch(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_INCH)
        return value;
    return this->getFeet(value)*12;
}

float CUsnitLogic::getMile(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_MILE)
        return value;
    return this->getFeet(value)*0.0001894f;
}

float CUsnitLogic::getYard(float value) const {
    if (m_usnitData.nLongType == UsnitType::TYPE_YARD)
        return value;
    return this->getFeet(value)*0.3333333f;
}

float CUsnitLogic::getLitre(float value) const {
    float metric = value;
    switch (m_usnitData.nVolumeType) {
        case UsnitType::TYPE_MLITRE:
            metric *= 0.001f;
            break;
        case UsnitType::TYPE_GAL:
            metric = value*3.7854118f;
            break;
        case UsnitType::TYPE_LITRE:
        default:
            break;
    }
    return metric;
}

float CUsnitLogic::getMLitre(float value) const {
    if (m_usnitData.nVolumeType == UsnitType::TYPE_MLITRE)
        return value;
    return this->getLitre(value)*1000.0f;
}

float CUsnitLogic::getGal(float value) const {
    if (m_usnitData.nVolumeType == UsnitType::TYPE_GAL)
        return value;
    return this->getLitre(value)*0.2641721f;
}

float CUsnitLogic::getGram(float value) const {
    float gram = value;
    switch (m_usnitData.nMassType) {
        case UsnitType::TYPE_KGRAM:
            gram *= 1000.0f;
            break;
        case UsnitType::TYPE_POUND:
            gram *= 453.59237f;
            break;
        case UsnitType::TYPE_OZ:
            gram *= 28.3495231f;
        case UsnitType::TYPE_GRAM:
        default:
            break;
    }
    return gram;
}

float CUsnitLogic::getKGram(float value) const {
    if (m_usnitData.nMassType == UsnitType::TYPE_KGRAM)
        return value;
    return this->getGram(value)*0.001f;
}

float CUsnitLogic::getPound(float value) const {
    if (m_usnitData.nMassType == UsnitType::TYPE_POUND)
        return value;
    return this->getGram(value)*0.0022046f;
}

float CUsnitLogic::getOz(float value) const {
    if (m_usnitData.nMassType == UsnitType::TYPE_OZ)
        return value;
    return this->getGram(value)*0.035274f;
}

float CUsnitLogic::getSQmeter(float value) const {
    float sqmeter = value;
    switch (m_usnitData.nSquareType) {
        case UsnitType::TYPE_SQCM:
            sqmeter = value*0.00001f;
            break;
        case UsnitType::TYPE_SQF:
            sqmeter = value*0.092903f;
            break;
        case UsnitType::TYPE_SQINCH:
            sqmeter = value/1550.0031f;
        case UsnitType::TYPE_SQM:
        default:
            break;
    }
    
    return sqmeter;
}

float CUsnitLogic::getSQcmeter(float value) const {
    if (m_usnitData.nSquareType == UsnitType::TYPE_SQCM)
        return value;
    return this->getSQmeter(value)*10000.0f;
}

float CUsnitLogic::getSQfeet(float value) const {
    if (m_usnitData.nSquareType == UsnitType::TYPE_SQF)
        return value;
    return this->getSQmeter(value)*10.7639104f;
}

float CUsnitLogic::getSQinch(float value) const {
    if (m_usnitData.nMassType == UsnitType::TYPE_SQINCH)
        return value;
    return this->getSQmeter(value)*1550.0031f;
}

float CUsnitLogic::getCentigrand(float value) const {
    return 5.0f/9.0f*(value-32.0f);
}

float CUsnitLogic::getFahrenhat(float value) const {
    return 9.0f/5.0f*value + 32.0f;
}

float CUsnitLogic::getDollar(float value) const{
    return value/m_usnitData.fBid;
}

float CUsnitLogic::getRmb(float value) const{
    return value*m_usnitData.fAsk;
}

void CUsnitLogic::buildView(const std::string & view_id){
    if (nullptr==m_uilogic){
        G_LOG_FC(LOG_ERROR, "m_uilogic null sth very bad happened");
        return;
    }
    m_uilogic->buildUI(view_id);
}

