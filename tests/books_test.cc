﻿#include "tkc/utils.h"
#include "../demos/common/books.h"
#include "gtest/gtest.h"

TEST(Books, basic) {
  value_t v;
  uint32_t i = 0;
  view_model_t* view_model = books_create(NULL);

  books_clear(view_model);

  for (i = 0; i < 100; i++) {
    char name[32];
    book_info_t* iter = NULL;
    tk_snprintf(name, sizeof(name) - 1, "test%u", i);
    books_add(view_model, name, i + 1);

    iter = books_get(view_model, i);

    ASSERT_EQ(view_model_eval(view_model, "$item.stock * 2", &v), RET_OK);
    ASSERT_EQ(value_int(&v), 2 * iter->stock);

    ASSERT_EQ(view_model_eval(view_model, "item.stock", &v), RET_OK);
    ASSERT_EQ(value_int(&v), iter->stock);

    ASSERT_EQ(view_model_eval(view_model, "item.name", &v), RET_OK);
    ASSERT_STREQ(value_str(&v), iter->name);

    ASSERT_EQ(object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_ITEMS, -1), i + 1);
    ASSERT_EQ(object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_CURSOR, -1), i);
    ASSERT_EQ(object_get_prop_int(OBJECT(view_model), VIEW_MODEL_PROP_ITEMS, -1), i + 1);

    view_model_array_inc_cursor(view_model);
  }

  object_unref(OBJECT(view_model));
}
