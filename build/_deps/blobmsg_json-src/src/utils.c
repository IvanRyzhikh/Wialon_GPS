/*
 * utils - misc libubox utility functions
 *
 * Copyright (C) 2012 Felix Fietkau <nbd@openwrt.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/mman.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ubox/utils.h"

#define foreach_arg(_arg, _addr, _len, _first_addr, _first_len) \
	for (_addr = (_first_addr), _len = (_first_len); \
		_addr; \
		_addr = va_arg(_arg, void **), _len = _addr ? va_arg(_arg, size_t) : 0)

#define C_PTR_ALIGN	(sizeof(size_t))
#define C_PTR_MASK	(-C_PTR_ALIGN)

void *__calloc_a(size_t len, ...)
{
	va_list ap, ap1;
	void *ret;
	void **cur_addr;
	size_t cur_len;
	int alloc_len = 0;
	char *ptr;

	va_start(ap, len);

	va_copy(ap1, ap);
	foreach_arg(ap1, cur_addr, cur_len, &ret, len)
		alloc_len += (cur_len + C_PTR_ALIGN - 1 ) & C_PTR_MASK;
	va_end(ap1);

	ptr = calloc(1, alloc_len);
	if (!ptr) {
		va_end(ap);
		return NULL;
	}

	alloc_len = 0;
	foreach_arg(ap, cur_addr, cur_len, &ret, len) {
		*cur_addr = &ptr[alloc_len];
		alloc_len += (cur_len + C_PTR_ALIGN - 1) & C_PTR_MASK;
	}
	va_end(ap);

	return ret;
}

#ifdef LIBUBOX_COMPAT_CLOCK_GETTIME
#include <mach/mach_host.h>		/* host_get_clock_service() */
#include <mach/mach_port.h>		/* mach_port_deallocate() */
#include <mach/mach_init.h>		/* mach_host_self(), mach_task_self() */
#include <mach/clock.h>			/* clock_get_time() */

static clock_serv_t clock_realtime;
static clock_serv_t clock_monotonic;

static void __constructor clock_name_init(void)
{
	mach_port_t host_self = mach_host_self();

	host_get_clock_service(host_self, CLOCK_REALTIME, &clock_realtime);
	host_get_clock_service(host_self, CLOCK_MONOTONIC, &clock_monotonic);
}

static void __destructor clock_name_dealloc(void)
{
	mach_port_t self = mach_task_self();

	mach_port_deallocate(self, clock_realtime);
	mach_port_deallocate(self, clock_monotonic);
}

int clock_gettime(int type, struct timespec *tv)
{
	int retval = -1;
	mach_timespec_t mts;

	switch (type) {
		case CLOCK_REALTIME:
			retval = clock_get_time(clock_realtime, &mts);
			break;
		case CLOCK_MONOTONIC:
			retval = clock_get_time(clock_monotonic, &mts);
			break;
		default:
			goto out;
	}

	tv->tv_sec = mts.tv_sec;
	tv->tv_nsec = mts.tv_nsec;
out:
	return retval;
}

#endif

void *cbuf_alloc(unsigned int order)
{
	char path[] = "/tmp/cbuf-XXXXXX";
	unsigned long size = cbuf_size(order);
	void *ret = NULL;
	int fd;

	fd = mkstemp(path);
	if (fd < 0)
		return NULL;

	if (unlink(path))
		goto close;

	if (ftruncate(fd, cbuf_size(order)))
		goto close;

#ifndef MAP_ANONYMOUS
#define MAP_ANONYMOUS MAP_ANON
#endif

	ret = mmap(NULL, size * 2, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (ret == MAP_FAILED) {
		ret = NULL;
		goto close;
	}

	if (mmap(ret, size, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED,
		 fd, 0) != ret ||
	    mmap(ret + size, size, PROT_READ | PROT_WRITE,
		 MAP_FIXED | MAP_SHARED, fd, 0) != ret + size) {
		munmap(ret, size * 2);
		ret = NULL;
	}

close:
	close(fd);
	return ret;
}

void cbuf_free(void *ptr, unsigned int order)
{
	munmap(ptr, cbuf_size(order) * 2);
}

int mkdir_p(char *dir, mode_t mask)
{
	char *l;
	int ret;

	ret = mkdir(dir, mask);
	if (!ret || errno == EEXIST)
		return 0;

	if (ret && (errno != ENOENT))
		return -1;

	l = strrchr(dir, '/');
	if (!l || l == dir)
		return -1;

	*l = '\0';

	if (mkdir_p(dir, mask))
		return -1;

	*l = '/';

	ret = mkdir(dir, mask);
	if (!ret || errno == EEXIST)
		return 0;
	else
		return -1;
}
