%ifndef _OFFS_INC
%define _OFFS_INC

TASK_STRUCT		equ		0
GSREG			equ		TASK_STRUCT
FSREG			equ		GSREG+4
ESREG			equ		FSREG+4
DSREG			equ		ESREG+4
EDIREG			equ		DSREG+4
ESIREG			equ		EDIREG+4
EBPREG			equ		ESIREG+4
KERNEL_ESPREG	equ		EBPREG+4
EBXREG			equ		KERNEL_ESPREG+4
EDXREG			equ		EBXREG+4
ECXREG			equ		EDXREG+4
EAXREG			equ		ECXREG+4
RETADR			equ		EAXREG+4
EIPREG			equ		RETADR+4
CSREG			equ		EIPREG+4
EFLAGSREG		equ		CSREG+4
ESPREG			equ		EFLAGSREG+4
SSREG			equ		ESPREG+4
LDT_SELECTOR	equ		SSREG+4
LDTS			equ		LDT_SELECTOR+4
PID				equ		LDTS+4


TSS_STRUCT		equ		0
BACK_LINK		equ		TSS_STRUCT
ESP0REG			equ		BACK_LINK+4		

%endif 