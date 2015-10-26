#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trueip.h"


int pcache_key;
char *pcache_value;

//injects publicip and localips in the log message
char *inject_trueip(char *from)
{
	//const char* from = "Oct  2 14:16:20 sor-dash systemd: Starting Session 122 of user root.";
	//const char* from = "Oct  2 14:16:20 sor-dash sshd[13950]: Accepted password for root from 172.20.20.4 port 50107 ssh2";

	char *tuple1 = (char*) malloc(17);
	char *tuple2 = (char*) malloc(32);
	char *tuple3 = (char*) malloc(strlen(from)-15);
	char *rtuple3;
	char *publicip;
	char *localip;
	//char *localip = (char*) malloc(200);
	char *rtext; 

	strncpy(tuple1, from, 15);
	strncpy(tuple3, from+16, strlen(from)-16);
	sscanf(tuple3,"%s%*[^\n]",tuple2);

 	rtuple3 = (char*) malloc(strlen(from)-14-strlen(tuple2));
	strncpy(rtuple3, tuple3+14+strlen(tuple2), strlen(from)-strlen(tuple2)-16 );

	localip = get_localip();
	//printf("localip=[%s]\n",localip);
	//strcpy(localip,"en1=10.1.1.2,en2=172.20.3.4");
	//publicip = get_publicip();
	publicip = get_publicip_fromcache(localip);	/*if cache miss, it gets it from public*/

	//rtext = (char*) malloc(strlen(tuple1)+strlen(tuple2)+strlen(rtuple3));
	//rtext = (char*) malloc(strlen(tuple1)+strlen(tuple2)+strlen(rtuple3)+strlen(publicip)+10);
	rtext = (char*) malloc(strlen(tuple1)+strlen(tuple2)+strlen(rtuple3)+strlen(publicip)+strlen(localip)+10);

	//sprintf(rtext,"%s %s %s",tuple1,tuple2,rtuple3);
	//sprintf(rtext,"%s %s[public=%s] %s",tuple1,tuple2,publicip,rtuple3);
	sprintf(rtext,"%s %s[public=%s,%s] %s",tuple1,tuple2,publicip,localip,rtuple3);

	free(tuple1);
	free(tuple2);
	free(tuple3);
	free(rtuple3);
	//free(publicip);
//	free(rtext);
	free(localip);
	return rtext;
//	return "test";
}


static size_t get_hash(const char* source)
{
    if (source == NULL) {
        return 0;
    }

    size_t hash = 0;
    while (*source != '\0') {
        char c = *source++;
        int a = c - '0';
        hash = (hash * 10) + a;
    }
    return hash;
}

//try to get the publicip from local memory cache first.
//update the chache if any of localip addresses changes.
char *get_publicip_fromcache(char *localip){
	int localiphash = get_hash(localip);
	if (pcache_key == localiphash){
		//printf("cache hit: found publicip for localip hash: [%s] = [%s]\n",localip,pcache_value);
		return pcache_value;
	}
	pcache_key = localiphash;
	pcache_value = get_publicip();
	//printf("cache miss: setting cache for localip hash: [%s] = [%s]\n",localip,pcache_value);
	return pcache_value;
}

/*
int main()
{
	int i;
	const char* log = "Oct  2 14:16:20 sor-dash sshd[13950]: Accepted password for root from 172.20.20.4 port 50107 ssh2";
	char *hold;

	for(i=0; i<100; i++){
		hold=inject_trueip(log);
		printf("%s\n", hold);
		free(hold);
	}
	free(pcache_value);
	return 0;
}

*/
