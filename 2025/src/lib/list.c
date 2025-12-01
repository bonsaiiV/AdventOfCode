#include <stdlib.h>

#include "lib/list.h"

void resize_list(list * l) {
	if (l->size == l->cap) {
		l->cap = (l->cap * 3) / 2 + 1;
		l->data = (void **) reallocarray(l->data, l->cap, sizeof(void*));
	}
}

list * create_list_with_cap(size_t cap) {
	list *ret = (list*) malloc(sizeof(list));
	ret->cap = cap;
	ret->size = 0;
	ret->data = (void **) calloc(ret->cap, sizeof(void*));
	return ret;
}

list * create_list(void) {
	return create_list_with_cap(5);
}

void list_push(list * l, void * e) {
	resize_list(l);
	l->data[l->size++] = e;
}

void * list_pop(list * l) {
	if (!l->size) {
		return NULL;
	}
	return l->data[--(l->size)];
}

list * list_clone(list * l) {
	list * ret = malloc(sizeof(list));
	ret->cap = l->cap;
	ret->data = calloc(ret->cap, sizeof(void*));
	ret->size = l->size;
	for (size_t i = 0; i < ret->size; i++) {
		ret->data[i] = l->data[i];
	}
	return ret;
}

void list_swap(list * l, size_t i1, size_t i2) {
	void * tmp = l->data[i1];
	l->data[i1] = l->data[i2];
	l->data[i2] = tmp;
}

void list_free(list * l) {
	free(l->data);
	free(l);
}
void list_free_all(list * l) {
	for (size_t i = 0; i < l->size; i++) {
		free(l->data[i]);
	}
	free(l->data);
	free(l);
}
