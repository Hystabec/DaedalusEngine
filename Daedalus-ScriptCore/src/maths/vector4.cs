namespace Daedalus.Types
{
    public struct Vector4
    {
        public float X, Y, Z, W;

        public static Vector4 Zero => new Vector4(0.0f);

        public Vector4(float scalar)
        {
            this.X = scalar;
            this.Y = scalar;
            this.Z = scalar;
            this.W = scalar;
        }

        public Vector4(float x, float y, float z, float w)
        {
            this.X = x;
            this.Y = y;
            this.Z = z;
            this.W = w;
        }

        public static Vector4 operator +(Vector4 left, Vector4 right)
        {
            return new Vector4(left.X + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
        }

        public static Vector4 operator -(Vector4 left, Vector4 right)
        {
            return new Vector4(left.X - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
        }

        public static Vector4 operator *(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
        }

        public static Vector4 operator /(Vector4 vector, float scalar)
        {
            return new Vector4(vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar);
        }

        public override string ToString()
        {
            return ($"x: {X}, y: {Y}, z: {Z}, w: {W}").ToString();
        }
    }

}
