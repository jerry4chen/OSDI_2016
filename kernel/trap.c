#include <inc/mmu.h>
#include <inc/x86.h>

#include<inc/kbd.h>
#include<inc/timer.h>
#include<inc/stdio.h>
#include<kernel/trap.h>
#include<inc/shell.h>

/* For debugging, so print_trapframe can distinguish between printing
 * a saved trapframe and printing the current trapframe and print some
 * additional information in the latter case.
 */
static struct Trapframe *last_tf;

/* TODO: You should declare an interrupt descriptor table.
 *       In x86, there are at most 256 it.
 *
 * Note: You can leverage the Gatedesc data structure inside mmu.h
 *       Interrupt descriptor table must be built at run time because shifted
 *       function addresses can't be represented in relocation records.
 */
struct Gatedesc idt[256] = {{0}};
struct Pseudodesc idt_pd = {
    sizeof(idt) - 1, (uint32_t) idt    
};

/*
struct Gatedesc  idt[256] = {{0}};
struct Psuedodesc idt_pd = {sizeof(idt)-1, (uint32_t) idt};

*/
//
// Psuedodesc : 
//struct Pseudodesc {
//         uint16_t pd_lim;                // Limit
//         uint32_t pd_base;               // Base address
//} __attribute__ ((packed));		// NO alignment 

/* For debugging */
static const char *trapname(int trapno)
{
	static const char * const excnames[] = {
		"Divide error",
		"Debug",
		"Non-Maskable Interrupt",
		"Breakpoint",
		"Overflow",
		"BOUND Range Exceeded",
		"Invalid Opcode",
		"Device Not Available",
		"Double Fault",
		"Coprocessor Segment Overrun",
		"Invalid TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection",
		"Page Fault",
		"(unknown trap)",
		"x87 FPU Floating-Point Error",
		"Alignment Check",
		"Machine-Check",
		"SIMD Floating-Point Exception"
	};

	if (trapno < sizeof(excnames)/sizeof(excnames[0]))
		return excnames[trapno];
	if (trapno == T_SYSCALL)
		return "System call";
	if (trapno >= IRQ_OFFSET && trapno < IRQ_OFFSET + 16)
		return "Hardware Interrupt";
	return "(unknown trap)";
}

/* For debugging */
void
print_trapframe(struct Trapframe *tf)
{
	cprintf("TRAP frame at %p \n");
	print_regs(&tf->tf_regs);
	cprintf("  es   0x----%04x\n", tf->tf_es);
	cprintf("  ds   0x----%04x\n", tf->tf_ds);
	cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
	// If this trap was a page fault that just happened
	// (so %cr2 is meaningful), print the faulting linear address.
	if (tf == last_tf && tf->tf_trapno == T_PGFLT)
		cprintf("  cr2  0x%08x\n", rcr2());
	cprintf("  err  0x%08x", tf->tf_err);
	// For page faults, print decoded fault error code:
	// U/K=fault occurred in user/kernel mode
	// W/R=a write/read caused the fault
	// PR=a protection violation caused the fault (NP=page not present).
	if (tf->tf_trapno == T_PGFLT)
		cprintf(" [%s, %s, %s]\n",
			tf->tf_err & 4 ? "user" : "kernel",
			tf->tf_err & 2 ? "write" : "read",
			tf->tf_err & 1 ? "protection" : "not-present");
	else
		cprintf("\n");
	cprintf("  eip  0x%08x\n", tf->tf_eip);
	cprintf("  cs   0x----%04x\n", tf->tf_cs);
	cprintf("  flag 0x%08x\n", tf->tf_eflags);
	if ((tf->tf_cs & 3) != 0) {
		cprintf("  esp  0x%08x\n", tf->tf_esp);
		cprintf("  ss   0x----%04x\n", tf->tf_ss);
	}
}

/* For debugging */
void
print_regs(struct PushRegs *regs)
{
	cprintf("  edi  0x%08x\n", regs->reg_edi);
	cprintf("  esi  0x%08x\n", regs->reg_esi);
	cprintf("  ebp  0x%08x\n", regs->reg_ebp);
	cprintf("  oesp 0x%08x\n", regs->reg_oesp);
	cprintf("  ebx  0x%08x\n", regs->reg_ebx);
	cprintf("  edx  0x%08x\n", regs->reg_edx);
	cprintf("  ecx  0x%08x\n", regs->reg_ecx);	
	cprintf("  eax  0x%08x\n", regs->reg_eax);
}


void page_fault_handler(struct Trapframe * tf){

	uint32_t fault_va ;
	fault_va = rcr2();
	cprintf("[0456069] Page fault @%08x\n",fault_va);
	while(1);
}

static void
trap_dispatch(struct Trapframe *tf)
{
	switch(tf->tf_trapno){
	case IRQ_OFFSET+IRQ_KBD :
		//shell();
	//	cprintf("IRQ_KBD\n");	
		kbd_intr();
		//shell();	
		break;
	case IRQ_OFFSET+IRQ_TIMER:
		//shell();
	//	cprintf("IRQ_TIMER\n");
		timer_handler();
		break;
	case T_PGFLT:
		page_fault_handler(tf);
		break;
	default:
		print_trapframe(tf);
	//	cprintf("0456069's trap_dispatch\n");
	}	
  /* TODO: Handle specific interrupts.
   *       You need to check the interrupt number in order to tell
   *       which interrupt is currently happening since every interrupt
   *       comes to this function called by default_trap_handler.
   *
   * NOTE: Checkout the Trapframe data structure for interrupt number,
   *       which we had pushed into the stack when going through the
   *       declared interface in trap_entry.S
   *
   *       The interrupt number is defined in inc/trap.h
   *
   *       We prepared the keyboard handler and timer handler for you
   *       already. Please reference in kernel/kbd.c and kernel/timer.c
   */

	// Unexpected trap: The user process or the kernel has a bug.
//	print_trapframe(tf);
}

/* 
 * Note: This is the called for every interrupt.
 */
void default_trap_handler(struct Trapframe *tf)
{
	// Record that tf is the last real trapframe so
	// print_trapframe can print some additional information.
	last_tf = tf;

	// Dispatch based on what type of trap occurred
	trap_dispatch(tf);
}


void trap_init()
{

	extern void irq0_entry();
	extern void irq1_entry();
	extern void page_fault_entry();
  /* TODO: You should initialize the interrupt descriptor table.
   *       You should setup at least keyboard interrupt and timer interrupt as
   *       the lab's requirement.
   *
   *       Noted that there is another file kernel/trap_entry.S, in which places
   *       all the entry of interrupt handler.
   *       Thus, you can declare an interface there by macro providing there and
   *       use that function pointer when setting up the corresponding IDT entry.
   *
   *       By doing so, we can have more flexibility in adding new IDT entry and 
   *       reuse the routine when interrupt occurs.
   *
   *       Remember to load you IDT with x86 assembly instruction lidt.
   *
   * Note:
   *       You might be benefitted from the macro SETGATE inside mmu.h      
   *       There are defined macros for Segment Selectors in mmu.h
   *       Also, check out inc/x86.h for easy-to-use x86 assembly instruction
   *       There is a data structure called Pseudodesc in mmu.h which might
   *       come in handy for you when filling up the argument of "lidt"
   */
	//SETGATE(gate, istrap, sel, off, dpl)   

	SETGATE(idt[T_PGFLT],0,GD_KT,page_fault_entry,0);	
	/* Keyboard interrupt setup */
	SETGATE(idt[IRQ_OFFSET+IRQ_KBD],0,GD_KT,irq1_entry,0);
	/* Timer Trap setup */
	SETGATE(idt[IRQ_OFFSET+IRQ_TIMER],0,GD_KT,irq0_entry,0);
  /* Load IDT */
	lidt(&idt_pd);
//	asm volatile("lidt idt_pd");
	//lgdt();
}
