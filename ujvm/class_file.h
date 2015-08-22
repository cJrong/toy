

#ifndef _FILE_H_
#define _FILE_H_

#include "config.h"
#include "options.h"
#include "reader.h"
#include "any.h"

#include <tuple>
#include <vector>
#include <memory>
#include <cstdint>

UJVM_NAMESPACE_BEGIN

const uint32_t MAGIC_TAG                            = 0xCAFEBABE;

const uint32_t CONSTANT_CLASS_TAG                   = 7;
const uint32_t CONSTANT_FIELD_REF_TAG               = 9;
const uint32_t CONSTANT_METHOD_REF_TAG              = 10;
const uint32_t CONSTANT_INTERFACE_METHOD_REF_TAG    = 11;
const uint32_t CONSTANT_STRING_TAG                  = 8;
const uint32_t CONSTANT_INTEGER_TAG                 = 3;
const uint32_t CONSTANT_FLOAT_TAG                   = 4;
const uint32_t CONSTANT_LONG_TAG                    = 5;
const uint32_t CONSTANT_DOUBLE_TAG                  = 6;
const uint32_t CONSTANT_NAME_AND_TYPE_TAG           = 12;
const uint32_t CONSTANT_UTF8_TAG                    = 1;
const uint32_t CONSTANT_METHOD_HANDLE_TAG           = 15;
const uint32_t CONSTANT_METHOD_TYPE_TAG             = 16;
const uint32_t CONSTANT_INVOKE_DYNAMIC_TAG          = 18;
const uint32_t CONSTANT_INVALID_TAG                 = 0xff;

const uint8_t ATTRIBUTE_CONSTANT_VALUE_TAG                          = 0x81;
const uint8_t ATTRIBUTE_CODE_TAG                                    = 0x82;
const uint8_t ATTRIBUTE_STACK_MAP_TABLE_TAG                         = 0x83;
const uint8_t ATTRIBUTE_EXCEPTIONS_TAG                              = 0x84;
const uint8_t ATTRIBUTE_INNER_CLASSES_TAG                           = 0x85;
const uint8_t ATTRIBUTE_ENCLOSING_METHOD_TAG                        = 0x86;
const uint8_t ATTRIBUTE_SYNTHETIC_TAG                               = 0x87;
const uint8_t ATTRIBUTE_SIGNATURE_TAG                               = 0x88;
const uint8_t ATTRIBUTE_SOURCE_FILE_TAG                             = 0x89;
const uint8_t ATTRIBUTE_SOURCE_DEBUG_EXTENSION_TAG                  = 0x8A;
const uint8_t ATTRIBUTE_LINE_NUMBER_TABLE_TAG                       = 0x8B;
const uint8_t ATTRIBUTE_LOCAL_VARIABLE_TABLE_TAG                    = 0x8C;
const uint8_t ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE_TAG               = 0x8D;
const uint8_t ATTRIBUTE_DEPRECATED_TAG                              = 0x8E;
const uint8_t ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS_TAG             = 0x8F;
const uint8_t ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS_TAG           = 0x91;
const uint8_t ATTRIBUTE_RUNTIME_VISIABLE_PARAMETER_ANNOTATIONS_TAG  = 0x92;
const uint8_t ATTRIBUTE_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS_TAG = 0x93;
const uint8_t ATTRIBUTE_ANNOTATION_DEFAULT_TAG                      = 0x94;
const uint8_t ATTRIBUTE_BOOTSTRAP_METHODS_TAG                       = 0x95;

const uint8_t STACK_MAP_FRAME_SAME_FRAME_TAG                         = 0x01;
const uint8_t STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_TAG          = 0x02;
const uint8_t STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED_TAG = 0x03;
const uint8_t STACK_MAP_FRAME_CHOP_FRAME_TAG                         = 0x04;
const uint8_t STACK_MAP_FRAME_SAME_FRAME_EXTENDED_TAG                = 0x05;
const uint8_t STACK_MAP_FRAME_APPEND_FRAME_TAG                       = 0x06;
const uint8_t STACK_MAP_FRAME_FULL_FRAME_TAG                         = 0x07;

const uint8_t VERIFICATION_TOP_TAG                          = 0x00;
const uint8_t VERIFICATION_INTEGER_TAG                      = 0x01;
const uint8_t VERIFICATION_FLOAT_TAG                        = 0x02;
const uint8_t VERIFICATION_DOUBLE_TAG                       = 0x03;
const uint8_t VERIFICATION_LONG_TAG                         = 0x04;
const uint8_t VERIFICATION_NULL_TAG                         = 0x05;
const uint8_t VERIFICATION_UNINITIALIZED_THIS_TAG           = 0x06;
const uint8_t VERIFICATION_OBJECT_TAG                       = 0x07;
const uint8_t VERIFICATION_UNINITIALIZED_VARIABLE_TAG       = 0x08;

struct constant_t {
    uint8_t tag;
};

struct constant_class_t : public constant_t {
    uint16_t name_index;
};

struct constant_field_ref_t : public constant_t {
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct constant_method_ref_t : public constant_t {
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct constant_interface_method_ref_t : public constant_t {
    uint16_t class_index;
    uint16_t name_and_type_index;
};

struct constant_string_t : public constant_t {
    uint16_t string_index;
};

struct constant_integer_t : public constant_t {
    uint32_t bytes;
};

struct constant_float_t : public constant_t {
    uint32_t bytes;
};

struct constant_long_t : public constant_t {
    uint64_t bytes;
};

struct constant_double_t : public constant_t {
    uint64_t bytes;
};

struct constant_name_and_type_t : public constant_t {
    uint16_t name_index;
    uint16_t descriptor_index;
};

struct constant_utf8_t : public constant_t {
    /* bytes can be removed */
    std::vector<uint8_t> bytes;
    std::string          str;
};

struct constant_method_handle_t : public constant_t {
    uint8_t  reference_kind;
    uint16_t reference_index;
};

struct constant_method_type_t : public constant_t {
    uint16_t descriptor_index;
};

struct constant_invoke_dynamic_t : public constant_t {
    uint16_t bootstrap_method_attr_index;
    uint16_t name_and_type_index;
};

struct constant_invalid_t : public constant_t {
    constant_invalid_t() {
        tag = CONSTANT_INVALID_TAG;
    }
};

struct attribute_t {
    attribute_t(uint8_t tag, std::string name) : 
        attribute_name_index(tag), attribute_name(name)
    {
    }

    uint16_t    attribute_name_index;
    uint32_t    attribute_length;
    std::string attribute_name;
};

struct attribute_constant_value_t : public attribute_t {
    attribute_constant_value_t() : 
        attribute_t(ATTRIBUTE_CONSTANT_VALUE_TAG, "ConstantValue") 
    {
    }

    uint16_t constantvalue_index;
};

struct attribute_code_t : public attribute_t {
    attribute_code_t() : 
        attribute_t(ATTRIBUTE_CODE_TAG, "Code") 
    {
    }

    struct exception_table_t {
        uint16_t start_pc;
        uint16_t end_pc;
        uint16_t handler_pc;
        uint16_t catch_type;
    };

    uint16_t max_stack;
    uint16_t max_locals;
    std::vector<uint8_t> code;
    std::vector<std::shared_ptr<attribute_t>> attributes;
    std::vector<std::shared_ptr<exception_table_t>> exception_table;
};

struct attribute_stack_map_table_t : public attribute_t {
    attribute_stack_map_table_t() : 
        attribute_t(ATTRIBUTE_STACK_MAP_TABLE_TAG, "StackMapTable") 
    {
    }

    struct verification_t {
        verification_t(uint8_t t) : 
            tag(t) 
        {
        }
        uint8_t tag;
    };

    struct verification_top_variable_t : public verification_t {
        verification_top_variable_t() : 
            verification_t(VERIFICATION_TOP_TAG) 
        {
        }
    };

    struct verification_integer_variable_t : public verification_t {
        verification_integer_variable_t() :
            verification_t(VERIFICATION_INTEGER_TAG)
        {
        }
    };

    struct verification_float_variable_t : public verification_t {
        verification_float_variable_t() : 
            verification_t(VERIFICATION_FLOAT_TAG)
        {
        }
    };

    struct verification_null_variable_t : public verification_t {
        verification_null_variable_t() : 
            verification_t(VERIFICATION_NULL_TAG)
        {
        }
    };

    struct verification_uninitialized_this_variable_t : public verification_t {
        verification_uninitialized_this_variable_t() : 
            verification_t(VERIFICATION_UNINITIALIZED_THIS_TAG)
        {
        }
    };

    struct verification_object_variable_t : public verification_t {
        verification_object_variable_t() : 
            verification_t(VERIFICATION_OBJECT_TAG) 
        {
        }
        uint16_t cpool_index;
    };

    struct verification_uninitialized_variable_t : public verification_t {
        verification_uninitialized_variable_t() : 
            verification_t(VERIFICATION_UNINITIALIZED_VARIABLE_TAG) 
        {
        }
        uint16_t offset;
    };

    struct verification_long_variable_t : public verification_t {
        verification_long_variable_t() : 
            verification_t(VERIFICATION_LONG_TAG) 
        {
        }
    };

    struct verification_double_variable_t : public verification_t {
        verification_double_variable_t() : 
            verification_t(VERIFICATION_DOUBLE_TAG) 
        {
        }
    };

    struct frame_t {
        frame_t(uint8_t tag) :
            frame_type(tag)
        {
        }
        uint8_t frame_type;
    };

    struct same_frame_t : public frame_t {
        same_frame_t() : 
            frame_t(STACK_MAP_FRAME_SAME_FRAME_TAG) 
        {
        }
    };

    struct same_locals_1_stack_item_frame_t : public frame_t {
        same_locals_1_stack_item_frame_t() : 
            frame_t(STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_TAG) 
        {
        }
        std::vector<std::shared_ptr<verification_t>> stack;
    };

    struct same_locals_1_stack_item_frame_extended_t : public frame_t {
        same_locals_1_stack_item_frame_extended_t() : 
            frame_t(STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED_TAG) 
        {
        }
        uint16_t offset_delta;
        std::vector<std::shared_ptr<verification_t>> stack;
    };

    struct chop_frame_t : public frame_t {
        chop_frame_t() : 
            frame_t(STACK_MAP_FRAME_CHOP_FRAME_TAG) 
        {
        }
        uint16_t offset_delta;
    };

    struct same_frame_extended_t : public frame_t {
        same_frame_extended_t() : 
            frame_t(STACK_MAP_FRAME_SAME_FRAME_EXTENDED_TAG) 
        {
        }
        uint16_t offset_delta;
    };

    struct append_frame_t : public frame_t {
        append_frame_t() : 
            frame_t(STACK_MAP_FRAME_APPEND_FRAME_TAG) 
        {
        }
        uint16_t offset_delta;
        std::vector<std::shared_ptr<verification_t>> locals;
    };

    struct full_frame_t : public frame_t {
        full_frame_t() : frame_t(STACK_MAP_FRAME_FULL_FRAME_TAG) 
        {
        }
        uint16_t offset_delta;
        std::vector<std::shared_ptr<verification_t>> locals;
        std::vector<std::shared_ptr<verification_t>> stack;
    };

    std::vector<std::shared_ptr<frame_t>> entries;
};

struct attribute_exceptions_t : public attribute_t {
    attribute_exceptions_t() : 
        attribute_t(ATTRIBUTE_EXCEPTIONS_TAG, "Exceptions")
    {
    }
    std::vector<uint16_t> exception_index_table;
};

struct attribute_inner_classes_t : public attribute_t {
    attribute_inner_classes_t() : 
        attribute_t(ATTRIBUTE_INNER_CLASSES_TAG, "InnerClasses")
    {
    }

    struct classes_t {
        uint16_t inner_class_info_index;
        uint16_t outer_class_info_index;
        uint16_t inner_name_index;
        uint16_t inner_class_access_flags;
    };

    std::vector<std::shared_ptr<classes_t>> classes;
};

struct attribute_enclosing_method_t : public attribute_t {
    attribute_enclosing_method_t() :
        attribute_t(ATTRIBUTE_ENCLOSING_METHOD_TAG, "EnclosingMethod")
    {
    }

    uint16_t class_index;
    uint16_t method_index;
};

struct attribute_synthetic_t : public attribute_t {
    attribute_synthetic_t() : 
        attribute_t(ATTRIBUTE_SYNTHETIC_TAG, "Synthetic")
    {
    }
};

struct attribute_signature_t : public attribute_t {
    attribute_signature_t() : 
        attribute_t(ATTRIBUTE_SIGNATURE_TAG, "Signature")
    {
    }
    uint16_t signature_index;
};

struct attribute_source_file_t : public attribute_t {
    attribute_source_file_t() : 
        attribute_t(ATTRIBUTE_SOURCE_FILE_TAG, "SourceFile")
    {
    }

    uint16_t sourcefile_index;
};

struct attribute_source_debug_extension_t : public attribute_t {
public:
    attribute_source_debug_extension_t() : 
        attribute_t(ATTRIBUTE_SOURCE_DEBUG_EXTENSION_TAG, "SourceDebugExtension")
    {
    }

    std::vector<uint8_t> debug_extension;
};

struct attribute_line_number_table_t : public attribute_t {
    attribute_line_number_table_t() : 
        attribute_t(ATTRIBUTE_LINE_NUMBER_TABLE_TAG, "LineNumberTable")
    {
    }

    struct line_number_table_t {
        uint16_t start_pc;
        uint16_t line_number;
    };

    std::vector<std::shared_ptr<line_number_table_t>> line_number_table;
};


struct attribute_local_variable_table_t : public attribute_t {
    attribute_local_variable_table_t() : 
        attribute_t(ATTRIBUTE_LOCAL_VARIABLE_TABLE_TAG, "LocalVariableTable")
    {
    }

    struct local_variable_t {
        uint16_t start_pc;
        uint16_t length;
        uint16_t name_index;
        uint16_t descriptor_index;
        uint16_t index;
    };

    std::vector<std::shared_ptr<local_variable_t>> local_variable_table;
};

struct attribute_local_variable_type_table_t : public attribute_t {
    attribute_local_variable_type_table_t() : 
        attribute_t(ATTRIBUTE_LOCAL_VARIABLE_TYPE_TABLE_TAG, "LocalVariableTypeTable")
    {
    }

    struct local_variable_type_t {
        uint16_t start_pc;
        uint16_t length;
        uint16_t name_index;
        uint16_t signature_index;
        uint16_t index;
    };

    std::vector<std::shared_ptr<local_variable_type_t>> local_variable_type_table;
};

struct attribute_deprecated_t : public attribute_t {
    attribute_deprecated_t() : 
        attribute_t(ATTRIBUTE_DEPRECATED_TAG, "Deprecated")
    {
    }
};

struct element_value_t {
    struct enum_const_value_t {
        uint16_t type_name_index;
        uint16_t const_name_index;
    };

    struct array_value_t {
        std::vector<std::shared_ptr<element_value_t>> values;
    };

    uint8_t tag;
    any_t value;
};

struct annotation_t {
    struct element_value_pairs_t {
        uint16_t element_name_index;
        element_value_t value;
    };

    uint16_t type_index;
    std::vector<std::shared_ptr<element_value_pairs_t>> element_value_pairs;
};

struct attribute_runtime_visible_annotations_t : public attribute_t {
    attribute_runtime_visible_annotations_t() :
        attribute_t(ATTRIBUTE_RUNTIME_VISIBLE_ANNOTATIONS_TAG, "RuntimeVisibleAnnotations")
    {
    }

    std::vector<std::shared_ptr<annotation_t>> annotations;
};

struct attribute_runtime_invisible_annotations_t : public attribute_t {
    attribute_runtime_invisible_annotations_t() : 
        attribute_t(ATTRIBUTE_RUNTIME_INVISIBLE_ANNOTATIONS_TAG, "RuntimeInvisibleAnnotations")
    {
    }

    std::vector<std::shared_ptr<annotation_t>> annotations;
};

struct attribute_runtime_visible_parameter_annotations_t : public attribute_t {
    attribute_runtime_visible_parameter_annotations_t() :
        attribute_t(ATTRIBUTE_RUNTIME_VISIABLE_PARAMETER_ANNOTATIONS_TAG, "RuntimeVisibleParameterAnnotations")
    {

    }

    struct parameter_annotations_t {
        std::vector<std::shared_ptr<annotation_t>> annotations;
    };
    std::vector<std::shared_ptr<parameter_annotations_t>> parameter_annotations;
};

struct attribute_runtime_invisible_parameter_annotations_t : public attribute_t {
    attribute_runtime_invisible_parameter_annotations_t() :
        attribute_t(ATTRIBUTE_RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS_TAG, "RuntimeInvisibleParameterAnnotations")
    {
    }

    struct parameter_annotations_t {
        std::vector<std::shared_ptr<annotation_t>> annotations;
    };
    std::vector<std::shared_ptr<parameter_annotations_t>> parameter_annotations;
};

struct attribute_annotation_default_t : public attribute_t {
    attribute_annotation_default_t() : 
        attribute_t(ATTRIBUTE_ANNOTATION_DEFAULT_TAG, "AnnotationDefault")
    {
    }

    element_value_t default_value;
};

struct attribute_bootstrap_methods_t : public attribute_t {
    attribute_bootstrap_methods_t() :
        attribute_t(ATTRIBUTE_BOOTSTRAP_METHODS_TAG, "BootstrapMethods")
    {
    }

    struct bootstrap_methods_t {
        uint16_t bootstrap_method_ref;
        std::vector<uint16_t> bootstrap_arguments;
    };

    std::vector<std::shared_ptr<bootstrap_methods_t>> bootstrap_methods;
};

struct field_info_t {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    std::vector<std::shared_ptr<attribute_t>> attributes;
};

struct method_info_t {
    uint16_t access_flags;
    uint16_t name_index;
    uint16_t descriptor_index;
    std::vector<std::shared_ptr<attribute_t>> attributes;
};

class class_file_t {
public:
    class_file_t(std::string& file)
    {
        parse(file);
    }

    class_file_t(std::vector<uint8_t> &v)
    {
        parse(v);
    }

    class_file_t(options_t& options)
    {
        parse(options.classfile);
    }
 
    typedef std::vector<uint16_t>                       interfaces_t;
    typedef std::vector<std::shared_ptr<field_info_t>>  fields_t;
    typedef std::vector<std::shared_ptr<method_info_t>> methods_t;
    typedef std::vector<std::shared_ptr<constant_t>>    constant_pool_t;
    typedef std::vector<std::shared_ptr<attribute_t>>   attributes_t;

    inline uint32_t magic(void)
    {
        return m_magic;
    }

    inline uint16_t this_class(void)
    {
        return m_this_class;
    }

    inline uint16_t super_class(void)
    {
        return m_super_class;
    }

    inline uint16_t access_flags(void)
    {
        return m_access_flags;
    }

    inline std::tuple<uint16_t, uint16_t> version(void) {
        return std::make_tuple(m_major_version, m_minor_version);
    }

    interfaces_t& interfaces(void) 
    {
        return m_interfaces;
    }

    constant_pool_t& constant_pool(void)
    {
        return m_constant_pool;
    }

    fields_t& fields(void)
    {
        return m_fields;
    }

    methods_t& methods(void)
    {
        return m_methods;
    }

    attributes_t& attributes(void)
    {
        return m_attributes;
    }

private:
    inline void parse(std::string classfile)
    {
        reader_t reader(classfile);
        parse(reader);
    }

    inline void parse(std::vector<uint8_t>& v)
    {
        reader_t reader(v);
        parse(reader);
    }

    void parse(reader_t& reader);
    bool parse_constant_pool(reader_t &reader, uint16_t constant_pool_count);
    bool parse_fields(reader_t &reader, uint16_t fields_count);
    bool parse_methods(reader_t &reader, uint16_t methods_count);
    bool parse_attributes(reader_t &reader, class_file_t::attributes_t& attributes, uint16_t attributes_count);
    std::tuple<bool, std::shared_ptr<attribute_t>> parse_attribute(reader_t &reader);
    bool parse_annotation(reader_t &reader, annotation_t &annotation);
    bool parse_element_value(reader_t &reader, element_value_t &element_value);

private:
    uint32_t m_magic;
    uint16_t m_this_class;
    uint16_t m_super_class;
    uint16_t m_access_flags;
    uint16_t m_minor_version;
    uint16_t m_major_version;
    
    interfaces_t    m_interfaces;
    constant_pool_t m_constant_pool;
    fields_t        m_fields;
    methods_t       m_methods;
    attributes_t    m_attributes;
};

UJVM_NAMESPACE_END

#endif
