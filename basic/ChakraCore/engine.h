//-------------------------------------------------------------------------------------------------------
// Copyright (C) Microsoft. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.
//-------------------------------------------------------------------------------------------------------

#include "ChakraCore.h"
#include <exception>

namespace ChakraCore
{

#define FAIL_CHECK(cmd)                       \
    do                                        \
    {                                         \
        JsErrorCode errCode = cmd;            \
        if (errCode != JsNoError)             \
        {                                     \
            printf("Error %d at '%s'\n",      \
                errCode, #cmd);               \
            throw std::runtime_error(""); \
        }                                     \
    } while(0)

struct Engine
{
    Engine()
    {
        // Create a runtime.
        FAIL_CHECK(JsCreateRuntime(JsRuntimeAttributeNone, nullptr, &runtime));

        // Create an execution context.
        FAIL_CHECK(JsCreateContext(runtime, &context));

        // Now set the current execution context.
        FAIL_CHECK(JsSetCurrentContext(context));
    }

    ~Engine()
    {
        FAIL_CHECK(JsSetCurrentContext(JS_INVALID_REFERENCE));
        FAIL_CHECK(JsDisposeRuntime(runtime));
    }

    JsRuntimeHandle runtime;
    JsContextRef context;
};

}
