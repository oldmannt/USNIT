// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#pragma once

#include <functional>

namespace usnit {

enum class UsnitType : int {
    TYPE_NONE,
    TYPE_METER,
    TYPE_CMETER,
    TYPE_KMETER,
    TYPE_FEET,
    TYPE_INCH,
    TYPE_MILE,
    TYPE_YARD,
    TYPE_LITRE,
    TYPE_MLITRE,
    TYPE_GAL,
    TYPE_GRAM,
    TYPE_KGRAM,
    TYPE_POUND,
    TYPE_OZ,
    TYPE_SQM,
    TYPE_SQCM,
    TYPE_SQF,
    TYPE_CELSI,
    TYPE_FAHRE,
    TYPE_SQINCH,
    TYPE_DOLLAR,
    TYPE_RMB,
    TYPE_RATE,
    TYPE_MAX,
};

}  // namespace usnit

namespace std {

template <>
struct hash<::usnit::UsnitType> {
    size_t operator()(::usnit::UsnitType type) const {
        return std::hash<int>()(static_cast<int>(type));
    }
};

}  // namespace std
