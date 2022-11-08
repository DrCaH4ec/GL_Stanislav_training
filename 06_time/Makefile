
KERNEL_MAKE=./kernelspace
USER_MAKE=./userspace

KERNELDIR ?= ~/repos/linux-stable/

kernel:
	export ARCH=arm\
	&& export CROSS_COMPILE='ccache arm-eabi-'\
	&& export PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-eabi/bin:$(PATH)\
	&& cd $(KERNEL_MAKE)\
	&& $(MAKE) KDIR=$(KERNELDIR);

user:
	cd $(USER_MAKE) && $(MAKE);

all: kernel user

clean_kernel:
	cd $(KERNEL_MAKE) && $(MAKE) clean;

clean_user:
	cd $(USER_MAKE) && $(MAKE) clean;

clean_all: clean_kernel clean_user
	