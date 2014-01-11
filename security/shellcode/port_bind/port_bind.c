#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>	// sockaddr_in

int main(int argc, char **argv[])
{
	int sockfd, new_sockfd; //listen on sock_fd, new connections on new_sockfd
	struct sockaddr_in host_addr, client_addr; // address info
	socklen_t sin_size;
	int yes=1;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	host_addr.sin_family = AF_INET; // host byte order
	host_addr.sin_port = htons(31337); // net byte order
	host_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my ip

	memset(&(host_addr.sin_zero), '\0', 8);

	bind(sockfd, (struct sockaddr*) &host_addr, sizeof(struct sockaddr));

	listen(sockfd, 4);
	sin_size = sizeof(struct sockaddr_in);

	new_sockfd = accept(sockfd, (struct sockaddr *) &client_addr, &sin_size);
}
