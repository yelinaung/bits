#include "snek_obj.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bootlib.h"

snek_object_t *snek_add(snek_object_t *a, snek_object_t *b) {
  if (a == NULL || b == NULL) {
    return NULL;
  }

  switch (a->kind) {
  case INTEGER:
    switch (b->kind) {
    case INTEGER:
      // a is int, b is int
      return new_snek_integer(a->data.v_int + b->data.v_int);
    case FLOAT:
      // a is int, b is float
      return new_snek_float((float)a->data.v_int + b->data.v_float);
    default:
      return NULL;
    }
  case FLOAT:
    switch (b->kind) {
    case INTEGER:
      // a is float, b is int
      return new_snek_float(a->data.v_float + (float)b->data.v_int);
    case FLOAT:
      // a is float, b is float
      return new_snek_float(a->data.v_float + b->data.v_float);
    default:
      return NULL;
    }
  case STRING:
    switch (b->kind) {
    case STRING:
      // Calculate the length of the new string by combining the length of the
      // two strings (properly handling the null terminator)
      size_t new_str_len =
          strlen(a->data.v_string) + strlen(b->data.v_string) + 1;
      char *temp_str = calloc(sizeof(char), new_str_len);
      // Use strcat to append the data from a and then b to the temporary
      // string.
      strcat(temp_str, a->data.v_string);
      strcat(temp_str, b->data.v_string);
      // Create a new_snek_string and pass in the temporary string.
      snek_object_t *n = new_snek_string(temp_str);
      // Free the memory for the temporary string
      free(temp_str);
      return n;
    default:
      return NULL;
    }
  case VECTOR3:
    switch (b->kind) {
    case VECTOR3:
      return new_snek_vector3(
          snek_add(a->data.v_vector3.x, b->data.v_vector3.x),
          snek_add(a->data.v_vector3.y, b->data.v_vector3.y),
          snek_add(a->data.v_vector3.z, b->data.v_vector3.z));
    default:
      return NULL;
    }
  case ARRAY:
    switch (b->kind) {
    case ARRAY:
      snek_object_t *s =
          new_snek_array(a->data.v_array.size + b->data.v_array.size);
      // Iterate over each index in "a" and use snek_array_set and
      // snek_array_get to copy the values from "a" to the new array.
      for (size_t i = 0; i < a->data.v_array.size; i++) {
        snek_object_t *x = snek_array_get(a, i);
        snek_array_set(s, i, x);
      }
      // Do the same for "b".
      for (size_t i = 0; i < b->data.v_array.size; i++) {
        snek_object_t *x = snek_array_get(b, i);
        snek_array_set(s, a->data.v_array.size + i, x);
      }
      return s;
    default:
      return NULL;
    }
  default:
    return NULL;
  }
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
    return (int)strlen(obj->data.v_string);
  case VECTOR3:
    return 3;
  case ARRAY:
    return (int)obj->data.v_array.size;
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
