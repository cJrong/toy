

#include <iterator>
#include <iostream>
#include <iomanip>
#include <clocale>
#include <stdio.h>
#include <string>

#include "class_file.h"
#include "utf8.h"

static void print_constant_pool(ujvm::class_file_t::constant_pool_t &cp);
static void print_interfaces(ujvm::class_file_t::interfaces_t& interfaces, ujvm::class_file_t::constant_pool_t& cp);
static void print_fields(ujvm::class_file_t::fields_t& fields, ujvm::class_file_t::constant_pool_t& cp);
static void print_methods(ujvm::class_file_t::methods_t& methods, ujvm::class_file_t::constant_pool_t& cp);
static void print_attributes(int nos, ujvm::class_file_t::attributes_t& attributes, ujvm::class_file_t::constant_pool_t& cp);

void dump_class_file(std::string& classfile)
{
    try {
        ujvm::class_file_t file(classfile);
        std::cout << "magic:                  0x" << std::hex << std::uppercase << file.magic() << std::dec << std::endl;
        std::cout << "minor_version:          "   << std::get<1>(file.version())  << std::endl;
        std::cout << "major_version:          "   << std::get<0>(file.version())  << std::endl;
        std::cout << "constant_pool_count:    "   << file.constant_pool(). size() << std::endl; 
        print_constant_pool(file.constant_pool());
        std::cout << "access_flags:           0x" << std::hex << std::uppercase << file.access_flags() << std::dec << std::endl;
        std::cout << "this_class:             "   << file.this_class()        << std::endl;
        std::cout << "super_class:            "   << file.super_class()       <<  std::endl;
        std::cout << "interfaces_count:       "   << file.interfaces().size() << std::endl;
        print_interfaces(file.interfaces(),     file.constant_pool());
        std::cout << "fields_count:           "   << file.fields().size() << std::endl;
        print_fields(file.fields(),             file.constant_pool());
        std::cout << "methods_count:          "   << file.methods().size() << std::endl;
        print_methods(file.methods(),           file.constant_pool());
        std::cout << "attributes_count:       "   << file.attributes().size() << std::endl;
        print_attributes(0, file.attributes(),  file.constant_pool());
    } catch (ujvm::exception_t& ex) {
        std::cout << ex.what() << std::endl;
    }
}

void dump_class_file(ujvm::class_file_t& file)
{
    try {
        std::cout << "magic:                  0x" << std::hex << std::uppercase << file.magic() << std::dec << std::endl;
        std::cout << "minor_version:          " << std::get<1>(file.version()) << std::endl;
        std::cout << "major_version:          " << std::get<0>(file.version()) << std::endl;
        std::cout << "constant_pool_count:    " << file.constant_pool().size() << std::endl;
        print_constant_pool(file.constant_pool());
        std::cout << "access_flags:           0x" << std::hex << std::uppercase << file.access_flags() << std::dec << std::endl;
        std::cout << "this_class:             " << file.this_class() << std::endl;
        std::cout << "super_class:            " << file.super_class() << std::endl;
        std::cout << "interfaces_count:       " << file.interfaces().size() << std::endl;
        print_interfaces(file.interfaces(), file.constant_pool());
        std::cout << "fields_count:           " << file.fields().size() << std::endl;
        print_fields(file.fields(), file.constant_pool());
        std::cout << "methods_count:          " << file.methods().size() << std::endl;
        print_methods(file.methods(), file.constant_pool());
        std::cout << "attributes_count:       " << file.attributes().size() << std::endl;
        print_attributes(0, file.attributes(), file.constant_pool());
    } catch (ujvm::exception_t& ex) {
        std::cout << ex.what() << std::endl;
    }
}

static void print_constant_pool(ujvm::class_file_t::constant_pool_t &cp)
{
    int i = 0;
    for (auto constant : cp) {
        switch (constant->tag) {
        case ujvm::CONSTANT_CLASS_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_class_t>(constant);
                std::cout << "constant_class[" << i << "]:" << std::endl;
                std::cout << "  name_index:  " << p->name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[p->name_index])->str << ")" << std::endl;
                break;
            }
        case ujvm::CONSTANT_FIELD_REF_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_field_ref_t>(constant);
                std::cout << "constant_field_ref[" << i << "]:" << std::endl;
                std::cout << "  class_index:         " << p->class_index << std::endl;
                std::cout << "  name_and_type_index: " << p->name_and_type_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_METHOD_REF_TAG: 
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_ref_t>(constant);
                std::cout << "constant_method_ref[" << i << "]:" << std::endl;
                std::cout << "  class_index:         " << p->class_index << std::endl;
                std::cout << "  name_and_type_index: " << p->name_and_type_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_INTERFACE_METHOD_REF_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_interface_method_ref_t>(constant);
                std::cout << "interface_method_ref[" << i << "]:" << std::endl;
                std::cout << "  class_index:         " << p->class_index << std::endl;
                std::cout << "  name_and_type_index: " << p->name_and_type_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_STRING_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_string_t>(constant);
                std::cout << "constant_string[" << i << "]:" << std::endl;
                std::cout << "  string_index:  " << p->string_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_INTEGER_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_integer_t>(constant);
                std::cout << "constant_integer[" << i << "]:" << std::endl;
                std::cout << "  bytes:  " << p->bytes << "(0x" << std::hex << std::uppercase << p->bytes << ")" << std::dec << std::endl;
                break;
            }
        case ujvm::CONSTANT_FLOAT_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_float_t>(constant);
                std::cout << "constant_float[" << i << "]:" << std::endl;
                std::cout << "  bytes:  " << *(float*)&p->bytes << "(0x" << std::hex << std::uppercase << p->bytes << ")" << std::dec << std::endl;
                break;
            }
        case ujvm::CONSTANT_LONG_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_long_t>(constant);
                std::cout << "constant_long[" << i << "]:" << std::endl;
                std::cout << "  bytes:  " << p->bytes << "(0x" << std::hex << std::uppercase << p->bytes << ")" << std::dec << std::endl;
                break;
            }
        case ujvm::CONSTANT_DOUBLE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_double_t>(constant);
                std::cout << "constant_double[" << i << "]:" << std::endl;
                std::cout << "  bytes:  " << *(double*)&p->bytes << "(0x" << std::hex << std::uppercase << p->bytes << ")" << std::dec << std::endl;
                break;
            }
        case ujvm::CONSTANT_NAME_AND_TYPE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_name_and_type_t>(constant);
                std::cout << "constant_name_and_type[" << i << "]:" << std::endl;
                std::cout << "  name_index:       " << p->name_index << std::endl;
                std::cout << "  descriptor_index: " << p->descriptor_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_UTF8_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_utf8_t>(constant);
                std::cout << "constant_utf8[" << i << "]:" << std::endl;
                std::cout << "  length:  " << p->bytes.size() << std::endl;
                std::cout << "  string:  " << p->str << std::endl;
                break;
            }
        case ujvm::CONSTANT_METHOD_HANDLE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_handle_t>(constant);
                std::cout << "constant_method_handle[" << i << "]:" << std::endl;
                std::cout << "  sreference_kind:  " << p->reference_kind << std::endl;
                std::cout << "  reference_index:  " << p->reference_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_METHOD_TYPE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_type_t>(constant);
                std::cout << "constant_method_type[" << i << "]:" << std::endl;
                std::cout << "  descriptor_index:       " << p->descriptor_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_INVOKE_DYNAMIC_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_invoke_dynamic_t>(constant);
                std::cout << "constant_invoke_dynamic[" << i << "]:" << std::endl;
                std::cout << "  bootstrap_method_attr_index:  " << p->bootstrap_method_attr_index << std::endl;
                std::cout << "  name_and_type_index:          " << p->name_and_type_index << std::endl;
                break;
            }
        case ujvm::CONSTANT_INVALID_TAG:
            {
                std::cout << "constant_placeholder[" << i << "]:" << std::endl;
                break;
            }
        }
        i++;
    }
}

static void print_interfaces(ujvm::class_file_t::interfaces_t& interfaces, ujvm::class_file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto interface : interfaces) {
        auto constant_class = std::static_pointer_cast<ujvm::constant_class_t>(cp[interface]);
        auto class_name = std::static_pointer_cast<ujvm::constant_utf8_t>(cp[constant_class->name_index]);
        std::cout << "interface[" << i++ << "]:  " << interface << "(" << class_name->str << ")" << std::endl;
    }
}

static std::string generate_spaces(int c)
{
    return std::string(c, L' ');
}

#undef  spaces
#define spaces(n) generate_spaces(n).c_str()

static void print_annotation(int nos, std::shared_ptr<ujvm::annotation_t> p, ujvm::class_file_t::constant_pool_t& cp);

static void print_element_value(int nos, ujvm::element_value_t &ev, ujvm::class_file_t::constant_pool_t& cp)
{
    std::cout << spaces(nos + 2) << "element_value(" << (char)ev.tag << ")" << std::endl; 

    switch (ev.tag) {
        case 'B': case 'C':
        case 'D': case 'F':
        case 'I': case 'J':
        case 'S': case 'Z':
        case 's':
        {
            auto &const_value_index = ev.value.cast_to_ref<uint16_t>();
            std::cout << spaces(nos + 4) << "const_value_index:  " << const_value_index << std::endl;
            break;
        }
        case 'e':
        {
            auto &v = ev.value.cast_to_ref<std::shared_ptr<ujvm::element_value_t::enum_const_value_t>>();
            std::cout << spaces(nos + 4) << "type_name_index:   " << v->type_name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->type_name_index])->str<< ")" << std::endl;
            std::cout << spaces(nos + 4) << "const_name_index:  " << v->const_name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->const_name_index])->str << ")" << std::endl;
            break;
        }
        case 'c':
        {
            auto &class_info_index = ev.value.cast_to_ref<uint16_t>();
            std::cout << spaces(nos + 4) << "class_info_index:  " << class_info_index << std::endl;
            break;
        }
        case '@':
        {
            auto &v = ev.value.cast_to_ref<std::shared_ptr<ujvm::annotation_t>>();
            print_annotation(nos + 2, v, cp);
            break;
        }
        case '[':
        {
            auto &v = ev.value.cast_to_ref<std::vector<std::shared_ptr<ujvm::element_value_t>>>();
            int i = 0;
            std::cout << spaces(nos + 4) << "array_value:" << std::endl;
            std::cout << spaces(nos + 6) << "number_of_values:  " << v.size() << std::endl;
            for (auto value : v) {
                std::cout << spaces(nos + 6) << "value[" << i++ << "]:" << std::endl;
                print_element_value(nos + 6, *value, cp);
            }
            break;
        }
    }
}

static void print_annotation(int nos, std::shared_ptr<ujvm::annotation_t> p, ujvm::class_file_t::constant_pool_t& cp)
{
    std::cout << spaces(nos + 4) << "type_index:  " << p->type_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[p->type_index])->str << ")" << std::endl;

    int i = 0;
    std::cout << spaces(nos + 4) << "number_of_element_value_pairs:  " << p->element_value_pairs.size() << std::endl;
    for (auto element_value : p->element_value_pairs) {
        std::cout << spaces(nos + 4) << "element_value_pairs[" << i++ << "]:" << std::endl;
        std::cout << spaces(nos + 6) << "element_name_index:  " << element_value->element_name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[element_value->element_name_index])->str << ")" << std::endl;
        print_element_value(nos + 4, element_value->value, cp);
    }
}

static void print_attributes(int nos, ujvm::class_file_t::attributes_t& attributes, ujvm::class_file_t::constant_pool_t& cp)
{
#undef  SWITCH
#undef  CASE 
#define SWITCH               if (false)
#define CASE(value)          } else if (attribute->attribute_name == value) {

    int index = 0;
    for (auto attribute : attributes) {
        std::cout << spaces(nos) << "attributes[" << index++ << "]:" << std::endl;
        std::cout << spaces(nos + 2) << "attribute_name_index: " << attribute->attribute_name_index
                                                                      << "("
                                                                      << std::string(attribute->attribute_name.begin(), attribute->attribute_name.end())
                                                                      << ")" << std::endl;
        std::cout << spaces(nos + 2) << "attribute_length:     " << attribute->attribute_length << std::endl;

        SWITCH{
        CASE("ConstantValue")
            auto v = std::static_pointer_cast<ujvm::attribute_constant_value_t>(attribute);
            std::cout << spaces(nos + 2) << "constantvalue_index:  " << v->constantvalue_index << std::endl;

        CASE("Code")
            auto v = std::static_pointer_cast<ujvm::attribute_code_t>(attribute);
            
            std::cout << spaces(nos + 2) << "max_stack:   " << v->max_stack << std::endl;
            std::cout << spaces(nos + 2) << "max_locals:  " << v->max_locals << std::endl;
            std::cout << spaces(nos + 2) << "code_length: " << v->code.size() << std::endl;
            std::cout << spaces(nos + 2) << "code: ";
            std::cout << std::hex << std::uppercase;
            std::copy(v->code.begin(), v->code.end(), std::ostream_iterator<int, char>(std::cout, " "));
            std::cout << std::dec << std::endl;
            std::cout << spaces(nos + 2) << "exception_table_length: " << v->exception_table.size() << std::endl;
            int i = 0;
            for (auto exception : v->exception_table) {
                std::cout << spaces(nos + 2) << "exception_table[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "start_pc:   " << exception->start_pc << std::endl;
                std::cout << spaces(nos + 4) << "end_pc:     " << exception->end_pc << std::endl;
                std::cout << spaces(nos + 4) << "handler_pc: " << exception->handler_pc << std::endl;
                std::cout << spaces(nos + 4) << "catch_type: " << exception->catch_type << std::endl;
            }
            std::cout << spaces(nos + 2) << "attributes_count: " << v->attributes.size() << std::endl;
            print_attributes(nos + 2, v->attributes, cp);

        CASE("StackMapTable")
            auto v = std::static_pointer_cast<ujvm::attribute_stack_map_table_t>(attribute);

            auto print_verification_type_info = [&](std::shared_ptr<ujvm::attribute_stack_map_table_t::verification_t> p) {
                switch (p->tag)
                {
                    case ujvm::VERIFICATION_TOP_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Top" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_INTEGER_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Integer" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_FLOAT_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Float" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_DOUBLE_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Double" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_LONG_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Long" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_NULL_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Null" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_UNINITIALIZED_THIS_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_UninitializedThis" << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_OBJECT_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Object" << std::endl;
                        std::cout << spaces(nos + 6) << "cpool_index: " << std::static_pointer_cast<ujvm::attribute_stack_map_table_t::verification_object_variable_t>(p)->cpool_index << std::endl;
                        break;
                    }
                    case ujvm::VERIFICATION_UNINITIALIZED_VARIABLE_TAG:
                    {
                        std::cout << spaces(nos + 6) << "tag:         ITEM_Uninitialized" << std::endl;
                        std::cout << spaces(nos + 6) << "offset: " << std::static_pointer_cast<ujvm::attribute_stack_map_table_t::verification_uninitialized_variable_t>(p)->offset << std::endl;
                        break;
                    }
                }
            };

            std::cout << spaces(nos + 2) << "number_of_entries:    " << v->entries.size() << std::endl;
            
            int i = 0;
            for (auto entrie : v->entries) {
                std::cout << spaces(nos + 2) << "entries[" << i++ << "]:" << std::endl;
                switch (entrie->frame_type) {
                    case ujvm::STACK_MAP_FRAME_SAME_FRAME_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  SAME" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  SAME_LOCALS_1_STACK_ITEM" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  SAME_LOCALS_1_STACK_ITEM_EXTENDED" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_CHOP_FRAME_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  CHOP" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_SAME_FRAME_EXTENDED_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  SAME_FRAME_EXTENDED" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_APPEND_FRAME_TAG:
                    {
                        std::cout << spaces(nos + 4) << "frame_type:  APPEND" << std::endl;
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_FULL_FRAME_TAG:
                    {
                        auto full_frame = std::static_pointer_cast<ujvm::attribute_stack_map_table_t::full_frame_t>(entrie);
                        std::cout << spaces(nos + 4) << "frame_type:       FULL_FRAME" << std::endl;
                        std::cout << spaces(nos + 4) << "offset_delta:     " << full_frame->offset_delta  << std::endl;
                        std::cout << spaces(nos + 4) << "number_of_locals: " << full_frame->locals.size() << std::endl;
                        
                        int j = 0;
                        for (auto p : full_frame->locals) {
                            std::cout << spaces(nos + 4) << "locals[" << j++ << "]:  " << std::endl;
                            print_verification_type_info(p);
                        }
                        
                        std::cout << spaces(nos + 4) << "number_of_stack_items: " << full_frame->stack.size() << std::endl;

                        j = 0;
                        for (auto p : full_frame->stack) {
                            std::cout << spaces(nos + 4) << "stacks[" << j++ << "]:  " << std::endl;
                            print_verification_type_info(p);
                        }
                        break;
                    }
                }
            }

        CASE("Exceptions")
            auto v = std::static_pointer_cast<ujvm::attribute_exceptions_t>(attribute);
            std::cout << spaces(nos + 2) << "number_of_exceptions: " << v->exception_index_table.size() << std::endl;
            std::cout << spaces(nos + 2) << "exceptions:  ";
            std::copy(v->exception_index_table.begin(), v->exception_index_table.end(), std::ostream_iterator<int, char>(std::cout, " "));
            std::cout << std::endl;

        CASE("InnerClasses")
            auto v = std::static_pointer_cast<ujvm::attribute_inner_classes_t>(attribute);
            std::cout << spaces(nos + 2) << "number_of_classes:    " << v->classes.size() << std::endl;
            
            int i = 0;
            for (auto c : v->classes) {
                std::cout << spaces(nos + 2) << "classes[" << i++ << "]:  " << std::endl;
                std::cout << spaces(nos + 4) << "inner_class_info_index:   " << c->inner_class_info_index << std::endl;
                std::cout << spaces(nos + 4) << "outer_class_info_index:   " << c->outer_class_info_index << std::endl;
                std::cout << spaces(nos + 4) << "inner_name_index:         " << c->inner_name_index << std::endl;
                std::cout << spaces(nos + 4) << "inner_class_access_flags: " << c->inner_class_access_flags << std::endl;
            }

        CASE("EnclosingMethod")
            auto v = std::static_pointer_cast<ujvm::attribute_enclosing_method_t>(attribute);
            std::cout << spaces(nos + 2) << "class_index:  " << v->class_index << std::endl;
            std::cout << spaces(nos + 2) << "method_index: " << v->method_index << std::endl;

        CASE("Synthetic")
        CASE("Signature")
            auto v = std::static_pointer_cast<ujvm::attribute_signature_t>(attribute);
            std::cout << spaces(nos + 2) << "signature_index:      " << v->signature_index << std::endl;

        CASE("SourceFile")
            auto v = std::static_pointer_cast<ujvm::attribute_source_file_t>(attribute);
            auto sourcefile = std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->sourcefile_index])->str;
            std::cout << spaces(nos + 2) << "sourcefile_index:     " << v->sourcefile_index << "(" << std::string(sourcefile.begin(), sourcefile.end()) << "L)" << std::endl;

        CASE("SourceDebugExtension")
            auto v = std::static_pointer_cast<ujvm::attribute_source_debug_extension_t>(attribute);
            std::cout << spaces(nos + 2) << "debug_extension:  ";
            std::copy(v->debug_extension.begin(), v->debug_extension.end(), std::ostream_iterator<int, char>(std::cout, " "));
            std::cout << std::endl;

        CASE("LineNumberTable")
            auto v = std::static_pointer_cast<ujvm::attribute_line_number_table_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "line_number_table_length:  " << v->line_number_table.size() << std::endl;
            for (auto ln : v->line_number_table) {
                std::cout << spaces(nos + 2) << "line_number_table[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "start_pc:     " << ln->start_pc << std::endl;
                std::cout << spaces(nos + 4) << "line_number:  " << ln->line_number << std::endl;
            }

        CASE("LocalVariableTable")
            auto v = std::static_pointer_cast<ujvm::attribute_local_variable_table_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "local_variable_table_length:  " << v->local_variable_table.size() << std::endl;
            for (auto lvt : v->local_variable_table) {
                std::cout << spaces(nos + 2) << "local_variable_table[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "start_pc:         " << lvt->start_pc << std::endl;
                std::cout << spaces(nos + 4) << "length:           " << lvt->length << std::endl;
                std::cout << spaces(nos + 4) << "name_index:       " << lvt->name_index << std::endl;
                std::cout << spaces(nos + 4) << "descriptor_index: " << lvt->descriptor_index << std::endl;
                std::cout << spaces(nos + 4) << "index:            " << lvt->index << std::endl;
            }

        CASE("LocalVariableTypeTable")
            auto v = std::static_pointer_cast<ujvm::attribute_local_variable_type_table_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "local_variable_type_table_length:  " << v->local_variable_type_table.size() << std::endl;
            for (auto lvt : v->local_variable_type_table) {
                std::cout << spaces(nos + 2) << "local_variable_table[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "start_pc:         " << lvt->start_pc << std::endl;
                std::cout << spaces(nos + 4) << "length:           " << lvt->length << std::endl;
                std::cout << spaces(nos + 4) << "name_index:       " << lvt->name_index << std::endl;
                std::cout << spaces(nos + 4) << "signature_index:  "  << lvt->signature_index << std::endl;
                std::cout << spaces(nos + 4) << "index:            " << lvt->index << std::endl;
            }

        CASE("Deprecated")
   
        CASE("RuntimeVisibleAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_visible_annotations_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "number_of_annotations:  " << v->annotations.size() << std::endl;
            for (auto annotation : v->annotations) {
                std::cout << spaces(nos + 2) << "annotations[" << i++ << "]:" << std::endl;
                print_annotation(nos, annotation, cp);
            }

        CASE("RuntimeInvisibleAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_invisible_annotations_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "number_of_annotations:  " << v->annotations.size() << std::endl;
            for (auto annotation : v->annotations) {
                std::cout << spaces(nos + 2) << "annotations[" << i++ << "]:" << std::endl;
                print_annotation(nos, annotation, cp);
            }

        CASE("RuntimeVisibleParameterAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_visible_parameter_annotations_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "number_of_annotations:  " << v->parameter_annotations.size() << std::endl;
            for (auto parameter : v->parameter_annotations) {
                int j = 0;
                std::cout << spaces(nos + 2) << "parameter_annotations[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "number_of_annotations:  " << parameter->annotations.size() << std::endl;
                for (auto annotation : parameter->annotations) {
                    std::cout << spaces(nos + 4) << "annotations[" << j++ << "]:" << std::endl;
                    print_annotation(nos + 2, annotation, cp);
                }
            }

        CASE("RuntimeInvisibleParameterAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_invisible_parameter_annotations_t>(attribute);
            int i = 0;

            std::cout << spaces(nos + 2) << "number_of_annotations:  " << v->parameter_annotations.size() << std::endl;
            for (auto parameter : v->parameter_annotations) {
                int j = 0;
                std::cout << spaces(nos + 2) << "parameter_annotations[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "number_of_annotations:  " << parameter->annotations.size() << std::endl;
                for (auto annotation : parameter->annotations) {
                    std::cout << spaces(nos + 4) << "annotations[" << j++ << "]:" << std::endl;
                    print_annotation(nos + 2, annotation, cp);
                }
            }

        CASE("AnnotationDefault")
            auto v = std::static_pointer_cast<ujvm::attribute_annotation_default_t>(attribute);
            print_element_value(nos, v->default_value, cp);

        CASE("BootstrapMethods")
            auto v = std::static_pointer_cast<ujvm::attribute_bootstrap_methods_t>(attribute);
            std::cout << spaces(nos + 2) << "number_bootstrap_methods:  " << v->bootstrap_methods.size() << std::endl;
            
            int i = 0;
            for (auto bm : v->bootstrap_methods) {
                std::cout << spaces(nos + 2) << "bootstrap_methods:[" << i++ << "]:" << std::endl;
                std::cout << spaces(nos + 4) << "bootstrap_method_ref:        " << bm->bootstrap_method_ref << std::endl;
                std::cout << spaces(nos + 4) << "number_bootstrap_arguments:  " << bm->bootstrap_arguments.size() << std::endl;
                std::cout << spaces(nos + 4) << "bootstrap_arguments: ";
                std::copy(bm->bootstrap_arguments.begin(), bm->bootstrap_arguments.end(), std::ostream_iterator<int, char>(std::cout, " "));
                std::cout << std::endl;
            }
        }
    }

#undef  SWITCH
#undef  CASE
}

#undef  spaces

static void print_fields(ujvm::class_file_t::fields_t& fields, ujvm::class_file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto field : fields) {
        std::cout << "field_info[" << i++ << "]:" << std::endl;
        std::cout << "  access_flags:     0x" << std::hex << std::uppercase << field->access_flags << std::dec << std::endl;
        std::cout << "  name_index:       "   << field->name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[field->name_index])->str << ")" << std::endl;
        std::cout << "  descriptor_index: " << field->descriptor_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[field->descriptor_index])->str << ")" << std::endl;
        std::cout << "  attributes_count: "   << field->attributes.size() << std::endl;
        print_attributes(2, field->attributes, cp);
    }
}

static void print_methods(ujvm::class_file_t::methods_t& methods, ujvm::class_file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto method : methods) {
        std::cout << "methods_info[" << i++ << "]:" << std::endl;
        std::cout << "  access_flags:     0x" << std::hex << std::uppercase << method->access_flags << std::dec << std::endl;
        std::cout << "  name_index:       " << method->name_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[method->name_index])->str << ")" << std::endl;
        std::cout << "  descriptor_index: " << method->descriptor_index << "(" << std::static_pointer_cast<ujvm::constant_utf8_t>(cp[method->descriptor_index])->str << ")" << std::endl;
        std::cout << "  attributes_count: " << method->attributes.size() << std::endl;
        print_attributes(2, method->attributes, cp);
    }
}