/* SPDX-License-Identifier: GPL-2.0 */
#ifndef MAIN_H
# define MAIN_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/miscdevice.h>

ssize_t fortwo_read(struct file *filp, char __user *usr_spac_buff, size_t count, loff_t *offset);
ssize_t fortwo_write(struct file *filp, const char __user *usr_spac_buff, size_t count,
		     loff_t *offset);
int fortwo_open(struct inode *inode, struct file *filp);
int fortwo_release(struct inode *inode, struct file *filp);

int fortwo_init(void);
void	driver_exit(void);

#endif
