using System;
using System.Runtime.CompilerServices;

namespace daedalus
{
    public struct Vector3
    {
        public float x, y, z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }
    }

    // internal means it can only be used in this assembely
    public static class InternalCalls
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void nativeLog(string text, int param);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void nativeLogVector3(ref Vector3 param, out Vector3 result);
    }

    public class Entity
    {
        public Entity()
        {
            Console.WriteLine("Main constuctor!");
            Log("Hello there", 5234);

            Vector3 pos = new Vector3(2.9f, 31, 621);
            Vector3 res = Log(ref pos);
            Console.WriteLine($"{res.x}, {res.y}, {res.z}");
        }

        public static void PrintMessage()
        {
            Console.WriteLine("Hello World from C#!");
        }

        public static void PrintInt(int value)
        {
            Console.WriteLine($"C# says: {value}");
        }

        public static void PrintInts(int value1, int value2)
        {
            Console.WriteLine($"C# says: {value1}, {value2}");
        }

        public static void PrintCustomMessage(string message)
        {
            Console.WriteLine($"C# says: {message}");
        }

        private static void Log(string text, int param)
        {
            InternalCalls.nativeLog(text, param);
        }

        private static Vector3 Log(ref Vector3 param)
        {
            InternalCalls.nativeLogVector3(ref param, out Vector3 result);
            return result;
        }
    }
}