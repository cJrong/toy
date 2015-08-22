

#include <iterator>
#include <iostream>
#include <iomanip>
#include <clocale>
#include <stdio.h>
#include <string>
#include <valarray>
#include "../file.h"
#include "../utf8.h"

static void print_constant_pool(ujvm::file_t::constant_pool_t &cp);
static void print_interfaces(ujvm::file_t::interfaces_t& interfaces, ujvm::file_t::constant_pool_t& cp);
static void print_fields(ujvm::file_t::fields_t& fields, ujvm::file_t::constant_pool_t& cp);
static void print_methods(ujvm::file_t::methods_t& methods, ujvm::file_t::constant_pool_t& cp);
static void print_attributes(int nos, ujvm::file_t::attributes_t& attributes, ujvm::file_t::constant_pool_t& cp);
static void dump_class_file(std::string file);

int main(int argc, char** args)
{

    std::setlocale(LC_ALL, "");

    dump_class_file(args[1]);

    return 0;
}

static void dump_class_file(std::string classfile)
{
    try {
        ujvm::file_t file(classfile);
        std::wcout << L"magic:          0x" << std::hex << std::uppercase << file.magic() << std::endl;
        std::wcout << L"minor_version:  "   << std::dec << std::get<1>(file.version()) << std::endl;
        std::wcout << L"major_version:  "   << std::get<0>(file.version()) << std::endl;
        printf("constant_pool_count: %d\n",     file.constant_pool().size());
        print_constant_pool(file.constant_pool());
        printf("access_flags:        0x%.4X\n", file.access_flags());
        printf("this_class:          %d\n",     file.this_class());
        printf("super_class:         %d\n",     file.super_class());
        printf("interfaces_count:    %d\n",     file.interfaces().size());
        print_interfaces(file.interfaces(),     file.constant_pool());
        printf("fields_count:        %d\n",     file.fields().size());
        print_fields(file.fields(),             file.constant_pool());
        printf("methods_count:       %d\n",     file.methods().size());
        print_methods(file.methods(),           file.constant_pool());
        printf("attributes_count:    %d\n",     file.attributes().size());
        print_attributes(0, file.attributes(),  file.constant_pool());
    } catch (ujvm::exception_t& ex) {
        std::cout << ex.what() << std::endl;
    }
}

static void print_constant_pool(ujvm::file_t::constant_pool_t &cp)
{
    int i = 0;
    for (auto constant : cp) {
        switch (constant->tag) {
        case ujvm::CONSTANT_CLASS_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_class_t>(constant);
                printf("constant_class[%d]:\n", i);
                printf("  name_index:  %d(%ws)\n", p->name_index, std::static_pointer_cast<ujvm::constant_utf8_t>(cp[p->name_index])->str.c_str());
                break;
            }
        case ujvm::CONSTANT_FIELD_REF_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_field_ref_t>(constant);
                printf("constant_field_ref[%d]:\n", i);
                printf("  class_index:          %d\n", p->class_index);
                printf("  name_and_type_index:  %d\n", p->name_and_type_index);
                break;
            }
        case ujvm::CONSTANT_METHOD_REF_TAG: 
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_ref_t>(constant);
                printf("constant_method_ref[%d]:\n", i);
                printf("  class_index:          %d\n", p->class_index);
                printf("  name_and_type_index:  %d\n", p->name_and_type_index);
                break;
            }
        case ujvm::CONSTANT_INTERFACE_METHOD_REF_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_interface_method_ref_t>(constant);
                printf("interface_method_ref[%d]:\n", i);
                printf("  class_index:          %d\n", p->class_index);
                printf("  name_and_type_index:  %d\n", p->name_and_type_index);
                break;
            }
        case ujvm::CONSTANT_STRING_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_string_t>(constant);
                printf("constant_string[%d]:\n", i);
                printf("  string_index:  %d\n", p->string_index);
                break;
            }
        case ujvm::CONSTANT_INTEGER_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_integer_t>(constant);
                printf("constant_integer[%d]:\n", i);
                printf("  bytes:  %d(0x%.8X)\n", p->bytes, p->bytes);
                break;
            }
        case ujvm::CONSTANT_FLOAT_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_float_t>(constant);
                printf("constant_float[%d]:\n", i);
                printf("  bytes:  %f(0x%.8X)\n", p->bytes, p->bytes);
                break;
            }
        case ujvm::CONSTANT_LONG_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_long_t>(constant);
                printf("constant_long[%d]:\n", i);
                printf("  bytes:  %lld(0x%.16llX)\n", (uint64_t)p->bytes, (uint64_t)p->bytes);
                break;
            }
        case ujvm::CONSTANT_DOUBLE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_double_t>(constant);
                printf("constant_double[%d]:\n", i);
                printf("  bytes:  %f(0x%.16llX)\n", p->bytes, p->bytes);
                break;
            }
        case ujvm::CONSTANT_NAME_AND_TYPE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_name_and_type_t>(constant);
                printf("constant_name_and_type[%d]:\n", i);
                printf("  name_index:       %d\n", p->name_index);
                printf("  descriptor_index: %d\n", p->descriptor_index);
                break;
            }
        case ujvm::CONSTANT_UTF8_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_utf8_t>(constant);
                printf("constant_utf8[%d]:\n", i);
                printf("  length:  %d\n", p->bytes.size());
                (!p->bytes.empty()) && std::wcout << L"  " << p->str << std::endl;
                
                break;
            }
        case ujvm::CONSTANT_METHOD_HANDLE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_handle_t>(constant);
                printf("constant_method_handle[%d]:\n", i);
                printf("  sreference_kind:  %d\n", p->reference_kind);
                printf("  reference_index:  %d\n", p->reference_index);
                break;
            }
        case ujvm::CONSTANT_METHOD_TYPE_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_method_type_t>(constant);
                printf("constant_method_type[%d]:\n", i);
                printf("  descriptor_index:  %d\n", p->descriptor_index);
                break;
            }
        case ujvm::CONSTANT_INVOKE_DYNAMIC_TAG:
            {
                auto p = std::static_pointer_cast<ujvm::constant_invoke_dynamic_t>(constant);
                printf("constant_invoke_dynamic[%d]:\n", i);
                printf("  bootstrap_method_attr_index:  %d\n", p->bootstrap_method_attr_index);
                printf("  name_and_type_index:          %d\n", p->name_and_type_index);
                break;
            }
        case ujvm::CONSTANT_INVALID_TAG:
            {
                printf("constant_placeholder[%d]:\n", i);
                break;
            }
        }
        i++;
    }
}

static void print_interfaces(ujvm::file_t::interfaces_t& interfaces, ujvm::file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto interface : interfaces) {
        auto constant_class = std::static_pointer_cast<ujvm::constant_class_t>(cp[interface]);
        auto class_name = std::static_pointer_cast<ujvm::constant_utf8_t>(cp[constant_class->name_index]);
        printf("interface[%d]:  %d(%ws)\n", i++, interface, class_name->str.c_str());
    }
}

static std::string generate_spaces(int c)
{
    return std::string(c, ' ');
}

#undef  spaces
#define spaces(n) generate_spaces(n).c_str()

static void print_annotation(int nos, std::shared_ptr<ujvm::annotation_t> p, ujvm::file_t::constant_pool_t& cp);

static void print_element_value(int nos, ujvm::element_value_t &ev, ujvm::file_t::constant_pool_t& cp)
{
    printf("%selement_value(%c):\n", spaces(nos + 2), ev.tag);

    switch (ev.tag) {
        case 'B': case 'C':
        case 'D': case 'F':
        case 'I': case 'J':
        case 'S': case 'Z':
        case 's':
        {
            auto &const_value_index = ev.value.cast_to_ref<uint16_t>();
            printf("%sconst_value_index:  %d\n", spaces(nos + 4), const_value_index);
            break;
        }
        case 'e':
        {
            auto &v = ev.value.cast_to_ref<std::shared_ptr<ujvm::element_value_t::enum_const_value_t>>();
            printf("%stype_name_index:   %d(%ws)\n", spaces(nos + 4), v->type_name_index, std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->type_name_index])->str.c_str());
            printf("%sconst_name_index:  %d(%ws)\n", spaces(nos + 4), v->const_name_index, std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->const_name_index])->str.c_str());
            break;
        }
        case 'c':
        {
            auto &class_info_index = ev.value.cast_to_ref<uint16_t>();
            printf("%sclass_value_index:  %d\n", spaces(nos + 4), class_info_index);
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
            printf("%sarray_value:\n", spaces(nos + 4));
            printf("%snumber_of_values:  %d\n", spaces(nos + 6), v.size());
            for (auto value : v) {
                printf("%svalue[%d]:\n", spaces(nos + 6), i++);
                print_element_value(nos + 6, *value, cp);
            }
            break;
        }
    }
}

static void print_annotation(int nos, std::shared_ptr<ujvm::annotation_t> p, ujvm::file_t::constant_pool_t& cp)
{
    printf("%stype_index:  %d(%ws)\n", spaces(nos + 4), p->type_index, std::static_pointer_cast<ujvm::constant_utf8_t>(cp[p->type_index])->str.c_str());

    int i = 0;
    printf("%snumber_of_element_value_pairs:  %d\n", spaces(nos + 4), p->element_value_pairs.size());
    for (auto element_value : p->element_value_pairs) {
        printf("%selement_value_pairs[%d]:\n", spaces(nos + 4), i++);
        printf("%selement_name_index:  %d(%ws)\n", spaces(nos + 6), element_value->element_name_index, std::static_pointer_cast<ujvm::constant_utf8_t>(cp[element_value->element_name_index])->str.c_str());
        print_element_value(nos + 4, element_value->value, cp);
    }
}

static void print_attributes(int nos, ujvm::file_t::attributes_t& attributes, ujvm::file_t::constant_pool_t& cp)
{
#undef  SWITCH
#undef  CASE 
#define SWITCH               if (false)
#define CASE(value)          } else if (attribute->attribute_name == value) {

    int index = 0;
    for (auto attribute : attributes) {
        printf("%sattributes[%d]:\n", spaces(nos), index++);
        printf("%sattribute_name_index: %d(%s)\n", spaces(nos+2), attribute->attribute_name_index, attribute->attribute_name.c_str());
        printf("%sattribute_length:     %d\n", spaces(nos + 2), attribute->attribute_length);

        SWITCH{
        CASE("ConstantValue")
            auto v = std::static_pointer_cast<ujvm::attribute_constant_value_t>(attribute);
            printf("%sconstantvalue_index:  %d\n", spaces(nos + 2), v->constantvalue_index);

        CASE("Code")
            auto v = std::static_pointer_cast<ujvm::attribute_code_t>(attribute);
            printf("%smax_stack:    %d\n",  spaces(nos + 2), v->max_stack);
            printf("%smax_locals:   %d\n",  spaces(nos + 2), v->max_locals);
            printf("%scode_length:  %d\n",  spaces(nos + 2), v->code.size());
            printf("%scode: ",      spaces(nos + 2));
            for (auto c : v->code) {
                printf("0x%.2X ", c);
            }
            printf("\n");
            printf("%sexception_table_length:  %d\n", spaces(nos + 2), v->exception_table.size());
            int i = 0;
            for (auto exception : v->exception_table) {
                printf("%sexception_table[%d]:\n",  spaces(nos + 2), i++);
                printf("%sstart_pc:     %d\n",      spaces(nos + 4), exception->start_pc);
                printf("%send_pc:       %d\n",      spaces(nos + 4), exception->end_pc);
                printf("%shandler_pc:   %d\n",      spaces(nos + 4), exception->handler_pc);
                printf("%scatch_type:   %d\n",      spaces(nos + 4), exception->catch_type);
            }
            printf("%sattributes_count: %d\n",      spaces(nos + 2), v->attributes.size());
            print_attributes(nos + 2, v->attributes, cp);

        CASE("StackMapTable")
            auto v = std::static_pointer_cast<ujvm::attribute_stack_map_table_t>(attribute);

            auto print_verification_type_info = [&](std::shared_ptr<ujvm::attribute_stack_map_table_t::verification_t> p) {
                switch (p->tag)
                {
                    case ujvm::VERIFICATION_TOP_TAG:
                    {
                        printf("%stag:  ITEM_Top\n",        spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_INTEGER_TAG:
                    {
                        printf("%stag:  ITEM_Integer\n",    spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_FLOAT_TAG:
                    {
                        printf("%stag:  ITEM_Float\n",      spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_DOUBLE_TAG:
                    {
                        printf("%stag:  ITEM_Double\n",     spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_LONG_TAG:
                    {
                        printf("%stag:  ITEM_Long\n",       spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_NULL_TAG:
                    {
                        printf("%stag:  ITEM_Null\n",       spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_UNINITIALIZED_THIS_TAG:
                    {
                        printf("%stag:  ITEM_UninitializedThis\n", spaces(nos + 6));
                        break;
                    }
                    case ujvm::VERIFICATION_OBJECT_TAG:
                    {
                        printf("%stag:          ITEM_Object\n", spaces(nos + 6));
                        printf("%scpool_index:  %d\n",  spaces(nos + 6), std::static_pointer_cast<ujvm::attribute_stack_map_table_t::verification_object_variable_t>(p)->cpool_index);
                        break;
                    }
                    case ujvm::VERIFICATION_UNINITIALIZED_VARIABLE_TAG:
                    {
                        printf("%stag:      ITEM_Uninitialized\n",  spaces(nos + 6));
                        printf("%soffset:   %d\n",               spaces(nos + 6), std::static_pointer_cast<ujvm::attribute_stack_map_table_t::verification_uninitialized_variable_t>(p)->offset);
                        break;
                    }
                }
            };

            int i = 0;
            printf("%snumber_of_entries:  %d\n", spaces(nos + 2), v->entries.size());
            for (auto entrie : v->entries) {
                printf("%sentries[%d]:\n", spaces(nos + 2), i++);
                switch (entrie->frame_type) {
                    case ujvm::STACK_MAP_FRAME_SAME_FRAME_TAG:
                    {
                        printf("%sframe_type:  SAME\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_TAG:
                    {
                        printf("%sframe_type:  SAME_LOCALS_1_STACK_ITEM\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_LOCALS_1_STACK_ITEM_FRAME_EXTENDED_TAG:
                    {
                        printf("%sframe_type:  SAME_LOCALS_1_STACK_ITEM_EXTENDED\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_CHOP_FRAME_TAG:
                    {
                        printf("%sframe_type:  CHOP\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_SAME_FRAME_EXTENDED_TAG:
                    {
                        printf("%sframe_type:  SAME_FRAME_EXTENDED\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_APPEND_FRAME_TAG:
                    {
                        printf("%sframe_type:  APPEND\n", spaces(nos + 4));
                        break;
                    }
                    case ujvm::STACK_MAP_FRAME_FULL_FRAME_TAG:
                    {
                        auto full_frame = std::static_pointer_cast<ujvm::attribute_stack_map_table_t::full_frame_t>(entrie);
                        printf("%sframe_type:  FULL_FRAME\n", spaces(nos + 4));
                        printf("%soffset_delta:  %d\n", spaces(nos + 4), full_frame->offset_delta);
                        printf("%snumber_of_locals:  %d\n", spaces(nos + 4), full_frame->locals.size());
                        int j = 0;
                        for (auto p : full_frame->locals) {
                            printf("%slocals[%d]:\n", spaces(nos + 4), j++);
                            print_verification_type_info(p);
                        }
                        printf("%snumber_of_stack_items:  %d\n", spaces(nos + 4), full_frame->stack.size());
                        j = 0;
                        for (auto p : full_frame->stack) {
                            printf("%sstacks[%d]:\n", spaces(nos + 4), j++);
                            print_verification_type_info(p);
                        }
                        break;
                    }
                }
            }

        CASE("Exceptions")
            auto v = std::static_pointer_cast<ujvm::attribute_exceptions_t>(attribute);
            printf("%snumber_of_exceptions:  %d\n", spaces(nos + 2), v->exception_index_table.size());
            printf("%sexceptions:  ", spaces(nos + 2));
            std::copy(v->exception_index_table.begin(), v->exception_index_table.end(), std::ostream_iterator<int>(std::cout, " "));
            printf("\n");

        CASE("InnerClasses")
            auto v = std::static_pointer_cast<ujvm::attribute_inner_classes_t>(attribute);
            printf("%snumber_of_classes:  %d\n", spaces(nos + 2), v->classes.size());
            
            int i = 0;
            for (auto c : v->classes) {
                printf("%sclasses[%d]:\n",                  spaces(nos + 2), i++);
                printf("%sinner_class_info_index:   %d\n",  spaces(nos + 4), c->inner_class_info_index);
                printf("%souter_class_info_index:   %d\n",  spaces(nos + 4), c->outer_class_info_index);
                printf("%sinner_name_index:         %d\n",  spaces(nos + 4), c->inner_name_index);
                printf("%sinner_class_access_flags: %d\n",  spaces(nos + 4), c->inner_class_access_flags);
            }

        CASE("EnclosingMethod")
            auto v = std::static_pointer_cast<ujvm::attribute_enclosing_method_t>(attribute);
            printf("%sclass_index:  %d\n",  spaces(nos + 2), v->class_index);
            printf("%smethod_index: %d\n", spaces(nos + 2), v->method_index);

        CASE("Synthetic")
        CASE("Signature")
            auto v = std::static_pointer_cast<ujvm::attribute_signature_t>(attribute);
            printf("%ssignature_index:  %d\n", spaces(nos + 2), v->signature_index);

        CASE("SourceFile")
            auto v = std::static_pointer_cast<ujvm::attribute_source_file_t>(attribute);
            printf("%ssourcefile_index:  %d(%ws)\n", spaces(nos + 2), v->sourcefile_index,std::static_pointer_cast<ujvm::constant_utf8_t>(cp[v->sourcefile_index])->str.c_str());

        CASE("SourceDebugExtension")
            auto v = std::static_pointer_cast<ujvm::attribute_source_debug_extension_t>(attribute);
            printf("%sdebug_extension:", spaces(nos + 2));
            std::copy(v->debug_extension.begin(), v->debug_extension.end(), std::ostream_iterator<int>(std::cout, " "));
            printf("\n");

        CASE("LineNumberTable")
            auto v = std::static_pointer_cast<ujvm::attribute_line_number_table_t>(attribute);
            int i = 0;

            printf("%sline_number_table_length:  %d\n", spaces(nos + 2), v->line_number_table.size());        
            for (auto ln : v->line_number_table) {
                printf("%sline_number_table[%d]:\n", spaces(nos + 2), i++);
                printf("%sstart_pc:     %d\n", spaces(nos + 4), ln->start_pc);
                printf("%sline_number:  %d\n", spaces(nos + 4), ln->line_number);
            }

        CASE("LocalVariableTable")
            auto v = std::static_pointer_cast<ujvm::attribute_local_variable_table_t>(attribute);
            int i = 0;

            printf("%slocal_variable_table_length:  %d\n", spaces(nos + 2), v->local_variable_table.size());
            for (auto lvt : v->local_variable_table) {
                printf("%slocal_variable_table[%d]:\n", spaces(nos + 2), i++);
                printf("%sstart_pc:  %d\n", spaces(nos + 4), lvt->start_pc);
                printf("%slength:  %d\n", spaces(nos + 4), lvt->length);
                printf("%sname_index:  %d\n", spaces(nos + 4), lvt->name_index);
                printf("%sdescriptor_index:  %d\n", spaces(nos + 4), lvt->descriptor_index);
                printf("%sindex:  %d\n", spaces(nos + 4), lvt->index);
            }

        CASE("LocalVariableTypeTable")
            auto v = std::static_pointer_cast<ujvm::attribute_local_variable_type_table_t>(attribute);
            int i = 0;

            printf("%slocal_variable_table_length:  %d\n", spaces(nos + 2), v->local_variable_type_table.size());
            for (auto lvt : v->local_variable_type_table) {
                printf("%slocal_variable_table[%d]:\n", spaces(nos + 2), i++);
                printf("%sstart_pc:  %d\n", spaces(nos + 4), lvt->start_pc);
                printf("%slength:  %d\n", spaces(nos + 4), lvt->length);
                printf("%sname_index:  %d\n", spaces(nos + 4), lvt->name_index);
                printf("%ssignature_index:  %d\n", spaces(nos + 4), lvt->signature_index);
                printf("%sindex:  %d\n", spaces(nos + 4), lvt->index);
            }

        CASE("Deprecated")
   
        CASE("RuntimeVisibleAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_visible_annotations_t>(attribute);
            int i = 0;

            printf("%snumber_of_annotations:  %d\n", spaces(nos + 2), v->annotations.size());
            for (auto annotation : v->annotations) {
                printf("%sannotations[%d]:\n", spaces(nos + 2), i++);
                print_annotation(nos, annotation, cp);
            }

        CASE("RuntimeInvisibleAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_invisible_annotations_t>(attribute);
            int i = 0;

            printf("%snumber_of_annotations:  %d\n", spaces(nos + 2), v->annotations.size());
            for (auto annotation : v->annotations) {
                printf("%sannotations[%d]:\n", spaces(nos + 2), i++);
                print_annotation(nos, annotation, cp);
            }

        CASE("RuntimeVisibleParameterAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_visible_parameter_annotations_t>(attribute);
            int i = 0;

            printf("%snumber_of_annotations:  %d\n", spaces(nos + 2), v->parameter_annotations.size());
            for (auto parameter : v->parameter_annotations) {
                int j = 0;
                
                printf("%sparameter_annotations[%d]:\n", i++);
                printf("%snumber_of_annotations:  %d\n", spaces(nos + 4), parameter->annotations.size());
                for (auto annotation : parameter->annotations) {
                    printf("%sannotations[%d]:\n", spaces(nos + 4), i++);
                    print_annotation(nos + 2, annotation, cp);
                }
            }

        CASE("RuntimeInvisibleParameterAnnotations")
            auto v = std::static_pointer_cast<ujvm::attribute_runtime_invisible_parameter_annotations_t>(attribute);
            int i = 0;

            printf("%snumber_of_annotations:  %d\n", spaces(nos + 2), v->parameter_annotations.size());
            for (auto parameter : v->parameter_annotations) {
                int j = 0;

                printf("%sparameter_annotations[%d]:\n", i++);
                printf("%snumber_of_annotations:  %d\n", spaces(nos + 4), parameter->annotations.size());
                for (auto annotation : parameter->annotations) {
                    printf("%sannotations[%d]:\n", spaces(nos + 4), i++);
                    print_annotation(nos + 2, annotation, cp);
                }
            }

        CASE("AnnotationDefault")
            auto v = std::static_pointer_cast<ujvm::attribute_annotation_default_t>(attribute);
            print_element_value(nos, v->default_value, cp);

        CASE("BootstrapMethods")
            auto v = std::static_pointer_cast<ujvm::attribute_bootstrap_methods_t>(attribute);
            printf("%snumber_bootstrap_methods:  %d\n", spaces(nos + 4), v->bootstrap_methods.size());
            printf("%sbootstrap_methods:\n", spaces(nos + 4));
            for (auto bm : v->bootstrap_methods) {
                printf("%sbootstrap_method_ref:  %d\n", spaces(nos + 6), bm->bootstrap_method_ref);
                printf("%snumber_bootstrap_arguments:  %d\n", spaces(nos + 6), bm->bootstrap_arguments.size());
                printf("%sbootstrap_arguments:");
                std::copy(bm->bootstrap_arguments.begin(), bm->bootstrap_arguments.end(), std::ostream_iterator<int>(std::cout, " "));
                printf("\n");
            }
        }
    }

#undef  SWITCH
#undef  CASE
}

#undef  spaces

static void print_fields(ujvm::file_t::fields_t& fields, ujvm::file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto field : fields) {
        printf("field_info[%d]:\n", i++);
        printf("  access_flags:  0x%.4X\n", field->access_flags);
        printf("  name_index:  %d\n",       field->name_index);
        printf("  descriptor_index:  %d\n", field->descriptor_index);
        printf("  attributes_count:  %d\n", field->attributes.size());
        print_attributes(2, field->attributes, cp);
    }
}

static void print_methods(ujvm::file_t::methods_t& methods, ujvm::file_t::constant_pool_t& cp)
{
    int i = 0;
    for (auto method : methods) {
        printf("methods_info[%d]:\n", i++);
        printf("  access_flags:     0x%.4X\n", method->access_flags);
        printf("  name_index:       %d\n",       method->name_index);
        printf("  descriptor_index: %d\n", method->descriptor_index);
        printf("  attributes_count: %d\n", method->attributes.size());
        print_attributes(2, method->attributes, cp);
    }
}