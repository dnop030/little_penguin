#include "../include/main.h"

struct dentry *root42;
struct dentry *id;
struct dentry *jiffies_file;
struct dentry *foo;

ssize_t foo_read (struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	intra_name[9] = "psrikamo";
	int	intra_len = strlen(intra_name);
	int	result;

	printk(KERN_INFO "id rd offset:%lld\n", *offset);

	// check wheather pointer of file is out of length of data available in kernel or not
	if (*offset >= intra_len)
		return 0;

	printk(KERN_INFO "id rd fn count:%zu\n", count);

	result = copy_to_user(usr_spac_buff, intra_name, intra_len);
	printk(KERN_INFO "id rd fn res cpy2usr:%d\n", result);
	*offset += intra_len;

	return intra_len;
}

ssize_t foo_write (struct file *filp, const char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	tmp_buff[9];
	char	intra_name[9] = "psrikamo";
	int		result;
	int		i;

	printk(KERN_INFO "id wr offset:%lld\n", *offset);
	printk(KERN_INFO "id wr fn count:%zu\n", count);

	result = copy_from_user(tmp_buff, usr_spac_buff, strlen(intra_name));

	if ((tmp_buff[0] == '\n') && (count == 1)) {
		return count;
	}

	i = 0;
	while (i <= strlen(intra_name))
	{
		if (tmp_buff[i] != intra_name[i])
		{
			printk(KERN_INFO "id wr fn wrong wr msg\n");
			return -EINVAL;
		}
		i++;
	}

	printk(KERN_INFO "id wr fn correct wr msg\n");
	return strlen(intra_name);
}

struct file_operations foo_fops = {
	.owner = THIS_MODULE,
	.read = foo_read,
	.write = foo_write,
};

ssize_t jiffies_file_read (struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset) {
	unsigned long	j = jiffies;				// ดึงค่า jiffies ปัจจุบัน
	unsigned int	total_seconds = j / HZ;		// แปลงเป็นวินาที
	unsigned int	mins = total_seconds / 60;
	unsigned int	secs = total_seconds % 60;
	char	buff[50];
	static unsigned int	buff_len = 50;
	unsigned int	result;
	unsigned int	res_snprintf;

	printk(KERN_INFO "jiffies rd offset:%lld\n", *offset);
	printk(KERN_INFO "jiffies :%lu %u %u %u\n", *offset, total_seconds, mins, secs);

	// check wheather pointer of file is out of length of data available in kernel or not
	if (*offset >= buff_len)
		return 0;

	printk(KERN_INFO "jiffies rd fn count:%zu\n", count);
	res_snprintf = snprintf(buff, 26,"uptime: %.2u min %.2u sec\n", mins, secs);
	buff_len = strlen(buff);
	result = copy_to_user(usr_spac_buff, buff, buff_len);
	printk(KERN_INFO "jiffies rd fn res cpy2usr:%d\n", result);
	*offset += buff_len;

	return buff_len;
}

struct file_operations jiffies_file_fops = {
	.owner = THIS_MODULE,
	.read = jiffies_file_read,
	// .write = id_write,
};

ssize_t id_read (struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	intra_name[9] = "psrikamo";
	int	intra_len = strlen(intra_name);
	int	result;

	printk(KERN_INFO "id rd offset:%lld\n", *offset);

	// check wheather pointer of file is out of length of data available in kernel or not
	if (*offset >= intra_len)
		return 0;

	printk(KERN_INFO "id rd fn count:%zu\n", count);

	result = copy_to_user(usr_spac_buff, intra_name, intra_len);
	printk(KERN_INFO "id rd fn res cpy2usr:%d\n", result);
	*offset += intra_len;

	return intra_len;
}

ssize_t id_write (struct file *filp, const char __user *usr_spac_buff, size_t count, loff_t *offset) {
	char	tmp_buff[9];
	char	intra_name[9] = "psrikamo";
	int		result;
	int		i;

	printk(KERN_INFO "id wr offset:%lld\n", *offset);
	printk(KERN_INFO "id wr fn count:%zu\n", count);

	result = copy_from_user(tmp_buff, usr_spac_buff, strlen(intra_name));

	if ((tmp_buff[0] == '\n') && (count == 1)) {
		return count;
	}

	i = 0;
	while (i <= strlen(intra_name))
	{
		if (tmp_buff[i] != intra_name[i])
		{
			printk(KERN_INFO "id wr fn wrong wr msg\n");
			return -EINVAL;
		}
		i++;
	}

	printk(KERN_INFO "id wr fn correct wr msg\n");
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

	jiffies_file = debugfs_create_file("jiffies", 0444,
                                   root42, NULL,
                                   &jiffies_file_fops);

	foo = debugfs_create_file("foo", 0644,
                                   root42, NULL,
                                   &jiffies_file_fops);

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
