

#ifndef _THREAD_H_
#define _THREAD_H_

#include "config.h"
#include "any.h"
#include "jvm.h"
#include "class.h"
#include "stack_frame.h"


#include <stack>

UJVM_NAMESPACE_BEGIN

class thread_t {
public:
    thread_t(class_t& c) : uclass(c), pc(0)
    {
    }

    void push_stack_frame(std::shared_ptr<stack_frame_item_t> item)
    {
        stack_frame.push(item);
    }

    void pop_stack_frame()
    {
        if (!stack_frame.empty()) {
            stack_frame.pop();
        }
    }
    
    std::shared_ptr<operand_stack_t> get_operand_stack(void)
    {
        return stack_frame.top()->operand_stack;
    }

    bool invoke_method(std::shared_ptr<class_t::method_t> method);
    void invoke_main_method();


public:
    
    /* runtime registor */
    int pc;
    stack_frame_t stack_frame;
    const std::vector<uint8_t>*  codes;

    /* other */
    jvm_t* jvm;
    class_t& uclass;
};

UJVM_NAMESPACE_END

#endif