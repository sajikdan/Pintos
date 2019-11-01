#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/user/syscall.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/process.h"
#include <stdlib.h>
#include <stdint.h>

#define BLANK sizeof(uint32_t)

bool is_user_vad(void *addr);
static void syscall_handler(struct intr_frame *);


void
syscall_init(void)
{
	intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
	void *f_esp = f->esp;
	uint32_t sysnum = *(uint32_t *)f->esp;
	is_user_vad(f_esp);

	/*printf("syscall : %d\n", sysnum);
	hex_dump (f->esp, f->esp, 100,1);*/

	switch (sysnum) {
	case SYS_HALT:
		shutdown_power_off();
		break;


	case SYS_WAIT :
		f->eax = wait((pid_t)*(uint32_t *) (f_esp+BLANK));
		break;

	case SYS_EXIT:
		if (!is_user_vad(f_esp + BLANK)){
			//printf("\n\n%d\n\n",1);
			exit(-1);
		}

		exit((int) *(uint32_t *)(f_esp + BLANK));
		//printf("\n\n%d\n\n",2);
		
		break;

	case SYS_EXEC:
		if (!is_user_vad(f_esp + BLANK))
			exit(-1);

		f->eax = exec((const char*) *(uint32_t *)(f_esp + BLANK));
		break;

	case SYS_READ:
		f->eax = read((int) *(uint32_t *)(f_esp + BLANK),
			(void *) *(uint32_t *)(f_esp + 2 * BLANK),
			(unsigned) *((uint32_t *)(f_esp + 3 * BLANK)));
		break;

	case SYS_WRITE:
		f->eax = write((int) *(uint32_t *)(f_esp + BLANK),
			(void*) *(uint32_t *)(f_esp + 2 * BLANK),
			(unsigned) *((uint32_t *)(f_esp + 3 * BLANK)));
		break;

	default:
		break;
	}
}


void exit(int status) {
	thread_current() -> exitstat = status;

	printf("%s: exit(%d)\n", thread_name(), status);
	//printf("-----------------------------------///\n");
	thread_exit();
	//printf("-===============-----------=======\n");
}

int exec(const char *file) {
	return process_execute(file);
}

int wait(pid_t pid) {
	return process_wait(pid);
}

int read(int fd, void *buffer, unsigned size) {
	unsigned i = 0;

	if (fd == 1) {
		while (((char *)buffer)[i] != '\0' && ++i < size);
	}
	return i;
}

int write(int fd, const void *buffer, unsigned size) {
	if (fd == 1){
		putbuf(buffer, size);
		return size;}
	return -1;
}


bool is_user_vad(void *addr) {
	if (!is_user_vaddr(addr)) return false;
	else return true;
}

int fibonacci(int n){
	int f1 = 1;
	int f2 = 1;
	int f3 = 1;
	int i;

	if(n == 0){
		return 0;
	}
	for (i = 2; i < n; i++)
	{
		f3 = f2 + f1;
		f1 = f2;
		f2 = f3;
	}
	return f3;
}

int sum_of_four_int(int a, int b, int c, int d){
	return (a + b + c + d);
}