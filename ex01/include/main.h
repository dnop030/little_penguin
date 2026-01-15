/* SPDX-License-Identifier: GPL-2.0 */
#ifndef MAIN_H
# define MAIN_H

#include <linux/kernel.h>
#include <linux/module.h>

int driver_init(void);
void	driver_exit(void);

#endif
