// SPDX-License-Identifier: MIT and GPL
// Made by CYB3RSP1D3R

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/jiffies.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <asm/div64.h>
#include <linux/ctype.h>


MODULE_DESCRIPTION("time_kernel");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");


static ktime_t last_read;


static ssize_t absolute_show(struct class *class, struct class_attribute *attr, char *buf)
{
	ktime_t sec, nsec;

	sec = ktime_get_ns();
	do_div(sec, 1000000000);
	nsec = ktime_get_ns() - sec * 1000000000;
	last_read = ktime_get_ns();
	return sprintf(buf, "%llu.%llu\n", sec, nsec);
}


static ssize_t relative_show(struct class *class, struct class_attribute *attr, char *buf)
{
	ktime_t sec, nsec;

	sec = ktime_get_ns() - last_read;
	do_div(sec, 1000000000);
	nsec = ktime_get_ns() - sec * 1000000000;

	return sprintf(buf, "%llu.%llu\n", sec, nsec);
}


struct class_attribute class_attr_absolute = {
	.attr = { .name = "absolute", .mode = 0444 },
	.show = absolute_show,
};

struct class_attribute class_attr_relative = {
	.attr = { .name = "relative", .mode = 0444 },
	.show = relative_show,
};

static struct class *attr_class_time_kernel;


static int __init init_mod(void)
{
	int ret_jiffies, ret_tick;

	attr_class_time_kernel = class_create(THIS_MODULE, "time_kernel");
	if (attr_class_time_kernel == NULL) {
		pr_err("time_kernel: error creating sysfs class\n");
		return -ENOMEM;
	}

	ret_jiffies = class_create_file(attr_class_time_kernel, &class_attr_absolute);
	ret_tick = class_create_file(attr_class_time_kernel, &class_attr_relative);
	if (ret_jiffies || ret_tick) {
		pr_err("time_kernel: error creating sysfs classes attributes\n");
		class_destroy(attr_class_time_kernel);
		return -EPERM;
	}

	last_read = ktime_get_ns();

	pr_info("time_kernel: module loaded\n");
	return 0;
}

static void __exit cleanup_mod(void)
{
	class_remove_file(attr_class_time_kernel, &class_attr_absolute);
	class_remove_file(attr_class_time_kernel, &class_attr_relative);
	class_destroy(attr_class_time_kernel);

	pr_info("time_kernel: Uninstalled the module\n");
}

module_init(init_mod);
module_exit(cleanup_mod);
