//
//  UILogic.cpp
//  usnit
//
//  Created by dyno on 5/28/16.
//
//

#include "UILogic.hpp"
#include "ui_manager_gen.hpp"
#include "view_gen.hpp"
#include "constraint_type.hpp"
#include "view_constraint.hpp"
#include "ILog.h"
#include "timer_handler_gen.hpp"
#include "argb_color.hpp"
#include "ui_conf_gen.hpp"
#include "view_event_param.hpp"
#include "platform_utility_gen.hpp"
#include "platform_executor_gen.hpp"
#include "language_store_gen.hpp"
#include "user_config_gen.hpp"
#include "utils.hpp"
#include "config_gen.hpp"
#include "view_type.hpp"
#include <vector>
#include <math.h>
#include <cmath>

using namespace usnit;
using namespace gearsbox;

UsnitSuperType parseSuperType(const std::string& conf){
    if (conf == "length")
        return UsnitSuperType::TYPE_LENGTH;
    else if (conf == "mass")
        return UsnitSuperType::TYPE_MASS;
    else if (conf == "volume")
        return UsnitSuperType::TYPE_VOLUME;
    else if (conf == "square")
        return UsnitSuperType::TYPE_SQUARE;
    else if (conf == "termperature")
        return UsnitSuperType::TYPE_TERMPERATURE;
    else if (conf == "exchange")
        return UsnitSuperType::TYPE_EXCHANGE;
    else{
        G_LOG_FC(LOG_ERROR, "get error supertype:%s", conf.c_str());
        return UsnitSuperType::TYPE_NONE;
    }
}

const std::string SUPPER_TYPE_EXCHANGE = "exchange";
std::string superTypeString(int type){
    switch (type) {
        case (int)UsnitSuperType::TYPE_LENGTH:
            return "length";
        case (int)UsnitSuperType::TYPE_MASS:
            return "mass";
        case (int)UsnitSuperType::TYPE_VOLUME:
            return "volume";
        case (int)UsnitSuperType::TYPE_SQUARE:
            return "square";
        case (int)UsnitSuperType::TYPE_TERMPERATURE:
            return "termperature";
        case (int)UsnitSuperType::TYPE_EXCHANGE:
            return "exchange";
        case (int)UsnitSuperType::TYPE_NONE:
        default:
            break;
    }
    return std::to_string(type);
}

class TimerClock:public gearsbox::TimerHandlerGen{
public:
    virtual void handler(int64_t time_passed, int32_t repeated){
        
        char buf[64] = {0};
        setenv("TZ", m_targetTimeZone.c_str(), 1);
        tzset();
        time_t lt=time(NULL);
        struct tm now =*localtime(&lt);
        sprintf(buf, "%s", m_timePrefix.c_str());
        
        ::strftime(buf+m_timePrefix.size(), sizeof(buf)-m_timePrefix.size(),
                       LanguageStoreGen::instance()->getString("time_format").c_str(), &now);
        m_label->setText(std::move(buf));
        
        setenv("TZ", m_originalTimeZone.c_str(), 1);
        tzset();
    };
    
    virtual ~TimerClock(){}
    TimerClock(std::shared_ptr<ViewGen> label):m_label(std::move(label)){
        initilize();
    }
    
private:
    void initilize(){
        char buf[64] = {0};
        time_t lt=time(NULL);   //epoch seconds
        struct tm now = *localtime(&lt); // get local time struct tm
        
        ::strftime(buf, sizeof(buf), "%z", &now);
        if (strcmp(buf, "+0800")==0){
            m_targetTimeZone = (isDaylightTime()) ? "UTC7" : "UTC8";
            m_timePrefix = LanguageStoreGen::instance()->getString("time_zone_usw");
        }else{
            m_targetTimeZone = "GMT-8";
            m_timePrefix = m_timePrefix = LanguageStoreGen::instance()->getString("time_zone_bj");
        }
        
        int ioriginal = atoi(buf);
        sprintf(buf, "UTC%d", ioriginal*(-1)/100);
        m_originalTimeZone = buf;
    }
    
    bool isDaylightTime(){
        
        setenv("TZ", "UTC8", 1);
        tzset();
        time_t lt=time(NULL);
        struct tm now = *localtime(&lt); // get local time struct tm
        
        if (now.tm_mon>2 && now.tm_mon<9){
            return true;
        }
        if (now.tm_mon>9 || now.tm_mon<2){
            return false;
        }
        
        if (now.tm_mon==2){
            struct tm begin = now;
            begin.tm_hour = begin.tm_min = begin.tm_sec = 0;
            begin.tm_mon = 2;
            begin.tm_mday = 1;
            lt = mktime(&begin);
            begin = *localtime(&lt);
            
            begin.tm_yday += 14 - begin.tm_wday;
            return (now.tm_yday<=begin.tm_yday);
        }
        
        setenv("TZ", "UTC7", 1);
        tzset();
        lt=time(NULL);
        now = *localtime(&lt);
        
        if (now.tm_mon != 9)
            return false;
        
        struct tm begin = now;
        begin.tm_hour = begin.tm_min = begin.tm_sec = 0;
        begin.tm_mon = 9;
        begin.tm_mday = 31;
        lt = mktime(&begin);
        begin = *localtime(&lt);
        
        begin.tm_yday -= begin.tm_wday;
        return (begin.tm_yday>now.tm_yday);
    }
    
    std::shared_ptr<ViewGen> m_label;
    std::string m_timePrefix;
    std::string m_targetTimeZone;
    std::string m_originalTimeZone;
    time_t m_t;
};


std::shared_ptr<UilogicGen> UilogicGen::instance(){
    static std::shared_ptr<UilogicGen> s_ptr = std::make_shared<UILogic>();
    return s_ptr;
}
UILogic::UILogic():m_value(0.0f),m_unit_font_size(10),m_exchange_font_size(10)
                ,m_value_font_size(10){
    
}

UILogic::~UILogic(){
    m_uibuilding = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    m_valueLabels.clear();
}

bool selectUnit(std::shared_ptr<SuperTypeInfo> supter_type_info, const std::string &select_name){
    if (select_name.size()==0){
        return false;
    }
    
    bool rt = false;
    LstUnit::iterator itUnits(supter_type_info->units.begin());
    for (; itUnits!=supter_type_info->units.end(); ++itUnits) {
        std::shared_ptr<Unit> unit = *itUnits;
        if (nullptr == unit || nullptr == unit->label_value)
            continue;
        
        if (unit->name != select_name)
            continue;
        
        rt = true;
        supter_type_info->selectUnit = unit;
        UserConfigGen::instance()->setString(supter_type_info->name, select_name);
    }
    
    return rt;

}

void UILogic::updateValueLabel(const std::string& select_name, bool all){
    
    MapSuperType::iterator itSuper(m_superTypes.begin());
    for (; itSuper!=m_superTypes.end(); ++itSuper) {
        
        std::shared_ptr<SuperTypeInfo> supter_type_info = itSuper->second;
        if (supter_type_info==nullptr){
            G_LOG_FC(LOG_ERROR,"supter_type_info null %s", superTypeString(itSuper->first).c_str());
            continue;
        }
        if (!all && !::selectUnit(supter_type_info, select_name))
            continue;
        
        if (!supter_type_info->selectUnit || !supter_type_info->selectUnit->label_value){
            G_LOG_FC(LOG_ERROR,"selectUnit or it's valuse label null %s", supter_type_info->name.c_str());
            continue;
        }
        
        supter_type_info->selectUnit->value = m_value;
        //G_LOG_FC(LOG_INFO, "value:%f select:%s m:%f base:%s m:%f", m_value,
        //         supter_type_info->selectUnit->name.c_str(), supter_type_info->selectUnit->multiply,
        //         supter_type_info->baseUnit->name.c_str(), supter_type_info->baseUnit->multiply);
        supter_type_info->baseUnit->value = supter_type_info->selectUnit->getBase();
        //G_LOG_FC(LOG_INFO, "base value:%f",supter_type_info->baseUnit->value);

        LstUnit::iterator itUnits(supter_type_info->units.begin());
        for (; itUnits!=supter_type_info->units.end(); ++itUnits) {
            std::shared_ptr<Unit> unit = *itUnits;
            if (nullptr == unit || nullptr == unit->label_value)
                continue;
            
            if (unit != supter_type_info->baseUnit &&
                unit != supter_type_info->selectUnit){
                unit->calculateValue(supter_type_info->baseUnit->value);
            }
            
            float x = 1.0f;
            float y = fmod(unit->value, x);
            if (y>0.9995f){
                unit->value = unit->value+1.0f-y;
            }
            char buf[128] = {0};
            if (unit->value>1.0f)
                sprintf(buf, "%.04f", unit->value);
            else
                sprintf(buf, "%f", unit->value);
            unit->label_value->setText(buf);
            unit->label_value->setTextColor(ArgbColor::BLACKCOLOR);
        }
        
        supter_type_info->selectUnit->label_value->setTextColor(ArgbColor::BLUECOLOR);
    } // end MapSuperType

}

bool UILogic::handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view){
    if (nullptr == view){
        G_LOG_FC(LOG_ERROR, "handle event viewgen null");
        return false;
    }
    
    //G_LOG_FC(LOG_INFO, "handle event view:%s event:%d text:%s", view->getId().c_str(), param.type, param.text.c_str());
    
    if (ViewType::LABEL == view->getType()){
        
        if (m_input->getText().size() != 0)
            updateValueLabel(view->getId(), false);
        PlatformUtilityGen::instance()->getExcutor()->endEniting(true);
    }
    else if (ViewType::BASE == view->getType()){
        PlatformUtilityGen::instance()->getExcutor()->endEniting(true);
    }
    else if (ViewType::INPUT == view->getType()){
        if (view == m_input && param.text.size()>0){
            view->getText();
            m_value = atof(param.text.c_str());
            //UsnitGen::instance()->setInput(value);
            updateValueLabel("", true);
        }
    }
    
    return true;
}

std::vector<std::string> UILogic::getSubviews(){
    std::vector<std::string> unit_names;
    unit_names.push_back("time_label");
    unit_names.push_back("input_text");
    
    for (MapUnit::iterator it(m_allUnits.begin()); m_allUnits.end()!=it; ++it) {
        std::shared_ptr<Unit> unit = it->second;
        if (nullptr == unit){
            G_LOG_FC(LOG_ERROR, "null in m_allUnits:%s", it->first.c_str());
        }
        unit_names.push_back(it->first);
        unit_names.push_back("label_"+it->first);
    }
    
    unit_names.push_back("exchange_info");
    unit_names.push_back("line1");
    unit_names.push_back("line2");
    unit_names.push_back("line3");
    unit_names.push_back("line4");
    unit_names.push_back("line5");
    unit_names.push_back("line6");

    return unit_names;
}

bool UILogic::buildUi(){
    buildInputView("input_view");
    buildUnitView("unit_view");
}

std::string formatRate(){
    char buf[64] = {0};
    sprintf(buf, LanguageStoreGen::instance()->getString("exchange_info").c_str(),
                 UserConfigGen::instance()->getString("exchange_date").c_str(),
                 UserConfigGen::instance()->getFloat("exchange_ask"),
                 UserConfigGen::instance()->getFloat("exchange_bid"));
    return buf;
}

bool parseRateJson(const char* str_json) {
    if (!str_json){
        G_LOG_FC(LOG_ERROR, "parseRateJson str_json null");
        return false;
    }
    
    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(str_json, root)){
        std::string err = reader.getFormatedErrorMessages();
        G_LOG_FC(LOG_ERROR, "read rate err:%s json:%s", err.c_str(), str_json);
        return false;
    }
    float fAsk = 0.0f;
    float fBid = 0.0f;
    float fRate = 0.0f;
    try {
        Json::Value rate_json = root["query"]["results"]["rate"];
        fAsk = atof(rate_json["Ask"].asString().c_str());
        fBid = atof(rate_json["Bid"].asString().c_str());
        fRate = atof(rate_json["Rate"].asString().c_str());

        UserConfigGen::instance()->setFloat("exchange_ask", fAsk);
        UserConfigGen::instance()->setFloat("exchange_bid", fBid);
        UserConfigGen::instance()->setFloat("exchange_rate", fRate);
        UserConfigGen::instance()->setString("exchange_date", rate_json["Date"].asString());
    } catch (...) {
        G_LOG_FC(LOG_ERROR, "read rate catchs ask:%f bid:%f rate:%f",
                 fAsk, fBid, fRate);
    }
    
    return true;
}

void UILogic::HttpRequestCallback(std::shared_ptr<gearsbox::IHttpRequest> request){
    if (!request){
        G_LOG_FC(LOG_ERROR,"result null");
        return;
    }
    const gearsbox::HTTP_RESULT& result = request->getResult();
    G_LOG_FC(LOG_INFO, "%s, data:%s", result.result?"sucess":"fail", result.content.c_str());
    if (result.result) {
        parseRateJson(result.content.c_str());
    }
    
    std::shared_ptr<ViewGen> label = UiManagerGen::instance()->getView("exchange_info");
    if (label){
        label->setText(formatRate());
    }
    
    MapUnit::iterator it(m_allUnits.find("rmb"));
    if (it != m_allUnits.end()){
        it->second->multiply = UserConfigGen::instance()->getFloat("exchange_rate");
    }
    
    if (std::abs(m_value-0)>0.000001f)
        updateValueLabel("", true);
    //request = nullptr;
}

bool UILogic::initialize(const std::string& conf_path){
    Json::Value config;
    if (!readJson(conf_path, config)){
        return false;
    }
    
    m_http_request = gearsbox::IHttpRequest::create();
    int rt =m_http_request->setRequest(UserConfigGen::instance()->getString("exchange_url"),
                                       std::bind(&UILogic::HttpRequestCallback, this,std::placeholders::_1));
    if (rt != gearsbox::REQUEST_OK){
        G_LOG_FC(LOG_ERROR, "http query rate err");
    }
    m_http_request->start();
    
    try {
        m_unit_font_size = config["unit_name_font_size"].asInt();
        m_exchange_font_size = config["exchange_font_size"].asInt();
        m_value_font_size = config["unit_value_font_size"].asInt();
    } catch (std::exception &ex) {
        G_LOG_FC(LOG_ERROR, "get ui font config err:%s", ex.what());
    }
    readAllUnits(config["typesets"]);
}

bool UILogic::readAllUnits(Json::Value& conf){
    if (conf.type()!=Json::ValueType::arrayValue){
        G_LOG_FC(LOG_ERROR,"conf is not an array");
        return false;
    }
    
    std::string strMsg;
    try {
        //G_LOG_C(LOG_INFO, "to read %d supertype", conf.size());
        for (Json::ArrayIndex i = 0; i< conf.size(); ++i){
            Json::Value& item = conf[i];
            
            std::shared_ptr<SuperTypeInfo> super_type_info = std::make_shared<SuperTypeInfo>();
            super_type_info->name = item["name"].asString().c_str();
            //super_type_info->type = super_type;
            std::string base_name = item["base_unit"].asString();
            std::string select_name = UserConfigGen::instance()->getString(super_type_info->name);
            if (select_name.empty())
                select_name = base_name;
            
            Json::Value& types = item["units"];
            strMsg += super_type_info->name + "to read units \n";
            //G_LOG_C(LOG_INFO, "to read %d units in supertype:%s", types.size(), super_type_info->name.c_str());
            for (Json::ArrayIndex ii=0; ii<types.size(); ++ii) {
                Json::Value& type = types[ii];
                
                std::shared_ptr<Unit> unit = std::make_shared<Unit>();
                unit->name = type["name"].asString();
                if (unit->name == base_name){
                    super_type_info->baseUnit = unit;
                }
                
                if (unit->name == select_name) {
                    super_type_info->selectUnit = unit;
                }
                
                if (type["us"].type() != Json::ValueType::nullValue)
                    unit->isUs = type["us"].asBool();
                if (type["multiply"].type() != Json::ValueType::nullValue)
                    unit->multiply = type["multiply"].asFloat();
                if (type["offset"].type() != Json::ValueType::nullValue)
                    unit->offset = type["offset"].asFloat();
                
                if (unit->name == "rmb"){
                    unit->multiply = UserConfigGen::instance()->getFloat("exchange_rate");
                }
                
                super_type_info->units.push_back(unit);
                m_allUnits[unit->name] = unit;
                strMsg += unit->name + " unit read ok \n";
                //G_LOG_C(LOG_INFO, "readed unit %s done", unit->name.c_str());
            }
            
            //G_LOG_C(LOG_INFO, "readed %d units in supertype:%s", units.size(), super_type_info->name.c_str());
            m_superTypes[i] = super_type_info;
            strMsg += item["name"].asString() + " supertype reak ok \n";
        }
        
    } catch (std::exception& ex) {
        G_LOG_FC(LOG_ERROR,"read units err:%s readed:", ex.what(), strMsg.c_str());
    }
    G_LOG_FC(LOG_INFO,"readAllUnits done %d suppertypes, %d units",
             m_superTypes.size(), m_allUnits.size());
    return true;
}

std::string UILogic::getRelativeViewto(const char* name){
    if (nullptr == name){
        G_LOG_FC(LOG_INFO,"getRelativeViewto return name null");
        return "";
    }
    std::shared_ptr<gearsbox::ViewGen> view = UiManagerGen::instance()->getView(name);
    std::shared_ptr<gearsbox::ViewGen> find_unit = nullptr;
    if (view!=nullptr)
        return name;
    else if (strcmp(name, "first_unit")==0){
        find_unit = m_uibuilding.first_unit;
    }
    else if(strcmp(name, "last_unit")==0) {
        find_unit = m_uibuilding.last_unit;
    }
    else if(strcmp(name, "first_us_unit")==0){
        find_unit = m_uibuilding.first_us_unit;
    }
    else if(strcmp(name, "last_us_unit")==0){
        find_unit = m_uibuilding.last_us_unit;
    }
    else if(strcmp(name, "first_line")==0){
        find_unit = m_uibuilding.first_line;
    }
    else if(strcmp(name, "lower_view")==0){
        find_unit = m_uibuilding.lower_view;
    }
    
    if (find_unit!=nullptr){
        return find_unit->getId();
    }else{
        G_LOG_FC(LOG_INFO,"can't find_unit %s", name?"null":name);
        return "";
    }
    
    G_LOG_FC(LOG_INFO,"can't find viewto %s", name?"null":name);
    return "";
}

void UILogic::addConstrainsts(const char* constraints_name, const char* viewfrom){
    if (!constraints_name || !viewfrom){
        G_LOG_FC(LOG_ERROR,"constraint or viewfrom null");
        return;
    }
    
    if (!m_uibuilding.parent){
        G_LOG_FC(LOG_ERROR,"parent null");
        return;
    }
    
    std::vector<ViewConstraint> constraints_vec = std::move(UiConfGen::instance()->getConstraints(constraints_name));
    if (constraints_vec.empty()){
        G_LOG_FC(LOG_ERROR,"can't find constraints:%s", constraints_name);
        return;
    }
    //G_LOG_FC(LOG_INFO,"to add %d constraints:%s for %s", constraints_vec.size(), constraints_name, viewfrom);
    for (size_t i=0; i<constraints_vec.size(); ++i) {
        ViewConstraint& item = constraints_vec[i];
        item.view_from = viewfrom;
        
        if (item.view_to != "self" && item.view_to != ""){
            item.view_to = getRelativeViewto(item.view_to.c_str());
        }
        m_uibuilding.parent->addConstraint(item);
    }
}

bool UILogic::buildUnitView(const std::string& view_id){

    m_uibuilding = {nullptr, nullptr, nullptr, nullptr};
    std::shared_ptr<ViewGen> parent_view = UiManagerGen::instance()->getView("unit_view");
    if (nullptr == parent_view){
        G_LOG_FC(LOG_ERROR,"unit_view null");
        return false;
    }
    m_uibuilding.parent = parent_view;
    parent_view->setEventHandler(ViewEvent::TAP, shared_from_this());
    
    G_LOG_C(LOG_INFO, "buildUnitView allUnits %d supertype", m_superTypes.size());
    int line = 0;
    MapSuperType::iterator itSuper(m_superTypes.begin());
    for (; itSuper!=m_superTypes.end(); ++itSuper) {
        
        std::shared_ptr<SuperTypeInfo> supter_type_info = itSuper->second;
        if (supter_type_info==nullptr){
            G_LOG_FC(LOG_ERROR,"supter_type_info null %s", superTypeString(itSuper->first).c_str());
            continue;
        }
        
        if (supter_type_info->name == SUPPER_TYPE_EXCHANGE){
            m_uibuilding.exchange_info = parent_view->addSubViewById("exchange_info", ViewType::LABEL);
            if (!m_uibuilding.exchange_info){
                G_LOG_FC(LOG_ERROR,"add subview exchange_info null");
            } else{
                m_uibuilding.exchange_info->setText(formatRate());
                m_uibuilding.exchange_info->setFontSize(m_exchange_font_size);
            }
        }
        
        //G_LOG_C(LOG_INFO,"%d units in supertype:%s", supter_type_info->units.size(), supter_type_info->name.c_str());
        LstUnit::iterator itUnits(supter_type_info->units.begin());
        for (; itUnits!=supter_type_info->units.end(); ++itUnits) {
            std::shared_ptr<Unit> unit = *itUnits;
            if (nullptr == unit)
                continue;
            
            // to create view_gen, set first_unit
            unit->label_value = parent_view->addSubViewById(unit->name, ViewType::LABEL);
            unit->label_name = parent_view->addSubViewById("label_"+unit->name, ViewType::LABEL);
            if (!unit->label_value || !unit->label_name){
                G_LOG_FC(LOG_ERROR,"add subview %s null", unit->name.c_str());
                continue;
            }
            
            m_valueLabels.push_back(unit->label_value);
            unit->label_value->setEventHandler(ViewEvent::TAP, shared_from_this());
            unit->label_value->setBoard(1.0f,ArgbColor::BLACKCOLOR);
            unit->label_value->setFontSize(m_value_font_size);
            unit->label_value->setText("");
            unit->label_name->setText(LanguageStoreGen::instance()->getString(unit->name));
            unit->label_name->setFontSize(m_unit_font_size);
            
            if (!unit->isUs && m_uibuilding.first_unit==nullptr)
                m_uibuilding.first_unit = unit->label_value;
            if (unit->isUs && m_uibuilding.first_us_unit==nullptr)
                m_uibuilding.first_us_unit = unit->label_value;
            //G_LOG_C(LOG_INFO,"create unit name %s", unit->name.c_str());
        }
        
        char buf[16] = {0};
        sprintf(buf, "line%d", ++line);
        std::shared_ptr<ViewGen> line = parent_view->addSubViewById(buf, ViewType::BASE);
        if (line==nullptr){
            G_LOG_FC(LOG_ERROR,"addsubview line %s failed", buf);
            continue;
        }
        line->setBackgroundColor(1, 0, 0, 0);
        if (!m_uibuilding.first_line)
            m_uibuilding.first_line = line;
    }
    
    G_LOG_C(LOG_INFO,"create subview done");
    line = 0;
    for (itSuper = m_superTypes.begin(); itSuper!=m_superTypes.end(); ++itSuper) {
        
        std::shared_ptr<SuperTypeInfo> supter_type_info = itSuper->second;
        if (supter_type_info==nullptr){
            G_LOG_FC(LOG_ERROR,"supter_type_info null %s", superTypeString(itSuper->first).c_str());
            continue;
        }

        if (supter_type_info->name == SUPPER_TYPE_EXCHANGE && m_uibuilding.exchange_info){
            addConstrainsts("exchange_info", m_uibuilding.exchange_info->getId().c_str());
            m_uibuilding.last_unit = m_uibuilding.exchange_info;
            m_uibuilding.last_us_unit = m_uibuilding.exchange_info;
        }
        
        //G_LOG_C(LOG_INFO,"constraints supertype:%d %d units", itSuper->first, supter_type_info->units.size());
        LstUnit::iterator itUnits(supter_type_info->units.begin());
        int metric_count = 0;
        int us_count = 0;
        for (; itUnits!=supter_type_info->units.end(); ++itUnits) {
            std::shared_ptr<Unit> unit = *itUnits;
            if (nullptr == unit)
                continue;
            
            if (unit->label_value == m_uibuilding.first_unit){
                addConstrainsts("first_unit", unit->label_value->getId().c_str());
            }
            else if (unit->label_value == m_uibuilding.first_us_unit){
                addConstrainsts("first_us_unit", unit->label_value->getId().c_str());
            }
            else if(!unit->isUs){
                addConstrainsts("line_unit", unit->label_value->getId().c_str());
            }
            else {
                addConstrainsts("line_us_unit", unit->label_value->getId().c_str());
            }
            
            if (!unit->isUs){
                ++metric_count;
                m_uibuilding.last_unit = unit->label_value;
                addConstrainsts("line_unit_name", unit->label_name->getId().c_str());
            }
            else{
                ++us_count;
                m_uibuilding.last_us_unit = unit->label_value;
                addConstrainsts("line_us_unit_name", unit->label_name->getId().c_str());
            }
            
            if (metric_count>us_count)
                m_uibuilding.lower_view = m_uibuilding.last_unit;
            else
                m_uibuilding.lower_view = m_uibuilding.last_us_unit;
            
            //G_LOG_C(LOG_INFO,"constraints value:%s", unit->label_value->getId().c_str());
        }
        
        char buf[16] = {0};
        sprintf(buf, "line%d", ++line);
        std::shared_ptr<ViewGen> line = parent_view->getSubView(buf);
        if (line == nullptr){
            G_LOG_FC(LOG_ERROR,"get %s null", buf);
            continue;
        }
        
        if (line == m_uibuilding.first_line)
            addConstrainsts("first_line", buf);
        else{
            addConstrainsts("lines", buf);
        }
        
        m_uibuilding.last_unit = line;
        m_uibuilding.last_us_unit = line;
    }

    G_LOG_C(LOG_INFO, "buildUnitView done");
    return true;
}

bool UILogic::buildInputView(const std::string& view_id){
    
    std::shared_ptr<gearsbox::ViewGen> time_lable = gearsbox::UiManagerGen::instance()->getView("time_label");
    std::shared_ptr<gearsbox::ViewGen> input_text = gearsbox::UiManagerGen::instance()->getView("input_text");
    if (nullptr == time_lable || nullptr == input_text)
        return false;
    
    std::shared_ptr<TimerClock> clock = std::make_shared<TimerClock>(time_lable);
    m_timer = gearsbox::TimerGen::create(60*1000, -1, clock);
    m_timer->start();
    clock->handler(0, 0);
    
    std::shared_ptr<gearsbox::ViewGen> input_view = UiManagerGen::instance()->getView("input_view");
    if (input_view){
        input_view->setEventHandler(ViewEvent::TAP, shared_from_this());
    }

    m_input = input_text;
    input_text->setEventHandler(ViewEvent::TEXTCHANGE, shared_from_this());
    m_input->becomeFirstResponder();
    G_LOG_C(LOG_INFO,"buildInputView done");
    return true;
}

