/* SPDX-License-Identifier: GPL-2.0 */

#ifndef MAIN_H
#define MAIN_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

ssize_t foo_read(struct file *filp, char __user *usr_spac_buff, size_t count,
		 loff_t *offset);
ssize_t foo_write(struct file *filp, const char __user *usr_spac_buff,
		  size_t count, loff_t *offset);

ssize_t jiffies_file_read(struct file *filp, char __user *usr_spac_buff,
			  size_t count, loff_t *offset);

ssize_t id_read(struct file *filp, char __user *usr_spac_buff, size_t count,
		loff_t *offset);
ssize_t id_write(struct file *filp, const char __user *usr_spac_buff,
		 size_t count, loff_t *offset);

int driver_init(void);
void driver_exit(void);

#endif
