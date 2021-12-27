/* Copyright © 2021 Taylor C. Richberger
 * This code is released under the license described in the LICENSE file
 */

#include <ctime>
#include <cerrno>
#include <algorithm>
#include <iostream>

#include "timegm.hxx"

namespace absperf {
    template <typename I>
    inline bool check_range(const I value, const I min, const I max) noexcept {
        return value >= min && value <= max;
    }

std::time_t timegm(std::tm *tm) noexcept {
    constexpr std::time_t msum[2][12] = {
      {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334}, // normal years
      {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335}, // leap years
    };

    constexpr std::time_t mlen[2][12] = {
      {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
      {31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
    };

    // base of 'tm_year' in 'struct tm'
    constexpr std::time_t tm_year_base = 1900;

    if (!tm) {
        errno = EINVAL;
        return -1;
    }

    const auto year = tm->tm_year + tm_year_base;
    const bool isleapyear = year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);

    if (!(
        check_range(tm->tm_mon, 0, 11)
        && check_range(static_cast<std::time_t>(tm->tm_mday), static_cast<std::time_t>(1), mlen[isleapyear][tm->tm_mon])
        && check_range(tm->tm_hour, 0, 23)
        && check_range(tm->tm_min, 0, 59)
        && check_range(tm->tm_sec, 0, 60))) {
        errno = EINVAL;
        return -1;
    }

    const auto days = days_from_civil(year, tm->tm_mon + 1, tm->tm_mday);

    // 0..6=Sun..Sat; adding 4 to adjust 1970.01.01.=Thursday;
    tm->tm_wday = (days + 4) % 7;
    if (tm->tm_wday < 0)
        tm->tm_wday += 7;
    tm->tm_yday = msum[isleapyear][tm->tm_mon] + tm->tm_mday - 1;
    tm->tm_isdst = 0;

    return timegm(*tm);
}
}
