// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#include "NativeUsnitGen.hpp"  // my header
#include "Marshal.hpp"
#include "NativeLangType.hpp"
#include "NativeUsnitEventGen.hpp"
#include "NativeUsnitGen.hpp"
#include "NativeUsnitType.hpp"

namespace djinni_generated {

NativeUsnitGen::NativeUsnitGen() : ::djinni::JniInterface<::usnit::UsnitGen, NativeUsnitGen>("dyno/fun/usnit/UsnitGen$CppProxy") {}

NativeUsnitGen::~NativeUsnitGen() = default;


CJNIEXPORT void JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_nativeDestroy(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        delete reinterpret_cast<::djinni::CppProxyHandle<::usnit::UsnitGen>*>(nativeRef);
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT jobject JNICALL Java_dyno_fun_usnit_UsnitGen_instance(JNIEnv* jniEnv, jobject /*this*/)
{
    try {
        DJINNI_FUNCTION_PROLOGUE0(jniEnv);
        auto r = ::usnit::UsnitGen::instance();
        return ::djinni::release(::djinni_generated::NativeUsnitGen::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jboolean JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1initialize(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jstring j_conf, jobject j_lang, jobject j_callback)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->initialize(::djinni::String::toCpp(jniEnv, j_conf),
                                 ::djinni_generated::NativeLangType::toCpp(jniEnv, j_lang),
                                 ::djinni_generated::NativeUsnitEventGen::toCpp(jniEnv, j_callback));
        return ::djinni::release(::djinni::Bool::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jboolean JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1save(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->save();
        return ::djinni::release(::djinni::Bool::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT void JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1setType(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jobject j_type)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        ref->setType(::djinni_generated::NativeUsnitType::toCpp(jniEnv, j_type));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT void JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1setInput(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jfloat j_value)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        ref->setInput(::djinni::F32::toCpp(jniEnv, j_value));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

CJNIEXPORT jstring JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getResult(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jobject j_type)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getResult(::djinni_generated::NativeUsnitType::toCpp(jniEnv, j_type));
        return ::djinni::release(::djinni::String::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jstring JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getUnitName(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jobject j_type)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getUnitName(::djinni_generated::NativeUsnitType::toCpp(jniEnv, j_type));
        return ::djinni::release(::djinni::String::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jobject JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getLongType(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getLongType();
        return ::djinni::release(::djinni_generated::NativeUsnitType::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jobject JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getMassType(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getMassType();
        return ::djinni::release(::djinni_generated::NativeUsnitType::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jobject JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getSquareType(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getSquareType();
        return ::djinni::release(::djinni_generated::NativeUsnitType::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT jobject JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1getVolumeType(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        auto r = ref->getVolumeType();
        return ::djinni::release(::djinni_generated::NativeUsnitType::fromCpp(jniEnv, r));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, 0 /* value doesn't matter */)
}

CJNIEXPORT void JNICALL Java_dyno_fun_usnit_UsnitGen_00024CppProxy_native_1buildui(JNIEnv* jniEnv, jobject /*this*/, jlong nativeRef, jstring j_viewName)
{
    try {
        DJINNI_FUNCTION_PROLOGUE1(jniEnv, nativeRef);
        const auto& ref = ::djinni::objectFromHandleAddress<::usnit::UsnitGen>(nativeRef);
        ref->buildui(::djinni::String::toCpp(jniEnv, j_viewName));
    } JNI_TRANSLATE_EXCEPTIONS_RETURN(jniEnv, )
}

}  // namespace djinni_generated
