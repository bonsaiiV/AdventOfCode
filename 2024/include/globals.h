extern int dbg;
#define DBG(...) if (dbg) printf(__VA_ARGS__);

#define LIST(T)\
typedef struct __##T##_list { T *data; size_t cap; size_t len; } T##_list;\
static struct __##T##_list T##_list_create (void) { T *data = calloc(8, sizeof( T )); struct __##T##_list ret = {data, 8, 0}; return ret;}\
static void T##_list_insert(struct __##T##_list* list, T elem) {\
	if (list->cap == list->len) {\
		list->cap *= 2;\
		list->data = reallocarray(list->data, list->cap, sizeof(T));\
	}\
	list->data[list->len++] = elem;\
}
#define LIST_CONTAINS(T)\
static int T##_list_contains (struct __##T##_list *list, T elem) {\
	for (size_t i = 0; i < list->len; i++) {\
		if ( T##_equal(list->data[i], elem)) return 1;\
	}\
	return 0;\
}
#define LIST_CLEAN(T)\
static void T##_list_clean (struct __##T##_list *list) {\
	for (size_t i = 0; i < list->len; i++) {\
		free(list->data[i]);\
	}\
	list->len = 0;\
}
#define HASH_SET(T)\
typedef struct __##T##_set { T##_list* buckets; size_t bucket_count; } T##_set;\
static T##_set create_##T##_set(size_t number_of_buckets){ T##_set ret; size_t i;\
	ret.buckets = calloc(number_of_buckets, sizeof( T##_list));\
	ret.bucket_count = number_of_buckets;\
	for (i = 0; i < ret.bucket_count; i++) { ret.buckets[i] = T##_list_create(); }\
	return ret; }\
static size_t count_##T##_set( T##_set* set) { size_t i; size_t count = 0;\
	for (i = 0; i < set->bucket_count; i++) { count += set->buckets[i].len; }\
	return count; }\
static void empty_##T##_set( T##_set* set) { size_t i;\
	for (i = 0; i < set->bucket_count; i++) { set->buckets[i].len = 0; } }\
static void free_##T##_set( T##_set* set) { size_t i;\
	for (i = 0; i < set->bucket_count; i++) { free(set->buckets[i].data); } free(set->buckets); }\
static int get_next_from_##T##_set( T##_set* in_set, T * ret) { static T##_set* set = 0;\
	if (in_set) { set = in_set; return 1; }\
	if (!set || !ret) return -1;\
	static size_t next_bucket = 0; static size_t next_index = 0;\
	while (1) {\
		if (next_bucket >= set->bucket_count) {\
			next_bucket = 0;\
			next_index = 0;\
			return 2;\
		}\
		if (next_index < set->buckets[next_bucket].len) {\
			*ret = set->buckets[next_bucket].data[next_index++];\
			return 0;\
		} else { next_index = 0; next_bucket++; }\
	}\
}
#define HASH_SET_CLEAN(T)\
static void add_to_##T##_set( T##_set* set, T in) { T##_list_add(set->buckets + T##_hash(in, set->bucket_count), in); }
