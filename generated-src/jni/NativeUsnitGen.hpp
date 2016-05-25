// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#pragma once

#include "djinni_support.hpp"
#include "usnit_gen.hpp"

namespace djinni_generated {

class NativeUsnitGen final : ::djinni::JniInterface<::usnit::UsnitGen, NativeUsnitGen> {
public:
    using CppType = std::shared_ptr<::usnit::UsnitGen>;
    using CppOptType = std::shared_ptr<::usnit::UsnitGen>;
    using JniType = jobject;

    using Boxed = NativeUsnitGen;

    ~NativeUsnitGen();

    static CppType toCpp(JNIEnv* jniEnv, JniType j) { return ::djinni::JniClass<NativeUsnitGen>::get()._fromJava(jniEnv, j); }
    static ::djinni::LocalRef<JniType> fromCppOpt(JNIEnv* jniEnv, const CppOptType& c) { return {jniEnv, ::djinni::JniClass<NativeUsnitGen>::get()._toJava(jniEnv, c)}; }
    static ::djinni::LocalRef<JniType> fromCpp(JNIEnv* jniEnv, const CppType& c) { return fromCppOpt(jniEnv, c); }

private:
    NativeUsnitGen();
    friend ::djinni::JniClass<NativeUsnitGen>;
    friend ::djinni::JniInterface<::usnit::UsnitGen, NativeUsnitGen>;

};

}  // namespace djinni_generated