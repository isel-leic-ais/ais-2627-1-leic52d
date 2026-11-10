
#define	error_verify_posix(condition, fmt, ...) ({ \
	if (!(condition)) { \
		fprintf(stderr, "%s: " fmt "\n", strerror(errno), ##__VA_ARGS__); \
		exit(EXIT_FAILURE); \
	} \
})
