#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

enum {
	NVME_CMD_FUSE_FIRST     = (1 << 0),	// 1   : 0000 0001 
        NVME_CMD_FUSE_SECOND    = (1 << 1),	// 2   : 0000 0010
	NVME_CMD_CLEANING	= (1 << 2),	// 4   : 0000 0100

	NVME_CMD_SGL_METABUF    = (1 << 6),	// 64  : 0100 0000
 	NVME_CMD_SGL_METASEG    = (1 << 7),	// 128 : 1000 0000
 	NVME_CMD_SGL_ALL        = NVME_CMD_SGL_METABUF | NVME_CMD_SGL_METASEG,	// 192 : 1100 0000
};

int __init hello_module_init(void)
{
	__u8 flags = NVME_CMD_FUSE_FIRST;

	/* check current flags */
	printk("flags : %u\n", flags);	// flags : 0000 0001(1)

	/* flags usage */
	if(flags & (NVME_CMD_FUSE_FIRST | NVME_CMD_FUSE_SECOND))
		printk("flags have FIRST or SECOND flag\n");
	
	if(!(flags & NVME_CMD_CLEANING))
		printk("flags have not CLEANING flag\n");

	/* add flags */
	flags |= NVME_CMD_CLEANING;	// flags : 0000 0101(5)
	printk("flags : %u\n", flags);

	if(flags & NVME_CMD_CLEANING)
		printk("flags added CLEANING flag\n");

	return 0;
}

void __exit hello_module_cleanup(void)
{
	printk("Bye Module\n");
}

module_init(hello_module_init);
module_exit(hello_module_cleanup);
