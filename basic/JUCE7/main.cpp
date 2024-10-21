#include <juce_core/juce_core.h>
#include "juce_Javascript.h"

int main(int argc, char* argv[]) {

  juce::JavascriptEngine7 engine;
  const auto result = engine.evaluate("'Hello' + ', World!'");

  std::cout << result.toString() << '\n';

  return 0;
}