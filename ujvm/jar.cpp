
#include "jar.h"
#include "class_file.h"
#include "zlib/zlib.h"

#include <fstream>
#include <cstring>
#include <iostream>


UJVM_NAMESPACE_BEGIN

struct zip_head
{
    uint32_t head;
    uint16_t pk;
    uint16_t q;
    uint16_t type;
    uint16_t time;
    uint16_t date;
    uint32_t crc;
    uint32_t d_len;
    uint32_t len;
    uint16_t f_len;
    uint16_t e_len;
};

static std::shared_ptr<zip_head> read_zip_head(std::ifstream& ifs)
{
    auto head = std::make_shared<zip_head>();

    ifs.read(reinterpret_cast<char*>(&head->head),  sizeof(uint32_t));
    ifs.read(reinterpret_cast<char*>(&head->pk),    sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->q),     sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->type),  sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->time),  sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->date),  sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->crc),   sizeof(uint32_t));
    ifs.read(reinterpret_cast<char*>(&head->d_len), sizeof(uint32_t));
    ifs.read(reinterpret_cast<char*>(&head->len),   sizeof(uint32_t));
    ifs.read(reinterpret_cast<char*>(&head->f_len), sizeof(uint16_t));
    ifs.read(reinterpret_cast<char*>(&head->e_len), sizeof(uint16_t));

    return head;
}

jar_t::jar_t(std::vector<std::string>& jars) : m_jars(jars)
{
}

std::shared_ptr<class_file_t> jar_t::load_class_file_by_jars(const std::string& classname)
{
    for (auto jarn : m_jars) {
        auto p = load_class_file_by_jar(jarn, classname);
        if (p != nullptr) {
            return p;
        }
    }

    return nullptr;
}

bool jar_t::load_all_class_file_by_jars(void)
{
    for (auto jarn : m_jars) {
        if (!load_all_class_file_by_jar(jarn)) {
            return false;
        }
    }

    return true;
}

bool jar_t::load_all_class_file_by_jar(const std::string& jarname)
{
    std::ifstream ifs(jarname, std::ios::binary);

    if (!ifs.is_open()) {
        throw exception_t("No such file or directory.");
    }

    while (!ifs.eof()) {
        auto ziph = read_zip_head(ifs);

        if (ziph->f_len <= 0) {
            break;
        }

        std::string name(ziph->f_len, '\0');
        ifs.read(const_cast<char*>(name.c_str()), ziph->f_len);

        if (ziph->e_len > 0) {
            ifs.seekg(ziph->e_len, ifs.cur);
        }

        if (ziph->d_len == 0) {
            /* folder */

        } else if (ziph->d_len == ziph->len){
            if (name == "META-INF/MANIFEST.MF") {
                ifs.seekg(ziph->d_len, ifs.cur);
                continue;
            }

            std::vector<uint8_t> classfile(ziph->d_len);
            ifs.read(reinterpret_cast<char*>(&classfile[0]), ziph->d_len);
            
            m_cfs[name] = std::make_shared<class_file_t>(classfile);

        } else {
            if (name == "META-INF/MANIFEST.MF") {
                ifs.seekg(ziph->d_len, ifs.cur);
                continue;
            }

            const uint16_t s = 0x9C78;
            std::vector<uint8_t> buf(ziph->d_len+2);
            std::vector<uint8_t> classfile(ziph->len);

            std::memcpy(&buf[0], &s, sizeof(uint16_t));
            ifs.read(reinterpret_cast<char*>(&buf[2]), ziph->d_len);

            auto b_len = compressBound(ziph->len);
            uncompress(reinterpret_cast<Bytef*>(&classfile[0]), &b_len, reinterpret_cast<Bytef*>(&buf[0]), ziph->len);

            m_cfs[name] = std::make_shared<class_file_t>(classfile);
        }
    }
    
    ifs.close();
    return true;
}

std::shared_ptr<class_file_t> jar_t::load_class_file_by_jar(const std::string& jarname, const std::string& classname)
{
    std::ifstream ifs(jarname, std::ios::binary);

    if (!ifs.is_open()) {
        return nullptr;
    }

    while (!ifs.eof()) {
        auto ziph = read_zip_head(ifs);

        if (ziph->f_len <= 0) { 
            break;
        }

        std::string name(ziph->f_len, '\0');
        ifs.read(const_cast<char*>(name.c_str()), ziph->f_len);

        if (ziph->e_len > 0) {
            ifs.seekg(ziph->e_len, ifs.cur);
        }

        if (ziph->d_len == 0) {
            /* folder */

        } else if (ziph->d_len == ziph->len) {
            if (name == classname) {
                std::vector<uint8_t> classfile(ziph->d_len);
                ifs.read(reinterpret_cast<char*>(&classfile[0]), ziph->d_len);
                return std::make_shared<class_file_t>(classfile);
            }

            ifs.seekg(ziph->d_len, ifs.cur);

        } else {
            if (name == classname) {
                const uint16_t s = 0x9C78;
                std::vector<uint8_t> buf(ziph->d_len + 2);
                std::vector<uint8_t> classfile(ziph->len);

                std::memcpy(&buf[0], &s, sizeof(uint16_t));
                ifs.read(reinterpret_cast<char*>(&buf[2]), ziph->d_len);

                auto b_len = compressBound(ziph->len);
                uncompress(reinterpret_cast<Bytef*>(&classfile[0]), &b_len, reinterpret_cast<Bytef*>(&buf[0]), ziph->len);

                return std::make_shared<class_file_t>(classfile);
            }

            ifs.seekg(ziph->d_len, ifs.cur);
        }
    }

    ifs.close();

    return nullptr;
}

UJVM_NAMESPACE_END