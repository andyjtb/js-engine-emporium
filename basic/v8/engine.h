#include "libplatform/libplatform.h"
#include "v8-context.h"
#include "v8-initialization.h"
#include "v8-isolate.h"
#include "v8-local-handle.h"
#include "v8-primitive.h"
#include "v8-script.h"

namespace V8
{
    struct Engine
    {
        Engine()
        {
            v8::V8::InitializeICUDefaultLocation(nullptr);
            v8::V8::InitializeExternalStartupData(nullptr);
            platform = v8::platform::NewDefaultPlatform();
            v8::V8::InitializePlatform(platform.get());
            v8::V8::Initialize();

            create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();
            isolate = v8::Isolate::New(create_params);

            v8::HandleScope handle_scope(isolate);
            v8::Isolate::Scope isolate_scope(isolate);
            context = v8::Context::New(isolate);
        }

        ~Engine()
        {
            isolate->Dispose();
            v8::V8::Dispose();
            v8::V8::DisposePlatform();
            delete create_params.array_buffer_allocator;
        }

        v8::Isolate::CreateParams create_params;
        v8::Isolate* isolate = nullptr;
        std::unique_ptr<v8::Platform> platform;
        v8::Local<v8::Context> context;
    };
}
