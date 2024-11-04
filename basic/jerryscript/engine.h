#include "jerryscript.h"

namespace JerryScript
{
    struct Engine
    {
        Engine()
        {
            jerry_init (JERRY_INIT_EMPTY);
        }

        ~Engine()
        {
            jerry_cleanup ();
        }
    };

}
