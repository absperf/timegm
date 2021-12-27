/* Copyright © 2021 Taylor C. Richberger
 * This code is released under the license described in the LICENSE file
 */
#pragma once

#include <ctime>
#include <algorithm>

namespace absperf {
constexpr std::time_t days_from_civil(std::time_t y, std::time_t m, std::time_t d) noexcept
{
    y -= m <= 2;
    const std::time_t era = (y >= 0 ? y : y-399) / 400;
    const std::time_t year_of_era = y - era * 400;      // [0, 399]
    const std::time_t day_of_year = (153*(m > 2 ? m-3 : m+9) + 2)/5 + d-1;  // [0, 365]
    const std::time_t day_of_era = year_of_era * 365 + year_of_era/4 - year_of_era/100 + day_of_year;         // [0, 146096]
    return era * 146097 + day_of_era - 719468;
}

std::time_t timegm(std::tm *tm) noexcept;

constexpr std::time_t timegm(const std::tm tm) noexcept {
    // base of 'tm_year' in 'struct tm'
    constexpr std::time_t tm_year_base = 1900;

    const auto year = tm.tm_year + tm_year_base;

    std::time_t time = days_from_civil(year, tm.tm_mon + 1, tm.tm_mday);

    // Convert from days to seconds
    time = time * 24 + tm.tm_hour;
    time = time * 60 + tm.tm_min;
    time = time * 60 + std::min(tm.tm_sec, 59);

    return time;
}
}
