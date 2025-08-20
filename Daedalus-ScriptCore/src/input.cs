using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Daedalus
{
    internal class Input
    {
        public static bool IsKeyDown(InputCode inputCode)
        {
            return InternalCalls.input_get_key_down(inputCode);
        }
        public static bool IsKeyUp(InputCode inputCode)
        {
            return InternalCalls.input_get_key_up(inputCode);
        }

    }
}
