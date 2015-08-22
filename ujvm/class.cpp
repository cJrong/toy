

#include "class.h"
#include "jvm.h"

UJVM_NAMESPACE_BEGIN

class_t::class_t(jvm_t* jvm, class_file_t& file)
{
    constant_pool.swap(file.constant_pool());

    access_flags = file.access_flags();

    class_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[std::static_pointer_cast<constant_class_t>(constant_pool[file.this_class()])->name_index])->str;

    if (file.super_class() != 0) {
        super_class_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[std::static_pointer_cast<constant_class_t>(constant_pool[file.super_class()])->name_index])->str;

        super_class = jvm->load_class(super_class_name + ".class");
    }

    for (auto it : file.interfaces()) {
        interfaces.push_back(std::static_pointer_cast<constant_utf8_t>(constant_pool[std::static_pointer_cast<constant_class_t>(constant_pool[it])->name_index])->str);
    }

    for (auto v : file.fields()) {
        auto field = std::make_shared<field_t>();
        field->access_flags = v->access_flags;
        field->name = std::static_pointer_cast<constant_utf8_t>(constant_pool[v->name_index])->str;
        field->descriptor = std::static_pointer_cast<constant_utf8_t>(constant_pool[v->descriptor_index])->str;
        fields.push_back(field);
    }

    for (auto v : file.methods()) {
        auto method = std::make_shared<method_t>();
        method->access_flags = v->access_flags;
        method->name = std::static_pointer_cast<constant_utf8_t>(constant_pool[v->name_index])->str;
        method->descriptor = std::static_pointer_cast<constant_utf8_t>(constant_pool[v->descriptor_index])->str;
        
        for (auto attr : v->attributes) {
            if (attr->attribute_name == "Code") {
                auto c = std::static_pointer_cast<attribute_code_t>(attr);
                method->code.swap(c->code);
                method->max_stack = c->max_stack;
                method->max_locals = c->max_locals;
            }
        }

        methods.push_back(method); 
    }
}

std::shared_ptr<class_t::method_t> class_t::get_method(std::string name, std::string descriptor, uint16_t acc)
{
    for (auto c = this; c ; c = c->super_class.get()) {
        for (auto method : c->methods) {
            if (method->name == name && 
                method->descriptor == descriptor && 
                method->access_flags == acc) {
                return method;
            }
        }
    }

    return nullptr;
}

UJVM_NAMESPACE_END