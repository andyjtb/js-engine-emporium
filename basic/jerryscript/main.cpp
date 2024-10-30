#include "jerryscript.h"
#include <iostream>

int main (void)
{
    const jerry_char_t script[] = "'Hello, ' + 'World!';";
    const jerry_length_t script_size = sizeof (script) - 1;
    /* Note: sizeof can be used here only because the compiler knows the static character arrays's size.
    * If this is not the case, strlen should be used instead.
    */

    /* Initialize engine */
    jerry_init (JERRY_INIT_EMPTY);

    /* Run the demo script with 'eval' */
    jerry_value_t eval_ret = jerry_eval (script,
                                       script_size,
                                       JERRY_PARSE_NO_OPTS);

    const size_t str_size = jerry_string_size(eval_ret, JERRY_ENCODING_UTF8) + 1;
    auto result_str = new uint8_t[str_size];
    jerry_string_to_buffer(eval_ret, JERRY_ENCODING_UTF8, result_str, str_size);
    std::cout << result_str << std::endl;

    /* Check if there was any error (syntax or runtime) */
    bool run_ok = !jerry_value_is_exception (eval_ret);

    /* Parsed source code must be freed */
    jerry_value_free (eval_ret);

    /* Cleanup engine */
    jerry_cleanup ();

    return (run_ok ? 0 : 1);
}