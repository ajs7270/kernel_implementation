obj-m		:= list.o

KERNEL_DIR	:= /lib/modules/$(shell uname -r)/build
PWD		:= $(shell pwd)

default :
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules
clean :
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) clean
