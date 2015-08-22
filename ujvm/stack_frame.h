

#ifndef _STACK_FRAME_H_
#define _STACK_FRAME_H_

#include "config.h"
#include "any.h"

#include <stack>
#include <vector>
#include <cstdint>

UJVM_NAMESPACE_BEGIN

class operand_stack_t {
public:
    inline void push(any_t v)
    {
        m_slots.push(v);
    }

    inline void push_null(void) 
    {
        m_slots.push(nullptr);
    }

    inline void push_int(int32_t v) 
    {
        m_slots.push(v);
    }

    inline void push_long(int64_t v)
    {
        m_slots.push(v);
    }

    inline void push_float(float v)
    {
        m_slots.push(v);
    }

    inline void push_double(double v)
    {
        m_slots.push(v);
    }

    inline any_t pop_any(void)
    {
        auto v = m_slots.top();
        m_slots.pop();
        return v;
    }

    inline int32_t pop_int(void) 
    {
        auto v = m_slots.top();
        m_slots.pop();
        return v.cast_to_ref<int32_t>();
    }

    inline int64_t pop_long(void)
    {
        auto v = m_slots.top();
        m_slots.pop();
        return v.cast_to_ref<int64_t>();
    }

    inline float pop_float(void)
    {
        auto v = m_slots.top();
        m_slots.pop();
        return v.cast_to_ref<float>();
    }

    inline double pop_double(void)
    {
        auto v = m_slots.top();
        m_slots.pop();
        return v.cast_to_ref<double>();
    }

    inline bool empty(void) 
    {
        return m_slots.empty();
    }

private:
    std::stack<any_t> m_slots;
};

class local_variable_table_t {
public:
    local_variable_table_t(int size) 
    {
        m_slots.resize(size);
    }

    inline int32_t get_int(int index)
    {
        return m_slots[index].cast_to_ref<int32_t>();
    }

    inline int64_t get_long(int index) 
    {
        return m_slots[index].cast_to_ref<int64_t>();
    }

    inline float get_float(int index)
    {
        return m_slots[index].cast_to_ref<float>();
    }

    inline double get_double(int index)
    {
        return m_slots[index].cast_to_ref<double>();
    }

    inline void set_int(int index, int32_t v)
    {
        m_slots[index] = v;
    }

    inline void set_long(int index, int64_t v) 
    {
        m_slots[index] = v;
    }

    inline void set_float(int index, float v)
    {
        m_slots[index] = v;
    }

    inline void set_double(int index, double v)
    {
        m_slots[index] = v;
    }

    inline any_t get(int index)
    {
        return m_slots[index];
    }

    inline void set(int index, any_t v)
    {
        m_slots[index] = v;
    }

private:
    std::vector<any_t> m_slots;
};

struct stack_frame_item_t {
    std::shared_ptr<operand_stack_t> operand_stack;
    std::shared_ptr<local_variable_table_t> local_variable_table;
};

typedef std::stack<std::shared_ptr<stack_frame_item_t>> stack_frame_t;

UJVM_NAMESPACE_END

#endif