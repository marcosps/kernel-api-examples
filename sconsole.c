/* Simple console implementation for experimenting legacy and NBCON variants */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/console.h>

static struct console scon = {
	.name = "sconsole",
	.flags = CON_ENABLED,
};

static int sconsole_init(void)
{
	//pr_info("Registering simple console (sconsole)\n");
	register_console(&scon);
	return 0;
}

static void sconsole_exit(void)
{
	unregister_console(&scon);
}

module_init(sconsole_init);
module_exit(sconsole_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marcos Paulo de Souza <marcos@mpdesouza.com>");
MODULE_DESCRIPTION("Simple console module");
