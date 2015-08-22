

#include "jvm.h"
#include "class_file.h"
#include "thread.h"
#include "class.h"

#include <string>
#include <iostream>

UJVM_NAMESPACE_BEGIN

const std::string BOOTSTRAP_METHOD_NAME = "main"; 

std::shared_ptr<class_t> jvm_t::load_class(std::string name)
{
    if (m_classes.find(name) != m_classes.end()) {
        return m_classes[name];
    }

    auto cf = m_jar.load_class_file_by_jars(name);
    if (cf) {
        auto c = std::make_shared<class_t>(this, *cf);
        if (c) {
            m_classes[name] = c;
            return c;
        }
    }

    return nullptr;
}

bool jvm_t::run(void)
{
    class_file_t cf(m_options.classfile);
    class_t c(this, cf);
    thread_t thread(c);
    
    thread.jvm = this;
    thread.invoke_main_method();

    return true;
}

UJVM_NAMESPACE_END