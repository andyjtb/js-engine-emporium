#include <quickjs.h>

struct Engine
{
    Engine()
    {
        rt = JS_NewRuntime();
        ctx = JS_NewContext(rt);
    }

    ~Engine()
    {
        JS_FreeContext(ctx);
        JS_FreeRuntime(rt);
    }

    JSRuntime *rt  = nullptr;
    JSContext *ctx = nullptr;
};
