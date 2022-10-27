// SPDX-License-Identifier: MIT and GPL
// Made by CYB3RSP1D3R

#include <linux/module.h>
#include <linux/list.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/moduleparam.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/ctype.h>


MODULE_DESCRIPTION("String convert");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");


static ssize_t buffer_len = 256;
module_param(buffer_len, uint, 0);
MODULE_PARM_DESC(buffer, "Buffer length for input strings to the proc and sys files, default value is 256");


// The minimal required size is 120,
// to make sure it the buffer can fit 256 bytes are taken
#define PROC_STAT_BUFFER ((ssize_t)256)

struct statistics {
	unsigned int total_calls;
	unsigned int total_chars;
	unsigned int total_nonchars;
	unsigned int convert_num;
};

static struct statistics char_stat = {0, 0, 0, 0};
static char *proc_buffer;
static char *sys_buffer;


//  SYSFS Class attributes functions and struct
static ssize_t rw_show(struct class *class, struct class_attribute *attr, char *buf)
{
	ssize_t count = 0;
	char *current_pos_in = sys_buffer;
	char *current_pos_out = buf;
	char char_buffer;

	while (*current_pos_in != '\0') {
		char_buffer = *(current_pos_in);
		if (isupper(char_buffer)) {
			char_buffer = tolower(char_buffer);
			char_stat.convert_num++;
		}
		*(current_pos_in++) = char_buffer;
		*(current_pos_out++) = char_buffer;
		count++;
	}
	char_stat.total_calls++;
	return count;
}

static ssize_t rw_store(
	struct class *class,
	struct class_attribute *attr,
	const char *buf,
	size_t count)
{
	const char *current_pos_in = buf;
	char *current_pos_out = sys_buffer;

	while (*current_pos_in != '\0') {
		if (!isalpha(*current_pos_in))
			char_stat.total_nonchars++;
		else
			char_stat.total_chars++;
		*(current_pos_out++) = *(current_pos_in++);
	}
	return count;
}


static ssize_t stat_show(struct class *class, struct class_attribute *attr, char *buf)
{
	sprintf(buf,
		"Total number of calls: %d\n"
		"Total number of characters: %d\n"
		"Total number of non-characters: %d\n"
		"Total number of conversions: %d\n",
		char_stat.total_calls,
		char_stat.total_chars,
		char_stat.total_nonchars,
		char_stat.convert_num);

	return strlen(buf);
}


/*CLASS_ATTR_RW(rw);*/
struct class_attribute class_attr_rw = {
	.attr = { .name = "string_convert", .mode = 0644 },
	.show	= rw_show,
	.store	= rw_store
};

// Statistics class attribute
struct class_attribute class_attr_stat = {
	.attr = { .name = "statistics", .mode = 0444 },
	.show	= stat_show,
};

static struct class *attr_class_char;
static struct class *attr_class_stat;


// PROCFS part
static ssize_t proc_write(struct file *file, const char __user *buf, size_t length, loff_t *offset)
{
	if (*offset > 0 || length > buffer_len)
		return -EFAULT;

	const char *current_pos = proc_buffer;
	ssize_t non_copied;

	non_copied = copy_from_user(proc_buffer, buf, length);
	while (*current_pos != '\0') {
		if (!isalpha(*(current_pos++)))
			char_stat.total_nonchars++;
		else
			char_stat.total_chars++;
	}
	return length - non_copied;
}

static ssize_t proc_read(struct file *file, char __user *buf, size_t length, loff_t *offset)
{
	if (*offset > 0 || length < buffer_len)
		return 0;

	char *current_pos_in = proc_buffer;
	ssize_t msg_len = 0;

	while (*current_pos_in != '\0') {
		if (islower(*current_pos_in)) {
			*(current_pos_in) = toupper(*current_pos_in);
			char_stat.convert_num++;
		}
		current_pos_in++;
		msg_len++;
	}
	if (copy_to_user(buf, proc_buffer, msg_len))
		return -EFAULT;
	char_stat.total_calls++;
	return msg_len;
}

static ssize_t proc_stat(struct file *file, char __user *buf, size_t length, loff_t *offset)
{
	if (*offset > 0 || length < PROC_STAT_BUFFER)
		return 0;

	char print_buffer[PROC_STAT_BUFFER];
	ssize_t msg_len;

	msg_len = sprintf(print_buffer,
		"Total number of calls: %d\n"
		"Total number of characters: %d\n"
		"Total number of non-characters: %d\n"
		"Total number of conversions: %d\n",
		char_stat.total_calls,
		char_stat.total_chars,
		char_stat.total_nonchars,
		char_stat.convert_num);
	if (copy_to_user(buf, print_buffer, msg_len))
		return -EFAULT;
	return msg_len;
}


static const struct file_operations toupper_fs = {
	.owner = THIS_MODULE,
	.read = proc_read,
	.write = proc_write,
};

static const struct file_operations stat = {
	.owner = THIS_MODULE,
	.read = proc_stat,
};

static struct proc_dir_entry *toupper_dir;
static struct proc_dir_entry *stat_dir;


// Init and exit functions
static int __init init_mod(void)
{
	// PROCFS
	toupper_dir = proc_create("string_convert", 0644, NULL, &toupper_fs);
	stat_dir = proc_create("statistics", 0444, NULL, &stat);

	if (toupper_dir == NULL || stat_dir == NULL) {
		pr_err("String convert: error creating procfs entries\n");
		return -ENOMEM;
	}

	// SYSFS
	int ret_char, ret_stat;

	proc_buffer = kmalloc(buffer_len, GFP_KERNEL);
	sys_buffer = kmalloc(buffer_len, GFP_KERNEL);
	memset(proc_buffer, 0, buffer_len);
	memset(sys_buffer, 0, buffer_len);

	if (proc_buffer == NULL || sys_buffer == NULL) {
		pr_err("String convert: not enough memory for the buffers\n");
		return -ENOMEM;
	}

	attr_class_char = class_create(THIS_MODULE, "string_convert");
	attr_class_stat = class_create(THIS_MODULE, "statistics");
	if (attr_class_char == NULL || attr_class_stat == NULL) {
		pr_err("String convert: error creating sysfs classes\n");
		return -ENOMEM;
	}

	ret_char = class_create_file(attr_class_char, &class_attr_rw);
	ret_stat = class_create_file(attr_class_stat, &class_attr_stat);
	if (ret_char || ret_stat) {
		pr_err("String convert: error creating sysfs classes attributes\n");
		class_destroy(attr_class_char);
		class_destroy(attr_class_stat);
		return -EPERM;
	}

	pr_info("String convert: module loaded\n");
	return 0;
}

static void __exit cleanup_mod(void)
{
	proc_remove(toupper_dir);
	proc_remove(stat_dir);
	class_remove_file(attr_class_char, &class_attr_rw);
	class_destroy(attr_class_char);
	class_remove_file(attr_class_stat, &class_attr_stat);
	class_destroy(attr_class_stat);

	pr_info("String convert: Uninstalled the module\n");
}

module_init(init_mod);
module_exit(cleanup_mod);
