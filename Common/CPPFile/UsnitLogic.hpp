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
#include "json/json.h"

#include "USNIT.h"

class CUsnitLogic {
private:
    typedef std::map<int, std::string> MAP_ISTR;
    struct UsnitData {
        UsnitData()
        :fInput(0.0f)
        ,nLongMetricType(-1)
        ,nLongUSType(-1)
        ,nMassMetricType(-1)
        ,nMassUSType(-1)
        ,nSquareMetricType(-1)
        ,nSquareUSType(-1)
        ,nVolumeMetricType(-1)
        ,nVolumeUSType(-1)
        ,nLongType(-1)
        ,nMassType(-1)
        ,nSquareType(-1)
        ,nVolumeType(-1)
        ,nStartCounter(0){}
        // inputs
        float fInput;
        int nLongMetricType;
        int nLongUSType;
        int nMassMetricType;
        int nMassUSType;
        int nSquareMetricType;
        int nSquareUSType;
        int nVolumeMetricType;
        int nVolumeUSType;
        
        int nLongType;
        int nMassType;
        int nSquareType;
        int nVolumeType;
        
        int nStartCounter;

        // outputs
        MAP_ISTR mapOutputs;
        std::string strMeter;
        std::string strCMeter;
        std::string strKMeter;
        
        std::string strFeet;
        std::string strInch;
        std::string strMile;
        std::string strYard;
        
        std::string strLitre;
        std::string strMLitre;
        
        std::string strGal;
        
        std::string strGram;
        std::string strKGram;
        
        std::string strPound;
        std::string strOz;
        
        std::string strSQmeter;
        std::string strSQcmeter;
        
        std::string strSQfeet;
        
        std::string strCentigrand;
        std::string strFahrenhat;
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
    
    float transforValue(int type, float value) const;
    
public:
    CUsnitLogic();
    ~CUsnitLogic();
    static CUsnitLogic& Instance(){
        static CUsnitLogic instance;
        return instance;
    }
    bool init(const char* conf_str, int lang);
    void setLanguage(int lang);
    bool setLongType(int type);
    bool setMassType(int type);
    bool setSquareType(int type);
    bool setVolumeType(int type);
    bool setInput(float value);
    const char* GetResult(int type);
};

#endif /* UsnitLogic_hpp */
