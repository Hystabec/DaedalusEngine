namespace Daedalus
{
    public class Input
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
