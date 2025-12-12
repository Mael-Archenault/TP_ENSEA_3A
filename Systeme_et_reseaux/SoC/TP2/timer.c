#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>


static struct timer_list timer;
static unsigned int count = 0;

static void timer_callback(struct timer_list *timer)
{
    printk(KERN_INFO "Timer fired! count = %u\n", count++);
    mod_timer(timer, jiffies + HZ);
}

static int __init timer_init(void)
{
    printk(KERN_INFO "Module loaded, starting timer\n");
    timer_setup(&timer, timer_callback, 0);
    mod_timer(&timer, jiffies + HZ);

    return 0;
}

static void __exit timer_exit(void)
{
    int ret = del_timer_sync(&timer);
    printk(KERN_INFO "Module exiting, timer deleted (was active = %d)\n", ret);
}

module_init(timer_init);
module_exit(timer_exit);



