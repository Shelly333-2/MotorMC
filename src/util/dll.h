#pragma once
#include "../main.h"
#include "id_vector.h"

typedef struct {

	void* element;
	uint32_t previous;
	uint32_t next;

} utl_dll_node_t;

typedef struct {

	utl_id_vector_t nodes;
	uint32_t first;
	uint32_t last;
	uint32_t length;

} utl_dll_t;

typedef struct {

	utl_dll_t* dll;
	uint32_t next_idx;
	uint32_t next_node;

} utl_dll_iterator_t;

#define UTL_DLL_INITIALIZER { .nodes = UTL_ID_VECTOR_INITIALIZER(utl_dll_node_t), .first = 0, .last = 0, .length = 0 }
#define UTL_DLL_ITERATOR_INITIALIZER(ptr) (utl_dll_iterator_t) { .dll = ptr, .next_idx = 0, .next_node = (ptr)->first }

static inline utl_dll_t* utl_create_dll() {

	utl_dll_t* dll = calloc(1, sizeof(utl_dll_t));
	utl_init_id_vector(&dll->nodes, sizeof(utl_dll_node_t));

	return dll;

}

static inline void utl_init_dll(utl_dll_t* dll) {

	memset(dll, 0, sizeof(utl_dll_t));
	utl_init_id_vector(&dll->nodes, sizeof(utl_dll_node_t));

}

static inline void* utl_dll_iterator_next(utl_dll_iterator_t* iterator) {

	if (iterator->next_idx >= iterator->dll->length) return NULL;

	utl_dll_node_t* node = utl_id_vector_get(&iterator->dll->nodes, iterator->next_node);
	iterator->next_idx++;
	iterator->next_node = node->next;

	return node->element;

}

static inline uint32_t utl_dll_push(utl_dll_t* dll, void* element) {

	if (dll->length == 0) {
		
		const utl_dll_node_t node = {
			.element = element
		};

		uint32_t node_id = utl_id_vector_push(&dll->nodes, &node);
		dll->first = node_id;
		dll->last = node_id;
		dll->length++;

		return node_id;

	} else {
		
		const utl_dll_node_t node = {
			.element = element,
			.previous = dll->last
		};
		
		uint32_t node_id = utl_id_vector_push(&dll->nodes, &node);
		utl_dll_node_t* last = utl_id_vector_get(&dll->nodes, dll->last);

		last->next = node_id;
		dll->last = node_id;
		dll->length++;

		return node_id;

	}

}

static inline void* utl_dll_shift(utl_dll_t* dll) {

	utl_dll_node_t* node = utl_id_vector_get(&dll->nodes, dll->first);
	uint32_t node_id = dll->first;
	dll->first = node->next;
	dll->length--;

	void* element = node->element;
	utl_id_vector_remove(&dll->nodes, node_id);

	return element;

}

static inline void* utl_dll_remove(utl_dll_t* dll, uint32_t id) {

	utl_dll_node_t* node = utl_id_vector_get(&dll->nodes, id);
	if (id != dll->first) {
		utl_dll_node_t* previous = utl_id_vector_get(&dll->nodes, node->previous);
		previous->next = node->next;
	} else {
		dll->first = node->next;
	}
	if (id != dll->last) {
		utl_dll_node_t* next = utl_id_vector_get(&dll->nodes, node->next);
		next->previous = node->previous;
	} else {
		dll->last = node->previous;
	}
	dll->length--;

	void* element = node->element;
	utl_id_vector_remove(&dll->nodes, id);

	return element;

}

static inline void utl_term_dll(utl_dll_t* dll) {

	utl_term_id_vector(&dll->nodes);

}

static inline void utl_destroy_dll(utl_dll_t* dll) {

	utl_term_dll(dll);
	free(dll);

}