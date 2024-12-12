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
#define HASH_MAP(type)\
typedef struct __##type##_set { stone_list* buckets; size_t bucket_count; } type##_set;\
type##_set create_##type##_set(size_t number_of_buckets){ type##_set ret; size_t i;\
	ret.buckets = calloc(number_of_buckets, sizeof( type##_list));\
	ret.bucket_count = number_of_buckets;\
	for (i = 0; i < ret.bucket_count; i++) { ret.buckets[i] = stone_list_create(); }\
	return ret; }\
void add_to_##type##_set( type##_set* set, stone in) { stone_list_add(set->buckets + type##_hash(in, set->bucket_count), in); }\
size_t count_##type##_set( type##_set* set) { size_t i; size_t count = 0;\
	for (i = 0; i < set->bucket_count; i++) { count += set->buckets[i].len; }\
	return count; }\
size_t sum_##type##_set( type##_set* set) { size_t i, j; size_t sum = 0;\
	for (i = 0; i < set->bucket_count; i++) {\
		for (j = 0; j < set->buckets[i].len; j++) { sum += set->buckets[i].data[j].count; }\
	} return sum; }\
void empty_##type##_set( type##_set* set) { size_t i;\
	for (i = 0; i < set->bucket_count; i++) { set->buckets[i].len = 0; } }\
void clean_##type##_set( type##_set* set) { size_t i;\
	for (i = 0; i < set->bucket_count; i++) { free(set->buckets[i].data); } }\
int get_next_from_##type##_set( type##_set* in_set, type * ret) { static type##_set* set = 0;\
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
