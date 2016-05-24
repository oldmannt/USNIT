//
//  UsnitLogic.hpp
//  USNIT
//
//  Created by dyno on 4/8/16.
//  Copyright © 2016 dyno. All rights reserved.
//

#ifndef UsnitLogic_hpp
#define UsnitLogic_hpp

#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <list>
#include "json/json.h"

#include "usnit_gen.hpp"
#include "IHttpRequest.h"
#include "usnit_event_gen.hpp"
#include "lang_type.hpp"
#include "usnit_event_type.hpp"
#include "usnit_type.hpp"

namespace usnit {

class CUsnitLogic:public usnit::UsnitGen {
public:
    virtual bool initialize(const std::string & conf, LangType lang, const std::shared_ptr<usnit::UsnitEventGen> & callback);
    virtual bool save(){return this->saveConf();}
    virtual void setType(UsnitType type);
    virtual void setInput(float value);
    virtual std::string getResult(UsnitType type);
    virtual std::string getUnitName(UsnitType type);
    virtual UsnitType getLongType(){return m_usnitData.nLongType;}
    virtual UsnitType getMassType(){return m_usnitData.nMassType;}
    virtual UsnitType getSquareType(){return m_usnitData.nSquareType;}
    virtual UsnitType getVolumeType(){return m_usnitData.nVolumeType;}

public:
    CUsnitLogic();
    virtual ~CUsnitLogic();
    static CUsnitLogic& Instance(){
        static CUsnitLogic instance;
        return instance;
    }

    void setLanguage(UsnitType lang);
    bool setLongType(UsnitType type);
    bool setMassType(UsnitType type);
    bool setSquareType(UsnitType type);
    bool setVolumeType(UsnitType type);
    
    void HttpRequestCallback(std::shared_ptr<gearsbox::IHttpRequest> http_request);

private:
    typedef std::map<UsnitType, std::string> MAP_UNIT_NAME;
    typedef std::set<UsnitType> SET_UNIT_TYPE;
    typedef std::list<UsnitType> LST_UNIT_TYPE;
    struct UsnitData {
        UsnitData()
        :fInput(0.0f)
        ,nType(UsnitType::TYPE_NONE)
        ,nLongType(UsnitType::TYPE_NONE)
        ,nMassType(UsnitType::TYPE_NONE)
        ,nSquareType(UsnitType::TYPE_NONE)
        ,nVolumeType(UsnitType::TYPE_NONE)
        ,fAsk(0.0f), fBid(0.0f), fRate(0.0f), bRateReady(false),nStartCounter(0) {}
        // inputs
        float fInput;
        
        UsnitType nType;
        UsnitType nLongType;
        UsnitType nMassType;
        UsnitType nSquareType;
        UsnitType nVolumeType;
        
        float fAsk; // 卖出价
        float fBid; // 买入价
        float fRate;    // USDCNY
        std::string strDate; //日期
        std::string strRateUrl;
        bool bRateReady;
        
        LST_UNIT_TYPE lstDelayResut;
        
        int nStartCounter;

        // outputs
        MAP_UNIT_NAME mapOutputs;

        SET_UNIT_TYPE setLong;
        SET_UNIT_TYPE setVolume;
        SET_UNIT_TYPE setMass;
        SET_UNIT_TYPE setSquare;
    };
    
    struct LangData {
        LangData():cur(LangType::LANG_NONE), pmap(0){}
        MAP_UNIT_NAME ch;
        MAP_UNIT_NAME eng;
        LangType cur;
        MAP_UNIT_NAME* pmap;
        void setLang(LangType lang){
            switch(lang){
                case LangType::LANG_CH:
                    pmap = &ch;
                    break;
                case LangType::LANG_ENG:
                    pmap = &eng;
                    break;
                default:
                    return;
            }
            cur = lang;
        }
        const char* getWords(UsnitType type){
            if (pmap && pmap->find(type)!=pmap->end()){
                return pmap->at(type).c_str();
            }
            return "";
        }
    };
    
    UsnitData m_usnitData;
    LangData m_langData;
    Json::Value m_conf;
    std::string m_conf_file;
    std::shared_ptr<UsnitEventGen> m_observerResult;
    std::shared_ptr<gearsbox::IHttpRequest> m_http_request;
    
    float getMeter(float value) const;
    float getCMeter(float value) const;
    float getKMeter(float value) const;
    float getFeet(float value) const;
    float getInch(float value) const;
    float getMile(float value) const;
    float getYard(float value) const;
    float getLitre(float value) const;
    float getMLitre(float value) const;
    float getGal(float value) const;
    float getGram(float value) const;
    float getKGram(float value) const;
    float getPound(float value) const;
    float getOz(float value) const;
    float getSQmeter(float value) const;
    float getSQcmeter(float value) const;
    float getSQfeet(float value) const;
    float getCentigrand(float value) const;
    float getFahrenhat(float value) const;
    float getSQinch(float value) const;
    float getDollar(float value) const;
    float getRmb(float value) const;
    
    float transforValue(UsnitType type, float value) const;
    void updateResult();
    void read_type_set(Json::Value* array, SET_UNIT_TYPE* set) const;
    bool parseRateJson(const char* str_json, UsnitData& data) const;
    bool saveConf();
    void formatRate();
};
}
#endif /* UsnitLogic_hpp */
