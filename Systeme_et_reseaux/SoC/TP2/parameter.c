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

module_init(parameter_init);
