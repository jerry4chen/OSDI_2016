/* This file use for NCTU OSDI course */

// It's handel the file system APIs 
#include <inc/stdio.h>
#include <inc/syscall.h>
#include <fs.h>

/*TODO: Lab7, file I/O system call interface.*/
/*NOte: Here you need handle the file system call from user.
*       1. When user open a new file, you can use the fd_new() to alloc a file object(struct fs_fd)
*       2. When user R/W the file, use the fd_get() to get file object.
*       3. After get file object call file_* functions into VFS level
*       4. Update the file objet's position or size when user R/W or seek the file.(You can find the useful marco in ff.h)
*       5. Handle the error code, for example, if user call open() but no fd slot can be use, sys_open should return -STATUS_ENOSPC.
*/

// Below is POSIX like I/O system call 
int sys_open(const char *file, int flags, int mode)
{
    //We dont care the mode.
/* TODO */
	int idx = fd_new();	
	struct fs_fd* d;
	d = fd_get(idx);
	//printk("constcharfile:%s\n",file);
	int ret = file_open(d, file, flags);
	if(ret ==0)
	return idx;
	else 
	{
	printk("sys_open error:%d",ret);
	return ret;
	}
	
	//int file_open(struct fs_fd* fd, const char *path, int flags)
	
}

int sys_close(int fd)
{
/* TODO */
	file_close(fd_get(fd));
//	fd_put(fd);
	return 0;

}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	return file_read(fd_get(fd), buf, len);
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	return file_write(fd_get(fd), buf, len);
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	off_t newoffset;
	switch(whence){
	case SEEK_SET:
		newoffset = offset;
		break;
	case SEEK_END:
		newoffset = fd_get(fd)+offset;
		break;
	}
	return file_lseek(fd, newoffset);
}

int sys_unlink(const char *pathname)
{
/* TODO */
	return file_unlink(pathname); 
}


              

