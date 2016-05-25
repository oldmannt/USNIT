// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#pragma once

#include <functional>

namespace usnit {

enum class LangType : int {
    LANG_NONE,
    LANG_CH,
    LANG_ENG,
};

}  // namespace usnit

namespace std {

template <>
struct hash<::usnit::LangType> {
    size_t operator()(::usnit::LangType type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std