// SPDX-License-Identifier: MIT and GPL
// Made by CYB3RSP1D3R

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/ktime.h>
#include <linux/vmalloc.h>
#include <asm/div64.h>


MODULE_DESCRIPTION("kernel_memory");
MODULE_AUTHOR("CYB3RSP1D3R");
MODULE_VERSION("1.0");
MODULE_LICENSE("Dual MIT/GPL");

static int epochs_max = 100;
module_param(epochs_max, int, 0);
MODULE_PARM_DESC(epochs_max, "Number of epochs for the memory test");


#define MAX_ITER (64)

static const char *break_table =
"|------------------------------------------------------------------------------------------------|";
static char *malloc_name[] = {"kmalloc",  "kzalloc", "vmalloc", "get_free_pages"};


static uint64_t average_uint64(uint64_t *array, ssize_t size)
{
	uint64_t sum = 0;
	ssize_t i;

	for (i = 0; i < size; i++)
		sum += array[i];
	do_div(sum, size);
	return sum;
}


static int __init init_mod(void)
{
	uint64_t alloc_sample[epochs_max];
	uint64_t free_sample[epochs_max];
	uint64_t alloc_max, alloc_min, alloc_avr;
	uint64_t free_max, free_min, free_avr;
	ssize_t mem_size, pages_num;
	ktime_t begin, end;
	int8_t *memory;
	unsigned long addr;
	int test_count, count, epoch;

	for (test_count = 0; test_count < 0x4; test_count++) {
		pr_info("**** %s, size %s ****\n",
			 malloc_name[test_count],
			 test_count == 3 ? "in pages" : "is a power of 2");
		pr_info("%s\n", break_table);
		pr_info("| Size | allocation time (ns)%18c | free time (ns)%32c |\n",  ' ', ' ');
		pr_info("%s\n", break_table);
		pr_info("| %4c | %12s %12s %12s | %12s %20s %12s |\n",
			' ', "min", "max", "avr", "min", "max", "avr");
		pr_info("%s\n", break_table);

		for (count = 0; count < MAX_ITER; count++) {
			alloc_min = ULONG_MAX;
			alloc_max = 0;
			mem_size = 2 << count;
			pages_num = count + 1;

			for (epoch = 0; epoch < epochs_max; epoch++) {

				// allocate
				if (test_count == 0) {
					begin = ktime_get_ns();
					memory = kmalloc(mem_size, GFP_KERNEL);
					end = ktime_get_ns();
					if (memory == NULL) {
						pr_info("%s\n", break_table);
						pr_info("|%31cCannot allocate the memory%39c|\n"
							, ' ', ' ');
						break;
					}
					alloc_sample[epoch] = end - begin;
					begin = ktime_get_ns();
					kfree(memory);
					end = ktime_get_ns();
					free_sample[epoch] = end - begin;
				} else if (test_count == 1) {
					begin = ktime_get_ns();
					memory = kzalloc(mem_size, GFP_KERNEL);
					end = ktime_get_ns();
					if (memory == NULL) {
						pr_info("%s\n", break_table);
						pr_info("|%31cCannot allocate the memory%39c|\n"
							, ' ', ' ');
						break;
					}
					alloc_sample[epoch] = end - begin;
					begin = ktime_get_ns();
					kfree(memory);
					end = ktime_get_ns();
					free_sample[epoch] = end - begin;
				} else if (test_count == 2) {
					begin = ktime_get_ns();
					memory = vmalloc(mem_size);
					end = ktime_get_ns();
					if (memory == NULL) {
						pr_info("%s\n", break_table);
						pr_info("|%31cCannot allocate the memory%39c|\n"
							, ' ', ' ');
						break;
					}
					alloc_sample[epoch] = end - begin;
					begin = ktime_get_ns();
					vfree(memory);
					end = ktime_get_ns();
					free_sample[epoch] = end - begin;
				} else {
					begin = ktime_get_ns();
					addr = __get_free_pages(GFP_KERNEL, pages_num);
					end = ktime_get_ns();
					if (addr == (unsigned int) NULL) {
						pr_info("%s\n", break_table);
						pr_info("|%31cCannot allocate the memory%39c|\n"
							, ' ', ' ');
						break;
					}
					alloc_sample[epoch] = end - begin;
					begin = ktime_get_ns();
					free_pages(addr, pages_num);
					end = ktime_get_ns();
					free_sample[epoch] = end - begin;
				}
				alloc_min = (alloc_sample[epoch] < alloc_min
					? alloc_sample[epoch] : alloc_min);
				alloc_max = (alloc_sample[epoch] > alloc_max
					? alloc_sample[epoch] : alloc_max);
				free_min = (free_sample[epoch] < free_min
					? free_sample[epoch] : free_min);
				free_max = (free_sample[epoch] > free_max
					? free_sample[epoch] : free_max);
			}
			if (alloc_max == 0)
				break;

			alloc_avr = average_uint64(alloc_sample, epochs_max);
			free_avr = average_uint64(free_sample, epochs_max);
			pr_info("| %4d | %12llu %12llu %12llu | %12llu %20llu %12llu |\n",
				test_count == 3 ? pages_num : count,
				alloc_min, alloc_max, alloc_avr,
				free_min, free_max, free_avr
			);
		}
		pr_info("%s\n", break_table);
	}

	return 0;
}

static void __exit cleanup_mod(void)
{
	pr_info("kernel_memory: Uninstalled the module\n");
}

module_init(init_mod);
module_exit(cleanup_mod);
