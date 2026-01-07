#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/io.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>

static int speed = 200;
module_param(speed, int, 0644);
MODULE_PARM_DESC(speed, "Brillez speed");

#define LED_COUNT 9
#define CLKW 0
#define CNTCLKW 1
#define CIRCLING_MODE 0
#define BOUNCING_MODE 1

static struct timer_list led_timer;
static int pos = 1;
static int direction = CLKW;

static struct proc_dir_entry *ensea_dir;
static struct proc_dir_entry *speed_entry;
static struct proc_dir_entry *dir_entry;

static int leds_probe(struct platform_device *pdev);
static int leds_remove(struct platform_device *pdev);
static ssize_t leds_read(struct file *file, char *buffer, size_t len, loff_t *offset);
static ssize_t leds_write(struct file *file, const char *buffer, size_t len, loff_t *offset);
static ssize_t dir_read(struct file *file, char *buffer, size_t len, loff_t *offset);
static ssize_t dir_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos);
static ssize_t speed_read(struct file *file, char *buffer, size_t len, loff_t *offset);


// Files operations
static const struct file_operations ensea_leds_fops = {
    .owner = THIS_MODULE,
    .read = leds_read,
    .write = leds_write
};

static const struct file_operations speed_file_fops = {
    .owner = THIS_MODULE,
    .read = speed_read
};

static const struct file_operations dir_file_fops = {
    .owner = THIS_MODULE,
    .read = dir_read,
    .write = dir_write
};

// An instance of this structure will be created for every ensea_led IP in the system
struct ensea_leds_dev {
    struct miscdevice miscdev;
    void __iomem *regs;
    u8 leds_value;
    int mode;
    struct timer_list timer;
};

// Specify which device tree devices this driver supports
static struct of_device_id ensea_leds_dt_ids[] = {
    {
        .compatible = "dev,ensea"
    },
    { /* end of table */ }
};

// Inform the kernel about the devices this driver supports
MODULE_DEVICE_TABLE(of, ensea_leds_dt_ids);

// Data structure that links the probe and remove functions with our driver
static struct platform_driver leds_platform = {
    .probe = leds_probe,
    .remove = leds_remove,
    .driver = {
        .name = "Ensea LEDs Driver",
        .owner = THIS_MODULE,
        .of_match_table = ensea_leds_dt_ids
    }
};


// Speed

static ssize_t speed_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
    char msg[64];
    int len;
    if (*ppos > 0) {
        return 0;
    }
    len = snprintf(msg, sizeof(msg), "speed = %d\n", speed);
    return simple_read_from_buffer(buffer, count, ppos, msg, len);
}

// Direction

static ssize_t dir_read(struct file *file, char __user *buffer, size_t count, loff_t *ppos) {
    char msg[64];
    int len;
    if (*ppos > 0) {
        return 0;
    }
    len = snprintf(msg, sizeof(msg), "direction = %d (%s)\n", direction, (direction == CLKW)?"Clockwise":"Counter-clockwise");
    return simple_read_from_buffer(buffer, count, ppos, msg, len);
}

static ssize_t dir_write(struct file *file, const char __user *buffer, size_t count, loff_t *ppos) {
    char buf[16];
    int new_dir;
    if (count >= sizeof(buf)) {
        return -EINVAL;
    }
    if (copy_from_user(buf, buffer, count)) {
        return -EFAULT;
    }
    buf[count] = '\0';
    if (kstrtoint(buf,10,&new_dir)) {
        return -EINVAL;
    }
    if (new_dir != CNTCLKW && new_dir != CLKW) {
        return -EINVAL;
    }
    direction = new_dir;
    printk(KERN_INFO "Cyclistes: dir change %d (%s)\n", direction, (direction == CLKW)?"Clockwise":"Counter-clockwise");
    return count;
}





// Timer callback


static void led_timer_fn(struct timer_list *t) {
    struct ensea_leds_dev *dev = container_of(t, struct ensea_leds_dev, timer);
    if (dev->mode == CIRCLING_MODE) {
        if (direction == CLKW) {
            pos <<= 1;
            if (pos > (1 << (LED_COUNT-1))) {
                pos = 1;
            }
        }
        else {
            pos >>= 1;
            if (pos <= 0) {
                pos = 1 << (LED_COUNT - 1);
            }
        }
    }
    else if (dev->mode == BOUNCING_MODE) {
        if (direction == CLKW) {
            pos <<= 1;
            if (pos >= (1 << (LED_COUNT-1))) {
                direction = CNTCLKW;
            }
        }
        else {
            pos >>= 1;
            if (pos <= 1) {
                direction = CLKW;
            }
        }
    }
   

    printk(KERN_INFO "Cycliste brillez simul: led allumee: 0x%02x\n",pos);
    dev->leds_value = pos;
    iowrite32(dev->leds_value, dev->regs);
    mod_timer(&led_timer, jiffies + msecs_to_jiffies(speed));
}

// Leds
static ssize_t leds_read(struct file *file, char *buffer, size_t len, loff_t *offset)
{

    struct ensea_leds_dev *dev = container_of(file->private_data, struct ensea_leds_dev, miscdev);

    char msg[64];
    int ret;
    if (*offset > 0) {
        return 0;
    }
    ret = snprintf(msg, sizeof(msg), "mode = %d (%s)\n", dev->mode, (dev->mode == CIRCLING_MODE)?"Circling":"Bouncing");
    return simple_read_from_buffer(buffer, len, offset, msg, ret);

    return 0;
}

// This function gets called whenever a write operation occurs on one of the character files
static ssize_t leds_write(struct file *file, const char *buffer, size_t len, loff_t *offset)
{

    struct ensea_leds_dev *dev = container_of(file->private_data, struct ensea_leds_dev, miscdev);
    char msg[16];
    int new_mode;

    if (len >= sizeof(msg)) {
        return -EINVAL;
    }
    if (copy_from_user(msg,buffer,len)) {
        return -EFAULT;
    }
    msg[len] = '\0';
    if (kstrtoint(msg,10,&new_mode)) {
        return -EINVAL;
    }
    if (new_mode != CIRCLING_MODE && new_mode != BOUNCING_MODE) {
        return -EINVAL;
    }

    dev->mode = new_mode;
    printk(KERN_INFO "ensea-leds mode change: %d (%s)\n", dev->mode, (dev->mode == CIRCLING_MODE)?"Circling":"Bouncing");

    return len;
}

static int leds_probe(struct platform_device *pdev)
{
    int ret_val = -EBUSY;
    struct ensea_leds_dev *dev;
    struct resource *r = 0;

    pr_info("leds_probe enter\n");

    // Get the memory resources for this LED device
    r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
    if(r == NULL) {
        pr_err("IORESOURCE_MEM (register space) does not exist\n");
        goto bad_exit_return;
    }

    // Create structure to hold device-specific information (like the registers)
    dev = devm_kzalloc(&pdev->dev, sizeof(struct ensea_leds_dev), GFP_KERNEL);

    // Both request and ioremap a memory region
    // This makes sure nobody else can grab this memory region
    // as well as moving it into our address space so we can actually use it
    dev->regs = devm_ioremap_resource(&pdev->dev, r);
    if(IS_ERR(dev->regs))
        goto bad_ioremap;

    // Turn the LEDs on (access the 0th register in the ensea LEDs module)
    dev->leds_value = 0xFF;
    iowrite32(dev->leds_value, dev->regs);

    // Initialize the misc device (this is used to create a character file in userspace)
    dev->miscdev.minor = MISC_DYNAMIC_MINOR;    // Dynamically choose a minor number
    dev->miscdev.name = "ensea_leds";
    dev->miscdev.fops = &ensea_leds_fops;

    ret_val = misc_register(&dev->miscdev);
    if(ret_val != 0) {
        pr_info("Couldn't register misc device :(");
        goto bad_exit_return;
    }

    // Initialize timer
    setup_timer(&dev->timer, led_timer_fn, 0);
    mod_timer(&dev->timer, jiffies + msecs_to_jiffies(speed));

    // Initialize mode

    dev->mode = CIRCLING_MODE;

    // Give a pointer to the instance-specific data to the generic platform_device structure
    // so we can access this data later on (for instance, in the read and write functions)
    platform_set_drvdata(pdev, (void*)dev);

    pr_info("leds_probe exit\n");

    return 0;

bad_ioremap:
   ret_val = PTR_ERR(dev->regs);
bad_exit_return:
    pr_info("leds_probe bad exit :(\n");
    return ret_val;
}

static int leds_remove(struct platform_device *pdev)
{
    // Grab the instance-specific information out of the platform device
    struct ensea_leds_dev *dev = (struct ensea_leds_dev*)platform_get_drvdata(pdev);

    del_timer_sync(&dev->timer);
    pr_info("leds_remove enter\n");

    // Turn the LEDs off
    iowrite32(0x00, dev->regs);

    // Unregister the character file (remove it from /dev)
    misc_deregister(&dev->miscdev);

    pr_info("leds_remove exit\n");

    return 0;
}



// Module bases methods

static int __init cyclistes_init(void) {
    printk(KERN_INFO "Cycliste initialisez, speed = %d\n",speed);

    ensea_dir = proc_mkdir("ensea", NULL);
    speed_entry = proc_create("speed", 0666, ensea_dir, &speed_file_fops);
    dir_entry = proc_create("dir", 0666, ensea_dir, &dir_file_fops);
    if((speed_entry == NULL) || (dir_entry == NULL)){
        return -ENOMEM;
    }

    int ret_val = 0;
    pr_info("Initializing the Ensea LEDs module\n");

    // Register our driver with the "Platform Driver" bus
    ret_val = platform_driver_register(&leds_platform);
    if(ret_val != 0) {
        pr_err("platform_driver_register returned %d\n", ret_val);
        return ret_val;
    }

    pr_info("Ensea LEDs module successfully initialized!\n");
    return 0;
}

static void __exit cyclistes_exit(void) {
    printk(KERN_INFO "Cycliste exitez\n");
    proc_remove(ensea_dir);
    proc_remove(speed_entry);
    proc_remove(dir_entry);

    pr_info("Ensea LEDs module exit\n");

    // Unregister our driver from the "Platform Driver" bus
    // This will cause "leds_remove" to be called for each connected device
    platform_driver_unregister(&leds_platform);

    pr_info("Ensea LEDs module successfully unregistered\n");
}

module_init(cyclistes_init);
module_exit(cyclistes_exit);