//
//  UILogic.cpp
//  usnit
//
//  Created by dyno on 5/28/16.
//
//

#include "UILogic.hpp"
#include "UsnitLogic.hpp"
#include "ui_manager_gen.hpp"
#include "view_gen.hpp"
#include "constraint_type.hpp"
#include "view_constraint.hpp"
#include "ILog.h"
#include "timer_handler_gen.hpp"
#include "argb_color.hpp"
#include "ui_conf_gen.hpp"
#include <vector>

using namespace usnit;
using namespace gearsbox;

class TimerClock:public gearsbox::TimerHandlerGen{
public:
    virtual void handler(int64_t time_passed, int32_t repeated){
        
        char buf[64] = {0};
        setenv("TZ", m_targetTimeZone.c_str(), 1);
        tzset();
        time_t lt=time(NULL);
        struct tm now =*localtime(&lt);
        sprintf(buf, "%s", m_timePrefix.c_str());
        ::strftime(buf+m_timePrefix.size(), sizeof(buf)-m_timePrefix.size(), " %m月%d日 %R", &now);
        
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
            m_timePrefix = "美西时间:";
        }else{
            m_targetTimeZone = "GMT-8";
            m_timePrefix = "北京时间:";
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



UILogic::UILogic(){
    
}

UILogic::~UILogic(){
    
}

bool UILogic::handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view){
    if (nullptr == view){
        G_LOG_FC(LOG_ERROR, "handle event viewgen null");
        return false;
    }
    
    if (view->getId() == "inpu_view"){
        
    }
    
    return true;
}

bool UILogic::buildUI(const std::string& view_id){
    if (view_id == "input_view")
        buildInputView(view_id);
    else if(view_id == "unit_view")
        buildUnitView(view_id);
    return true;
}

std::string UILogic::getRelativeViewto(const char* name){
    if (nullptr == name){
        G_LOG_FC(LOG_INFO,"getRelativeViewto return name null");
        return "";
    }
    std::shared_ptr<gearsbox::ViewGen> view = UiManagerGen::instance()->getView(name);
    if (view!=nullptr)
        return name;
    else if (strcmp(name, "first_unit")==0){
        return m_uibuilding.first_unit->getId();
    }
    else if(strcmp(name, "last_unit")==0) {
        return m_uibuilding.last_unit->getId();
    }
    else if(strcmp(name, "first_us_unit")==0){
        return m_uibuilding.first_us_unit->getId();
    }
    else if(strcmp(name, "last_us_unit")==0){
        return m_uibuilding.last_us_unit->getId();
    }
    else if(strcmp(name, "first_line")==0){
        return m_uibuilding.first_line->getId();
    }
    else if(strcmp(name, "lower_view")==0){
        return m_uibuilding.lower_view->getId();
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
    G_LOG_FC(LOG_INFO,"to add %d constraints:%s for %s", constraints_vec.size(), constraints_name, viewfrom);
    for (size_t i=0; i<constraints_vec.size(); ++i) {
        ViewConstraint& item = constraints_vec[i];
        item.view_from = viewfrom;
        
        if (item.view_to != "self" && item.view_to != ""){
            item.view_to = getRelativeViewto(item.view_to.c_str());
        }
        m_uibuilding.parent->addConstraint(item);
    }
}

std::shared_ptr<ViewGen> getLowerView(std::shared_ptr<ViewGen> view1, std::shared_ptr<ViewGen>view2){
    if (view2==nullptr)
        return view1;
    if (view1==nullptr)
        return view2;
    
    if (view1->getFrame().y > view2->getFrame().y)
        return view1;
    return view2;
}

bool UILogic::buildUnitView(const std::string& view_id){

    m_uibuilding = {nullptr, nullptr, nullptr, nullptr};
    std::shared_ptr<ViewGen> parent_view = UiManagerGen::instance()->getView("unit_view");
    if (nullptr == parent_view){
        G_LOG_FC(LOG_ERROR,"unit_view null");
        return false;
    }
    m_uibuilding.parent = parent_view;
    
    MapSuperType& allUnits = CUsnitLogic::instance()->getAllUnits();
    G_LOG_C(LOG_INFO, "buildUnitView allUnits %d supertype", allUnits.size());
    MapSuperType::iterator itSuper(allUnits.begin());
    for (; itSuper!=allUnits.end(); ++itSuper) {
        if (itSuper->first == UsnitSuperType::TYPE_EXCHANGE){
            m_uibuilding.exchange_info = parent_view->addSubViewById("label_exchange_info", ViewType::LABEL);
            if (!m_uibuilding.exchange_info){
                G_LOG_FC(LOG_ERROR,"add subview exchange_info null");
            }
            m_uibuilding.exchange_info->setText(CUsnitLogic::instance()->getUnitName(UsnitType::TYPE_RATE));
        }
        
        G_LOG_C(LOG_INFO,"%d units in supertype:%d", itSuper->second.size(), itSuper->first);
        LstUnit::iterator itUnits(itSuper->second.begin());
        for (; itUnits!=itSuper->second.end(); ++itUnits) {
            std::shared_ptr<Unit> unit = *itUnits;
            if (nullptr == unit)
                continue;
            
            // to create view_gen, set first_unit
            unit->label_value = parent_view->addSubViewById("label_"+unit->name, ViewType::LABEL);
            unit->label_name = parent_view->addSubViewById("label_name_"+unit->name, ViewType::LABEL);
            if (!unit->label_value || !unit->label_name){
                G_LOG_FC(LOG_ERROR,"add subview %s null", unit->name.c_str());
                continue;
            }
            
            unit->label_value->setBoardColor(ArgbColor::BLACKCOLOR);
            unit->label_value->setBoardWidth(1.0f);
            unit->label_name->setText(CUsnitLogic::instance()->getUnitName(unit->type));
            //unit->label_name->setBackgroundColor(1, 1, 0, 1);
            
            if (!unit->isUs && m_uibuilding.first_unit==nullptr)
                m_uibuilding.first_unit = unit->label_value;
            if (unit->isUs && m_uibuilding.first_us_unit==nullptr)
                m_uibuilding.first_us_unit = unit->label_value;
        }
        
        char buf[16] = {0};
        sprintf(buf, "line%d", (int)itSuper->first);
        std::shared_ptr<ViewGen> line = parent_view->addSubViewById(buf, ViewType::BASE);
        if (line==nullptr){
            G_LOG_FC(LOG_ERROR,"addsubview line %s failed", buf);
        }
        line->setBackgroundColor(1, 0, 0, 0);
        if (!m_uibuilding.first_line)
            m_uibuilding.first_line = line;
    }
    
    G_LOG_FC(LOG_INFO,"create unit subview done");
    
    for (itSuper=allUnits.begin(); itSuper!=allUnits.end(); ++itSuper) {
        if (itSuper->first == UsnitSuperType::TYPE_EXCHANGE && m_uibuilding.exchange_info){
            addConstrainsts("exchange_info", m_uibuilding.exchange_info->getId().c_str());
            m_uibuilding.last_unit = m_uibuilding.exchange_info;
            m_uibuilding.last_us_unit = m_uibuilding.exchange_info;
        }
        
        LstUnit::iterator itUnits(itSuper->second.begin());
        int metric_count = 0;
        int us_count = 0;
        for (; itUnits!=itSuper->second.end(); ++itUnits) {
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
        }
        
        char buf[16] = {0};
        sprintf(buf, "line%d", itSuper->first);
        std::shared_ptr<ViewGen> line = parent_view->getSubView(buf);
        if (line == nullptr){
            G_LOG_C(LOG_INFO,"get %s null", buf);
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
    /*
    std::shared_ptr<gearsbox::ViewGen> viewInput = gearsbox::UiManagerGen::instance()->getView(view_id);
    viewInput->setEventHandler(shared_from_this());
    
    std::shared_ptr<gearsbox::ViewGen> textInput = viewInput->addSubViewById("input_text");
    if (!textInput){
        G_LOG_FC(LOG_ERROR, "add text input failed null");
        return false;
    }
    
    textInput->setFrame(gearsbox::ViewFrame(0,0,100,30));
    
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::TOP,
                                                      gearsbox::ConstraintType::TOP,"input_text","parent",1,40));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::CENTERX,
                                                      gearsbox::ConstraintType::CENTERX,"input_text","parent",1,0));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::WIDTH,
                                                      gearsbox::ConstraintType::WIDTH,"input_text","parent",0.95,0));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::BOTTOM,
                                                      gearsbox::ConstraintType::BOTTOM,"input_text","parent",1,-1));
     */
    return true;
}

