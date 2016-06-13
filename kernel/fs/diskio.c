/* This file use for NCTU OSDI course */
#include <fs.h>
#include <fat/diskio.h>
#include <fat/ff.h>
#include <kernel/drv/disk.h>
#include <kernel/timer.h>
#define DISK_ID 1

#define MAX_DRIVES 10

/*TODO: Lab7, low level file operator.*/

/**
  * @brief  Initial IDE disk
  * @param  pdrv: disk type
  * @retval disk error status
  *         - RES_OK: Initial success
  *         - STA_NOINIT: Intial failed
  */
DSTATUS disk_initialize (BYTE pdrv)
{
   // printk("disk_initialize %d\n", pdrv);
    /* TODO */
/*	switch(pdrv){
		case IDE_ATA :
		disk_init();	
		return RES_OK;
		
		case IDE_ATAPI:
		disk_init();
		return RES_OK;

	}	
  */  //return disk_initialize();
//	disk_init();
	
/*	DSTATUS sta;
	if(pdrv >= MAX_DRIVES){
		sta = STA_NODISK;
	}else{
		if(ide_devices[DISK_ID].Reserved==0){
			disk_init();
			sta= RES_OK;
		}else sta = STA_NOINIT;
	}
	return sta;
	
  */  
	if(ide_devices[DISK_ID].Reserved==9){
			disk_init();
			return RES_OK;
		}else 	return RES_OK;
}

/**
  * @brief  Get disk current status
  * @param  pdrv: disk type
  * @retval disk status
  *         - RES_OK: Normal status
  *         - STA_NODISK: ...
  */
DSTATUS disk_status (BYTE pdrv)
{
    //printk("disk_status\n");
    /* TODO */
		//ide_status();
  //  		printk("disk_status\n");
    	return RES_OK;
	//return STA_NODISK;
}

/**
  * @brief  Read serval sector form a IDE disk
  * @param  pdrv: disk type
  * @param  buff: destination memory start address
  * @param  sector: start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    
 //   printk("disk_read %d sector %d count %d\n", pdrv, sector, count);
    
    /* TODO */
	//int ide_read_sectors(unsigned char drive, unsigned char numsects, unsigned int lba, unsigned int edi);
//	ide_read_sectors(DISK_ID, count,i,(unsigned int)ptr );	
 //   for (i = 0; i < 10; i++)
 //   {    
 //       for (j = 0; j < SECTOR_SIZE; j++)
   //         buf[j] = 0x0;
	for  (i=0;i<count;i++){
	
		ide_read_sectors(DISK_ID, 1, cur_sector, (unsigned int)ptr+i*512);
		cur_sector ++;
	}
	return RES_OK;
}


/**
  * @brief  Write serval sector to a IDE disk
  * @param  pdrv: disk type
  * @param  buff: memory start address
  * @param  sector: destination start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    
    printk("disk_write %d sector %d count %d\n", pdrv, sector, count);
    /* TODO */    
        	//ide_write_sectors(DISK_ID, count, cur_sector, (unsigned int)ptr);
		//return RES_OK;
        	for (i=0; i < count; i++)
			ide_write_sectors(DISK_ID,1 , cur_sector+i, (unsigned int)ptr+i*512);
		return RES_OK;
}

/**
  * @brief  Get disk information form disk
  * @param  pdrv: disk type
  * @param  cmd: disk control command (See diskio.h)
  *         - GET_SECTOR_COUNT
  *         - GET_BLOCK_SIZE (Same as sector size)
  * @param  buff: return memory space
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    uint32_t *retVal = (uint32_t *)buff;
//    printk("disk_ioctl drv=%d cmd=%d\n", pdrv, cmd);
    /* TODO */    
	switch(cmd){
		case GET_SECTOR_COUNT:
			*retVal =ide_devices[DISK_ID].Size;	
			return RES_OK;
		case GET_BLOCK_SIZE:
		case GET_SECTOR_SIZE:
			*retVal = 512; 
			return RES_OK;
	}	
	return 0;
}


/**
  * @brief  Get OS timestamp
  * @retval tick of CPU
  */
DWORD get_fattime (void)
{
    /* TODO */
	return sys_get_ticks();	
//    return 0;
}
