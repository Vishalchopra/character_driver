#include"header.h"
#include"declaration.h"

loff_t lseek_dev(struct file *fil, loff_t offset, int pos)
{
	struct DEV *lsdev;
	#ifdef DEBUG
	printk(KERN_INFO "Begin %s", __func__);
	#endif
	lsdev = fil->private_data;
	switch(pos)
	{
		case 0:
		#ifdef DEBUG
		printk(KERN_INFO "SEEK_BEGIN %s", __func__);
		#endif
		fil->f_pos = offset;
		break;

		case 1:
		#ifdef DEBUG
		printk(KERN_INFO "SEEK_CURR %s", __func__);
		#endif
		if(fil->f_pos + offset > lsdev->data_size || fil->f_pos + offset <= 0)
		{
			data_size = 0;
		}
		else
			fil->f_pos = fil->f_pos + offset;
		break;

		case 2:
		#ifdef DEBUG
		printk(KERN_INFO "SEEK_END %s", __func__);
		#endif
		fil->f_pos = lsdev->data_size + offset;
		
		break;
	}

	#ifdef DEBUG
	printk(KERN_INFO "END %s", __func__);
	#endif
	return 0;
}
