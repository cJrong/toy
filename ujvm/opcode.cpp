
#include "opcode.h"
#include "class_file.h"

#include <string>
#include <cstdint>
#include <iostream>

UJVM_NAMESPACE_BEGIN

struct execute_table_entry_t {
    uint8_t     opcode;
    std::string opname;
    bool (*exec)(thread_t* thread, std::string opname);
};

static bool execute_nop(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aconst_null(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_m1(thread_t *thread, std::string opname)
{

    return true;
}


static bool execute_iconst_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_4(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iconst_5(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lconst_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lconst_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fconst_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fconst_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fconst_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dconst_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dconst_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_bipush(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_sipush(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ldc(thread_t *thread, std::string opname)
{
    auto& constant_pool = thread->uclass.constant_pool;
    auto  value = constant_pool[(*thread->codes)[thread->pc++]];

    thread->get_operand_stack()->push(std::static_pointer_cast<constant_t>(value));

    return true;
}


static bool execute_ldc_w(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ldc2_w(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iload_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iload_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iload_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iload_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lload_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lload_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lload_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lload_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fload_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fload_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fload_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fload_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dload_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dload_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dload_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dload_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aload_0(thread_t *thread, std::string opname)
{
    
    return true;
}


static bool execute_aload_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aload_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aload_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iaload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_laload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_faload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_daload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aaload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_baload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_caload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_saload(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_istore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lstore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fstore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dstore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_astore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_istore_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_istore_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_istore_2(thread_t *thread, std::string opname)
{
    return true;
}

static bool execute_istore_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lstore_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lstore_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lstore_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lstore_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fstore_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fstore_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fstore_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fstore_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dstore_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dstore_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dstore_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dstore_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_astore_0(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_astore_1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_astore_2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_astore_3(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_aastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_bastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_castore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_sastore(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_pop(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_pop2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup_x1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup_x2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup2_x1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dup2_x2(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_swap(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iadd(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ladd(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fadd(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dadd(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_isub(thread_t *thread, std::string opname)
{
    return true;
}

static bool execute_lsub(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fsub(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dsub(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_imul(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lmul(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fmul(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dmul(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_idiv(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ldiv(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fdiv(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ddiv(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_irem(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lrem(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_frem(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_drem(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ineg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lneg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fneg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dneg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ishl(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lshl(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ishr(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lshr(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iushr(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lushr(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iand(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_land(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ior(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lor(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ixor(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lxor(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iinc(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2l(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2f(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2d(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_l2i(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_l2f(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_l2d(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_f2i(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_f2l(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_f2d(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_d2i(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_d2l(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_d2f(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2b(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2c(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_i2s(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lcmp(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fcmpl(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_fcmpg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dcmpl(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dcmpg(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifeq(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifne(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_iflt(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifge(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifgt(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifle(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmpeq(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmpne(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmplt(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmpge(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmpgt(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_icmple(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_acmpeq(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_if_acmpne(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_goto(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_jsr(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ret(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_tableswitch(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lookupswitch(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ireturn(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_lreturn(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_freturn(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_dreturn(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_areturn(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_return(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_getstatic(thread_t *thread, std::string opname)
{
    auto& constant_pool = thread->uclass.constant_pool;
    uint8_t index_byte1 = (*thread->codes)[thread->pc++];
    uint8_t index_byte2 = (*thread->codes)[thread->pc++];
    uint16_t index = (index_byte1 << 8) | index_byte2;

    auto field_ref = std::static_pointer_cast<constant_field_ref_t>(constant_pool[index]);

    auto field_ref_class = std::static_pointer_cast<constant_class_t>(constant_pool[field_ref->class_index]);

    auto field_ref_class_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[field_ref_class->name_index]);

    thread->jvm->load_class(field_ref_class_name->str + ".class");

    auto field_ref_name_and_type = std::static_pointer_cast<constant_field_ref_t>(constant_pool[field_ref->name_and_type_index]);

    auto field_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[field_ref_name_and_type->class_index]);
    
    thread->get_operand_stack()->push(field_ref_class_name->str + "/" + field_name->str);

    return true;
}


static bool execute_putstatic(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_getfield(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_putfield(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_invokevirtual(thread_t *thread, std::string opname)
{
    auto& constant_pool = thread->uclass.constant_pool;
    uint8_t index_byte1 = (*thread->codes)[thread->pc++];
    uint8_t index_byte2 = (*thread->codes)[thread->pc++];
    uint16_t index = (index_byte1 << 8) | index_byte2;

    auto method_ref = std::static_pointer_cast<constant_method_ref_t>(constant_pool[index]);

    auto method_ref_class = std::static_pointer_cast<constant_class_t>(constant_pool[method_ref->class_index]);

    auto method_ref_name_and_type = std::static_pointer_cast<constant_name_and_type_t>(constant_pool[method_ref->name_and_type_index]);

    auto method_ref_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[method_ref_name_and_type->name_index]);

    auto method_ref_descriptor = std::static_pointer_cast<constant_utf8_t>(constant_pool[method_ref_name_and_type->descriptor_index]);
    
    auto field_ref_class_name = std::static_pointer_cast<constant_utf8_t>(constant_pool[method_ref_class->name_index]);

/*
    auto c = thread->jvm->load_class(field_ref_class_name->str + ".class");

    auto method = c->get_method(method_ref_name->str, method_ref_descriptor->str, class_t::method_t::ACC_PUBLIC_TAG);

    auto frame = std::make_shared<stack_frame_item_t>();
    frame->operand_stack = std::make_shared<operand_stack_t>();
    frame->local_variable_table = std::make_shared<local_variable_table_t>(method->max_locals);
    thread->stack_frame.push(frame);
    thread->invoke_method(method);
*/
    if (method_ref_name->str == "println") {
        auto v = thread->get_operand_stack()->pop_any().cast_to_ref<std::shared_ptr<constant_t>>();
        
        switch (v->tag)
        {
        case CONSTANT_STRING_TAG:
            {
                auto index = std::static_pointer_cast<constant_string_t>(v)->string_index;
                std::cout << std::static_pointer_cast<constant_utf8_t>(constant_pool[index])->str << std::endl;
            }
        default:
            break;
        }
        
    }

    return true;
}


static bool execute_invokespecial(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_invokestatic(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_invokeinterface(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_invokedynamic(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_new(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_newarray(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_anewarray(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_arraylength(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_athrow(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_checkcast(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_instanceof(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_monitorenter(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_monitorexit(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_wide(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_multianewarray(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifnull(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_ifnonnull(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_goto_w(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_jsr_w(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_breakpoint(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_impdep1(thread_t *thread, std::string opname)
{
    return true;
}


static bool execute_impdep2(thread_t *thread, std::string opname)
{
    return true;
}

#undef  EXEC_TABLE_ITEM
#define EXEC_TABLE_ITEM(opname) \
    {opcode_##opname, #opname, execute_##opname},

execute_table_entry_t execute_table[] = {
    EXEC_TABLE_ITEM(nop)
    EXEC_TABLE_ITEM(aconst_null)
    EXEC_TABLE_ITEM(iconst_m1)
    EXEC_TABLE_ITEM(iconst_0)
    EXEC_TABLE_ITEM(iconst_1)
    EXEC_TABLE_ITEM(iconst_2)
    EXEC_TABLE_ITEM(iconst_3)
    EXEC_TABLE_ITEM(iconst_4)
    EXEC_TABLE_ITEM(iconst_5)
    EXEC_TABLE_ITEM(lconst_0)
    EXEC_TABLE_ITEM(lconst_1)
    EXEC_TABLE_ITEM(fconst_0)
    EXEC_TABLE_ITEM(fconst_1)
    EXEC_TABLE_ITEM(fconst_2)
    EXEC_TABLE_ITEM(dconst_0)
    EXEC_TABLE_ITEM(dconst_1)
    EXEC_TABLE_ITEM(bipush)
    EXEC_TABLE_ITEM(sipush)
    EXEC_TABLE_ITEM(ldc)
    EXEC_TABLE_ITEM(ldc_w)
    EXEC_TABLE_ITEM(ldc2_w)
    EXEC_TABLE_ITEM(iload)
    EXEC_TABLE_ITEM(lload)
    EXEC_TABLE_ITEM(fload)
    EXEC_TABLE_ITEM(dload)
    EXEC_TABLE_ITEM(aload)
    EXEC_TABLE_ITEM(iload_0)
    EXEC_TABLE_ITEM(iload_1)
    EXEC_TABLE_ITEM(iload_2)
    EXEC_TABLE_ITEM(iload_3)
    EXEC_TABLE_ITEM(lload_0)
    EXEC_TABLE_ITEM(lload_1)
    EXEC_TABLE_ITEM(lload_2)
    EXEC_TABLE_ITEM(lload_3)
    EXEC_TABLE_ITEM(fload_0)
    EXEC_TABLE_ITEM(fload_1)
    EXEC_TABLE_ITEM(fload_2)
    EXEC_TABLE_ITEM(fload_3)
    EXEC_TABLE_ITEM(dload_0)
    EXEC_TABLE_ITEM(dload_1)
    EXEC_TABLE_ITEM(dload_2)
    EXEC_TABLE_ITEM(dload_3)
    EXEC_TABLE_ITEM(aload_0)
    EXEC_TABLE_ITEM(aload_1)
    EXEC_TABLE_ITEM(aload_2)
    EXEC_TABLE_ITEM(aload_3)
    EXEC_TABLE_ITEM(iaload)
    EXEC_TABLE_ITEM(laload)
    EXEC_TABLE_ITEM(faload)
    EXEC_TABLE_ITEM(daload)
    EXEC_TABLE_ITEM(aaload)
    EXEC_TABLE_ITEM(baload)
    EXEC_TABLE_ITEM(caload)
    EXEC_TABLE_ITEM(saload)
    EXEC_TABLE_ITEM(istore)
    EXEC_TABLE_ITEM(lstore)
    EXEC_TABLE_ITEM(fstore)
    EXEC_TABLE_ITEM(dstore)
    EXEC_TABLE_ITEM(astore)
    EXEC_TABLE_ITEM(istore_0)
    EXEC_TABLE_ITEM(istore_1)
    EXEC_TABLE_ITEM(istore_2)
    EXEC_TABLE_ITEM(istore_3)
    EXEC_TABLE_ITEM(lstore_0)
    EXEC_TABLE_ITEM(lstore_1)
    EXEC_TABLE_ITEM(lstore_2)
    EXEC_TABLE_ITEM(lstore_3)
    EXEC_TABLE_ITEM(fstore_0)
    EXEC_TABLE_ITEM(fstore_1)
    EXEC_TABLE_ITEM(fstore_2)
    EXEC_TABLE_ITEM(fstore_3)
    EXEC_TABLE_ITEM(dstore_0)
    EXEC_TABLE_ITEM(dstore_1)
    EXEC_TABLE_ITEM(dstore_2)
    EXEC_TABLE_ITEM(dstore_3)
    EXEC_TABLE_ITEM(astore_0)
    EXEC_TABLE_ITEM(astore_1)
    EXEC_TABLE_ITEM(astore_2)
    EXEC_TABLE_ITEM(astore_3)
    EXEC_TABLE_ITEM(iastore)
    EXEC_TABLE_ITEM(lastore)
    EXEC_TABLE_ITEM(fastore)
    EXEC_TABLE_ITEM(dastore)
    EXEC_TABLE_ITEM(aastore)
    EXEC_TABLE_ITEM(bastore)
    EXEC_TABLE_ITEM(castore)
    EXEC_TABLE_ITEM(sastore)
    EXEC_TABLE_ITEM(pop)
    EXEC_TABLE_ITEM(pop2)
    EXEC_TABLE_ITEM(dup)
    EXEC_TABLE_ITEM(dup_x1)
    EXEC_TABLE_ITEM(dup_x2)
    EXEC_TABLE_ITEM(dup2)
    EXEC_TABLE_ITEM(dup2_x1)
    EXEC_TABLE_ITEM(dup2_x2)
    EXEC_TABLE_ITEM(swap)
    EXEC_TABLE_ITEM(iadd)
    EXEC_TABLE_ITEM(ladd)
    EXEC_TABLE_ITEM(fadd)
    EXEC_TABLE_ITEM(dadd)
    EXEC_TABLE_ITEM(isub)
    EXEC_TABLE_ITEM(lsub)
    EXEC_TABLE_ITEM(fsub)
    EXEC_TABLE_ITEM(dsub)
    EXEC_TABLE_ITEM(imul)
    EXEC_TABLE_ITEM(lmul)
    EXEC_TABLE_ITEM(fmul)
    EXEC_TABLE_ITEM(dmul)
    EXEC_TABLE_ITEM(idiv)
    EXEC_TABLE_ITEM(ldiv)
    EXEC_TABLE_ITEM(fdiv)
    EXEC_TABLE_ITEM(ddiv)
    EXEC_TABLE_ITEM(irem)
    EXEC_TABLE_ITEM(lrem)
    EXEC_TABLE_ITEM(frem)
    EXEC_TABLE_ITEM(drem)
    EXEC_TABLE_ITEM(ineg)
    EXEC_TABLE_ITEM(lneg)
    EXEC_TABLE_ITEM(fneg)
    EXEC_TABLE_ITEM(dneg)
    EXEC_TABLE_ITEM(ishl)
    EXEC_TABLE_ITEM(lshl)
    EXEC_TABLE_ITEM(ishr)
    EXEC_TABLE_ITEM(lshr)
    EXEC_TABLE_ITEM(iushr)
    EXEC_TABLE_ITEM(lushr)
    EXEC_TABLE_ITEM(iand)
    EXEC_TABLE_ITEM(land)
    EXEC_TABLE_ITEM(ior)
    EXEC_TABLE_ITEM(lor)
    EXEC_TABLE_ITEM(ixor)
    EXEC_TABLE_ITEM(lxor)
    EXEC_TABLE_ITEM(iinc)
    EXEC_TABLE_ITEM(i2l)
    EXEC_TABLE_ITEM(i2f)
    EXEC_TABLE_ITEM(i2d)
    EXEC_TABLE_ITEM(l2i)
    EXEC_TABLE_ITEM(l2f)
    EXEC_TABLE_ITEM(l2d)
    EXEC_TABLE_ITEM(f2i)
    EXEC_TABLE_ITEM(f2l)
    EXEC_TABLE_ITEM(f2d)
    EXEC_TABLE_ITEM(d2i)
    EXEC_TABLE_ITEM(d2l)
    EXEC_TABLE_ITEM(d2f)
    EXEC_TABLE_ITEM(i2b)
    EXEC_TABLE_ITEM(i2c)
    EXEC_TABLE_ITEM(i2s)
    EXEC_TABLE_ITEM(lcmp)
    EXEC_TABLE_ITEM(fcmpl)
    EXEC_TABLE_ITEM(fcmpg)
    EXEC_TABLE_ITEM(dcmpl)
    EXEC_TABLE_ITEM(dcmpg)
    EXEC_TABLE_ITEM(ifeq)
    EXEC_TABLE_ITEM(ifne)
    EXEC_TABLE_ITEM(iflt)
    EXEC_TABLE_ITEM(ifge)
    EXEC_TABLE_ITEM(ifgt)
    EXEC_TABLE_ITEM(ifle)
    EXEC_TABLE_ITEM(if_icmpeq)
    EXEC_TABLE_ITEM(if_icmpne)
    EXEC_TABLE_ITEM(if_icmplt)
    EXEC_TABLE_ITEM(if_icmpge)
    EXEC_TABLE_ITEM(if_icmpgt)
    EXEC_TABLE_ITEM(if_icmple)
    EXEC_TABLE_ITEM(if_acmpeq)
    EXEC_TABLE_ITEM(if_acmpne)
    EXEC_TABLE_ITEM(goto)
    EXEC_TABLE_ITEM(jsr)
    EXEC_TABLE_ITEM(ret)
    EXEC_TABLE_ITEM(tableswitch)
    EXEC_TABLE_ITEM(lookupswitch)
    EXEC_TABLE_ITEM(ireturn)
    EXEC_TABLE_ITEM(lreturn)
    EXEC_TABLE_ITEM(freturn)
    EXEC_TABLE_ITEM(dreturn)
    EXEC_TABLE_ITEM(areturn)
    EXEC_TABLE_ITEM(return)
    EXEC_TABLE_ITEM(getstatic)
    EXEC_TABLE_ITEM(putstatic)
    EXEC_TABLE_ITEM(getfield)
    EXEC_TABLE_ITEM(putfield)
    EXEC_TABLE_ITEM(invokevirtual)
    EXEC_TABLE_ITEM(invokespecial)
    EXEC_TABLE_ITEM(invokestatic)
    EXEC_TABLE_ITEM(invokeinterface)
    EXEC_TABLE_ITEM(invokedynamic)
    EXEC_TABLE_ITEM(new)
    EXEC_TABLE_ITEM(newarray)
    EXEC_TABLE_ITEM(anewarray)
    EXEC_TABLE_ITEM(arraylength)
    EXEC_TABLE_ITEM(athrow)
    EXEC_TABLE_ITEM(checkcast)
    EXEC_TABLE_ITEM(instanceof)
    EXEC_TABLE_ITEM(monitorenter)
    EXEC_TABLE_ITEM(monitorexit)
    EXEC_TABLE_ITEM(wide)
    EXEC_TABLE_ITEM(multianewarray)
    EXEC_TABLE_ITEM(ifnull)
    EXEC_TABLE_ITEM(ifnonnull)
    EXEC_TABLE_ITEM(goto_w)
    EXEC_TABLE_ITEM(jsr_w)
    EXEC_TABLE_ITEM(breakpoint)
    EXEC_TABLE_ITEM(impdep1)
    EXEC_TABLE_ITEM(impdep2)
};

#undef  EXEC_TABLE_ITEM

bool execute_method(thread_t *thread)
{
    while (thread->pc < static_cast<int>(thread->codes->size())) {
        auto opcode = (*thread->codes)[thread->pc++];
        if (!execute_table[opcode].exec(thread, execute_table[opcode].opname)) {
            return false;
        }
    }

    return true;
}

UJVM_NAMESPACE_END