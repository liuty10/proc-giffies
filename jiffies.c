#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>
#include <linux/unistd.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>
#include <linux/jiffies.h>

struct proc_dir_entry *Our_Proc_File;

// LICENSE
MODULE_LICENSE("GPL");

static char modname[]="jiffies";

static int procfile_read(char *buf, char **start, off_t off, int count, int *eof, void *data){
    return sprintf(buf, "jiffies=%lu\n",jiffies);
}

// init module
static int __init module_load(void) {
  Our_Proc_File = create_proc_entry(modname, 0644, NULL);
  if(Our_Proc_File == NULL){
     remove_proc_entry(modname, NULL);
     printk(KERN_ALERT, "Error: could not initialize /proc/%s\n", modname);
     return -1;
  }
  Our_Proc_File->read_proc = procfile_read;
  Our_Proc_File->owner = THIS_MODULE;
  Our_Proc_File->mode = S_IFREG | S_IRUGO;
  Our_Proc_File->uid = 0;
  Our_Proc_File->gid = 0;
  Our_Proc_File->size = 37;
  printk(KERN_INFO "/proc/%s created \n", modname); 
  return 0;
}


//unload modules
static void __exit module_unload(void) {
   remove_proc_entry(modname, NULL);
}

module_init(module_load);
module_exit(module_unload);
