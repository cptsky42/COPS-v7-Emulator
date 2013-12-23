/* port.h - portability defines */

#if defined(__FreeBSD__)
# define FreeBSD
# define ARCH "FreeBSD"
#elif defined(linux)
# define Linux
# define ARCH "Linux"
#elif defined(sun)
# define Solaris
# define ARCH "Solaris"
#else
# define UNKNOWN
# define ARCH "UNKNOWN"
#endif

#ifdef FreeBSD
#endif /* FreeBSD */

#ifdef Linux
#endif /* Linix */

#ifdef Solaris
# define HAVE_SIGSET
#endif /* Solaris */
