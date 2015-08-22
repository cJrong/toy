

#include "class_file.h"
#include "utf8.h"
#include "reader.h"
#include "exception.h"

#include <memory>
#include <iostream>

UJVM_NAMESPACE_BEGIN

void class_file_t::parse(reader_t& reader)
{
    if (!(reader.read_uint32(m_magic) && (m_magic == MAGIC_TAG))) {
        throw exception_t("Check magic failure.");
    }

    if (!reader.read_uint16(m_minor_version) ||
        !reader.read_uint16(m_major_version)) {
        throw exception_t("Can't read the version number field.");
    }

    uint16_t constant_pool_count;
    if (!reader.read_uint16(constant_pool_count) ||
        !parse_constant_pool(reader, constant_pool_count)) {
        throw exception_t("Can't read the constant pool field.");
    }

    if (!reader.read_uint16(m_access_flags)) {
        throw exception_t("Can't read the access flags field.");
    }

    if (!reader.read_uint16(m_this_class)) {
        throw exception_t("Can't read the this class field.");
    }

    if (!reader.read_uint16(m_super_class)) {
        throw exception_t("Can't read the super class field.");
    }

    uint16_t interfaces_count;
    if (!reader.read_uint16(interfaces_count)) {
        throw exception_t("Can't read the interfaces count field.");
    }

    for (auto i = 0; i < interfaces_count; i++) {
        uint16_t interface;
        if (!reader.read_uint16(interface)) {
            throw exception_t("Can't read the interfaces field.");
        }
        m_interfaces.push_back(interface);
    }

    uint16_t fields_count;
    if (!reader.read_uint16(fields_count) ||
        !parse_fields(reader, fields_count)) {
        throw exception_t("Can't read the fields count filed.");
    }

    uint16_t methods_count;
    if (!reader.read_uint16(methods_count) ||
        !parse_methods(reader, methods_count)) {
        throw exception_t("Can't read the methods filed.");
    }

    uint16_t attributes_count;
    if (!reader.read_uint16(attributes_count) ||
        !parse_attributes(reader, m_attributes, attributes_count)) {
        throw exception_t("Can't read the attributes filed.");
    }

    if (reader.size() > 0) {
        throw exception_t("File check error");
    }
}

bool class_file_t::parse_constant_pool(reader_t &reader, uint16_t constant_pool_count)
{
#undef  BEGIN_CASE
#undef  END_CASE
#define BEGIN_CASE(value)  case value: {
#define END_CASE           break;}

    m_constant_pool.push_back(std::make_shared<constant_invalid_t>());

    for (auto i = 0; i < constant_pool_count - 1; i++) {
        std::shared_ptr<constant_t> constant;
        uint8_t tag;

        if (!reader.read_uint8(tag)) {
            return false;
        }

        switch (tag) {
        BEGIN_CASE(CONSTANT_CLASS_TAG)
            auto p = std::make_shared<constant_class_t>();
            constant = p;
            if (!reader.read_uint16(p->name_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_DOUBLE_TAG)
            auto p = std::make_shared<constant_double_t>();
            constant = p;
            if (!reader.read_uint64(p->bytes)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_FIELD_REF_TAG)
        auto p = std::make_shared<constant_field_ref_t>();
        constant = p;
        if (!reader.read_uint16(p->class_index) ||
            !reader.read_uint16(p->name_and_type_index)) {
            return false;
        }
        END_CASE

        BEGIN_CASE(CONSTANT_FLOAT_TAG)
            auto p = std::make_shared<constant_float_t>();
            constant = p;
            if (!reader.read_uint32(p->bytes)) {
                return false;
            }   
        END_CASE

        BEGIN_CASE(CONSTANT_INTEGER_TAG)
            auto p = std::make_shared<constant_integer_t>();
            constant = p;
            if (!reader.read_uint32(p->bytes)) {
                return false;
            }
            END_CASE

        BEGIN_CASE(CONSTANT_INTERFACE_METHOD_REF_TAG)
            auto p = std::make_shared<constant_interface_method_ref_t>();
            constant = p;
            if (!reader.read_uint16(p->class_index) ||
                !reader.read_uint16(p->name_and_type_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_INVOKE_DYNAMIC_TAG)
            auto p = std::make_shared<constant_invoke_dynamic_t>();
            constant = p;
            if (!reader.read_uint16(p->bootstrap_method_attr_index) ||
                !reader.read_uint16(p->name_and_type_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_LONG_TAG)
            auto p = std::make_shared<constant_long_t>();
            constant = p;
            if (!reader.read_uint64(p->bytes)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_METHOD_HANDLE_TAG)
            auto p = std::make_shared<constant_method_handle_t>();
            constant = p;
            if (!reader.read_uint8(p->reference_kind) ||
                !reader.read_uint16(p->reference_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_METHOD_REF_TAG)
            auto p = std::make_shared<constant_method_ref_t>();
            constant = p;
            if (!reader.read_uint16(p->class_index) ||
                !reader.read_uint16(p->name_and_type_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_METHOD_TYPE_TAG)
            auto p = std::make_shared<constant_method_type_t>();
            if (!reader.read_uint16(p->descriptor_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_NAME_AND_TYPE_TAG)
            auto p = std::make_shared<constant_name_and_type_t>();
            constant = p;
            if (!reader.read_uint16(p->name_index) ||
                !reader.read_uint16(p->descriptor_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_STRING_TAG)
            auto p = std::make_shared<constant_string_t>();
            constant = p;
            if (!reader.read_uint16(p->string_index)) {
                return false;
            }
        END_CASE

        BEGIN_CASE(CONSTANT_UTF8_TAG)
            auto p = std::make_shared<constant_utf8_t>();
            uint16_t length;
            constant = p;
            /* p->bytes can be removed */
            if (!reader.read_uint16(length) ||
                !reader.read_vector(length, p->bytes)) {
                return false;
            }

            if (length) {
                p->str = ujvm::wstrint_to_string(ujvm::utf8_to_wstring(p->bytes));
            }           

        END_CASE

        default:
            return false;
        }
        
        constant->tag = tag;
        m_constant_pool.push_back(constant);

        if (tag == CONSTANT_LONG_TAG || tag == CONSTANT_DOUBLE_TAG) {
            i++;
            m_constant_pool.push_back(std::make_shared<constant_invalid_t>());
        }
    }

#undef  BEGIN_CASE
#undef  END_CASE
    return true;
}

bool class_file_t::parse_fields(reader_t &reader, uint16_t fields_count)
{
    for (auto i = 0; i < fields_count; i++) {
        auto p = std::make_shared<field_info_t>();
        uint16_t attributes_count;

        if (!reader.read_uint16(p->access_flags) ||
            !reader.read_uint16(p->name_index) ||
            !reader.read_uint16(p->descriptor_index) ||
            !reader.read_uint16(attributes_count) ||
            !parse_attributes(reader, p->attributes, attributes_count)) {
            return false;
        }

        m_fields.push_back(p);
    }

    return true;
}

bool class_file_t::parse_methods(reader_t &reader, uint16_t methods_count)
{
    for (auto i = 0; i < methods_count; i++) {
        auto p = std::make_shared<method_info_t>();
        uint16_t attributes_count;

        if (!reader.read_uint16(p->access_flags) ||
            !reader.read_uint16(p->name_index) ||
            !reader.read_uint16(p->descriptor_index) ||
            !reader.read_uint16(attributes_count) ||
            !parse_attributes(reader, p->attributes, attributes_count)) {
            return false;
        }

        m_methods.push_back(p);
    }
    return true;
}

bool class_file_t::parse_attributes(reader_t &reader, class_file_t::attributes_t& attributes, uint16_t attributes_count)
{
    for (auto i = 0; i < attributes_count; i++) {
        auto attribute = parse_attribute(reader);
        if (!std::get<0>(attribute)) {
            return false;
        }
        attributes.push_back(std::get<1>(attribute));
    }
    return true;
}

std::tuple<bool, std::shared_ptr<attribute_t>> class_file_t::parse_attribute(reader_t &reader)
{
#undef  SWITCH
#undef  CASE
#undef  DEFAULT
#undef  RETURN_FALSE
#define SWITCH               if (false)
#define CASE(value)          } else if (utf8->str == value) {
#define DEFAULT              } else {
#define RETURN_FALSE         return std::make_tuple(false, nullptr)

    uint16_t attribute_name_index;
    uint32_t attribute_length;
    std::shared_ptr<attribute_t> attribute;

    if (!reader.read_uint16(attribute_name_index) ||
        !reader.read_uint32(attribute_length)) {
        RETURN_FALSE;
    }

    if (m_constant_pool[attribute_name_index]->tag != CONSTANT_UTF8_TAG) {
        RETURN_FALSE;
    }

    auto utf8 = std::static_pointer_cast<constant_utf8_t>(m_constant_pool[attribute_name_index]);
    
    SWITCH{
    CASE("ConstantValue")
        auto v = std::make_shared<attribute_constant_value_t>();
        attribute = v;

        if (!reader.read_uint16(v->constantvalue_index)) {
            RETURN_FALSE;
        }

    CASE("Code")
        auto v = std::make_shared<attribute_code_t>();
        attribute = v;

        uint32_t code_length;
        uint16_t exception_table_length;
        uint16_t attributes_count;

        if (!reader.read_uint16(v->max_stack) ||
            !reader.read_uint16(v->max_locals) ||
            !reader.read_uint32(code_length) ||
            !reader.read_vector(code_length, v->code) ||
            (code_length <= 0) ||
            !reader.read_uint16(exception_table_length)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < exception_table_length; i++) {
            auto exception = std::make_shared<attribute_code_t::exception_table_t>();

            if (!reader.read_uint16(exception->start_pc) ||
                !reader.read_uint16(exception->end_pc) ||
                !reader.read_uint16(exception->handler_pc) ||
                !reader.read_uint16(exception->catch_type)) {
                RETURN_FALSE;
            }

            v->exception_table.push_back(exception);
        }

        if (!reader.read_uint16(attributes_count) ||
            !parse_attributes(reader, v->attributes, attributes_count)) {
            RETURN_FALSE;
        }

    CASE("StackMapTable")
        auto v = std::make_shared<attribute_stack_map_table_t>();
        attribute = v;

        uint16_t number_of_entries;

        if (!reader.read_uint16(number_of_entries)) {
            RETURN_FALSE;
        }

        auto parse_verification_info = [&](std::vector<std::shared_ptr<attribute_stack_map_table_t::verification_t>> &v, uint16_t count)-> bool {
            for (auto i = 0; i < count; i++) {
                std::shared_ptr<attribute_stack_map_table_t::verification_t> vti;
                uint8_t tag;

                if (!reader.read_uint8(tag)) {
                    return false;
                }

                switch (tag) {
                case VERIFICATION_TOP_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_top_variable_t>();
                    break;

                case VERIFICATION_INTEGER_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_integer_variable_t>();
                    break;

                case VERIFICATION_FLOAT_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_float_variable_t>();
                    break;

                case VERIFICATION_DOUBLE_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_double_variable_t>();
                    break;

                case VERIFICATION_LONG_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_long_variable_t>();
                    break;

                case VERIFICATION_NULL_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_null_variable_t>();
                    break;

                case VERIFICATION_UNINITIALIZED_THIS_TAG:
                    vti = std::make_shared<attribute_stack_map_table_t::verification_uninitialized_this_variable_t>();
                    break;

                case VERIFICATION_OBJECT_TAG:
                    {
                        auto p = std::make_shared<attribute_stack_map_table_t::verification_object_variable_t>();
                        if (!reader.read_uint16(p->cpool_index)) {
                            return false;
                        }
                        vti = p;
                        break;
                    }
                    
                case VERIFICATION_UNINITIALIZED_VARIABLE_TAG:
                    {
                        auto p = std::make_shared<attribute_stack_map_table_t::verification_uninitialized_variable_t>();
                        if (!reader.read_uint16(p->offset)) {
                            return false;
                        }
                        vti = p;
                        break;
                    }

                default:
                    return false;
                }

                v.push_back(vti);
            }

            return true;
        };

        for (auto i = 0; i < number_of_entries; i++) {
            std::shared_ptr<attribute_stack_map_table_t::frame_t> frame;
            uint8_t frame_type;

            if (!reader.read_uint8(frame_type)) {
                RETURN_FALSE;
            }

            if (frame_type >= 0 && frame_type <= 63) {
                frame = std::make_shared<attribute_stack_map_table_t::same_frame_t>();

            } else if (frame_type >= 64 && frame_type <= 127) {
                auto p = std::make_shared<attribute_stack_map_table_t::same_locals_1_stack_item_frame_t>();

                if (!parse_verification_info(p->stack, 1)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else if (frame_type == 247) {
                auto p = std::make_shared<attribute_stack_map_table_t::same_locals_1_stack_item_frame_extended_t>();

                if (!reader.read_uint16(p->offset_delta) ||
                    !parse_verification_info(p->stack, 1)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else if (frame_type >= 248 && frame_type <= 250) {
                auto p = std::make_shared<attribute_stack_map_table_t::chop_frame_t>();

                if (!reader.read_uint16(p->offset_delta)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else if (frame_type == 251) {
                auto p = std::make_shared<attribute_stack_map_table_t::same_frame_extended_t>();

                if (!reader.read_uint16(p->offset_delta)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else if (frame_type >= 252 && frame_type <= 254) {
                auto p = std::make_shared<attribute_stack_map_table_t::append_frame_t>();

                if (!reader.read_uint16(p->offset_delta) ||
                    !parse_verification_info(p->locals, frame_type - 251)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else if (frame_type == 255) {
                auto p = std::make_shared<attribute_stack_map_table_t::full_frame_t>();
                uint16_t number_of_locals;
                uint16_t number_of_stack_items;

                if (!reader.read_uint16(p->offset_delta) ||
                    !reader.read_uint16(number_of_locals) ||
                    !parse_verification_info(p->locals, number_of_locals) ||
                    !reader.read_uint16(number_of_stack_items) ||
                    !parse_verification_info(p->stack, number_of_stack_items)) {
                    RETURN_FALSE;
                }

                frame = p;

            } else {
                RETURN_FALSE;
            }

            v->entries.push_back(frame);
        }

    CASE("Exceptions")
        auto v = std::make_shared<attribute_exceptions_t>();
        attribute = v;

        uint16_t number_of_exceptions;

        if (!reader.read_uint16(number_of_exceptions)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_of_exceptions; i++) {
            uint16_t exception;
            if (!reader.read_uint16(exception)) {
                RETURN_FALSE;
            }
            v->exception_index_table.push_back(exception);
        }

    CASE("InnerClasses")
        auto v = std::make_shared<attribute_inner_classes_t>();
        attribute = v;

        uint16_t number_of_classes;

        if (!reader.read_uint16(number_of_classes)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_of_classes; i++) {
            auto p = std::make_shared<attribute_inner_classes_t::classes_t>();

            if (!reader.read_uint16(p->inner_class_info_index) ||
                !reader.read_uint16(p->outer_class_info_index) ||
                !reader.read_uint16(p->inner_name_index) ||
                !reader.read_uint16(p->inner_class_access_flags)) {
                RETURN_FALSE;
            }

            v->classes.push_back(p);
        }

    CASE("EnclosingMethod")
        auto v = std::make_shared<attribute_enclosing_method_t>();
        attribute = v;

        if (!reader.read_uint16(v->class_index) ||
            !reader.read_uint16(v->method_index)) {
            RETURN_FALSE;
        }

    CASE("Synthetic")
        attribute = std::make_shared<attribute_synthetic_t>();

    CASE("Signature")
        auto v = std::make_shared<attribute_signature_t>();
        attribute = v;

        if (!reader.read_uint16(v->signature_index)) {
            RETURN_FALSE;
        }

    CASE("SourceFile")
        auto v = std::make_shared<attribute_source_file_t>();
        attribute = v;

        if (!reader.read_uint16(v->sourcefile_index)) {
            RETURN_FALSE;
        }

    CASE("SourceDebugExtension")
        auto v = std::make_shared<attribute_source_debug_extension_t>();
        attribute = v;

        for (uint32_t i = 0; i < attribute_length; i++) {
            uint8_t extension;

            if (!reader.read_uint8(extension)) {
                RETURN_FALSE;
            }

            v->debug_extension.push_back(extension);
        }

    CASE("LineNumberTable")
        auto v = std::make_shared<attribute_line_number_table_t>();
        attribute = v;

        uint16_t line_number_table_length;

        if (!reader.read_uint16(line_number_table_length)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < line_number_table_length; i++) {
            auto p = std::make_shared<attribute_line_number_table_t::line_number_table_t>();
            if (!reader.read_uint16(p->start_pc) ||
                !reader.read_uint16(p->line_number)) {
                RETURN_FALSE;
            }

            v->line_number_table.push_back(p);
        }

    CASE("LocalVariableTable")
        auto v = std::make_shared<attribute_local_variable_table_t>();
        attribute = v;

        uint16_t local_variable_table_length;

        if (!reader.read_uint16(local_variable_table_length)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < local_variable_table_length; i++) {
            auto p = std::make_shared<attribute_local_variable_table_t::local_variable_t>();

            if (!reader.read_uint16(p->start_pc) ||
                !reader.read_uint16(p->length) ||
                !reader.read_uint16(p->name_index) ||
                !reader.read_uint16(p->descriptor_index) ||
                !reader.read_uint16(p->index)) {
                RETURN_FALSE;
            }

            v->local_variable_table.push_back(p);
        }

    CASE("LocalVariableTypeTable")
        auto v = std::make_shared<attribute_local_variable_type_table_t>();
        attribute = v;

        uint16_t local_variable_type_table_length;

        if (!reader.read_uint16(local_variable_type_table_length)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < local_variable_type_table_length; i++) {
            auto p = std::make_shared<attribute_local_variable_type_table_t::local_variable_type_t>();

            if (!reader.read_uint16(p->start_pc) ||
                !reader.read_uint16(p->length) ||
                !reader.read_uint16(p->name_index) ||
                !reader.read_uint16(p->signature_index) ||
                !reader.read_uint16(p->index)) {
                RETURN_FALSE;
            }

            v->local_variable_type_table.push_back(p);
        }

    CASE("Deprecated")
        attribute = std::make_shared<attribute_deprecated_t>();

    CASE("RuntimeVisibleAnnotations")
        auto v = std::make_shared<attribute_runtime_visible_annotations_t>();
        attribute = v;

        uint16_t number_annotations;

        if (!reader.read_uint16(number_annotations)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_annotations; i++) {
            auto annotation = std::make_shared<annotation_t>();
            if (!parse_annotation(reader, *annotation)) {
                RETURN_FALSE;
            }
            v->annotations.push_back(annotation);
        }

    CASE("RuntimeInvisibleAnnotations")
        auto v = std::make_shared<attribute_runtime_invisible_annotations_t>();
        attribute = v;

        uint16_t number_annotations;

        if (!reader.read_uint16(number_annotations)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_annotations; i++) {
            auto annotation = std::make_shared<annotation_t>();
            if (!parse_annotation(reader, *annotation)) {
                RETURN_FALSE;
            }
            v->annotations.push_back(annotation);
        }

    CASE("RuntimeVisibleParameterAnnotations")
        auto v = std::make_shared<attribute_runtime_visible_parameter_annotations_t>();
        attribute = v;

        uint8_t number_parameters;

        if (!reader.read_uint8(number_parameters)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_parameters; i++) {
            uint16_t number_annotations;
            auto parameter = std::make_shared<attribute_runtime_visible_parameter_annotations_t::parameter_annotations_t>();

            if (!reader.read_uint16(number_annotations)) {
                RETURN_FALSE;
            }

            for (auto i = 0; i < number_annotations; i++) {
                auto annotation = std::make_shared<annotation_t>();
                if (!parse_annotation(reader, *annotation)) {
                    RETURN_FALSE;
                }
                parameter->annotations.push_back(annotation);
            }

            v->parameter_annotations.push_back(parameter);
        }

    CASE("RuntimeInvisibleParameterAnnotations")
        auto v = std::make_shared<attribute_runtime_invisible_parameter_annotations_t>();
        attribute = v;

        uint8_t number_parameters;

        if (!reader.read_uint8(number_parameters)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_parameters; i++) {
            uint16_t number_annotations;
            auto parameter = std::make_shared<attribute_runtime_invisible_parameter_annotations_t::parameter_annotations_t>();

            if (!reader.read_uint16(number_annotations)) {
                RETURN_FALSE;
            }

            for (auto i = 0; i < number_annotations; i++) {
                auto annotation = std::make_shared<annotation_t>();
                if (!parse_annotation(reader, *annotation)) {
                    RETURN_FALSE;
                }
                parameter->annotations.push_back(annotation);
            }

            v->parameter_annotations.push_back(parameter);
        }

    CASE("AnnotationDefault")
        auto v = std::make_shared<attribute_annotation_default_t>();
        attribute = v;

        if (!parse_element_value(reader, v->default_value)) {
            RETURN_FALSE;
        }

    CASE("BootstrapMethods")
        auto v = std::make_shared<attribute_bootstrap_methods_t>();
        attribute = v;

        uint16_t number_bootstrap_methods;

        if (!reader.read_uint16(number_bootstrap_methods)) {
            RETURN_FALSE;
        }

        for (auto i = 0; i < number_bootstrap_methods; i++) {
            uint16_t number_bootstrap_arguments;
            auto bootstrap_method = std::make_shared<attribute_bootstrap_methods_t::bootstrap_methods_t>();

            if (!reader.read_uint16(bootstrap_method->bootstrap_method_ref) ||
                !reader.read_uint16(number_bootstrap_arguments)) {
                RETURN_FALSE;
            }

            for (auto i = 0; i < number_bootstrap_arguments; i++) {
                uint16_t bootstrap_argument;

                if (!reader.read_uint16(bootstrap_argument)) {
                    RETURN_FALSE;
                }

                bootstrap_method->bootstrap_arguments.push_back(bootstrap_argument);
            }

            v->bootstrap_methods.push_back(bootstrap_method);
        }

        DEFAULT
            RETURN_FALSE;
    }


    attribute->attribute_name_index = attribute_name_index;
    attribute->attribute_length = attribute_length;

#undef  SWITCH
#undef  CASE
#undef  DEFAULT
#undef  RETURN_FALSE
    return std::make_tuple(true, attribute);
}

bool class_file_t::parse_annotation(reader_t &reader, annotation_t &annotation)
{
    uint16_t number_element_value_pairs;

    if (!reader.read_uint16(annotation.type_index) ||
        !reader.read_uint16(number_element_value_pairs)) {
        return true;
    }

    for (auto i = 0; i < number_element_value_pairs; i++) {
        auto v = std::make_shared<annotation_t::element_value_pairs_t>();
        if (!reader.read_uint16(v->element_name_index) ||
            !parse_element_value(reader, v->value)) {
            return false;
        }
        annotation.element_value_pairs.push_back(v);
    }

    return true;
}

bool class_file_t::parse_element_value(reader_t &reader, element_value_t &element_value)
{
    if (!reader.read_uint8(element_value.tag)) {
        return true;
    }

    switch (element_value.tag) {
    case 'B': case 'C':
    case 'D': case 'F':
    case 'I': case 'J':
    case 'S': case 'Z':
    case 's':
        {
            uint16_t const_value_index;
            if (!reader.read_uint16(const_value_index)) {
                return false;
            }
            element_value.value = const_value_index;
            break;
        }
    case 'e':
        {
            auto enum_const_value = std::make_shared<element_value_t::enum_const_value_t>();
            if (!reader.read_uint16(enum_const_value->type_name_index) ||
                !reader.read_uint16(enum_const_value->const_name_index)) {
                return false;
            }
            element_value.value = enum_const_value;
            break;
        }
    case 'c':
        {
            uint16_t class_info_index;
            if (!reader.read_uint16(class_info_index)) {
                return false;
            }
            element_value.value = class_info_index;
            break;
        }
    case '@':
        {
            auto annotation = std::make_shared<annotation_t>();
            if (!parse_annotation(reader, *annotation)) {
                return false;
            }
            element_value.value = annotation;
            break;
        }
    case '[':
        {
            uint16_t number_of_values;
            if (!reader.read_uint16(number_of_values)) {
                return false;
            }
            std::vector<std::shared_ptr<element_value_t>> array_value;
            for (auto i = 0; i < number_of_values; i++) {
                auto v = std::make_shared<element_value_t>();
                if (!parse_element_value(reader, *v)) {
                    return false;
                }
                array_value.push_back(v);
            }
            element_value.value = std::move(array_value);
            break;
        }
    }

    return true;
}

UJVM_NAMESPACE_END
