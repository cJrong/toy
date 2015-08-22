

#ifndef _READER_H_
#define _READER_H_

#include "config.h"
#include "exception.h"

#include <vector>
#include <cstdint>
#include <fstream>
#include <iterator>

UJVM_NAMESPACE_BEGIN

class reader_t {
public:
    reader_t(std::string classfile)
    {
        std::ifstream ifs(classfile, std::ios::binary);

        if (!ifs.is_open()) {
            throw exception_t("No such file or directory.");
        }
        
        m_class.assign(std::istreambuf_iterator<char>(ifs),
            std::istreambuf_iterator<char>());      

        ifs.close();
    }

    reader_t(std::vector<uint8_t>& v)
    {
        m_class.swap(v);
    }

    inline bool read_uint64(uint64_t &u64)
    {
        if (m_class.size() < 8) {
            return false;
        }

        if (is_bigendian()) {
            u64 |= static_cast<uint64_t>(m_class[0]) << 0;
            u64 |= static_cast<uint64_t>(m_class[1]) << 8;
            u64 |= static_cast<uint64_t>(m_class[2]) << 16;
            u64 |= static_cast<uint64_t>(m_class[3]) << 24;
            u64 |= static_cast<uint64_t>(m_class[4]) << 32;
            u64 |= static_cast<uint64_t>(m_class[5]) << 40;
            u64 |= static_cast<uint64_t>(m_class[6]) << 48;
            u64 |= static_cast<uint64_t>(m_class[7]) << 56;

        } else {
            u64 |= static_cast<uint64_t>(m_class[0]) << 56;
            u64 |= static_cast<uint64_t>(m_class[1]) << 48;
            u64 |= static_cast<uint64_t>(m_class[2]) << 40;
            u64 |= static_cast<uint64_t>(m_class[3]) << 32;
            u64 |= static_cast<uint64_t>(m_class[4]) << 24;
            u64 |= static_cast<uint64_t>(m_class[5]) << 16;
            u64 |= static_cast<uint64_t>(m_class[6]) << 8;
            u64 |= static_cast<uint64_t>(m_class[7]) << 0;
        }

        m_class.erase(m_class.begin(), m_class.begin() + 8);
        return true;
    }

    inline bool read_int64(int64_t &i64)
    {
        return read_uint64(reinterpret_cast<uint64_t&>(i64));
    }

    inline bool read_uint32(uint32_t &u32)
    {
        if (m_class.size() < 4) {
            return false;
        }

        u32 = 0;
        if (is_bigendian()) {
            u32 |= static_cast<uint64_t>(m_class[0]) << 0;
            u32 |= static_cast<uint64_t>(m_class[1]) << 8;
            u32 |= static_cast<uint64_t>(m_class[2]) << 16;
            u32 |= static_cast<uint64_t>(m_class[3]) << 24;

        } else {
            u32 |= static_cast<uint64_t>(m_class[0]) << 24;
            u32 |= static_cast<uint64_t>(m_class[1]) << 16;
            u32 |= static_cast<uint64_t>(m_class[2]) << 8;
            u32 |= static_cast<uint64_t>(m_class[3]) << 0;
        }

        m_class.erase(m_class.begin(), m_class.begin() + 4);
        return true;
    }

    inline bool read_int32(int32_t &i32)
    {
        return read_uint32(reinterpret_cast<uint32_t&>(i32));
    }

    inline bool read_uint16(uint16_t &u16)
    {
        if (m_class.size() < 2) {
            return false;
        }

        u16 = 0;
        if (is_bigendian()) {
            u16 |= static_cast<uint64_t>(m_class[0]) << 0;
            u16 |= static_cast<uint64_t>(m_class[1]) << 8;

        } else {
            u16 |= static_cast<uint64_t>(m_class[0]) << 8;
            u16 |= static_cast<uint64_t>(m_class[1]) << 0;
        }

        m_class.erase(m_class.begin(), m_class.begin() + 2);

        return true;
    }

    inline bool read_int16(int16_t &i16)
    {
        return read_uint16(reinterpret_cast<uint16_t&>(i16));
    }

    inline bool read_uint8(uint8_t &u8)
    {
        if (m_class.empty()) {
            return false;
        }

        u8 = m_class[0];
        m_class.erase(m_class.begin());
        return true;
    }

    inline bool read_int8(int8_t &i8)
    {
        return read_uint8(reinterpret_cast<uint8_t&>(i8));
    }

    inline bool read_string(std::string &str)
    {
        if (m_class.size() < 2) {
            return false;
        }

        uint16_t len;

        if (!read_uint16(len)) {
            return false;
        }

        if (m_class.size() < len) {
            return false;
        }

        str.append(m_class.begin(), m_class.begin() + len);

        m_class.erase(m_class.begin(), m_class.begin() + len);

        return true;
    }

    inline bool read_vector(uint32_t length, std::vector<uint8_t> &v)
    {
        if ((uint32_t)m_class.size() < length) {
            return false;
        }

        v.assign(m_class.begin(), m_class.begin() + length);

        m_class.erase(m_class.begin(), m_class.begin() + length);

        return true;
    }

    inline size_t size(void)
    {
        return m_class.size();
    }

private:

    inline bool is_bigendian(void)
    {
        union {
            uint16_t u16;
            uint8_t  u8;
        }u;

        u.u16 = 0x00ff;

        return u.u8 != 0xff;
    }

private:
    std::vector<uint8_t> m_class;
};

UJVM_NAMESPACE_END

#endif