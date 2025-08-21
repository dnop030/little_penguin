#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/slab.h>
// Dont have a license, LOL
// MODULE_LICENSE("LICENSE");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Louis Solofrizzo <louis@ne02ptzero.me>");
MODULE_DESCRIPTION("Useless module");
static ssize_t myfd_read(struct file *fp, char __user *user, size_t size,
			 loff_t *offs);

static ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
			  loff_t *offs);

static struct file_operations myfd_fops = { .owner = THIS_MODULE,
					    .read = &myfd_read,
					    .write = &myfd_write };

static struct miscdevice myfd_device = { .minor = MISC_DYNAMIC_MINOR,
					 .name = "reverse",
					 .fops = &myfd_fops };

char str[PAGE_SIZE];
// char *tmp;
static int __init myfd_init(void)
{
	int retval;
	retval = misc_register(&(*(&(myfd_device))));
	printk(KERN_INFO "useless init fn\n");
	return retval;
}
static void __exit myfd_cleanup(void)
{
	misc_deregister(&myfd_device);
	printk(KERN_INFO "useless cleanup fn\n");
}
ssize_t myfd_read(struct file *fp, char __user *user, size_t size, loff_t *offs)
{
	size_t t, i;
	char *tmp2;
	ssize_t ret;
	/***************
	* Malloc like a boss
	***************/
	// tmp2 = kmalloc(sizeof(char) * PAGE_SIZE * 2, GFP_KERNEL);
	tmp2 = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	if (!tmp2) {
		printk(KERN_INFO "useless rd fn malloc failed\n");
		return -ENOMEM;
	}
	// tmp = tmp2;
	// printk(KERN_INFO "useless rd fn msg len:%zd msg:%s\n", strlen(str), str);
	// for (t = strlen(str) - 1, i = 0; t >= 0; t--, i++) {
	for (t = strlen(str) - 1, i = 0; i < strlen(str); t--, i++) {
		// printk(KERN_INFO "useless rd fn index:%zu\n", t);
		tmp2[i] = str[t];
	}
	// tmp2[i] = 0x0;
	tmp2[i] = '\0';
	// printk(KERN_INFO "useless rd fn rev msg:%s\n", tmp2);
	// printk(KERN_INFO "useless rd fn i:%zu size:%zu len:%zu\n", i, size, strlen(tmp2));
	ret = simple_read_from_buffer(user, size, offs, tmp2, i);
	// printk(KERN_INFO "useless rd fn ret:%zu\n", ret);
	kfree(tmp2);
	return ret;
}
ssize_t myfd_write(struct file *fp, const char __user *user, size_t size,
		   loff_t *offs)
{
	ssize_t res;
	res = 0;
	res = simple_write_to_buffer(str, size, offs, user, size);
	// 0x0 = ’\0’
	// str[size + 1] = 0x0;
	str[size] = 0x0;
	// printk(KERN_INFO "useless wr fn msg:%s ret:%zu\n", str, res);
	return res;
}
module_init(myfd_init);
module_exit(myfd_cleanup);
