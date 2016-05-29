//
//  UILogic.cpp
//  usnit
//
//  Created by dyno on 5/28/16.
//
//

#include "UILogic.hpp"
#include "UsnitLogic.cpp"

using namespace usnit;

UILogic::UILogic(){
    
}

UILogic::~UILogic(){
    
}

bool UILogic::handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view){
    
}

bool UILogic::buildUI(){
        //std::shared_ptr<gearsbox::ViewGen> viewInput = gearsbox::UiInjecterGen::instance()->getView("input");
}

bool UILogic::buildInputView(){
    
}

bool UILogic::buildUnitView(){
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

}