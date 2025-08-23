using System;
using System.Runtime.CompilerServices;

namespace Daedalus
{ 
    // internal means it can only be used in this assembely
    internal static class InternalCalls
    {
        #region logging
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void native_log(string text);
        #endregion

        #region input
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool input_get_key_up(InputCode inputCode);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool input_get_key_down(InputCode inputCode);
        #endregion

        #region entity
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static bool entity_has_component(ulong entityID, Type componentType);
        #endregion

        #region transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void transform_component_get_position(ulong entityID, out Types.Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void transform_component_set_position(ulong entityID, ref Types.Vector3 position);
        #endregion

        #region rigidbody2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_add_linear_impulse_from_point(ulong entityID, ref Types.Vector2 linearImpulse, ref Types.Vector2 worldPoint, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_add_linear_impulse(ulong entityID, ref Types.Vector2 linearImpulse, bool wake);
        #endregion
    }
}