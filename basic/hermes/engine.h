#include <hermes/hermes.h>

namespace Hermes
{

struct Engine
{
    Engine()
    {
        auto runtimeConfig = hermes::vm::RuntimeConfig::Builder().withIntl(false).build();
        runtime = facebook::hermes::makeHermesRuntime(runtimeConfig);
    }

    ~Engine()
    {
    }

    std::unique_ptr<facebook::hermes::HermesRuntime> runtime;
};

}