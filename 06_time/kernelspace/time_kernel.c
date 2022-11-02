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
#include <linux/ctype.h>


MODULE_DESCRIPTION("time_kernel");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");


static ktime_t last_read;


static ssize_t jiffies_show(struct class *class, struct class_attribute *attr, char *buf)
{
	last_read = ktime_get_ns();
	return sprintf(buf, "%llu", get_jiffies_64() - INITIAL_JIFFIES);
}


static ssize_t tick_show(struct class *class, struct class_attribute *attr, char *buf)
{
	return sprintf(buf, "%llu", ktime_get_ns() - last_read);
}


struct class_attribute class_attr_jiffies = {
	.attr = { .name = "jiffies", .mode = 0444 },
	.show = jiffies_show,
};

struct class_attribute class_attr_tick = {
	.attr = { .name = "tick", .mode = 0444 },
	.show = tick_show,
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

	ret_jiffies = class_create_file(attr_class_time_kernel, &class_attr_jiffies);
	ret_tick = class_create_file(attr_class_time_kernel, &class_attr_tick);
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
	class_remove_file(attr_class_time_kernel, &class_attr_jiffies);
	class_remove_file(attr_class_time_kernel, &class_attr_tick);
	class_destroy(attr_class_time_kernel);

	pr_info("time_kernel: Uninstalled the module\n");
}

module_init(init_mod);
module_exit(cleanup_mod);
