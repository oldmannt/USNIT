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

class UILogic: public gearsbox::ViewEventHandler, public std::enable_shared_from_this<UILogic> {
public:
    UILogic();
    virtual ~UILogic();
    
    virtual bool handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view);
    
    bool buildUI(const std::string& view_id);
    
private:
    bool buildInputView(const std::string& view_id);
    bool buildUnitView(const std::string& view_id);
};
}
#endif /* UILogic_hpp */
