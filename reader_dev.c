#include"header.h"
#include"declaration.h"

ssize_t reader_dev(struct file *filp, char __user *ubuff, size_t size, loff_t *loff)
{
	int no_qs, qun_siz, ret, nocsr, noctr, lv, i, size_rd, siz, pos, init, start, rem;
	struct DEV *lsdev;
	struct Qset *lqset;
	lsdev = filp->private_data;
	ret = i = nocsr = 0;
	#ifdef DEBUG
	printk(KERN_INFO "BEGIN.....%s\n", __func__);
	#endif
	if(filp->f_pos == 0)
	wait_for_completion(&lsdev->comp);
	//down(&lsdev->sem);
	#ifdef DEBUG
	printk(KERN_INFO "loff_t = %d lsdev->data_size = %d f_pos = %d filp = %p data_size = %d\n", (int)*loff, lsdev->data_size, (int)filp->f_pos, &filp->f_pos, data_size);
	#endif
	if(data_size == 0)
	{
		#ifdef DEBUG
		printk(KERN_INFO "Nothing to read data_size = 0 %s\n", __func__);
		#endif
		goto OUT;
	}
	pos = (int)*loff;
	siz = data_size;
	//temp = lsdev->data_size + pos;
	
	//if(temp < data_size)
	//{
	//	pos = temp;
	//}


	if(siz >= size)
		size_rd = size;
	else
		size_rd = siz;
	start = init = pos/quantum_size;
	rem = pos % quantum_size;
	noctr = size_rd;
	#ifdef DEBUG
	printk(KERN_INFO "size_rd = %d\n", size_rd);
	#endif

	no_qs = (size_rd + *loff) / (qset_size * quantum_size);/*Calculation of number of struct qset*/
	if((size_rd + *loff) % (qset_size * quantum_size) )
			no_qs++;
	
	qun_siz = (size_rd + *loff) / quantum_size;
	if((size_rd + *loff) % quantum_size)
		qun_siz++;
	
	lqset = lsdev->qset;
	while(start >= qset_size)
	{
		start = start - quantum_size;
		lqset = lqset->next;
	}
	#ifdef DEBUG
	printk(KERN_INFO "no_qs = %d qun_siz = %d start = %d\n", no_qs, qun_siz, start);
	#endif
	for(lv = init; lv <= qun_siz; lv++)
	{
		if(lv == init)
			noctr = quantum_size - rem; 
		if(noctr > quantum_size)
			noctr = quantum_size;
		ret = copy_to_user(ubuff + nocsr, lqset->data[start] + rem, noctr);
		printk(KERN_INFO "nocsr ..%d noctr ..%d ret = %d\n", nocsr, noctr, ret);
		
		#ifdef DEBUG
		printk(KERN_INFO "Wrote on the device is %s\n", (char *)lqset->data[start] + rem);
		#endif

		rem = 0;
		nocsr = nocsr + noctr - ret;
		noctr = size_rd - nocsr; 
		start++;	
		if(start == qset_size)
		{
			if(lqset->next != NULL)
			{
				#ifdef DEBUG
				printk(KERN_ERR "Next qset is not present ...%s\n", __func__);
				#endif
				start = 0;
				lqset = lqset->next;
			}
			else 
				goto OUT;
		}
	}
        *loff = nocsr;               
	filp->f_pos = filp->f_pos + *loff;
	data_size = lsdev->data_size - filp->f_pos;
	#ifdef DEBUG
	printk(KERN_INFO "nocsr = %d f_pos = %d loff = %d data_size = %d\n", nocsr, (int)filp->f_pos, (int)*loff, data_size);
	#endif

	#ifdef DEBUG
	printk(KERN_INFO "END.....%s\n", __func__);
	#endif
	//up(&lsdev->sem);
	return nocsr;
OUT:
	#ifdef DEBUG
	printk(KERN_INFO "END.....%s\n", __func__);
	#endif
	filp->f_pos = filp->f_pos + nocsr;
	return nocsr;
}

