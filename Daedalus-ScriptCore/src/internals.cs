using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace Daedalus
{ 
    // internal means it can only be used in this assembely
    internal static class InternalCalls
    {
        #region Logging
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void native_log(string text);
        #endregion

        #region Input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool input_get_key_up(InputCode inputCode);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool input_get_key_down(InputCode inputCode);
        #endregion

        #region transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void entity_get_position(ulong entityID, out Types.Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void entity_set_position(ulong entityID, ref Types.Vector3 position);
        #endregion
    }
}