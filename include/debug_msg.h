#include <errno.h>

/* Using these 2 macros instead of traditional printf */
#define DEBUG_ERR(format, args...) fprintf(stderr, format, ##args)

#define DEBUG_MSG(string, args...) do{ \
	DEBUG_ERR("File: %s, Function: %s, Line: %d\nDescription: ", __FILE__, __FUNCTION__, __LINE__); \
	DEBUG_ERR(string, ##args); \
	DEBUG_ERR("-------------------------------\n"); \
}while(0);
