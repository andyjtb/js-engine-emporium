#include "EscargotPublic.h"
#include "engine.h"
#include <string>
#include <iostream>

using namespace Escargot;

int main()
{
    bool waitBeforeExit = false;

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

    int exitCode = 0;

    auto evalResult = Evaluator::execute(context, [](ExecutionStateRef* state, ScriptRef* script) -> ValueRef* {
        return script->execute(state);
    }, context->scriptParser()->initializeScript(StringRef::createFromASCII("'Hello' + ', World!'"), StringRef::createFromASCII("<BasicUsageExample>"), false).script.get());

    auto* strResult = evalResult.resultOrErrorToString(context);
    std::cout << strResult->toStdUTF8String() << '\n';

    context.release();
    instance.release();

    Globals::finalize();

    return exitCode;
}
