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
#include "timer_gen.hpp"
#include <memory>

namespace usnit {

class UILogic: public gearsbox::ViewEventHandler, public std::enable_shared_from_this<UILogic> {
public:
    UILogic();
    virtual ~UILogic();
    
    virtual bool handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view);
    
    bool buildUI(const std::string& view_id);
    
private:
    struct UiBuilding{
        std::shared_ptr<gearsbox::ViewGen> parent;
        std::shared_ptr<gearsbox::ViewGen> first_unit;
        std::shared_ptr<gearsbox::ViewGen> first_us_unit;
        std::shared_ptr<gearsbox::ViewGen> first_line;
        std::shared_ptr<gearsbox::ViewGen> last_unit;
        std::shared_ptr<gearsbox::ViewGen> last_us_unit;
        std::shared_ptr<gearsbox::ViewGen> lower_view;
        std::shared_ptr<gearsbox::ViewGen> exchange_info;
    };
    
    bool buildInputView(const std::string& view_id);
    bool buildUnitView(const std::string& view_id);
    std::string getRelativeViewto(const char* name);
    void addConstrainsts(const char* constraint, const char* viewfrom);
    
    std::shared_ptr<gearsbox::TimerGen> m_timer;
    UiBuilding m_uibuilding;
};
}
#endif /* UILogic_hpp */
