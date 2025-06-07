#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>

static struct task_struct *ktask;

static int print_messages(void *data)
{
	while (1) {
		pr_info("Printing something, and sleeping for 1 second...\n");
		ssleep(1);
	}

	return 0;
}

static int kthread_mod_init(void)
{
	ktask = kthread_run(print_messages, 0, "print-messages");
	if (IS_ERR(ktask)) {
		pr_err("Error while starting print-messages kthread: %ld\n", PTR_ERR(ktask));
		return PTR_ERR(ktask);
	}

	return 0;
}

static void kthread_mod_exit(void)
{
	kthread_stop(ktask);
}

module_init(kthread_mod_init);
module_exit(kthread_mod_exit);
MODULE_DESCRIPTION("Example of kthread and sleeping");
MODULE_LICENSE("GPLv2");
