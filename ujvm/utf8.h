

#ifndef _UTF8_H_
#define _UTF8_H_

#include "config.h"

#include <string>
#include <vector>
#include <cstdint>

UJVM_NAMESPACE_BEGIN

std::wstring utf8_to_wstring(uint8_t *bytes, int size);
std::string wstrint_to_string(const std::wstring& wstr);

inline std::wstring utf8_to_wstring(const std::vector<uint8_t>& bytes)
{
    return utf8_to_wstring((uint8_t*)&bytes[0], (int)bytes.size());
}

UJVM_NAMESPACE_END

#endif