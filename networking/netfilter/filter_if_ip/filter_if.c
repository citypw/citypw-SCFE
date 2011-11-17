/* This program will show you how to use netfilter framework to filtering
 * the packets by ip addresses, network interfaces and network protocols. 
 * happy hacking, Fellows!
 *
 *
 * monkey-coder: Shawn the R0ck@copyleft, <citypw@gmail.com>
 * 
 * This original code that it's from Hacking the Linux Kernel Network 
 * Stack that written by bioforge.Thanks bioforge, about your great article.
 *
 * I modified the source code make it fit in the kernel-2.6. And tested as 
 * well in GNU/linux2.6.25, 2.6.29.6 and 2.6.33.4.
 * 
 * This code is also GPL'ed!
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ip.h>		/* For IP header */
#include <linux/tcp.h>		/* For TCP header */
#include <linux/skbuff.h>
#include <linux/netdevice.h>
#include <linux/netfilter.h>
#ifdef __KERNEL__
#undef __KERNEL__
#include <linux/netfilter_ipv4.h>
#endif

#define __KERNEL__

MODULE_LICENSE("GPL");

#define BE_PRINT_IP(addr) \
	((unsigned char*)&addr)[0], \
	((unsigned char*)&addr)[1], \
	((unsigned char*)&addr)[2], \
	((unsigned char*)&addr)[3]

#define BITS16_REVERSE(addr) (((addr << 8) | (addr >> 8)))

/* This is the structure we shall use to register our function */
static struct nf_hook_ops nfho;

/* e.g: network interface "eth0", ip address "127.0.0.1" and protocol */
static char *drop_if = "eth0";

/* IP address we want to drop packets from, in network-byte order(Big-endian) */
static unsigned char *drop_ip = "\x7f\x00\x00\x01";

struct app {
	char name[32];
	unsigned char *deny_port;
};

struct app ssh;

/* filtering by network interface */
static unsigned int filter_by_if(unsigned int hooknum,
				 struct sk_buff *skb,
				 const struct net_deivce *in,
				 const struct net_device *out,
				 int (*okfn) (struct sk_buff *))
{
	struct net_device *if_in = in;

	if ((strcmp(if_in->name, drop_if)) == 0) {
		printk("dropped packet on %s...\n", drop_if);
		return NF_DROP;
	} else {
		return NF_ACCEPT;
	}

}

/* filtering by ip address */
static unsigned int filter_by_ipaddr(unsigned int hooknum,
				     struct sk_buff *skb,
				     const struct net_deivce *in,
				     const struct net_device *out,
				     int (*okfn) (struct sk_buff *))
{
	struct sk_buff *sb = skb;
	struct iphdr *iph;

	iph = ip_hdr(sb);

	printk("length is %d\n", iph->ihl);
	if (iph->saddr == *(unsigned int *) drop_ip) {
		printk("droped packet from ... %d.%d.%d.%d\n",
		       *drop_ip, *(drop_ip + 1),
		       *(drop_ip + 2), *(drop_ip + 3));
		return NF_DROP;
	} else {
		return NF_ACCEPT;
	}
}

/* filtering by TCP protocol */
static int check_tcp_packet(unsigned int hooknum,
			    struct sk_buff *skb,
			    const struct net_device *in,
			    const struct net_device *out,
			    int (*okfn) (struct sk_buff *))
{
	struct tcphdr *thead;
	struct iphdr *iph;
	unsigned short uport;

	struct sk_buff *sb = skb;
	iph = ip_hdr(sb);
	thead = (struct tcphdr *) (sb->data + (iph->ihl * 4));

	/* We don't want any NULL pointers in the chain to IP header. */
	if (!sb)
		return NF_ACCEPT;
	if (!iph)
		return NF_ACCEPT;

	/* Be sure this is a TCP packet */
	if (iph->protocol != IPPROTO_TCP) {
		return NF_ACCEPT;
	}

	if ((thead->dest) == *(unsigned short *) (ssh.deny_port)) {
		uport = BITS16_REVERSE(thead->dest);
		printk
		    ("droped tcp packet from...IP:%d.%d.%d.%d Port:%u Application:%s\n",
		     BE_PRINT_IP(iph->saddr), uport, ssh.name);
		return NF_DROP;
	}
	return NF_ACCEPT;
}

static int init_filter_if()
{
	printk("initializing the hooks!\n");

	memset(&ssh, 0x00, sizeof(ssh));
	strcpy(ssh.name, "ssh");
	ssh.deny_port = "\x00\x16";

	/* remember which hook you specified */
	nfho.hook = check_tcp_packet;
	nfho.hooknum = NF_IP_PRE_ROUTING;
	nfho.pf = PF_INET;	/* ipv4 protocols */
	nfho.priority = NF_IP_PRI_FIRST;

	nf_register_hook(&nfho);

	return 0;
}

static void cleanup_filter_if()
{
	printk("bye hooks\n");
	nf_unregister_hook(&nfho);
}

module_init(init_filter_if);
module_exit(cleanup_filter_if);
