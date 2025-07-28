#include <kernel/isr.h>
#include <kernel/panic.h>

static const char* exception_messages[] = {
	"Divide by zero"
};

void isr_handler(registers_t regs) {
	kpanic(exception_messages[regs.int_no]);
}