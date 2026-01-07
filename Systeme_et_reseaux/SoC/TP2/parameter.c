#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>


static int value = 315;
module_param(value, int, 0);

static int __init parameter_init(void)
{
  printk(KERN_INFO "Input parameter: %d\n", value);
  return 0;
}

static void __exit parameter_exit(void) {
    printk(KERN_INFO "Module removed\n");
}


module_init(parameter_init);
module_exit(parameter_exit);