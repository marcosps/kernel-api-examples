KDIR := /lib/modules/$(shell uname -r)/build

obj-m := test-ref.o mod-notify.o stop-machine.o lists.o kprobe.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
