// gcc -Wall -g -shared -fPIC -ldl -o libtest.so test.c

#include <resolv.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>

#define __USE_GNU
#include <dlfcn.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int gethostbyname_r(const char *name, 
		struct hostent *ret, char *buf, size_t buflen,
		struct hostent **result, int *h_errnop)
{
	int (*f)(const char *name, struct hostent *, char *, size_t, struct hostent **, int *) = dlsym (RTLD_NEXT, "gethostbyname_r");
	int r = f(name, ret, buf, buflen, result, h_errnop);
	if (r == 0 && ret->h_length == 0) {
		char new_name[1024];
		const char *domain;
		domain = getenv("DOMAIN");
		if (!domain) {
			fprintf(stderr, "DOMAIN environment variable not found!\n");
			exit(-1);
		}
		sprintf(new_name, "%s.%s", name, domain);
		r =  f(new_name, ret, buf, buflen, result, h_errnop);
	}
	return r;
}
