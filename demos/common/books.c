#include "tkc/mem.h"
#include "tkc/utils.h"
#include "mvvm/base/utils.h"
#include "books.h"

/***************book***************/;

book_t* book_create(void) {
  book_t* book = TKMEM_ZALLOC(book_t);
  return_value_if_fail(book != NULL, NULL);

  str_random(&(book->name), "book %d", 10000);
  book->stock = random() % 100;

  return book;
}

int book_cmp(book_t* a, book_t* b) {
  return_value_if_fail(a != NULL && b != NULL, -1);
  return strcmp(a->name.str, b->name.str);
}

static ret_t book_destroy(book_t* book) {
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  str_reset(&(book->name));

  TKMEM_FREE(book);

  return RET_OK;
}

static bool_t book_can_exec_sale(book_t* book, const char* args) {
  return book->stock > 0;
}

static ret_t book_sale(book_t* book, const char* args) {
  book->stock--;
  return RET_OBJECT_CHANGED;
}

/***************books_view_model***************/

ret_t books_view_model_remove(view_model_t* view_model, uint32_t index) {
  books_view_model_t* book_vm = (books_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, RET_BAD_PARAMS);

  return darray_remove_index(&(book_vm->books), index);
}

ret_t books_view_model_add(view_model_t* view_model, book_t* book) {
  books_view_model_t* book_vm = (books_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL && book != NULL, RET_BAD_PARAMS);

  return darray_push(&(book_vm->books), book);
}

uint32_t books_view_model_size(view_model_t* view_model) {
  books_view_model_t* book_vm = (books_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0);

  return book_vm->books.size;
}

ret_t books_view_model_clear(view_model_t* view_model) {
  books_view_model_t* book_vm = (books_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0);

  return darray_clear(&(book_vm->books));
}

book_t* books_view_model_get(view_model_t* view_model, uint32_t index) {
  books_view_model_t* book_vm = (books_view_model_t*)(view_model);
  return_value_if_fail(book_vm != NULL, 0);

  return_value_if_fail(book_vm != NULL && index < books_view_model_size(view_model), NULL);

  return (book_t*)(book_vm->books.elms[index]);
}

static ret_t books_view_model_set_prop(object_t* obj, const char* name, const value_t* v) {
  uint32_t index = 0;
  book_t* book = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    view_model_array_set_cursor(vm, value_int(v));

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  book = books_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq("name", name)) {
    str_from_value(&(book->name), v);
  } else if (tk_str_eq("stock", name)) {
    book->stock = value_uint32(v);
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static ret_t books_view_model_get_prop(object_t* obj, const char* name, value_t* v) {
  uint32_t index = 0;
  book_t* book = NULL;
  view_model_t* vm = VIEW_MODEL(obj);

  if (tk_str_eq(VIEW_MODEL_PROP_ITEMS, name)) {
    value_set_int(v, books_view_model_size(VIEW_MODEL(obj)));

    return RET_OK;
  } else if (tk_str_eq(VIEW_MODEL_PROP_CURSOR, name)) {
    value_set_int(v, VIEW_MODEL_ARRAY(obj)->cursor);

    return RET_OK;
  }

  name = destruct_array_prop_name(name, &index);
  return_value_if_fail(name != NULL, RET_BAD_PARAMS);
  book = books_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_eq("name", name)) {
    value_set_str(v, book->name.str);
  } else if (tk_str_eq("stock", name)) {
    value_set_uint32(v, book->stock);
  } else if (tk_str_eq("style", name)) {
    value_set_str(v, index % 2 ? "odd" : "even");
  } else {
    log_debug("not found %s\n", name);
    return RET_NOT_FOUND;
  }

  return RET_OK;
}

static bool_t books_view_model_can_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  book_t* book = NULL;

  if (tk_str_ieq(name, "add")) {
    return TRUE;
  } else if (tk_str_ieq(name, "clear")) {
    return books_view_model_size(vm) > 0;
  }

  book = books_view_model_get(vm, index);
  return_value_if_fail(book != NULL, FALSE);

  if (tk_str_ieq(name, "remove")) {
    return index < books_view_model_size(vm);
  } else if (tk_str_eq("sale", name)) {
    return book_can_exec_sale(book, args);
  } else {
    return FALSE;
  }
}

static ret_t books_view_model_exec(object_t* obj, const char* name, const char* args) {
  uint32_t index = tk_atoi(args);
  view_model_t* vm = VIEW_MODEL(obj);
  book_t* book = NULL;

  if (tk_str_ieq(name, "add")) {
    ENSURE(books_view_model_add(vm, book_create()) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_ieq(name, "clear")) {
    ENSURE(books_view_model_clear(vm) == RET_OK);
    return RET_ITEMS_CHANGED;
  }

  book = books_view_model_get(vm, index);
  return_value_if_fail(book != NULL, RET_BAD_PARAMS);

  if (tk_str_ieq(name, "remove")) {
    ENSURE(books_view_model_remove(vm, index) == RET_OK);
    return RET_ITEMS_CHANGED;
  } else if (tk_str_eq("sale", name)) {
    return book_sale(book, args);
  } else {
    log_debug(
        "not found %s\
",
        name);
    return RET_NOT_FOUND;
  }
}

static ret_t books_view_model_on_destroy(object_t* obj) {
  books_view_model_t* vm = (books_view_model_t*)(obj);
  return_value_if_fail(vm != NULL, RET_BAD_PARAMS);

  books_view_model_clear(VIEW_MODEL(obj));
  darray_deinit(&(vm->books));

  return view_model_array_deinit(VIEW_MODEL(obj));
}

static const object_vtable_t s_books_view_model_vtable = {
    .type = "book",
    .desc = "book store",
    .is_collection = TRUE,
    .size = sizeof(books_view_model_t),
    .exec = books_view_model_exec,
    .can_exec = books_view_model_can_exec,
    .get_prop = books_view_model_get_prop,
    .set_prop = books_view_model_set_prop,
    .on_destroy = books_view_model_on_destroy};

view_model_t* books_view_model_create(navigator_request_t* req) {
  object_t* obj = object_create(&s_books_view_model_vtable);
  view_model_t* vm = view_model_array_init(VIEW_MODEL(obj));
  books_view_model_t* book_vm = (books_view_model_t*)(vm);

  return_value_if_fail(vm != NULL, NULL);

  darray_init(&(book_vm->books), 100, (tk_destroy_t)book_destroy, (tk_compare_t)book_cmp);

  return vm;
}
