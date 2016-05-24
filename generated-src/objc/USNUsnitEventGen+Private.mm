// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#import "USNUsnitEventGen+Private.h"
#import "USNUsnitEventGen.h"
#import "DJIMarshal+Private.h"
#import "DJIObjcWrapperCache+Private.h"

static_assert(__has_feature(objc_arc), "Djinni requires ARC to be enabled for this file");

namespace djinni_generated {

class UsnitEventGen::ObjcProxy final
: public ::usnit::UsnitEventGen
, public ::djinni::ObjcProxyCache::Handle<ObjcType>
{
public:
    using Handle::Handle;
    bool callback(::usnit::UsnitEventType c_id, const std::string & c_data) override
    {
        @autoreleasepool {
            auto r = [Handle::get() callback:(::djinni::Enum<::usnit::UsnitEventType, USNUsnitEventType>::fromCpp(c_id))
                                        data:(::djinni::String::fromCpp(c_data))];
            return ::djinni::Bool::toCpp(r);
        }
    }
};

}  // namespace djinni_generated

namespace djinni_generated {

auto UsnitEventGen::toCpp(ObjcType objc) -> CppType
{
    if (!objc) {
        return nullptr;
    }
    return ::djinni::get_objc_proxy<ObjcProxy>(objc);
}

auto UsnitEventGen::fromCppOpt(const CppOptType& cpp) -> ObjcType
{
    if (!cpp) {
        return nil;
    }
    return dynamic_cast<ObjcProxy&>(*cpp).Handle::get();
}

}  // namespace djinni_generated
