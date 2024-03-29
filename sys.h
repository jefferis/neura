#ifdef HAVE_CONFIG_H		// This is just an example of what you could do
#include "config.h"		//   when using autoconf for your project.
#endif
#ifdef CWDEBUG			// This is needed so that others can compile
				//   your application without having libcwd installed.
#ifndef _GNU_SOURCE		// Already defined by g++ 3.0 and higher.
#define _GNU_SOURCE		// Needed for libpthread extensions.
#endif
#include <libcw/sysd.h>
#endif
