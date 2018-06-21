#include "common/common.h"
#include "sds/sds.h"
#include "postlist-cache.h"
#include "config.h"

struct postlist_cache postlist_cache_new()
{
	struct postlist_cache c;
	c.math_cache = math_postlist_cache_new(DEFAULT_MATH_CACHE_SZ);
	c.term_cache = term_postlist_cache_new(DEFAULT_TERM_CACHE_SZ);
	c.tot_used = 0;
	c.tot_limit = DEFAULT_MATH_CACHE_SZ + DEFAULT_TERM_CACHE_SZ;

	return c;
}

int postlist_cache_fork(struct postlist_cache *cache,
                        term_index_t ti, math_index_t mi)
{
	int res = 0;
	sds math_cache_path = sdsnew(mi->dir);
	math_cache_path = sdscat(math_cache_path, "/prefix");

	res |= math_postlist_cache_add(&cache->math_cache, math_cache_path);
	res |= term_postlist_cache_add(&cache->term_cache, ti);

	cache->tot_used = cache->math_cache.postlist_sz +
	                  cache->term_cache.postlist_sz;
	
	sdsfree(math_cache_path);
	return res;
}

void postlist_cache_printinfo(struct postlist_cache c)
{
	printf("Cache used: ");
	print_size(c.tot_used);
	printf(" / ");
	print_size(c.tot_limit);
	printf("\n");
}

void postlist_cache_free(struct postlist_cache cache)
{
	math_postlist_cache_free(cache.math_cache);
	term_postlist_cache_free(cache.term_cache);
}