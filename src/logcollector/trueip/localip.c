#include "trueip.h"

char *print_addresses(const int domain)
{
  int s;
  struct ifconf ifconf;
  struct ifreq ifr[50];
  int ifs;
  int i;
  int b=1;
  char* rtext = (char*) malloc(1000);

  s = socket(domain, SOCK_STREAM, 0);
  if (s < 0) {
    perror("socket");
    return 0;
  }

  ifconf.ifc_buf = (char *) ifr;
  ifconf.ifc_len = sizeof ifr;

  if (ioctl(s, SIOCGIFCONF, &ifconf) == -1) {
    perror("ioctl");
    return 0;
  }

  ifs = ifconf.ifc_len / sizeof(ifr[0]);
  //(domain == AF_INET ) ? printf("\"ipv4\": ") : printf("\"ipv6\": ");
  //printf("{\"interfaces\": [");

  sprintf(rtext, "");
  for (i = 0; i < ifs; i++) {
    char ip[INET_ADDRSTRLEN];
    struct sockaddr_in *s_in = (struct sockaddr_in *) &ifr[i].ifr_addr;

    if (!inet_ntop(domain, &s_in->sin_addr, ip, sizeof(ip))) {
      perror("inet_ntop");
      return 0;
    }

    if (!b) {
	//printf(",");
	sprintf(rtext, "%s,",rtext);
    }
    //printf("{\"name\": \"%s\", \"ip\": \"%s\"}", ifr[i].ifr_name, ip);
    if(strcmp(ip,"127.0.0.1")!=0){
	sprintf(rtext, "%s%s=%s", rtext,ifr[i].ifr_name,ip);
	b=0;
    }
  }
  //printf("]}");

  close(s);

  return rtext;
}

char *get_localip()
{
  //int domains[] = { AF_INET, AF_INET6 };
  int domains[] = { AF_INET };
  int i;
  int ob=1;
  char* rtext;

  //printf("{");
  for (i = 0; i < sizeof(domains) / sizeof(domains[0]); i++){
    //if(!ob) printf(",");
    ob=0;
    if ((rtext = print_addresses(domains[i]))<0)
      return "";
  }

  //printf("}");
	
  return rtext;
}
