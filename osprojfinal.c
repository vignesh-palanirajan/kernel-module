#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/sched/signal.h>

int mod_init(void)
{
    struct task_struct *task;
    struct list_head *list;
    printk(KERN_INFO "Loading Module\n");
    task = &init_task;
    list = &task->tasks;
    list_for_each(list, &init_task.tasks)
    {
        struct task_struct *task = list_entry(list, struct task_struct, tasks);
        printk(KERN_INFO "Parent Process: %s\tPID = %d\n", task->comm, task->pid);
        struct task_struct *child;
        list_for_each_entry(child, &task->children, sibling)
        {
            printk(KERN_INFO "|--Child Process: %s\tPID = %d\n", child->comm, child->pid);
            struct task_struct *sibling = list_entry(child->sibling.next, struct task_struct, sibling);
            while (&sibling->sibling != &task->children)
            {
                printk(KERN_INFO "  |--Sibl. Process: %s\tPID = %d\n", sibling->comm, sibling->pid);
                sibling = list_entry(sibling->sibling.next, struct task_struct, sibling);
            }
        }
    }
    return 0;
}

void mod_exit(void)
{
    printk(KERN_INFO "Removing Module\n");
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("A kernel module to display processes, child processes and its process IDs");
MODULE_AUTHOR("Vignesh Palanirajan PES2UG22CS661");

module_init(mod_init);
module_exit(mod_exit);

