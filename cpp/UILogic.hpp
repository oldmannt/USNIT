//
//  UILogic.hpp
//  usnit
//
//  Created by dyno on 5/28/16.
//
//

#ifndef UILogic_hpp
#define UILogic_hpp

#include "view_event_handler.hpp"

namespace usnit {

class UILogic: public gearsbox::ViewEventHandler {
public:
    UILogic();
    virtual ~UILogic();
    
    virtual bool handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view);
    
    bool buildUI();
    
private:
    bool buildInputView();
    bool buildUnitView();
};
}
#endif /* UILogic_hpp */
