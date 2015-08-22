#ifndef _ASSERT_H
#define _ASSERT_H

#define assert(exp) \
	if (exp) ;\
	else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)

#define assertion_failure(xx_, xxx_, xxxx_, xxxxx_)\
		printf ("assert:%s failed_file:%s base_file:%s line:%d", xx_, xxx_, xxxx_, xxxxx_)

#endif