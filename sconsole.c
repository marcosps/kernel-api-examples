/*
 * Simple console implementation for experimenting legacy and NBCON variants
 * (TODO).
 *
 * For testing, load the sconsole module, and read from file /dev/sconsole after
 * generating information o /dev/kmsg (dmesg). Take a look into your current
 * loglevel otherwise you might end up not recording the information.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/console.h>
#include <linux/miscdevice.h>
#include <linux/module.h>

#define MAX_RECORDS 100

struct scon_msg {
	size_t len;
	char msg[MAX_RECORDS];
};

static struct scon_msg msgs[MAX_RECORDS];
static size_t nrecs;

static ssize_t scon_read(struct file *f, char __user *buf, size_t count,
		loff_t *fpos)
{
	/* Each fpos means a record here */
	if (*fpos >= MAX_RECORDS || *fpos >= nrecs)
		return 0;

	/* only read the first message */
	if (count > msgs[0].len)
		count = msgs[0].len;

	if (copy_to_user(buf, msgs[*fpos].msg, count))
		return -ERESTARTSYS;

	(*fpos)++;

	return count;
}

static void write_msg(struct console *con, const char *msg, unsigned int len)
{
	/* For now store only MAX_RECORDS console entries */
	if (nrecs >= MAX_RECORDS)
		return;

	msgs[nrecs].len = len;
	strscpy(msgs[nrecs].msg, msg, sizeof(msgs[nrecs].msg));
	nrecs++;
}

static struct file_operations scon_fops = {
	.read = scon_read,
};

static struct miscdevice scon_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "sconsole",
	.mode = S_IRUGO,
	.fops = &scon_fops,
};

static struct console scon = {
	.name = "simple_console",
	.flags = CON_ENABLED,
	.write = write_msg,
};

static int sconsole_init(void)
{
	int ret;

	/*
	 * Register the miscdevice that will be responsible for reading the
	 * console messages.
	 */
	ret = misc_register(&scon_misc);
	if (ret)
		return ret;

	/* Registers simple console so kernel can start writing messages to it. */
	register_console(&scon);

	return 0;
}

static void sconsole_exit(void)
{
	unregister_console(&scon);
	misc_deregister(&scon_misc);
}

module_init(sconsole_init);
module_exit(sconsole_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcos Paulo de Souza <marcos@mpdesouza.com>");
MODULE_DESCRIPTION("Simple console module");
