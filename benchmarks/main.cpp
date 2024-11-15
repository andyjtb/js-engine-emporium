#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include <ChakraCore/engine.h>

#include <Escargot/engine.h>
#include <hermes/engine.h>
#include <JavascriptCore/engine.h>
#include <jerryscript/engine.h>
#include <juce_core/juce_core.h>
#include "juce_Javascript.h"

#include "ScriptX/ScriptX.h"

#if WITH_CHOC_DUKTAPE
#include "choc_javascript_Duktape.h"
#else
#include <duktape/engine.h>
#endif

#if WITH_CHOC_QUICKJS
#include "choc_javascript_QuickJS.h"
#else
#include <quickjs/engine.h>
#endif

#if WITH_V8
#include <v8/engine.h>
#endif

#if WITH_CHOC_V8
#include "choc_javascript_V8.h"
#endif

TEST_CASE("Hello Benchmark")
{
#if WITH_V8 && ! WITH_CHOC_V8
    v8::V8::InitializeICUDefaultLocation(nullptr);
    v8::V8::InitializeExternalStartupData(nullptr);
    std::unique_ptr<v8::Platform> platform = v8::platform::NewDefaultPlatform();
    v8::V8::InitializePlatform(platform.get());
    v8::V8::Initialize();
#endif
    const auto code = "10 + 10";
    const auto codeLength = strlen(code);

    BENCHMARK("ChakraCore")
    {
        ChakraCore::Engine engine;

        JsValueRef scriptSource;
        JsCreateString(code, codeLength, &scriptSource);

        unsigned currentSourceContext = 0;
        JsValueRef fname;
        JsCreateString("b", strlen("b"), &fname);

        JsValueRef res;
        JsRun(scriptSource, currentSourceContext++, fname, JsParseScriptAttributeNone, &res);

        int resNum = 0;
        JsNumberToInt(res, &resNum);
        REQUIRE (resNum == 20);
    };

    BENCHMARK("Escargot")
    {
        using namespace Escargot;
        auto* platform = new EscargotEngine::BasicPlatform();
        Globals::initialize(platform);

        Memory::setGCFrequency(24);

        PersistentRefHolder<VMInstanceRef> instance = VMInstanceRef::create();
        instance->registerPromiseRejectCallback([](ExecutionStateRef* state, PromiseObjectRef* promise, ValueRef* value, VMInstanceRef::PromiseRejectEvent event) {
            if (event == VMInstanceRef::PromiseRejectWithNoHandler) {
                fprintf(stderr, "Unhandled promise reject %s:\n", value->toStringWithoutException(state->context())->toStdUTF8String().data());
            }
        });
        PersistentRefHolder<ContextRef> context = EscargotEngine::createEscargotContext(instance.get());

        if (getenv("GC_FREE_SPACE_DIVISOR") && strlen(getenv("GC_FREE_SPACE_DIVISOR"))) {
            int d = atoi(getenv("GC_FREE_SPACE_DIVISOR"));
            Memory::setGCFrequency(d);
        }

        auto evalResult = Evaluator::execute(context, [](ExecutionStateRef* state, ScriptRef* script) -> ValueRef* {
            return script->execute(state);
        }, context->scriptParser()->initializeScript(StringRef::createFromASCII("10 + 10"), StringRef::createFromASCII("<BenchmarkExample>"), false).script.get());

        auto res = evalResult.result->asUInt32();
        REQUIRE (res == 20);

        context.release();
        instance.release();

        Globals::finalize();
    };

    BENCHMARK("Hermes")
    {
        Hermes::Engine engine;
        auto res = engine.runtime->evaluateJavaScript(std::make_unique<facebook::jsi::StringBuffer>(code), "main.js");
        REQUIRE (res.asNumber() == 20);
    };

    BENCHMARK("JavascriptCore")
    {
        JavascriptCore::Engine engine;
        JSStringRef script = JSStringCreateWithUTF8CString("10 + 10");
        JSValueRef result = JSEvaluateScript(engine.context, script, nullptr, nullptr, 0, nullptr);

        REQUIRE(JSValueToNumber(engine.context, result, NULL) == 20);
        JSStringRelease(script);
    };

    BENCHMARK("Jerryscript")
    {
        JerryScript::Engine engine;
        jerry_value_t eval_ret = jerry_eval((uint8_t*)code, codeLength, JERRY_PARSE_NO_OPTS);
        REQUIRE(jerry_value_as_uint32(eval_ret) == 20);
    };

    BENCHMARK("JUCE 7")
    {
        juce::JavascriptEngine7 engine;
        auto res = engine.evaluate(code);
        REQUIRE((int) res == 20);
    };

    BENCHMARK("JUCE 8")
    {
        juce::JavascriptEngine engine;
        auto res = engine.evaluate(code);
        REQUIRE((int) res == 20);
    };

#if WITH_CHOC_DUKTAPE
    BENCHMARK("Choc Duktape")
    {
        auto ctx = choc::javascript::createDuktapeContext();
        const auto result = ctx.evaluateExpression("10 + 10");

        REQUIRE((double) result == 20);
    };
#else
    BENCHMARK("Duktape")
    {
        Duktape::Engine engine;
        duk_eval_string(engine.ctx, code);

        auto res = duk_get_int (engine.ctx, -1);
        REQUIRE (res == 20);
    };
#endif

#if WITH_CHOC_QUICKJS
    BENCHMARK("Choc Quickjs")
    {
        auto ctx = choc::javascript::createQuickJSContext();
        const auto result = ctx.evaluateExpression("10 + 10");

        REQUIRE((double) result == 20);
    };
#else
    BENCHMARK("QuickJS")
    {
        QuickJS::Engine engine;
        auto res = JS_Eval(engine.ctx, code, codeLength, "<input>", JS_EVAL_FLAG_STRICT);
        uint32_t resUint = 0;
        JS_ToUint32(engine.ctx, &resUint, res);

        REQUIRE(resUint == 20);
    };
#endif

#if WITH_CHOC_V8
    BENCHMARK("Choc V8")
    {
        auto ctx = choc::javascript::createV8Context();
        const auto result = ctx.evaluateExpression("10 + 10");

        REQUIRE((int) result == 20);
    };
#endif

#if WITH_V8 && ! WITH_CHOC_V8
    BENCHMARK("v8")
    {
        v8::Isolate::CreateParams create_params;
        create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
        v8::Isolate* isolate = v8::Isolate::New(create_params);

        {
            v8::Isolate::Scope isolate_scope(isolate);
            v8::HandleScope handle_scope(isolate);

            v8::Local<v8::Context> context = v8::Context::New(isolate);
            v8::Context::Scope context_scope(context);

            {
                v8::Local<v8::String> source = v8::String::NewFromUtf8Literal(isolate, "10 + 10");
                v8::Local<v8::Script> script = v8::Script::Compile(context, source).ToLocalChecked();

                const auto result = script->Run(context).ToLocalChecked();

                int32_t intValue = result->Int32Value(context).ToChecked();
                REQUIRE(intValue == 20);
            }
        }

        isolate->Dispose();

        delete create_params.array_buffer_allocator;
    };

    v8::V8::Dispose();
    v8::V8::DisposePlatform();
#endif

#ifdef SCRIPTX_BACKEND_QUICKJS
    BENCHMARK("ScriptX QuickJS")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->eval(code);
        REQUIRE(res.asNumber().toInt32() == 20);
    };

    BENCHMARK("ScriptX QuickJS No Copy")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->evalInPlace(code, codeLength, {});
        REQUIRE(res.asNumber().toInt32() == 20);
    };
#endif
}