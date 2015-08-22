

#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include "config.h"

#include <string>
#include <sstream>

UJVM_NAMESPACE_BEGIN 

class exception_t {
public:
    exception_t() : m_exception_msg("") {}
    exception_t(char* msg) : m_exception_msg(msg) {}
    exception_t(const char* msg) : m_exception_msg(msg) {}
    exception_t(std::string& msg) : m_exception_msg(msg) {}

    virtual const char* what() const throw()
    {
        return m_exception_msg.c_str();
    }

protected:
    std::string m_exception_msg;
};

class bad_any_cast_t : public exception_t {
public:
    virtual const char* what() const throw()
    {
        return "bad_any_cast: failed conversion using any_cast";
    }
};

class unrecognized_option_t : public exception_t {
public:
    unrecognized_option_t(std::string& option) 
    {
        m_exception_msg = "unrecognized option: ";
        m_exception_msg += option;
    }

    virtual const char* what() const throw()
    {
        return m_exception_msg.c_str();
    }
};

UJVM_NAMESPACE_END

#endif