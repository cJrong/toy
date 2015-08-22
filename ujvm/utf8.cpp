

#include "utf8.h"

#include <string>
#include <cstdint>

UJVM_NAMESPACE_BEGIN

std::wstring utf8_to_wstring(uint8_t *bytes, int size)
{
    std::wstring str;

    for (auto i = 0; i < size; i++) {
        if (bytes[i] == 0) {
            str = L"null";

        } else if (bytes[i] >= uint8_t(0x01) && bytes[i] <= uint8_t(0x7f)) {
            str.push_back(bytes[i]);

        } else {
            uint16_t u16 = (uint16_t(bytes[i]) << 8) | (uint16_t(bytes[i + 1]));
            if (u16 >= uint16_t(0x80) && u16 <= uint16_t(0x7ff)) {
                uint16_t u = ((uint16_t(bytes[i]) & 0x1f) << 6) + (uint16_t(bytes[i + 1]) & 0x3f);
                str.push_back(u);
                i++;

            } else if (u16 >= uint16_t(0x800) && u16 <= uint16_t(0xffff)) {
                uint16_t u = ((uint16_t(bytes[i]) & 0xf) << 12) + (uint16_t((bytes[i + 1]) & 0x3f) << 6) + uint16_t((bytes[i + 2]) & 0x3f);
                str.push_back(u);
                i += 2;

            } else if (u16 > uint16_t(0xffff)) {
                uint32_t u = 0x10000 + ((uint32_t(bytes[i]) & 0x0f) << 16) + ((uint32_t(bytes[i + 1]) & 0x3f) << 10) + ((uint32_t(bytes[i + 2]) & 0x0f) << 6) + (uint32_t(bytes[i + 3]) & 0x3f);
                str.push_back(u & 0xffff);
                str.push_back((u >> 16) & 0xffff);
                i += 3;
            }
        }
    }

    return str;
}

std::string wstrint_to_string(const std::wstring& wstr)
{
    std::string str(wstr.size() * sizeof(wchar_t), ' ');
    auto len = std::wcstombs(const_cast<char*>(str.c_str()), wstr.c_str(), wstr.size() * sizeof(wchar_t));
    str.resize(len);
    return str;
}


UJVM_NAMESPACE_END