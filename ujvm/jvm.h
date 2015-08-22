

#ifndef _JVM_H_
#define _JVM_H_

#include "config.h"
#include "class.h"
#include "jar.h"

#include <map>
#include <string>

UJVM_NAMESPACE_BEGIN

class jvm_t {
public:
    jvm_t(options_t& options) : m_options(options) , m_jar(options.jars)
    {
    }

    bool run(void);

    std::shared_ptr<class_t> load_class(std::string name);
    
private:
    options_t&  m_options;
    std::map<std::string, std::shared_ptr<class_t>> m_classes;
    jar_t m_jar;
};

UJVM_NAMESPACE_END

#endif