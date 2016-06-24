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
#include "json/json.h"
#include "ihttprequest.h"
#include "uilogic_gen.hpp"
#include <memory>
#include <list>
#include <map>
#include <string>

namespace usnit {
    
    enum class UsnitSuperType : int {
        TYPE_NONE,
        TYPE_LENGTH,
        TYPE_MASS,
        TYPE_VOLUME,
        TYPE_SQUARE,
        TYPE_EXCHANGE,
        TYPE_TERMPERATURE,
        
        TYPE_MAX,
    };
    
    struct Unit{
        std::string name;
        bool isUs;
        float multiply;
        float offset;
        float value;
        std::string output;
        std::shared_ptr<gearsbox::ViewGen> label_value;
        std::shared_ptr<gearsbox::ViewGen> label_name;
        Unit(): isUs(false), multiply(0.0f), offset(0.0f), value(0.0f){}
        ~Unit(){
            label_value = nullptr;
            label_name = nullptr;
        }
        float getBase(){
            return (value-offset)/multiply;
        }
        void calculateValue(float base){
            value = base*multiply + offset;
        }
    };
    typedef std::list<std::shared_ptr<Unit>> LstUnit;
    
    struct SuperTypeInfo{
        UsnitSuperType type;
        std::string name;
        std::shared_ptr<Unit> selectUnit;
        std::shared_ptr<Unit> baseUnit;
        LstUnit units;
        
        ~SuperTypeInfo(){
            selectUnit = nullptr;
            baseUnit = nullptr;
        }
    };
    
    typedef std::map<UsnitSuperType, std::shared_ptr<SuperTypeInfo>> MapSuperType;
    typedef std::map<std::string,std::shared_ptr<Unit>> MapUnit;

class UILogic:public UilogicGen, public gearsbox::ViewEventHandler, public std::enable_shared_from_this<UILogic> {
public:
    UILogic();
    virtual ~UILogic();
    
    virtual bool initialize(const std::string & conf_path);
    virtual bool handle(const gearsbox::ViewEventParam & param, const std::shared_ptr<gearsbox::ViewGen> & view);

    bool buildUI(const std::string& view_id);
    bool readAllUnits(Json::Value& conf);
    void HttpRequestCallback(std::shared_ptr<gearsbox::IHttpRequest> http_request);
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
    typedef std::list<std::shared_ptr<gearsbox::ViewGen>> LstView;
    
    bool buildInputView(const std::string& view_id);
    bool buildUnitView(const std::string& view_id);
    std::string getRelativeViewto(const char* name);
    void addConstrainsts(const char* constraint, const char* viewfrom);
    
    void updateValueLabel(const std::string& select_name);
    
    std::shared_ptr<gearsbox::TimerGen> m_timer;
    UiBuilding m_uibuilding;
    
    std::shared_ptr<gearsbox::ViewGen> m_input;
    LstView m_valueLabels;

    MapSuperType m_superTypes;
    MapUnit m_allUnits;
    
    float m_value;
    std::shared_ptr<gearsbox::IHttpRequest> m_http_request;
    int m_unit_font_size;
    int m_exchange_font_size;
};
}
#endif /* UILogic_hpp */
