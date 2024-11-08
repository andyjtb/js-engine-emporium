add_library(choc_js INTERFACE)
target_include_directories(choc_js
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/choc/javascript
)

add_subdirectory(dukglue)

# set(SCRIPTX_BACKEND "JavaScriptCore" CACHE STRING "ScriptX Backend" FORCE)
set(SCRIPTX_BACKEND "QuickJs" CACHE STRING "ScriptX Backend" FORCE)
# set(SCRIPTX_BACKEND "Hermes" CACHE STRING "ScriptX Backend" FORCE)
# set(SCRIPTX_BACKEND "V8" CACHE STRING "ScriptX Backend" FORCE)
add_subdirectory(ScriptX)
