#include <linux/atomic.h>

#include "atomic_helper.h"

static atomic_t counter;

void counter_inc(void)
{
	atomic_inc(&counter);
}

void counter_dec(void)
{
	atomic_dec(&counter);
}

bool counter_nonzero(void)
{
	return atomic_read(&counter);
}
