#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "lib/user/syscall.h"
#include "threads/vaddr.h"
#include "devices/shutdown.h"
#include "userprog/process.h"

#include "filesys/off_t.h"
#include <stdlib.h>
#include <stdint.h>

#define BLANK sizeof(uint32_t)

struct file 
  {
    struct inode *inode;        /* File's inode. */
    off_t pos;                  /* Current position. */
    bool deny_write;            /* Has file_deny_write() been called? */
  };

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
		else {
			exit((int) *(uint32_t *)(f_esp + BLANK));
		//printf("\n\n%d\n\n",2);
		}
		break;

	case SYS_EXEC:
		if (!is_user_vad(f_esp + BLANK))
			exit(-1);

		else {
			f->eax = exec((const char*) *(uint32_t *)(f_esp + BLANK));
		}
		break;

	case SYS_READ:
		if (!is_user_vad(f_esp + BLANK))
			exit(-1);
		else if (!is_user_vad(f_esp + 2 * BLANK))
			exit(-1);
		else if (!is_user_vad(f_esp + 3 * BLANK))
			exit(-1);
		else {
		//hex_dump(f_esp, f_esp, 100, 1);
			f->eax = read((int) *(uint32_t *)(f_esp + BLANK),
				(void *) *(uint32_t *)(f_esp + 2 * BLANK),
				(unsigned) *((uint32_t *)(f_esp + 3 * BLANK)));
		}
		break;

	case SYS_WRITE:
		f->eax = write((int) *(uint32_t *)(f_esp + BLANK),
			(void*) *(uint32_t *)(f_esp + 2 * BLANK),
			(unsigned) *((uint32_t *)(f_esp + 3 * BLANK)));
		break;

	case SYS_FIB:
		f->eax = fibonacci((int) *(uint32_t *)(f_esp + BLANK));
		break;

	case SYS_SUM:
		f->eax = sum_of_four_int((int) *(uint32_t *)(f_esp + BLANK),
			(int) *(uint32_t *)(f_esp + 2 * BLANK),
			(int) *(uint32_t *)(f_esp + 3 * BLANK),
			(int) *(uint32_t *)(f_esp + 4 * BLANK));
		break;

	case SYS_CREATE:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else if (!is_user_vad(f_esp + 2 * BLANK)) {
			exit(-1);
		}
		else {
			f->eax = create((const char*) *(uint32_t *)(f_esp + BLANK), (unsigned)*(uint32_t *)(f_esp + 2 * BLANK));
		}
		break;

	case SYS_REMOVE :
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else {
			f->eax = remove((const char*) *(uint32_t *)(f_esp + BLANK));
		}
		break;
	
	case SYS_OPEN:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else {
			f->eax = open((const char*) *(uint32_t *)(f_esp + BLANK));
		}
		break;

	case SYS_FILESIZE:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else {
			f->eax = filesize((int) *(uint32_t *)(f_esp + BLANK));
		}
		break;
	
	case SYS_SEEK:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else if (!is_user_vad(f_esp + 2 * BLANK)) {
			exit(-1);
		}
		else {
			seek((int) *(uint32_t *)(f_esp + BLANK), (unsigned)*(uint32_t *)(f_esp + 2 * BLANK));
		}
		break;

	case SYS_TELL:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
		}
		else {
			f->eax = tell((int) *(uint32_t *)(f_esp + BLANK));
		}
		break;

	case SYS_CLOSE:
		if (!is_user_vad(f_esp + BLANK)) {
			exit(-1);
			printf("-------------is_user_vad in SYS_CLOSE\n");
		}
		//hex_dump(f_esp, f_esp, 100, 1);
		else {
			close((int) *(uint32_t *)(f_esp + BLANK));
		}
		break;

	default:
		break;
	}
}


void exit(int status) {
	thread_current() -> exitstat = status;
	int i;

	printf("%s: exit(%d)\n", thread_name(), status);
	//printf("-----------------------------------///\n");
	for (i = 3; i < 128; i++) {
		if (thread_current()->fd[i] != NULL) {
			close(i);
		}
	}
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

	if (fd == 0) {
		while (((char *)buffer)[i] != '\0' && ++i < size);
	}
	else if (fd > 2) {
			if (thread_current()->fd[fd] == NULL) {
			//if no such file is open, then exit
			exit(-1);
		}
		return file_read(thread_current()->fd[fd], buffer, size);
	}
	return i;
}

int write(int fd, const void *buffer, unsigned size) {
	if (fd == 1){
		putbuf(buffer, size);
		return size;
	}
	else if (fd > 2) {
		//if no such file is open, then exit
		if (thread_current()->fd[fd] == NULL) {
			exit(-1);
		}
		if (thread_current()->fd[fd]->deny_write) {
			file_deny_write(thread_current()->fd[fd]);
		}

		return file_write(thread_current()->fd[fd], buffer, size);
	}
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

bool create (const char* file, unsigned initial_size) {
	//invalid file name
	if (file == NULL) {
		exit(-1);
	}
	//invalid address
	if (!is_user_vad(file)) {
		exit(-1);
	}
	return filesys_create(file, initial_size);
}

bool remove (const char* file) {
	//invalid file name
	if (file == NULL) {
		exit(-1);
	}
	//invalid address
	if (!is_user_vad(file)) {
		exit(-1);
	}
	return filesys_remove(file);
}

int open(const char* file){
	int i;
	struct file* fp;
	//invalid file name
	if (file == NULL) {
		exit(-1);
	}
	//invalid address
	if (!is_user_vad(file)) {
		exit(-1);
	} 
	
	fp = filesys_open(file);

	//no such file
	if (fp == NULL) {
		return -1;
	}
	else {
		//open file in a thread file descriptor
		for (i = 3; i < 128; i++) {
			if (thread_current()->fd[i] == NULL) {
				if (strcmp(thread_current()->name, file) == 0) {
					file_deny_write(fp);
				}
				thread_current()->fd[i] = fp;
				return i;
			}
		}
	}
	return -1;
}

int filesize (int fd) {
	//if no such file is open, then exit
	if (thread_current()->fd[fd] == NULL)
		exit(-1);

	return file_length(thread_current()->fd[fd]);
}

void seek (int fd, unsigned position) {
	//if no such file is open, then exit
	if (thread_current()->fd[fd] == NULL)
		exit(-1);

	file_seek(thread_current()->fd[fd], position);
}

unsigned tell (int fd) {
	//if no such file is open, then exit
	if (thread_current()->fd[fd] == NULL)
		exit(-1);

	return file_tell(thread_current()->fd[fd]);
}

void close (int fd) {
	struct file* fp;
	//if no such file is open, then exit
	if (thread_current()->fd[fd] == NULL)
	{
		//printf("-------------------------thread_current()->fd[fd] == NULL\n");
		exit(-1);
	}
	fp = thread_current()->fd[fd];
	thread_current()->fd[fd] = NULL;
	return file_close(fp);
	//file_close(thread_current()->fd[fd]);
	//printf("-----------=-=--=file closed!\n");
	//printf("%d\n", sizeof(uint32_t));
}