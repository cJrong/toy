

#include "options.h"
#include "exception.h"

#include <string>
#include <sstream>
#include <iostream>

UJVM_NAMESPACE_BEGIN

options_t::options_t(char **args) : 
    disassemble(false), xss(1024 * 1024), classversion(false), dumpclassfile(false)
{
    while (*++args) {
        argv.push_back(*args);
    }
}

bool options_t::parse(void)
{
    if (argv.empty()) {
        usage_en();
        return false;
    }

    auto it = argv.begin();

    for (auto arg : argv) {
        if ((*it)[0] == '-') {
            parse_options(it);
            it++;
        }
        else {
            break;
        }
    }

    if (it == argv.end()) {
        usage_en();
        return false;
    }

    classfile = *it + ".class";
    classargs.assign(++it, argv.end());

    return true;
}

void options_t::usage_en(void)
{
    std::wcout << L"Usage:    ujvm [-options] class [args]" << std::endl;
    std::wcout << L"              (to execute a class)" << std::endl;
    std::wcout << L"where options include:" << std::endl;
    std::wcout << L"  -cp <class search path of directories and zip/jar files>" << std::endl;
    std::wcout << L"  -classpath <class search path of directories and zip/jar files>" << std::endl;
    std::wcout << L"              A ] separated list of directories, JAR archives," << std::endl;
    std::wcout << L"                  and ZIP archives to search for class files." << std::endl;
    std::wcout << L"  -disasm disassemble the code" << std::endl;
    std::wcout << L"  -disassemble" << std::endl;
    std::wcout << L"  -classversion show the class version" << std::endl;
    std::wcout << L"  -dcf dump" << std::endl;
    std::wcout << L"  --dumpclassfile" << std::endl;
}

void options_t::parse_options(std::vector<std::string>::iterator& it)
{
    if ((*it == "-cp") || (*it == "-classpath")) {
        jars = split(*++it, ']');

    } else if ((*it == "-disasm") || (*it == "-disassemble")) {
        disassemble = true;

    } else if ((*it).substr(0, 4) == "-xss") {
        char ch;
        double d;
        auto size = (*it).substr(4, (*it).size() - 5);
        std::stringstream ss(size);
        
        if ((!(ss >> d)) || (ss >> ch)) {
            throw unrecognized_option_t(*it);
        }
        
        switch (*(*it).rbegin()) {
        case 'k':
        case 'K':
            xss = std::stoul(size) * 1024;
            break;
        case 'm':
        case 'M':
            xss = std::stoul(size) * 1024 * 1024;
            break;
        case 'g':
        case 'G':
            xss = std::stoul(size) * 1024 * 1024 * 1024;
            break;
        }

    } else if ((*it == "-classversion") || (*it == "-cv")) {
        classversion = true;

    } else if ((*it == "-dumpclassfile") || (*it == "-dcf")) {
        dumpclassfile = true;

    } else {
        throw unrecognized_option_t(*it);
    }
}

std::vector<std::string> options_t::split(std::string str, char delim)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string tk;
    
    while (std::getline(ss, tk, delim)) {
        result.push_back(tk);
    }

    return std::move(result);
}

UJVM_NAMESPACE_END