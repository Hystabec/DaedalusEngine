namespace Daedalus
{
    // NOTE: Wanted to call this class Log, but cant do Log.Log. Consider renaming later
    public class Debug
    {
        // enum from Core - log.h
        internal enum LogLevel
        {
            trace,
            info,
            warn,
            error,
            critical
        }

        public static void Log(string message)
        {
            InternalCalls.native_log(message, LogLevel.trace);
        }
        public static void LogInfo(string message)
        {
            InternalCalls.native_log(message, LogLevel.info);
        }
        public static void LogWarn(string message)
        {
            InternalCalls.native_log(message, LogLevel.warn);
        }
        public static void LogError(string message)
        {
            InternalCalls.native_log(message, LogLevel.error);
        }
        public static void LogCritical(string message)
        {
            InternalCalls.native_log(message, LogLevel.critical);
        }
    }
}
