#include "mytime.h"
#include <ctime>
#include <sys/timeb.h>
#include <string>

using std::string;

mytime::mytime()
{
    old = get_time_ms();
    reset();
}

void mytime::set_time()
{
    old = get_time_ms();
    isset = true;
}

unsigned long long int mytime::get_time_ms()
{
    timeb* tp = new timeb;
    ftime(tp);

    return(tp->time * 1000 + tp->millitm);
}

void mytime::reset()
{
    isset = false;
}

bool mytime::is_set()
{
    return(isset);
}

unsigned long long int mytime::get_time_difference()
{
    return(get_time_ms() - old);
}

string mytime::get_time()
{
    string result;

    time_t seconds = mytime::get_seconds();
    tm* timeinfo = localtime(&seconds);

    char time_buf[80];
    const char* format = "%Y.%m.%d_%H:%M:%S";
    strftime(time_buf, 80, format, timeinfo);

    result = time_buf;

    return(result);
}

time_t mytime::get_seconds()
{
    time_t seconds = time(NULL);

    return(seconds);
}
