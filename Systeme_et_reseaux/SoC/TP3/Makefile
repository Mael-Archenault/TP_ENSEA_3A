obj-m := gpio-leds.o chenillard.o
module−objs := gpio-leds.o chenillard.o
KERNEL_SOURCE = /lib/modules/$(shell uname -r)/build

CFLAGS_MODULE=-fno-pic

all :
		make -C $(KERNEL_SOURCE) M=$(PWD) modules
clean :
		make -C $(KERNEL_SOURCE) M=$(PWD) clean
install :
		make −C $(KERNEL_SOURCE) M=$(PWD) modules install



