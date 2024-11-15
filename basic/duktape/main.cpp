#include "duktape.h"
#include <iostream>

int main(int, char**) {
  duk_context *ctx = duk_create_heap_default();
  duk_eval_string(ctx, "'Hello, ' + 'world!'");

  auto res = duk_get_string (ctx, -1);
  std::cout << res << std::endl;

  duk_destroy_heap(ctx);

  return 0;
}
