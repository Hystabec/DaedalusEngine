
namespace Daedalus
{
    // NOTE: not sure on this name might consider changing it
    public static class MathsHelpers
    {
        public static float Lerp(float a, float b, float t)
        {
            return a * (1.0f - t) + (b * t);
        }
    }
}
