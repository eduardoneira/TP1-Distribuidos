#ifndef _LOCKFILE_H_
#define _LOCKFILE_H_

#include <unistd.h>
#include <fcntl.h>
#include <string.h>

flock _getflock(int type) {
	struct flock lock;
	lock.l_type = type;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	return lock;
}

int abrirLock(const char* nombre) {
	return open(nombre,O_CREAT|O_WRONLY,0777);
}

int tomarLock(int fd){
	struct flock lock = _getflock(F_WRLCK);
	return fcntl (fd,F_SETLKW,&lock);
}

int liberarLock(int fd) {
	struct flock lock = _getflock(F_UNLCK);
	return fcntl (fd,F_SETLKW,&lock);
}

int escribir(int fd, const void* buffer, const ssize_t buffsize){
	lseek(fd,0, SEEK_END);
	return write(fd,buffer,buffsize);
}

int cerrarLock(int fd) {
	return close(fd);
}

int eliminarLock(const char* nombre){
	return unlink(nombre);
}

#endif