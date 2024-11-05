#include <JavaScriptCore/JavaScript.h>

namespace JavascriptCore
{

struct Engine
{
    Engine()
    {
        context = JSGlobalContextCreate(nullptr);
    }

    ~Engine()
    {
        JSGlobalContextRelease(context);
    }

    JSGlobalContextRef context = nullptr;
};

}
