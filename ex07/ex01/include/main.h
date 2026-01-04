#ifndef MAIN_H
# define MAIN_H

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/debugfs.h>
#include <linux/jiffies.h>

int driver_init(void);
void	driver_exit(void);

#endif
