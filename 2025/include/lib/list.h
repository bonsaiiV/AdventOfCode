#ifndef LIST_H
#define LIST_H
#include <stddef.h>
typedef struct _list {
	void ** data;
	size_t cap;
	size_t size;
} list;

void resize_list(list * l);
list * create_list(void);
list * create_list_with_cap(size_t cap);
void list_push(list * l, void * e);
list * list_clone(list * l);
void * list_pop(list * l);
void list_swap(list * l, size_t i1, size_t i2);
void list_free(list * l);
void list_free_all(list * l);

#define LIST_FOR_EACH(e, l) e = l->data[0]; for(size_t e##_vi = 0; e##_vi < l->size; e = l->data[++e##_vi])
#endif
