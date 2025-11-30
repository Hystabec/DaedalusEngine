using Daedalus.Types;
using System;
using System.Runtime.CompilerServices;

namespace Daedalus
{ 
    // internal means it can only be used in this assembely
    internal static class InternalCalls
    {
        #region logging
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void native_log(string text, Debug.LogLevel logType);
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
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static ulong entity_find_entity_by_name(string name);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static object entity_get_script_instance(ulong entityID, string MonoScriptTypeName, out bool scriptFound);
        #endregion

        #region Components

        #region transform
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void transform_component_get_position(ulong entityID, out Types.Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void transform_component_set_position(ulong entityID, ref Types.Vector3 position);
        #endregion

        #region Text
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static string text_component_get_text(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void text_component_set_text(ulong entityID, string text);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void text_component_get_colour(ulong entityID, out Vector4 outColour);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void text_component_set_colour(ulong entityID, ref Vector4 colour);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float text_component_get_kerning(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void text_component_set_kerning(ulong entityID, float kerning);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static float text_component_get_line_spacing (ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void text_component_set_line_spacing(ulong entityID, float lineSpacing);
        #endregion

        #region rigidbody2D
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_apply_force_from_point(ulong entityID, ref Types.Vector2 force, ref Types.Vector2 worldPoint, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_apply_force(ulong entityID, ref Types.Vector2 force, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_add_linear_impulse_from_point(ulong entityID, ref Types.Vector2 linearImpulse, ref Types.Vector2 worldPoint, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_add_linear_impulse(ulong entityID, ref Types.Vector2 linearImpulse, bool wake);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_apply_torque(ulong entityID, ref float torque, bool wake);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_get_linear_velocity(ulong entityID, out Types.Vector2 linearVelocity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static Rigidbody2DComponent.BodyType rigidbody2D_component_get_body_type(ulong entityID);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void rigidbody2D_component_set_body_type(ulong entityID, Rigidbody2DComponent.BodyType bodyType);

        #endregion

        #endregion
    }
}