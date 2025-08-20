using System;
using System.Diagnostics;
using System.Runtime.CompilerServices;

namespace Daedalus
{
    public struct Vector3
    {
        public float X, Y, Z;

        public static Vector3 Zero => new Vector3(0.0f);

        public Vector3(float scalar)
        {
            this.X = scalar;
            this.Y = scalar;
            this.Z = scalar;
        }

        public Vector3(float x, float y, float z)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
        }

        public static Vector3 operator+(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X + right.X, left.Y + right.Y, left.Z + right.Z);
        }

        public static Vector3 operator -(Vector3 left, Vector3 right)
        {
            return new Vector3(left.X - right.X, left.Y - right.Y, left.Z - right.Z);
        }

        public static Vector3 operator*(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
        }

        public static Vector3 operator /(Vector3 vector, float scalar)
        {
            return new Vector3(vector.X / scalar, vector.Y / scalar, vector.Z / scalar);
        }
    }

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
        internal extern static void entity_get_position(ulong entityID, out Vector3 position);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void entity_set_position(ulong entityID, ref Vector3 position);
        #endregion
    }

    // NOTE: This should probably be renamed as its not an entity,
    // it is more like a component. Rename to component?
    public class Entity
    {
        protected Entity()
        {
            ID = 0;
        }

        internal Entity(ulong id)
        {
            ID = id;
        }

        public readonly ulong ID;

        public Vector3 Position
        {
            get 
            {
                InternalCalls.entity_get_position(ID, out Vector3 position);
                return position;
            }
            set 
            {
                InternalCalls.entity_set_position(ID, ref value);
            }
        }
    }
}