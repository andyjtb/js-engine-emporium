#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark_all.hpp>

#include "ScriptX/ScriptX.h"

#include <fstream>
#include <stdexcept>

std::vector<char> loadFileToBuffer(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    file.read(buffer.data(), size);

    return buffer;
}
#ifdef SCRIPTX_BACKEND_HERMES
const std::string engineType = "Hermes";
const std::string fileExtension = "hbc";
#else
const std::string engineType = "QuickJS";
const std::string fileExtension = "qbc";
#endif

TEST_CASE("Simple Addition")
{
    const auto code = "10 + 10";
    const auto codeLength = strlen(code);

    BENCHMARK("ScriptX " + engineType)
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->eval(code);
        REQUIRE(res.asNumber().toInt32() == 20);
    };

    BENCHMARK("ScriptX " + engineType + " No Copy")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->evalInPlace(code, codeLength, {});
        REQUIRE(res.asNumber().toInt32() == 20);
    };

    const auto qjsBytecode = loadFileToBuffer("/Users/andy/Desktop/js-engine-emporium/benchmarks/bytecode/simple_addition." + fileExtension);
    BENCHMARK("ScriptX " + engineType + " Bytecode")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->evalInPlace(qjsBytecode.data(), qjsBytecode.size(), "simple_addition." + fileExtension);
        REQUIRE(res.asNumber().toInt32() == 20);
    };
}

TEST_CASE("Convoluted Addition")
{
    const auto code = R"(
class Calculator {
    constructor(initialValue) {
        this.value = initialValue;
    }

    add(num) {
        this.value += num;
        return this;
    }

    subtract(num) {
        this.value -= num;
        return this;
    }

    multiply(num) {
        this.value *= num;
        return this;
    }

    divide(num) {
        if (num === 0) throw new Error("Cannot divide by zero");
        this.value /= num;
        return this;
    }

    getValue() {
        return this.value;
    }
}

function performOperations(calculator) {
    return calculator
        .add(5)
        .multiply(4)
        .subtract(10)
        .divide(2)
        .add(7.5);
}

function roundToNearest(value, target) {
    return Math.round(value / target) * target;
}

(function() {
    const initialValue = 2.5;
    const calculator = new Calculator(initialValue);
    const result = performOperations(calculator);
    const roundedResult = roundToNearest(result.getValue(), 5);
    return roundedResult;
})();
)";
    const auto codeLength = strlen(code);

    BENCHMARK("ScriptX " + engineType)
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->eval(code);
        REQUIRE(res.asNumber().toInt32() == 20);
    };

    BENCHMARK("ScriptX "+ engineType + " No Copy")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->evalInPlace(code, codeLength, {});
        REQUIRE(res.asNumber().toInt32() == 20);
    };

    const auto qjsBytecode = loadFileToBuffer("/Users/andy/Desktop/js-engine-emporium/benchmarks/bytecode/convoluted_addition." + fileExtension);
    BENCHMARK("ScriptX " + engineType + " Bytecode")
    {
        auto engine = std::shared_ptr<script::ScriptEngine> { new script::ScriptEngineImpl(), script::ScriptEngine::Deleter() };

        script::EngineScope scope (engine.get());

        const auto res = engine->evalInPlace(qjsBytecode.data(), qjsBytecode.size(), "convoluted_addition." + fileExtension);
        REQUIRE(res.asNumber().toInt32() == 20);
    };

}