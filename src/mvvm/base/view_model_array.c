﻿/**
 * File:   view_model_array.c
 * Author: AWTK Develop Team
 * Brief:  view_model_array
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
#include "mvvm/base/view_model_array.h"

static const char* view_model_array_preprocess_expr(view_model_t* view_model, const char* expr) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(view_model != NULL && expr != NULL, NULL);

  str_set(&(vm_array->temp_expr), expr);
  str_replace(&(vm_array->temp_expr), "item.", "$item_");
  str_replace(&(vm_array->temp_expr), "$$", "$");

  return vm_array->temp_expr.str;
}

static const char* view_model_array_preprocess_prop(view_model_t* view_model, const char* prop) {
  char index[TK_NUM_MAX_LEN + 1];
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(view_model != NULL && prop != NULL, NULL);

  tk_snprintf(index, TK_NUM_MAX_LEN, "[%d].", vm_array->cursor);

  str_set(&(vm_array->temp_prop), prop);
  str_replace(&(vm_array->temp_prop), "item_", index);

  return vm_array->temp_prop.str;
}

ret_t view_model_array_inc_cursor(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor++;

  return RET_OK;
}

ret_t view_model_array_set_cursor(view_model_t* view_model, uint32_t cursor) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  vm_array->cursor = cursor;

  return RET_OK;
}

view_model_t* view_model_array_init(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, NULL);

  view_model_init(view_model);
  str_init(&(vm_array->temp_expr), 0);
  str_init(&(vm_array->temp_prop), 0);
  view_model->preprocess_expr = view_model_array_preprocess_expr;
  view_model->preprocess_prop = view_model_array_preprocess_prop;

  return view_model;
}

ret_t view_model_array_deinit(view_model_t* view_model) {
  view_model_array_t* vm_array = VIEW_MODEL_ARRAY(view_model);
  return_value_if_fail(vm_array != NULL, RET_BAD_PARAMS);

  str_reset(&(vm_array->temp_expr));
  str_reset(&(vm_array->temp_prop));
  view_model_deinit(view_model);

  return RET_OK;
}

ret_t view_model_array_notify_items_changed(view_model_t* view_model) {
  return emitter_dispatch_simple_event(EMITTER(view_model), EVT_ITEMS_CHANGED);
}
