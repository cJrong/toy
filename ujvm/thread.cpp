

#include "thread.h"
#include "opcode.h"


UJVM_NAMESPACE_BEGIN

bool execute_method(thread_t *thread);

bool thread_t::invoke_method(std::shared_ptr<class_t::method_t> method)
{
    auto retpc = pc;

    codes = &method->code;
    pc = 0;
    execute_method(this);
    pc = retpc;

    return true;
}

void thread_t::invoke_main_method(void)
{
    auto method = uclass.get_method("main", "([Ljava/lang/String;)V", class_t::method_t::ACC_PUBLIC_TAG | class_t::method_t::ACC_STATIC_TAG);
    auto frame = std::make_shared<stack_frame_item_t>();
    frame->operand_stack = std::make_shared<operand_stack_t>();
    frame->local_variable_table = std::make_shared<local_variable_table_t>(method->max_locals);
    stack_frame.push(frame);
    invoke_method(method);
}

UJVM_NAMESPACE_END