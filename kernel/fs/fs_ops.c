/* This file use for NCTU OSDI course */
/* It's contants fat file system operators */

#include <inc/stdio.h>
#include <fs.h>
#include <fat/ff.h>
#include <diskio.h>

extern struct fs_dev fat_fs;

/*TODO: Lab7, fat level file operator.
*       Implement below functions to support basic file system operators by using the elmfat's API(f_xxx).
*       Reference: http://elm-chan.org/fsw/ff/00index_e.html
*/

/* Note: 1. Get FATFS object from fs->data
*        2. Check fs->path parameter then call f_mount.
*/
int fat_mount(struct fs_dev *fs, const void* data)
{

}

/* Note: Just call f_mkfs at root path '/' */
int fat_mkfs(const char* device_name)
{

}

/* Note: Convert the POSIX's open flag to elmfat's flag.
*        Example: if file->flags == O_RDONLY then open_mode = FA_READ
*                 if file->flags & O_APPEND then f_seek the file to end after f_open
*        Reference: http://elm-chan.org/fsw/ff/en/open.html 
*/
int fat_open(struct fs_fd* file)
{
	//FRESULT f_open (
	//FIL* fp,			/* Pointer to the blank file object */
	//const TCHAR* path,	/* Pointer to the file name */
	//BYTE mode			/* Access mode and file open mode flags */
	//)
	FRESULT ret;
	FIL* fp;
	TCHAR* path;
	BYTE mode;
	int flag = file->flags;	
	switch(flag&0x0f){ //check the final 2 bit
		case O_RDONLY:
		flag = FA_READ;
		break;
		case O_WRONLY:
		flag = FA_WRITE;
		break;
		case O_RDWR:
		flag = FA_READ&FA_WRITE;
		break;
		case O_ACCMODE:	
		flag = FA_READ&FA_WRITE;
	//	panic("ACCMODE\n");
		break;
	}
	switch(flag&0xf00){
		case O_CREAT:
		if(flag&0xf000 == O_TRUNC)
			flag |= FA_CREATE_ALWAYS;
		break;
		case O_EXCL:
		flag |= FA_CREATE_NEW;
		break;
	
	}
	path = file->path;
	ret=f_open(file, path, mode);
	printk("ret:%d\n",ret);
return ret;
//	switch(flag&0xf000){
//
//	}
//	if(flag == O_RDONLY)
//#define O_CREAT			0x0000100
//#define O_EXCL			0x0000200
//#define O_TRUNC			0x0001000
//#define O_APPEND		0x0002000
//#define O_DIRECTORY		0x0200000
	
}

int fat_close(struct fs_fd* file)
{

}
int fat_read(struct fs_fd* file, void* buf, size_t count)
{

}
int fat_write(struct fs_fd* file, const void* buf, size_t count)
{
}
int fat_lseek(struct fs_fd* file, off_t offset)
{
}
int fat_unlink(struct fs_fd* file, const char *pathname)
{
}

struct fs_ops elmfat_ops = {
    .dev_name = "elmfat",
    .mount = fat_mount,
    .mkfs = fat_mkfs,
    .open = fat_open,
    .close = fat_close,
    .read = fat_read,
    .write = fat_write,
    .lseek = fat_lseek
};



