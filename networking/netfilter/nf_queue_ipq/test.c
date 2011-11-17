#include <netinet/in.h>
#include <linux/netfilter.h>
#include <libipq.h>
#include <stdio.h>
#define BUFSIZE 2048
static void die(struct ipq_handle *h)
{
	ipq_perror("passer");
	ipq_destroy_handle(h);
	exit(1);
}
int main(int argc, char **argv)
{
	int status;
	unsigned char buf[BUFSIZE];
	struct ipq_handle *h;

	h = ipq_create_handle(0, PF_INET);
	if (!h)
		die(h);

	status = ipq_set_mode(h, IPQ_COPY_PACKET, BUFSIZE);
	if (status < 0)
		die(h);

	do{
		status = ipq_read(h, buf, BUFSIZE, 0);
		if (status < 0)
			die(h);

		ipq_packet_msg_t *m = ipq_get_packet(buf);
		status = ipq_set_verdict(h, m->packet_id,
				NF_ACCEPT, 0, NULL);
	} while (1);

	ipq_destroy_handle(h);
	return 0;
}
