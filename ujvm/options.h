

#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "config.h"

#include <list>
#include <string>
#include <vector>
#include <cstdint>


UJVM_NAMESPACE_BEGIN

class options_t {
public:
    options_t(char **args);
    bool parse(void);

    bool disassemble;
    uint64_t xss;
    bool classversion;
    bool dumpclassfile;
    std::vector<std::string> jars;
   
    std::vector<std::string> classargs;
    std::string classfile;
    
private:
    void usage_en(void);
    void parse_options(std::vector<std::string>::iterator& it);
    std::vector<std::string> split(std::string str, char delim);

private:
    std::vector<std::string> argv;
};

UJVM_NAMESPACE_END

#endif