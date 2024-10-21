#include "EscargotPublic.h"
#include <string>
#include <iostream>

using namespace Escargot;

class BasicPlatform : public PlatformRef {
public:
    BasicPlatform()
    {
    }

    virtual void markJSJobEnqueued(ContextRef* relatedContext) override
    {
        // ignore. we always check pending job after eval script
    }

    virtual void markJSJobFromAnotherThreadExists(ContextRef* relatedContext) override
    {
        // ignore. we always check pending job after eval script
    }

    virtual LoadModuleResult onLoadModule(ContextRef* relatedContext, ScriptRef* whereRequestFrom, StringRef* moduleSrc, ModuleType type) override
    {
        std::string referrerPath = whereRequestFrom->src()->toStdUTF8String();
        auto& loadedModules = *reinterpret_cast<std::vector<std::tuple<std::string, ContextRef*, PersistentRefHolder<ScriptRef>>>*>(threadLocalCustomData());

        for (size_t i = 0; i < loadedModules.size(); i++) {
            if (std::get<2>(loadedModules[i]) == whereRequestFrom) {
                referrerPath = std::get<0>(loadedModules[i]);
                break;
            }
        }

        // std::string absPath = absolutePath(referrerPath, moduleSrc->toStdUTF8String());
        // if (absPath.length() == 0) {
        //     std::string s = "Error reading : " + moduleSrc->toStdUTF8String();
        //     return LoadModuleResult(ErrorObjectRef::Code::None, StringRef::createFromUTF8(s.data(), s.length()));
        // }
        //
        // for (size_t i = 0; i < loadedModules.size(); i++) {
        //     if (std::get<0>(loadedModules[i]) == absPath && std::get<1>(loadedModules[i]) == relatedContext) {
        //         return LoadModuleResult(std::get<2>(loadedModules[i]));
        //     }
        // }
        //
        // OptionalRef<StringRef> source = builtinHelperFileRead(nullptr, absPath.data(), "");
        // if (!source) {
        //     std::string s = "Error reading : " + absPath;
        //     return LoadModuleResult(ErrorObjectRef::Code::None, StringRef::createFromUTF8(s.data(), s.length()));
        // }
        //
        // ScriptParserRef::InitializeScriptResult parseResult;
        // StringRef* srcName = StringRef::createFromUTF8(absPath.data(), absPath.size());
        //
        // if (type == ModuleJSON) {
        //     parseResult = relatedContext->scriptParser()->initializeJSONModule(source.value(), srcName);
        // } else {
        //     parseResult = relatedContext->scriptParser()->initializeScript(source.value(), srcName, true);
        // }
        //
        // if (!parseResult.isSuccessful()) {
        //     return LoadModuleResult(parseResult.parseErrorCode, parseResult.parseErrorMessage);
        // }
        //
        // return LoadModuleResult(parseResult.script.get());
        return { nullptr };
    }

    virtual void didLoadModule(ContextRef* relatedContext, OptionalRef<ScriptRef> referrer, ScriptRef* loadedModule) override
    {
        std::string path;
        bool isAbs = false;
        if (loadedModule->src()->length() && loadedModule->src()->charAt(0) == '/') {
            isAbs = true;
        } else if (loadedModule->src()->length() > 3 && loadedModule->src()->charAt(1) == ':' && loadedModule->src()->charAt(2) == '\\') {
            isAbs = true;
        }

        if (referrer && loadedModule->src()->length() && !isAbs) {
            path = absolutePath(referrer->src()->toStdUTF8String(), loadedModule->src()->toStdUTF8String());
        } else {
            path = absolutePath(loadedModule->src()->toStdUTF8String());
        }
        auto& loadedModules = *reinterpret_cast<std::vector<std::tuple<std::string, ContextRef*, PersistentRefHolder<ScriptRef>>>*>(threadLocalCustomData());
        loadedModules.push_back(std::make_tuple(path, relatedContext, PersistentRefHolder<ScriptRef>(loadedModule)));
    }

    virtual void hostImportModuleDynamically(ContextRef* relatedContext, ScriptRef* referrer, StringRef* src, ModuleType type, PromiseObjectRef* promise) override
    {
        LoadModuleResult loadedModuleResult = onLoadModule(relatedContext, referrer, src, type);
        notifyHostImportModuleDynamicallyResult(relatedContext, referrer, src, promise, loadedModuleResult);
    }

    virtual bool canBlockExecution(ContextRef* relatedContext) override
    {
        return true;
    }

    virtual void* allocateThreadLocalCustomData() override
    {
        return new std::vector<std::tuple<std::string /* abs path */, ContextRef*, PersistentRefHolder<ScriptRef>>>();
    }

    virtual void deallocateThreadLocalCustomData() override
    {
        delete reinterpret_cast<std::vector<std::tuple<std::string, ContextRef*, PersistentRefHolder<ScriptRef>>>*>(threadLocalCustomData());
    }

private:
    std::string dirnameOf(const std::string& fname)
    {
        size_t pos = fname.find_last_of("/");
        if (std::string::npos == pos) {
            pos = fname.find_last_of("\\/");
        }
        return (std::string::npos == pos)
            ? ""
            : fname.substr(0, pos);
    }

    std::string absolutePath(const std::string& referrerPath, const std::string& src)
    {
        std::string utf8MayRelativePath = dirnameOf(referrerPath) + "/" + src;
        auto absPath = realpath(utf8MayRelativePath.data(), nullptr);
        if (!absPath) {
            return std::string();
        }
        std::string utf8AbsolutePath = absPath;
        free(absPath);

        return utf8AbsolutePath;
    }

    std::string absolutePath(const std::string& src)
    {
        auto absPath = realpath(src.data(), nullptr);
        if (!absPath) {
            return std::string();
        }
        std::string utf8AbsolutePath = absPath;
        free(absPath);

        return utf8AbsolutePath;
    }
};

PersistentRefHolder<ContextRef> createEscargotContext(VMInstanceRef* instance, bool isMainThread = true)
{
    return ContextRef::create(instance);
}

int main()
{
    bool waitBeforeExit = false;

    BasicPlatform* platform = new BasicPlatform();
    Globals::initialize(platform);

    Memory::setGCFrequency(24);

    PersistentRefHolder<VMInstanceRef> instance = VMInstanceRef::create();
    instance->registerPromiseRejectCallback([](ExecutionStateRef* state, PromiseObjectRef* promise, ValueRef* value, VMInstanceRef::PromiseRejectEvent event) {
        if (event == VMInstanceRef::PromiseRejectWithNoHandler) {
            fprintf(stderr, "Unhandled promise reject %s:\n", value->toStringWithoutException(state->context())->toStdUTF8String().data());
        }
    });
    PersistentRefHolder<ContextRef> context = createEscargotContext(instance.get());

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
