

#ifndef _CLASS_H_
#define _CLASS_H_

#include "config.h"
#include "class_file.h"

#include <string>
#include <memory>
#include <cstdint>

UJVM_NAMESPACE_BEGIN

class jvm_t;

class class_t {
public:
    class attribute_t {
    public:
        std::string source_file_name;
        std::string signature;
    };

    struct field_t {
        uint16_t access_flags;
        std::string name;
        std::string descriptor;
    };

    struct method_t {
        enum access_flags_e {
            ACC_PUBLIC_TAG = 0x0001,
            ACC_PRIVATE_TAG = 0x0002,
            ACC_PROTECTED_TAG = 0x0004,
            ACC_STATIC_TAG = 0x0008,
            ACC_FINAL_TAG = 0x0010,
            ACC_SYNCHRONIZED_TAG = 0x0020,
            ACC_BRIDGE_TAG = 0x0040,
            ACC_VARARGS_TAG = 0x0080,
            ACC_NATIVE_TAG = 0x0100,
            ACC_ABSTRACT_TAG = 0x0200,
            ACC_STRICT_TAG = 0x0800,
            ACC_SYNTHETIC_TAG = 0x1000,
        };
        uint16_t access_flags;
        std::string name;
        std::string descriptor;
        int max_stack;
        int max_locals;

        std::vector<uint8_t> code;
    };

    typedef class_file_t::constant_pool_t constant_pool_t;
    typedef std::vector<std::string> interfaces_t;
    typedef std::vector<std::shared_ptr<field_t>> fields_t;
    typedef std::vector<std::shared_ptr<method_t>> methods_t;
public:
    class_t(jvm_t* jvm, class_file_t& file);

    std::shared_ptr<method_t> get_method(std::string name, std::string descriptor, uint16_t acc);

    
    uint16_t        access_flags;
    std::string     class_name;
    std::string     super_class_name;
    std::shared_ptr<class_t> super_class;
    interfaces_t    interfaces;
    fields_t        fields;
    methods_t       methods;
    constant_pool_t constant_pool;
};

UJVM_NAMESPACE_END

#endif