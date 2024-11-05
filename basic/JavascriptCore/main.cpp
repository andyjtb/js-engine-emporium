#include <JavaScriptCore/JavaScript.h>
#include <iostream>

int main(int argc, char* argv[])
{
    JSGlobalContextRef context = JSGlobalContextCreate(nullptr);
    JSStringRef script = JSStringCreateWithUTF8CString("'Hello' + ', World!'");

    JSValueRef exception = nullptr;
    JSValueRef result = JSEvaluateScript(context, script, nullptr, nullptr, 0, &exception);

    auto resultString = JSValueToStringCopy(context, result, nullptr);
    const auto strLength = JSStringGetMaximumUTF8CStringSize(resultString);
    auto resultCStr = new char[strLength];
    JSStringGetUTF8CString(resultString, resultCStr, strLength);

    std::cout << resultCStr << std::endl;

    delete[] resultCStr;
    JSStringRelease(script);
    JSStringRelease(resultString);
    JSGlobalContextRelease(context);

    return 0;
}