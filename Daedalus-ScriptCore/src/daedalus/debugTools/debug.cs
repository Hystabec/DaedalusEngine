namespace Daedalus
{
    // NOTE: Wanted to call this class Log, but cant do Log.Log. Consider renaming later
    public class Debug
    {
        public static void Log(string message)
        {
            InternalCalls.native_log(message);
        }
    }
}
