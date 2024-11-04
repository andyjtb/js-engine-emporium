#include "choc_javascript.h"
#include <iostream>

#define CHOC_DUKTAPE 0
#define CHOC_QUICKJS 0
#define CHOC_V8 1

#if CHOC_DUKTAPE
#include "choc_javascript_Duktape.h"
#elif CHOC_QUICKJS
#include "choc_javascript_QuickJS.h"
#elif CHOC_V8 && WITH_V8
#include "choc_javascript_V8.h"
#endif

int main(int, char**)
{
#if CHOC_DUKTAPE
    auto ctx = choc::javascript::createDuktapeContext();
#elif CHOC_QUICKJS
    auto ctx = choc::javascript::createQuickJSContext();
#elif CHOC_V8
    auto ctx = choc::javascript::createV8Context();
#else
    static_assert(false, "No JavaScript engine selected");
#endif

    const auto result = ctx.evaluateExpression("'Hello' + ', World!'");

    std::cout << result.toString() << '\n';

    return 0;
}
