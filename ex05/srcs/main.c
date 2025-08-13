#include "../include/main.h"

ssize_t fortwo_read (struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	intra_name[9] = "psrikamo\n";
	size_t	intra_len = strlen(intra_name);
	int		result;

	if (*offset >= intra_len)
		return 0;

	result = copy_to_user(usr_spac_buff, intra_name, intra_len);
	printk(KERN_INFO "42 rd fn:%d\n", result);
	*offset += intra_len;

	return result;
}

ssize_t fortwo_write (struct file *filp, const char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	intra_name[8] = "psrikamo";
	int		result;

	printk(KERN_INFO "42 wr fn\n");
	return 0;
}

int fortwo_open (struct inode *inode, struct file *filp) {
	printk(KERN_INFO "42 open fn\n");
	return 0;
}

int fortwo_release (struct inode *inode, struct file *filp) {
	printk(KERN_INFO "42 release fn\n");
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = fortwo_read,
	.write = fortwo_write,
	.open = fortwo_open,
	.release = fortwo_release,
};

// // Misc device structure
// struct miscdevice {
// 	int minor;
// 	const char *name;
// 	const struct file_operations *fops;
// 	struct list_head list;
// 	struct device *parent;
// 	struct device *this_device;
// 	const struct attribute_group **groups;
// 	const char *nodename;
// 	umode_t mode;
// };
struct miscdevice fortytwo_misc_chr_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "fortytwo",
	.fops = &fops,
};

// int	__init driver_init(void) {
int	__init fortwo_init(void) {
	int	err_num;

	// register misc driver
	err_num = misc_register(&fortytwo_misc_chr_dev);
	if (err_num) {
		printk(KERN_INFO "register driver failed!\n");
		return err_num;
	}

	printk(KERN_INFO "register misc chr success!\n");
	return 0;
}

void	__exit driver_exit(void) {
	misc_deregister(&fortytwo_misc_chr_dev);
	printk(KERN_INFO "Cleaning up misc chr.\n");
}

module_init(fortwo_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psrikamo");
