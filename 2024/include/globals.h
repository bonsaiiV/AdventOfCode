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
