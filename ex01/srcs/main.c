#include "../include/main.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psrikamo");

int	__init driver_init(void) {
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

void	__exit driver_exit(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(driver_init);
module_exit(driver_exit);
