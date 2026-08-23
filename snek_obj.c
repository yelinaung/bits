#include "snek_obj.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bootlib.h"

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) {
  // ?
  return a;
}

int snek_length(snek_object_t *obj) {
  if (obj == NULL) {
    return -1;
  }
  switch (obj->kind) {
  case INTEGER:
    return 1;
  case FLOAT:
    return 1;
  case STRING:
    return strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return obj->data.v_array.size;
  default:
    return -1;
  }
  return 0;
}

snek_object_t *snek_array_get(snek_object_t *snek_obj, size_t index) {
  if (snek_obj == NULL || snek_obj->kind != ARRAY) {
    return NULL;
  }

  if (index >= snek_obj->data.v_array.size) {
    return NULL;
  }
  return snek_obj->data.v_array.elements[index];
}

bool snek_array_set(snek_object_t *snek_obj, size_t index,
                    snek_object_t *value) {
  if (snek_obj == NULL || value == NULL || snek_obj->kind != ARRAY) {
    return false;
  }

  // If the index is out of bounds, return false.
  // v_array field has a size field
  if (index >= snek_obj->data.v_array.size) {
    return false;
  }

  snek_obj->data.v_array.elements[index] = value;
  return true;
}

snek_object_t *new_snek_array(size_t size) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  snek_object_t **elements = calloc(sizeof(snek_object_t), size);
  if (elements == NULL) {
    free(elements);
    return NULL;
  }
  obj->kind = ARRAY;
  obj->data.v_array = (snek_array_t){.size = size, .elements = elements};
  return obj;
}

snek_object_t *new_snek_vector3(snek_object_t *x, snek_object_t *y,
                                snek_object_t *z) {
  if (x == NULL || y == NULL || z == NULL) {
    return NULL;
  }

  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = VECTOR3;
  obj->data.v_vector3 = (sneck_vector_t){.x = x, .y = y, .z = z};
  return obj;
}

snek_object_t *new_snek_string(char *value) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }

  size_t s_size = strlen(value) + 1;
  char *dst = malloc(s_size);
  if (dst == NULL) {
    free(dst);
    return NULL;
  }
  // NOTE: destination first
  strcpy(dst, value);
  obj->kind = STRING;
  obj->data.v_string = dst;
  return obj;
}

snek_object_t *new_snek_float(float value) {
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    return NULL;
  }
  obj->kind = FLOAT;
  obj->data.v_float = value;
  return obj;
}

snek_object_t *new_snek_integer(int value) {
  // Use malloc to allocate heap memory for a new pointer to a snek_object_t.
  snek_object_t *obj = malloc(sizeof(snek_object_t));
  if (obj == NULL) {
    // If the allocation fails, return NULL.
    return NULL;
  }
  // Set the kind field of the new snek object to the INTEGER enum value
  obj->kind = INTEGER;

  // Set the v_int field of the new snek object to
  // the integer value passed in.
  obj->data.v_int = value;

  // Return the pointer to the new snek object.
  return obj;
}
