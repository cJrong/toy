

#ifndef _ANY_H_
#define _ANY_H_

#include "config.h"
#include "exception.h"

#include <memory>
#include <exception>
#include <type_traits>

UJVM_NAMESPACE_BEGIN

class any_t {
public:
    any_t() : m_content(nullptr)
    {
    }

    any_t(const any_t& other) : m_content(other.m_content ? other.m_content->clone() : 0)
    {
    }

    template <typename ValueType>
    any_t(const ValueType& value) : m_content(new holder<typename std::decay<ValueType>::type>(value))
    {
    }

    any_t(any_t &&other) : m_content(other.m_content)
    {
        other.m_content = nullptr;
    }

    ~any_t()
    {
        delete m_content;
    }

    any_t& operator=(const any_t& rhs)
    {
        any_t(rhs).swap(*this);
        return *this;
    }

    any_t& operator=(any_t&& rhs)
    {
        rhs.swap(*this);
        any_t().swap(rhs);
        return *this;
    }

    template <typename ValueType>
    any_t& operator=(ValueType &&rhs)
    {
        any_t(std::move(rhs)).swap(*this);
        return *this;
    }

    template <class ValueType>
    ValueType& cast_to_ref()
    {
        if (empty() || (typeid(ValueType) != m_content->type())) {
            throw bad_any_cast_t();
        }

        return dynamic_cast<holder<ValueType>*>(m_content)->held;
    }

    template <class ValueType>
    ValueType* cast_to_pointer()
    {
        if (empty() || (typeid(ValueType) != m_content->type())) {
            return nullptr;
        }

        return &dynamic_cast<holder<ValueType>*>(m_content)->held;
    }

    inline any_t& swap(any_t& rhs)
    {
        std::swap(m_content, rhs.m_content);
        return *this;
    }

    inline bool empty(void) const
    {
        return !m_content;
    }

    inline void clear(void)
    {
        any_t().swap(*this);
    }

    const std::type_info& type() const
    {
        return m_content ? m_content->type() : typeid(void);
    }

    template <class ValueType>
    friend  ValueType* any_cast(any_t* operand);

private:

    class placeholder {
    public:
        virtual ~placeholder() {}

        virtual const std::type_info& type() const = 0;
        virtual placeholder* clone() const = 0;
    };

    template <typename ValueType>
    class holder : public placeholder {
    public:
        holder(const ValueType& value) : held(value) { }

        virtual const std::type_info& type() const
        {
            return typeid(ValueType);
        }

        virtual placeholder* clone() const
        {
            return new holder(held);
        }

    private:
        holder& operator=(const holder&) = delete;

    public:
        ValueType held;
    };

    placeholder* m_content;
};


inline void swap(any_t& lhs, any_t& rhs)
{
    lhs.swap(rhs);
}


template <typename ValueType>
inline ValueType* any_cast(any_t* operand)
{
    return operand->cast_to_pointer<ValueType>();
}


template <typename ValueType>
inline ValueType& any_cast(any_t& operand)
{
    return operand.cast_to_ref<ValueType>();
}

UJVM_NAMESPACE_END

#endif