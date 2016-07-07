// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#pragma once

#include "djinni_support.hpp"
#include "uilogic_gen.hpp"

namespace djinni_generated {

class NativeUilogicGen final : ::djinni::JniInterface<::usnit::UilogicGen, NativeUilogicGen> {
public:
    using CppType = std::shared_ptr<::usnit::UilogicGen>;
    using CppOptType = std::shared_ptr<::usnit::UilogicGen>;
    using JniType = jobject;

    using Boxed = NativeUilogicGen;

    ~NativeUilogicGen();

    static CppType toCpp(JNIEnv* jniEnv, JniType j) { return ::djinni::JniClass<NativeUilogicGen>::get()._fromJava(jniEnv, j); }
    static ::djinni::LocalRef<JniType> fromCppOpt(JNIEnv* jniEnv, const CppOptType& c) { return {jniEnv, ::djinni::JniClass<NativeUilogicGen>::get()._toJava(jniEnv, c)}; }
    static ::djinni::LocalRef<JniType> fromCpp(JNIEnv* jniEnv, const CppType& c) { return fromCppOpt(jniEnv, c); }

private:
    NativeUilogicGen();
    friend ::djinni::JniClass<NativeUilogicGen>;
    friend ::djinni::JniInterface<::usnit::UilogicGen, NativeUilogicGen>;

};

}  // namespace djinni_generated