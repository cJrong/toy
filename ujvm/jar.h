

#ifndef _JAR_H_
#define _JAR_H_

#include "config.h"
#include "class_file.h"

#include <map>
#include <vector>
#include <string>
#include <memory>

UJVM_NAMESPACE_BEGIN

class jar_t {
public:
    jar_t(std::vector<std::string>& jars);

public:
    std::shared_ptr<class_file_t> load_class_file_by_jars(const std::string& classname);
    bool load_all_class_file_by_jars(void);

    std::shared_ptr<class_file_t> load_class_file_by_jar(const std::string& jarname, const std::string& classname);
    bool load_all_class_file_by_jar(const std::string& jarname);

private:
    std::map<std::string, std::shared_ptr<class_file_t>> m_cfs;
    std::vector<std::string>& m_jars;
};

UJVM_NAMESPACE_END

#endif