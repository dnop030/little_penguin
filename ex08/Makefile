NAME := useless_driver
KERN_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

obj-m += $(NAME).o

# Source files
$(NAME)-y := srcs/ex08.o

all:
	make -C $(KERN_DIR) M=$(PWD) modules

clean:
	make -C $(KERN_DIR) M=$(PWD) clean

re: clean all
