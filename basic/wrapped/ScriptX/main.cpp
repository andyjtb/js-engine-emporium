#include "ScriptX/ScriptX.h"
#include <iostream>

int main(int, char**)
{
    auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

    script::EngineScope scope (engine.get());

    const auto res = engine->eval("'Hello, ' + 'world'");
    std::cout << res.asString().toString() << '\n';

    return 0;
}
