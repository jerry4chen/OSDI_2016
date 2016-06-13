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
	int ret;
	if(idx != -1){
			d = fd_get(idx);
			ret = file_open(d, file, flags);
			if(ret ==0){
			return idx;
			}	
			else{
			fd_put(d);
			return -ret;//file open error;
			}	
	}
	else {
			return idx;//return -1;
		}
	
	
	//int file_open(struct fs_fd* fd, const char *path, int flags)
	
}

int sys_close(int fd)
{
/* TODO */
	
 	struct fs_fd* d = fd_get(fd);
	file_close(d);
	fd_put(d);
	fd_put(d);
	return 0;

}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	int res;
 	struct fs_fd* d;
	d = fd_get(fd);
	res = file_read(d, buf, len);
	fd_put(d);
	fd_put(d);
	return res;
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	int res;
 	struct fs_fd* d;
	d = fd_get(fd);
	res = file_write(d, buf, len);
	fd_put(d);
	fd_put(d);
	return res;
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	off_t newoffset;
 	struct fs_fd* d;
	d = fd_get(fd);
	
	switch(whence){
	case SEEK_SET:
		newoffset = offset;
		break;
	case SEEK_END:
		newoffset = d+offset;
		break;
	}
	
	int res =  file_lseek(d, newoffset);
	fd_put(d);
	return res;
}

int sys_unlink(const char *pathname)
{
/* TODO */
	return file_unlink(pathname); 
}


              

