#include "duktape.h"

namespace Duktape
{

struct Engine
{
    Engine()
    {
        ctx = duk_create_heap_default();
    }

    ~Engine()
    {
        duk_destroy_heap(ctx);
    }

    duk_context *ctx = nullptr;
};

}
