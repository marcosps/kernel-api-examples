KDIR := /lib/modules/$(shell uname -r)/build

atomic-m := atomic_main.o atomic_helper.o

obj-m := test-ref.o mod-notify.o stop-machine.o lists.o kprobe.o ftrace-hijack.o wait-queue.o atomic.o \
	sconsole.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
