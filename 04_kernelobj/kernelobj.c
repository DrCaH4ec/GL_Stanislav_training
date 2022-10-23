// SPDX-License-Identifier: MIT and GPL
// Made by CYB3RSP1D3R

#include <linux/module.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>


MODULE_DESCRIPTION("Kernel object");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");

// List struct and head
struct str_list {
	struct list_head list;
	char *string;
};

LIST_HEAD(head_list);


// List funcitons
static ssize_t list_print(const char *buffer)
{
	if (list_empty(&head_list)) {
		pr_info("Kernel object: list is empty\n");
		return 0;
	}

	int ctr = 0;
	char *buffer_offset = buffer;
	ssize_t string_len = 0;
	struct str_list *entry = NULL;

	list_for_each_entry(entry, &head_list, list) {
		string_len = strlen(entry->string);
		strncpy(buffer_offset, entry->string, string_len);
		buffer_offset += string_len;
		*(buffer_offset++) = '\n';
		ctr += string_len + 1;
	}
	return ctr;
}

static ssize_t list_store(const char *buffer)
{

	struct str_list *new_entry = kmalloc(sizeof(*new_entry), GFP_KERNEL);

	if (new_entry == NULL)
		return -ENOMEM;

	new_entry->string = kmalloc(strlen(buffer) + 1, GFP_KERNEL);
	if (new_entry->string == NULL)
		return -ENOMEM;


	strcpy(new_entry->string, buffer);
	list_add_tail(&(new_entry->list), &head_list);
	return 0;
}

void list_free(void)
{
	if (list_empty(&head_list))
		return;

	struct str_list *entry, *tmp;

	list_for_each_entry_safe(entry, tmp, &head_list, list) {
		kfree(entry->string);
		list_del(&(entry->list));
		kfree(entry);
	}
}


// Class attributes functions and struct
static ssize_t rw_show(struct class *class, struct class_attribute *attr, char *buf)
{
	ssize_t len = list_print(buf);

	pr_info("Kernel object: %s: %s", __func__, buf);
	return len;
}

static ssize_t rw_store(
	struct class *class,
	struct class_attribute *attr,
	const char *buf,
	size_t count)
{
	list_store(buf);
	pr_info("Kernel object: %s: value = %s\n", __func__, buf);
	return count;
}

/*CLASS_ATTR_RW(rw);*/
struct class_attribute class_attr_rw = {
	.attr = { .name = "list", .mode = 0666 },
	.show	= rw_show,
	.store	= rw_store
};

static struct class *attr_class;


// Init and exit functions
static int __init init_mod(void)
{
	int ret;

	attr_class = class_create(THIS_MODULE, "kernelobj");
	if (attr_class == NULL) {
		pr_err("Kernel object: error creating sysfs class\n");
		return -ENOMEM;
	}

	ret = class_create_file(attr_class, &class_attr_rw);
	if (ret) {
		pr_err("Kernel object: error creating sysfs class attribute\n");
		class_destroy(attr_class);
		return ret;
	}

	pr_info("Kernel object: module loaded\n");
	return 0;
}

static void __exit cleanup_mod(void)
{
	list_free();
	class_remove_file(attr_class, &class_attr_rw);
	class_destroy(attr_class);
	pr_info("Kernel object: Uninstalled the module\n");
}

module_init(init_mod);
module_exit(cleanup_mod);
