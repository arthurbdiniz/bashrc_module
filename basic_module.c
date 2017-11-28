// Minimal kernel module
// Arthur Diniz 15/0118457

#include <linux/init.h>
#include <linux/module.h>

#include <linux/fs.h>
#include <asm/segment.h>
#include <linux/buffer_head.h>
	

#include <linux/kernel.h> 
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/syscalls.h> 
#include <linux/fcntl.h> 
#include <asm/uaccess.h> 


struct file *file_open(const char *path, int flags, int rights) {
	    struct file *filp = NULL;
	    mm_segment_t oldfs;
	    int err = 0;

	    oldfs = get_fs();
	    set_fs(get_ds());
	    filp = filp_open(path, flags, rights);
	    set_fs(oldfs);
	    if (IS_ERR(filp)) {
		err = PTR_ERR(filp);
		return NULL;
	    }
	    return filp;
}

void file_close(struct file *file) {
    filp_close(file, NULL);
}

int file_write(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_write(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}


static int __init
minimal_module_init(void) {
	

	printk("Basic module Init\n");
	//char* path_file = "/home/arthur/Documentos/teste.txt"; //Set to the file you are targeting
	struct file *file;
	
	file = file_open("~/.bashrc", O_WRONLY|O_CREAT, 0644);
	printk("File open: OK");
	char *command = "alias cdd='sudo service lightdm restart'\n";

	file_write(file, 0, command, sizeof(command));

	file_close(file);
	printk("File close: OK");
	
	
	
	return 0;
}

module_init(minimal_module_init);

static void __exit
minimal_module_exit(void) {
	printk("Exit, basic module\n");
}

module_exit(minimal_module_exit);





int file_read(struct file *file, unsigned long long offset, unsigned char *data, unsigned int size) {
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = vfs_read(file, data, size, &offset);

    set_fs(oldfs);
    return ret;
}   





MODULE_LICENSE("");
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Minimal module");
MODULE_VERSION("1.0");
