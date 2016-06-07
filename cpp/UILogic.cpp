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

using namespace usnit;

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

bool UILogic::buildInputView(const std::string& view_id){
    std::shared_ptr<gearsbox::ViewGen> viewInput = gearsbox::UiManagerGen::instance()->getView(view_id);
    viewInput->setEventHandler(shared_from_this());
    
    std::shared_ptr<gearsbox::ViewGen> textInput = viewInput->addSubView("input_text");
    if (!textInput){
        G_LOG_FC(LOG_ERROR, "add text input failed null");
        return false;
    }
    
    textInput->setFrame(gearsbox::ViewFrame(0,0,100,30));
    
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::TOP, gearsbox::ConstraintType::TOP,"input_text","parent",1,40));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::CENTERX, gearsbox::ConstraintType::CENTERX,"input_text","parent",1,0));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::WIDTH, gearsbox::ConstraintType::WIDTH,"input_text","parent",0.95,0));
    viewInput->addConstraint(gearsbox::ViewConstraint(gearsbox::ConstraintType::BOTTOM, gearsbox::ConstraintType::BOTTOM,"input_text","parent",1,-1));
    return true;
}

bool UILogic::buildUnitView(const std::string& view_id){
    struct UiBuilding{
        std::shared_ptr<gearsbox::ViewGen> first_unit;
        std::shared_ptr<gearsbox::ViewGen> last_unit;
        std::shared_ptr<gearsbox::ViewGen> last_metric_unit;
        std::shared_ptr<gearsbox::ViewGen> last_us_unit;
    };
    UiBuilding uiBuilding;
    
    MapSuperType& allUnits = CUsnitLogic::instance()->getAllUnits();
    for (int i=(int)UsnitSuperType::TYPE_NONE+1; i<(int)UsnitSuperType::TYPE_MAX; ++i) {
        UsnitSuperType superType = UsnitSuperType(i);
        MapSuperType::iterator itSuper(allUnits.find(superType));
        if (itSuper == allUnits.end())
            continue;
        
        LstUnit::iterator itUnits(itSuper->second.begin());
        for (; itUnits!=itSuper->second.end(); ++itUnits) {
            std::shared_ptr<Unit> unit = *itUnits;
            if (nullptr == unit)
                continue;
            
            // to create view_gen, set first_unit
        }
    }
    return true;
}