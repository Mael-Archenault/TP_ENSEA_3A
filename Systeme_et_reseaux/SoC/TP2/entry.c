#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>


static struct proc_dir_entry *entry;

static ssize_t proc_read(struct file *file, char __user *buf, size_t count, loff_t *pos) {
    char msg[] = "Entry content!\n";
    return simple_read_from_buffer(buf, count, pos, msg, sizeof(msg)-1);
}

static const struct file_operations proc_file_fops = {
 .owner = THIS_MODULE,
 .read = proc_read,
};

static int __init entry_init(void) {
    entry = proc_create("Test_Entry", 0, NULL, &proc_file_fops);
    if(entry == NULL)
        return -ENOMEM;
    return 0;
}

static void __exit entry_exit(void) {
    proc_remove(entry);
    printk(KERN_INFO "Proc entry removed\n");
}

module_init(entry_init);
module_exit(entry_exit);


