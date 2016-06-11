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
	int res;	
    //printk("fs->path:%s\n",fs->path);
	res = f_mount(fs->data, fs->path,1 );
	return res;
//FRESULT f_mount (FATFS* fs, const TCHAR* path, BYTE opt);			/* Mount/Unmount a logical drive */
    //return f_mount(fs, fs->path, fs->data);
//    return -STATUS_EIO;
}

/* Note: Just call f_mkfs at root path '/' */
int fat_mkfs(const char* device_name)
{
	return f_mkfs('/',0,0);
}

/* Note: Convert the POSIX's open flag to elmfat's flag.
*        Example: if file->flags == O_RDONLY then open_mode = FA_READ
*                 if file->flags & O_APPEND then f_seek the file to end after f_open
*        Reference: http://elm-chan.org/fsw/ff/en/open.html 
*/
int fat_open(struct fs_fd* file)
{
	FRESULT ret;
	FIL fp;
	TCHAR* path;
	BYTE mode;
	int flag4= file->flags;
	int flag3= file->flags;
	int flag2= file->flags;
	int flag = 0;	
	switch(flag3&0x0f){ //check the final 2 bit
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
	switch(flag2&0xf00){
		case O_CREAT:
		if(flag4&0xf000 == O_TRUNC)
			flag |= FA_CREATE_ALWAYS;
		break;
		case O_EXCL:
		flag |= FA_CREATE_NEW;
		break;
	
	}
	path = file->path;
	ret=f_open(file->data, path, flag);

//	printk("fat_open ret:%d\n",ret);
return ret;
//#define O_CREAT			0x0000100
//#define O_EXCL			0x0000200
//#define O_TRUNC			0x0001000
//#define O_APPEND		0x0002000
//#define O_DIRECTORY		0x0200000
	
}

int fat_close(struct fs_fd* file)
{
	
	return f_close(file->data);
}
int fat_read(struct fs_fd* file, void* buf, size_t count)
{
	FIL fp;
	UINT br;
	f_read(file->data, buf, count, &br);
	printk("fat_read br:%d\n",br);

//	FRESULT f_read (
//	FIL* fp, 	/* Pointer to the file object */
//	void* buff,	/* Pointer to data buffer */
//	UINT btr,	/* Number of bytes to read */
//	UINT* br	/* Pointer to number of bytes read */
//)
}
int fat_write(struct fs_fd* file, const void* buf, size_t count)
{
//FRESULT f_write (FIL* fp, const void* buff, UINT btw, UINT* bw);	/* Write data to a file */
	UINT bw;
	int ret;
	ret = f_write((FIL*)file->data, buf, count, &bw);
	if(ret == 0)
		return bw;
	printk("fat_write error -> ret:%d , bw:%d\n",ret,bw);
	return ret;
}
int fat_lseek(struct fs_fd* file, off_t offset)
{	
	return f_lseek(file->data, offset);
}
int fat_unlink(struct fs_fd* file, const char *pathname)
{
	return f_unlink(pathname);

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



