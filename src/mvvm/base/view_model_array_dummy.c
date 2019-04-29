﻿/**
 * File:   view_model_array_dummy.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array_dummy
 *
 * Copyright (c) 2019 - 2019  Guangzhou ZHIYUAN Electronics Co.,Ltd.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * License file for more details.
 *
 */

/**
 * History:
 * ================================================================
 * 2019-03-12 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include "tkc/utils.h"
#include "tkc/object_default.h"
#include "mvvm/base/utils.h"
#include "mvvm/base/view_model_array_dummy.h"

static ret_t view_model_array_dummy_on_destroy(object_t* obj) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);

  darray_deinit(&(dummy->array));
  view_model_array_deinit(VIEW_MODEL(dummy));

  return RET_OK;
}

static int32_t view_model_array_dummy_compare(object_t* obj, object_t* other) {
  return tk_str_cmp(obj->name, other->name);
}

static ret_t view_model_array_dummy_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  view_model_t* submodel = NULL;
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(VIEW_MODEL(obj), value_int(v));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < dummy->array.size, RET_BAD_PARAMS);
  submodel = VIEW_MODEL(dummy->array.elms[index]);

  return object_set_prop(OBJECT(submodel), name, v);
}

static ret_t view_model_array_dummy_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  view_model_t* submodel = NULL;
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(obj != NULL && name != NULL && v != NULL, RET_BAD_PARAMS);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, dummy->array.size);

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  return_value_if_fail(index < dummy->array.size, RET_BAD_PARAMS);
  submodel = VIEW_MODEL(dummy->array.elms[index]);

  return object_get_prop(OBJECT(submodel), name, v);
}

static bool_t view_model_array_dummy_can_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, FALSE);

  return FALSE;
}

static ret_t view_model_array_dummy_exec(object_t* obj, const char* name, const char* args) {
  return_value_if_fail(obj != NULL && name != NULL, RET_BAD_PARAMS);

  return RET_NOT_IMPL;
  ;
}

static const object_vtable_t s_model_array_vtable = {
    .type = "view_model_array_dummy",
    .desc = "view_model_array_dummy",
    .size = sizeof(view_model_array_dummy_t),
    .is_collection = TRUE,
    .on_destroy = view_model_array_dummy_on_destroy,

    .compare = view_model_array_dummy_compare,
    .get_prop = view_model_array_dummy_get_prop,
    .set_prop = view_model_array_dummy_set_prop,
    .can_exec = view_model_array_dummy_can_exec,
    .exec = view_model_array_dummy_exec};

view_model_t* view_model_array_dummy_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_model_array_vtable);
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(obj);
  return_value_if_fail(dummy != NULL, NULL);

  view_model_array_init(VIEW_MODEL(obj));
  darray_init(&(dummy->array), 10, (tk_destroy_t)(object_unref), NULL);

  return VIEW_MODEL(obj);
}

ret_t view_model_array_dummy_clear(view_model_t* view_model) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL, RET_BAD_PARAMS);

  return darray_clear(&(dummy->array));
}

ret_t view_model_array_dummy_add(view_model_t* view_model, view_model_t* submodel) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL && submodel != NULL, RET_BAD_PARAMS);

  object_ref(OBJECT(submodel));
  return darray_push(&(dummy->array), submodel);
}

ret_t view_model_array_dummy_remove(view_model_t* view_model, uint32_t index) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL && index < dummy->array.size, RET_BAD_PARAMS);

  return darray_remove_index(&(dummy->array), index);
}

view_model_t* view_model_array_dummy_get(view_model_t* view_model, uint32_t index) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL && index < dummy->array.size, NULL);

  return VIEW_MODEL(dummy->array.elms[index]);
}

int32_t view_model_array_dummy_size(view_model_t* view_model) {
  view_model_array_dummy_t* dummy = VIEW_MODEL_ARRAY_DUMMY(view_model);
  return_value_if_fail(dummy != NULL, 0);

  return dummy->array.size;
}
