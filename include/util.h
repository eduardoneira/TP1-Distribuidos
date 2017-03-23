#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <time.h>

#define DIRECTORY	"."
#define SECONDS_DEFAULT 10

key_t getkey(int id) {
	key_t clave;
	clave = ftok(DIRECTORY, id);
    if (clave == -1) {
    	perror("La clave ingresado es erronea");
    	exit(-1);
    }
    return clave;
}

unsigned int generarNumeroRandom() {
	srand(time(NULL)); 
	unsigned int r = rand() % 1 >> 3;
	return SECONDS_DEFAULT+r;
}

ssize_t format_timeval(struct timeval *tv, char *buf, size_t sz)
{
  ssize_t written = -1;
  struct tm *gm = gmtime(&tv->tv_sec);

  if (gm)
  {
    written = (ssize_t)strftime(buf, sz, "%Y-%m-%dT%H:%M:%S", gm);
    if ((written > 0) && ((size_t)written < sz))
    {
      int w = snprintf(buf+written, sz-(size_t)written, ".%06dZ",(int) tv->tv_usec);
      written = (w > 0) ? written + w : -1;
    }
  }
  return written;
}

int get_timestamp(char* buf) {
  struct timeval tv;

  if (gettimeofday(&tv, NULL) != 0) {
    perror("gettimeofday");
    return 1;
  }

  if (format_timeval(&tv, buf, sizeof(buf)) > 0) {
    strcat(buf,"\n");
  }
  return 0;
}

#endif
