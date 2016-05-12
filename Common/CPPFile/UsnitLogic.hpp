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

#include "USNIT.h"

class HttpRequest;
class CUsnitLogic {
private:
    typedef std::map<int, std::string> MAP_ISTR;
    typedef std::set<int> SETI;
    typedef std::list<int> LSTI;
    struct UsnitData {
        UsnitData()
        :fInput(0.0f)
        ,nType(-1)
        ,nLongType(-1)
        ,nMassType(-1)
        ,nSquareType(-1)
        ,nVolumeType(-1)
        ,nStartCounter(0)
        ,fAsk(0.0f), fBid(0.0f), fRate(0.0f), bRateReady(false){}
        // inputs
        float fInput;
        
        int nType;
        int nLongType;
        int nMassType;
        int nSquareType;
        int nVolumeType;
        
        float fAsk; // 卖出价
        float fBid; // 买入价
        float fRate;    // USDCNY
        std::string strDate; //日期
        std::string strRateUrl;
        bool bRateReady;
        
        LSTI lstDelayResut;
        
        int nStartCounter;

        // outputs
        MAP_ISTR mapOutputs;

        SETI setLong;
        SETI setVolume;
        SETI setMass;
        SETI setSquare;
    };
    
    struct LangData {
        LangData():cur(0), pmap(0){}
        MAP_ISTR ch;
        MAP_ISTR eng;
        int cur;
        MAP_ISTR* pmap;
        void setLang(int lang){
            switch(lang){
                case LANG_CH:
                    pmap = &ch;
                    break;
                case LANG_ENG:
                    pmap = &eng;
                    break;
                default:
                    return;
            }
            cur = lang;
        }
        const char* getWords(int type){
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
    my_cb_t m_observerResult;
    
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
    
    float transforValue(int type, float value) const;
    void updateResult();
    void read_type_set(Json::Value* array, SETI* set) const;
    bool parseRateJson(const char* str_json, UsnitData& data) const;
    bool saveConf();
    
public:
    CUsnitLogic();
    ~CUsnitLogic();
    static CUsnitLogic& Instance(){
        static CUsnitLogic instance;
        return instance;
    }
    bool init(const char* conf_path, int lang, my_cb_t cb_func);
    bool save(){return this->saveConf();}
    void setLanguage(int lang);
    
    bool setLongType(int type);
    bool setMassType(int type);
    bool setSquareType(int type);
    bool setVolumeType(int type);
    
    int getLongType() const{return m_usnitData.nLongType;}
    int getMassType() const{return m_usnitData.nMassType;}
    int getSquareType() const{return m_usnitData.nSquareType;}
    int getVolumeType() const{return m_usnitData.nVolumeType;}
    
    void setType(int type);
    bool setInput(float value);
    const char* getResult(int type);
    const char* getUnitName(int type);
    
    void HttpRequestCallback(HttpRequest* request);
};

#endif /* UsnitLogic_hpp */
