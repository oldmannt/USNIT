// AUTOGENERATED FILE - DO NOT MODIFY!
// This file generated by Djinni from usnit.djinni

#pragma once

#include <string>

namespace usnit {

enum class UsnitEventType;

class UsnitEventGen {
public:
    virtual ~UsnitEventGen() {}

    virtual bool callback(UsnitEventType id, const std::string & data) = 0;
};

}  // namespace usnit
