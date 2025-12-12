#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "data/list.h"

typedef struct _vertex {
	ssize_t x, y, z;
	size_t id;
	struct _vertex * next;
} vertex;

static list * get_input(char * filename) {
	FILE * fp = fopen(filename, "r");
	list * ret = create_list();
	size_t last_line_len = 8;
	char * line = calloc(last_line_len, sizeof(char));
	vertex * v = malloc(sizeof(vertex));
	int chars_read;
	size_t id = 0;
	while ((chars_read = getline(&line, &last_line_len, fp)) != -1) {
		if (!strcmp(line, "\n")){
			break;
		}

		sscanf(line, "%ld,%ld,%ld", &v->x, &v->y, &v->z);
		v->id = id++;
		v->next = v;

		list_push(ret, v);
		v = malloc(sizeof(vertex));
	}
	free(line);
	return ret;
}
typedef struct {
	vertex * s, * t;
	double len;
} edge;

double dist_v(const vertex * a, const vertex * b) {
	double d_x = (a->x - b->x) / 1.0;
	double d_y = (a->y - b->y) / 1.0;
	double d_z = (a->z - b->z) / 1.0;
	return sqrt(d_x * d_x + d_y * d_y + d_z * d_z);
}
int cmp_e(edge * e1, edge * e2) {
	if (e1->len < e2->len) return -1;
	return e1->len > e2->len;
}
size_t llist_len (vertex * s) {
	vertex * v = s;
	size_t ret = 0;
	do {
		ret++;
		v = v->next;
		if (v->id != s->id) {
			fprintf(stderr, "members in group have different id: %lu != %lu\n", v->id, s->id);
		}
	} while (v != s);
	return ret;
}

int check_done(list * vertices) {
	size_t id = ((vertex *) vertices->data[0])->id;
	vertex * v;
	LIST_FOR_EACH(v, vertices) {
		if (v->id != id) return 0;
	}
	return 1;
}

void day08part1(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	size_t edge_count = input->size * (input->size - 1) / 2;
	edge * all_edges = calloc(edge_count, sizeof(edge));

	vertex * v1, * v2;
	size_t e_i = 0;
	LIST_FOR_EACH(v1, input) {
		LIST_FOR_EACH(v2, input) {
			if (v1 == v2) {
				break;
			}
			all_edges[e_i++] = (edge) {v1, v2, dist_v(v1, v2)};
		}
	}
	qsort(all_edges, edge_count, sizeof(edge), (int (*) (const void *, const void *)) cmp_e);
	#define example_select_edges 10
	#define input_select_edges 1000
	for (size_t e_i = 0; e_i < input_select_edges; e_i++) {
		vertex * s = all_edges[e_i].s, * t = all_edges[e_i].t;
		if (s->id == t->id) continue;
		t->id = s->id;
		v1 = t;
		while (v1->next != t) {
			v1 = v1->next;
			v1->id = s->id;
		}
		v1->next = s->next;
		s->next = t;
	}
	size_t ll_len, tmp;
	size_t cc[3] = {0, 0, 0};
	size_t i = 0;
	LIST_FOR_EACH(v2, input) {
		if (v2->id != i++) continue;
		ll_len = llist_len(v2);
		if (ll_len > cc[0]) {
			tmp = cc[0];
			cc[0] = ll_len;
			ll_len = tmp;
		}
		if (ll_len > cc[1]) {
			tmp = cc[1];
			cc[1] = ll_len;
			ll_len = tmp;
		}
		if (ll_len > cc[2]) {
			tmp = cc[2];
			cc[2] = ll_len;
			ll_len = tmp;
		}
	}

	res = cc[0] * cc[1] * cc[2];

	list_free(input);
	printf("result: %ld\n", res);
}
void day08part2(char* filename){
	size_t res = 0;
	list * input = get_input(filename);

	size_t edge_count = input->size * (input->size - 1) / 2;
	edge * all_edges = calloc(edge_count, sizeof(edge));

	vertex * v1, * v2;
	size_t e_i = 0;
	LIST_FOR_EACH(v1, input) {
		LIST_FOR_EACH(v2, input) {
			if (v1 == v2) {
				break;
			}
			all_edges[e_i++] = (edge) {v1, v2, dist_v(v1, v2)};
		}
	}
	qsort(all_edges, edge_count, sizeof(edge), (int (*) (const void *, const void *)) cmp_e);
	e_i = 0;
	for (e_i = 0; e_i < edge_count; e_i++) {
		vertex * s = all_edges[e_i].s, * t = all_edges[e_i].t;
		if (s->id == t->id) continue;
		t->id = s->id;
		v1 = t;
		while (v1->next != t) {
			v1 = v1->next;
			v1->id = s->id;
		}
		v1->next = s->next;
		s->next = t;
		llist_len(s);
		llist_len(t);
		if (check_done(input)) {
			res = s->x * t->x;
		}
	}
	list_free(input);
	printf("result: %ld\n", res);
}
