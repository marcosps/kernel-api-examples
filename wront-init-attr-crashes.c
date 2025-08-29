#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/module.h>

static struct task_struct *ktask;

/*
 * This is going to crash, since setup_kthread is inlined to kthread_mod_init,
 * and print_messages being on .init.data means that it'll be removed once the
 * module loading phase finishes, but it will be executing into a kthread,
 * which implies that it will be called later on by sched, but the function
 * will be removed from memory.
 *
 * BOOM!
 */
static int __init print_messages(void *data)
{
	while (1) {
		if (kthread_should_stop())
			return 0;

		pr_info("Printing something, and sleeping for 1 second...\n");
		ssleep(1);
	}

	return 0;
}

static int inline setup_kthread(void)
{
	ktask = kthread_run(print_messages, 0, "print-messages");
	if (IS_ERR(ktask)) {
		pr_err("Error while starting print-messages kthread: %ld\n", PTR_ERR(ktask));
		return PTR_ERR(ktask);
	}

	return 0;
}

static int __init kthread_mod_init(void)
{
	return setup_kthread();
}

static void __exit kthread_mod_exit(void)
{
	pr_info("Stopping the print-messages kthread...\n");
	kthread_stop(ktask);
}

module_init(kthread_mod_init);
module_exit(kthread_mod_exit);
MODULE_DESCRIPTION("Example of kthread that crashes the kernel");
MODULE_LICENSE("GPLv2");
