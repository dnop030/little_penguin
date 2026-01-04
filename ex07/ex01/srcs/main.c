#include "../include/main.h"

struct dentry *root42;
struct dentry *id;
// struct dentry *jiffies;
// struct dentry *foo;

ssize_t id_read (struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	intra_name[9] = "psrikamo";
	int	intra_len = strlen(intra_name);
	int	result;

	printk(KERN_INFO "42 rd offset:%lld\n", *offset);

	// check wheather pointer of file is out of length of data available in kernel or not
	if (*offset >= intra_len)
		return 0;

	printk(KERN_INFO "42 rd fn count:%zu\n", count);

	result = copy_to_user(usr_spac_buff, intra_name, intra_len);
	printk(KERN_INFO "42 rd fn res cpy2usr:%d\n", result);
	*offset += intra_len;

	return intra_len;
}

ssize_t id_write (struct file *filp, const char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	tmp_buff[9];
	char	intra_name[9] = "psrikamo";
	int		result;
	int		i;

	result = copy_from_user(tmp_buff, usr_spac_buff, strlen(intra_name));
	i = 0;
	while (i <= strlen(intra_name))
	{
		if (tmp_buff[i] != intra_name[i])
		{
			printk(KERN_INFO "42 wr fn wrong wr msg\n");
			return -EINVAL;
		}
		i++;
	}
	printk(KERN_INFO "42 wr fn correct wr msg\n");
	return strlen(intra_name);
}

// int id_open (struct inode *inode, struct file *filp) {
// 	printk(KERN_INFO "42 open fn\n");
// 	return 0;
// }

// int id_release (struct inode *inode, struct file *filp) {
// 	printk(KERN_INFO "42 release fn\n");
// 	return 0;
// }

struct file_operations id_fops = {
	.owner = THIS_MODULE,
	.read = id_read,
	.write = id_write,
	// .open = id_open,
	// .release = id_release,
};

int	__init driver_init(void) {

	printk(KERN_INFO "debugfs 42 init\n");

	root42 = debugfs_create_dir("fortytwo", NULL);
	if (root42 == NULL) {
		printk(KERN_INFO "debugfs 42 create dir error\n");
		return root42;
	}

	// debugfs_create_file(const char *name, umode_t mode,
    //                                struct dentry *parent, void *data,
    //                                const struct file_operations *fops)

	// id = debugfs_create_file("id", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH,
    //                                root42, NULL,
    //                                &id_fops);
	id = debugfs_create_file("id", 0666,
                                   root42, NULL,
                                   &id_fops);

	// if root42 != complete
	// 	clear module and exit
	// else {
	// 	create id
	// 	if create id failed
	// 		clear id

	// 	create jiffies
	// 	if create jiffies failed
	// 		clear jiffies

	// 	create foo
	// 	if create foo failed
	// 		clear foo
	// }

	return 0;
}

void	__exit driver_exit(void) {
	printk(KERN_INFO "debugfs 42 clean-up\n");
	debugfs_remove(root42);
}

module_init(driver_init);
module_exit(driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("psrikamo");
