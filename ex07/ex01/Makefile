# Variables
NAME := fortytwo
KERN_DIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# Kernel module object
obj-m := $(NAME).o

# Source files
$(NAME)-y := srcs/main.o

# Build target
all:
	make -C $(KERN_DIR) M=$(PWD) modules

# Clean target
clean:
	make -C $(KERN_DIR) M=$(PWD) clean

