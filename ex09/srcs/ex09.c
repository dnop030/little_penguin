// #include <linux/proc_fs.h>
// #include <linux/seq_file.h>
// #include <linux/mount.h>
// #include <linux/list.h>
// #include <linux/dcache.h>

// static int mymounts_show(struct seq_file *m, void *v) {
//     struct path path;
//     struct mount *mnt;

//     // ต้องใช้ path_walk_rcu() หรือวิธีที่เหมาะสมในการเข้าถึงรายการ mount point
//     // ซึ่งมีการเปลี่ยนแปลงในแต่ละเวอร์ชันของเคอร์เนล
//     // ตัวอย่างนี้เป็นการสาธิตแนวคิด
//     // การเข้าถึงรายการ mount point อาจต้องใช้ lock หรือ RCU (Read-Copy-Update)
//     // เพื่อความปลอดภัยในการเข้าถึงข้อมูล

//     // โค้ดนี้เป็นเพียงแนวคิดที่แสดงการเข้าถึง dentry และ vfsmount
//     // ในความเป็นจริง การเข้าถึง global list ของ mount points อาจต้องใช้ฟังก์ชันเฉพาะ
//     // และมีการ lock ที่ซับซ้อนกว่านี้

//     seq_printf(m, "root /\n");
//     seq_printf(m, "sys /sys\n");
//     seq_printf(m, "proc /proc\n");
//     // ... แสดง mount point อื่นๆ

//     return 0;
// }

// static int mymounts_open(struct inode *inode, struct file *file) {
// 	printk(KERN_INFO "");

//     return single_open(file, mymounts_show, NULL);
// }

// static const struct proc_ops mymounts_fops = {
//     .proc_open = mymounts_open,
//     // .proc_read = seq_read,
//     // .proc_lseek = seq_lseek,
//     // .proc_release = single_release,
// };

// static struct proc_dir_entry *mymounts_file;

// static int __init mymodule_init(void) {
//     mymounts_file = proc_create("mymounts", 0444, NULL, &mymounts_fops);
//     if (!mymounts_file) {
//         return -ENOMEM;
//     }
//     return 0;
// }

// static void __exit mymodule_exit(void) {
//     proc_remove(mymounts_file);
// }

// module_init(mymodule_init);
// module_exit(mymodule_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("psrikamo");
// MODULE_DESCRIPTION("Module to list mount points.");

//////////////////////////////////////////////////////////////////////////////////

// #include <linux/proc_fs.h>
// #include <linux/seq_file.h>
// #include <linux/mount.h>
// #include <linux/list.h>
// #include <linux/fs.h>

// // Callback function สำหรับ for_each_mnt()
// static int show_mount_points(struct mount *m, void *data)
// {
//     struct seq_file *seq = (struct seq_file *)data;

//     // ตรวจสอบว่า mount point เป็นประเภท filesystem ที่เราต้องการแสดงหรือไม่
//     if (m->mnt_devname && m->mnt_mountpoint) {
//         // m->mnt_devname: ชื่ออุปกรณ์หรือที่มาของ mount (เช่น /dev/sda1)
//         // m->mnt_mountpoint->d_name.name: ชื่อไดเรกทอรีที่ถูก mount (เช่น /home)
//         seq_printf(seq, "%s %s\n", m->mnt_devname, m->mnt_mountpoint->d_name.name);
//     }

//     return 0;
// }

// // ฟังก์ชันหลักที่แสดงผล
// static int mymounts_show(struct seq_file *m, void *v)
// {
//     // ใช้ for_each_mnt() เพื่อสำรวจ mount tree ทั้งหมด
//     // และเรียก show_mount_points() สำหรับแต่ละ mount
//     for_each_mnt(show_mount_points, m);

//     return 0;
// }

// static int mymounts_open(struct inode *inode, struct file *file)
// {
//     return single_open(file, mymounts_show, NULL);
// }

// // File operations สำหรับไฟล์ /proc
// static const struct proc_ops mymounts_fops = {
//     .proc_open = mymounts_open,
//     // .proc_read = seq_read,
//     // .proc_lseek = seq_lseek,
//     // .proc_release = single_release,
// };

// static struct proc_dir_entry *mymounts_file;

// static int __init mymodule_init(void)
// {
//     // สร้างไฟล์ /proc/mymounts
//     mymounts_file = proc_create("mymounts", 0444, NULL, &mymounts_fops);
//     if (!mymounts_file) {
//         return -ENOMEM;
//     }
//     return 0;
// }

// static void __exit mymodule_exit(void)
// {
//     // ลบไฟล์ /proc/mymounts
//     proc_remove(mymounts_file);
// }

// module_init(mymodule_init);
// module_exit(mymodule_exit);

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Your Name");
// MODULE_DESCRIPTION("Module to list mount points.");


//////////////////////////////////////////////////////////////////////////////////


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/path.h>
#include <linux/mount.h>
#include <linux/dcache.h>

#include <linux/fs_struct.h>
// #include <linux/spinlock.h>

#include <linux/namei.h>

#define PROC_NAME "mydriver_paths"

static struct proc_dir_entry *proc_entry;

// Example: Show paths of some important directories
static int mydriver_show(struct seq_file *m, void *v)
{
    struct path root_path, proc_path, sys_path;
    int ret;

    seq_printf(m, "Device Driver Path Information:\n");
    seq_printf(m, "============================\n\n");

    // Get the root filesystem path
    ret = kern_path("/", LOOKUP_FOLLOW, &root_path);
    if (ret == 0) {
        seq_printf(m, "Root filesystem: ");
        seq_path(m, &root_path, " \t\n\\");
        seq_printf(m, "\n");
        path_put(&root_path);
    }

    // Get /proc path
    ret = kern_path("/proc", LOOKUP_FOLLOW, &proc_path);
    if (ret == 0) {
        seq_printf(m, "/proc mount point: ");
        seq_path(m, &proc_path, " \t\n\\");
        seq_printf(m, "\n");
        path_put(&proc_path);
    }

    // Get /sys path
    ret = kern_path("/sys", LOOKUP_FOLLOW, &sys_path);
    if (ret == 0) {
        seq_printf(m, "/sys mount point: ");
        seq_path(m, &sys_path, " \t\n\\");
        seq_printf(m, "\n");
        path_put(&sys_path);
    }

    // Example showing current working directory (if available)
    if (current->fs && current->fs->pwd.dentry) {
        seq_printf(m, "Current process pwd: ");
        seq_path(m, &current->fs->pwd, " \t\n\\");
        seq_printf(m, "\n");
    }

    return 0;
}

// // Alternative example showing device-specific paths
// static int show_device_paths(struct seq_file *m, void *v)
// {
//     struct path dev_path, block_path;
//     int ret;

//     seq_printf(m, "Device Path Information:\n");
//     seq_printf(m, "=======================\n\n");

//     // Show /dev path
//     ret = kern_path("/dev", LOOKUP_FOLLOW, &dev_path);
//     if (ret == 0) {
//         seq_printf(m, "Device directory: ");
//         seq_path(m, &dev_path, " \t\n\\");
//         seq_printf(m, "\n");
//         path_put(&dev_path);
//     }

//     // Show /sys/block path
//     ret = kern_path("/sys/block", LOOKUP_FOLLOW, &block_path);
//     if (ret == 0) {
//         seq_printf(m, "Block devices sysfs: ");
//         seq_path(m, &block_path, " \t\n\\");
//         seq_printf(m, "\n");
//         path_put(&block_path);
//     }

//     // Example of showing mount information
//     seq_printf(m, "\nMount information for /sys:\n");
//     ret = kern_path("/sys", LOOKUP_FOLLOW, &dev_path);
//     if (ret == 0) {
//         // struct vfsmount *mnt = dev_path.mnt;
//         // seq_printf(m, "Mount source: %s\n",
//         //            mnt->mnt_devname ? mnt->mnt_devname : "none");
//         // seq_printf(m, "Filesystem type: %s\n",
//         //            mnt->mnt_sb->s_type->name);
//         // seq_printf(m, "Mount path: ");
//         // seq_path(m, &dev_path, " \t\n\\");
//         // seq_printf(m, "\n");
//         // path_put(&dev_path);

//         struct vfsmount *mnt = dev_path.mnt;
//         seq_printf(m, "Mount source: %s\n",
//                    mnt->mnt_root ? mnt->mnt_root : "none");
//         seq_printf(m, "Filesystem type: %s\n",
//                    mnt->mnt_sb->s_type->name);
//         seq_printf(m, "Mount path: ");
//         seq_path(m, &dev_path, " \t\n\\");
//         seq_printf(m, "\n");
//         path_put(&dev_path);
//     }

//     return 0;
// }

static int mydriver_open(struct inode *inode, struct file *file)
{
    return single_open(file, mydriver_show, NULL);
}

static const struct proc_ops mydriver_proc_ops = {
    .proc_open = mydriver_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init mydriver_init(void)
{
    proc_entry = proc_create(PROC_NAME, 0644, NULL, &mydriver_proc_ops);
    if (!proc_entry) {
        printk(KERN_ERR "Failed to create /proc/%s\n", PROC_NAME);
        return -ENOMEM;
    }

    printk(KERN_INFO "MyDriver loaded. Check /proc/%s\n", PROC_NAME);
    return 0;
}

static void __exit mydriver_exit(void)
{
    proc_remove(proc_entry);
    printk(KERN_INFO "MyDriver unloaded\n");
}

module_init(mydriver_init);
module_exit(mydriver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Example Driver Author");
MODULE_DESCRIPTION("Example driver showing seq_path usage");
MODULE_VERSION("1.0");
