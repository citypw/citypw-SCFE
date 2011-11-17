#include <linux/kernel.h>
#include <linux/module.h>

#include <linux/init.h>
#include <linux/netdevice.h>
#include <linux/ip.h>
#include <linux/tcp.h>

#ifdef __KERNEL__
#undef __KERNEL__
#include <linux/netfilter_ipv4.h>

#endif

#define __KERNEL__
MODULE_LICENSE("GPL") ;


static unsigned int idrop(unsigned int hooknum,struct sk_buff**skb,const struct net_device*in,const struct net_device*out,int(*okfn)(struct sk_buff*)){

	return NF_QUEUE;
}


static struct nf_hook_ops ishnet=
{{NULL,NULL},
	idrop,
	NULL,
	PF_INET,
	NF_IP_LOCAL_IN,
	NF_IP_PRI_FIRST
};

static int init_hello(void){
	return nf_register_hook(&ishnet);
}
static void cleanup_hello(void){
	nf_unregister_hook(&ishnet);
}

module_init(init_hello);
module_exit(cleanup_hello);
