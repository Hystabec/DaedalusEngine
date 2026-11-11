namespace Daedalus
{
	public class Time
	{
		public static float DeltaTime
		{
			get
			{
				return InternalCalls.time_get_delta_time();
			}
		}
	}
}