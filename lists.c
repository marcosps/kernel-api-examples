#include <linux/list.h>
#include <linux/slab.h>
#include <linux/module.h>

static LIST_HEAD(mod_list);

/* simple_entry for testing purposes */
struct sentry {
	struct list_head list;
	int val;
};


static int lists_init(void)
{
	struct sentry *se1, *se2, *cursor, *tmp;

	se1 = kmalloc(sizeof(struct sentry), GFP_KERNEL);
	if (!se1)
		return -ENOMEM;

	se1->val = 10;
	list_add(&se1->list, &mod_list);

	se2 = kmalloc(sizeof(struct sentry), GFP_KERNEL);
	if (!se2) {
		kfree(se1);
		return -ENOMEM;
	}

	se2->val = 20;
	list_add(&se2->list, &mod_list);

	/* Shows the latest entry, with val as 20 */
	cursor = list_first_entry(&mod_list, struct sentry, list);
	pr_info("First entry: %d\n", cursor->val);

	/*
	 * Show that the list will be traversed from the last inserted node to
	 * the first one.
	 */
	list_for_each_entry_safe(cursor, tmp, &mod_list, list) {
		pr_info("Entry value: %d\n", cursor->val);
		list_del(&cursor->list);
		kfree(cursor);
	}

	return 0;
}

static void lists_exit(void)
{
}

module_init(lists_init);
module_exit(lists_exit);
MODULE_LICENSE("GPL");
MODULE_LICENSE("GPL");
