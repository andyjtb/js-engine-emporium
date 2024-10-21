#include <juce_core/juce_core.h>

int main(int argc, char* argv[]) {

  juce::JavascriptEngine engine;
  const auto result = engine.evaluate("'Hello' + ', World!'");

  std::cout << result.toString() << '\n';

  return 0;
}