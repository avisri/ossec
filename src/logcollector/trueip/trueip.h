#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/netdevice.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

extern int pcache_key;
extern char *pcache_value;

char *get_localip();
char *get_publicip();
char *get_publicip_fromcache(char *localip);
