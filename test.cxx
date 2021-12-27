/* Copyright © 2021 Taylor C. Richberger
 * This code is released under the license described in the LICENSE file
 */

#include "timegm.hxx"
#include <ctime>
#include <time.h>
#include <iostream>
#include <stdexcept>
#include <random>
#include <string>
#include <string.h>
#include <errno.h>

inline void check_stamp(const std::time_t stamp) {

    std::tm tm{};
    gmtime_r(&stamp, &tm);
    auto linux_tm = tm;
    auto our_tm = tm;

    errno = 0;
    const auto linux_stamp = ::timegm(&linux_tm);
    if (linux_stamp == -1 && errno != 0) {
        throw std::runtime_error(std::string("Could not set up linux time: ") + strerror(errno));
    }
    errno = 0;
    const auto our_stamp = absperf::timegm(&our_tm);
    if (our_stamp == -1 && errno != 0) {
        throw std::runtime_error(std::string("Could not set up our time: ") + strerror(errno));
    }

    if (linux_stamp != our_stamp) {
        throw std::runtime_error(std::string("Our stamp ") + std::to_string(our_stamp) + std::string(" did not match the linux stamp ") + std::to_string(linux_stamp) + std::string(" for original stamp ") + std::to_string(stamp) + std::string(", off by ") + std::to_string(our_stamp - linux_stamp));
    }
}

int main(int argc, char **argv)
{
    std::tm start{};
    std::tm end{};
    start.tm_year = -10000;
    end.tm_year = 10000;

    errno = 0;
    const auto start_stamp = timegm(&start);
    if (start_stamp == -1 && errno != 0) {
        throw std::runtime_error(std::string("Could not set up start time: ") + strerror(errno));
    }

    errno = 0;
    const auto end_stamp = timegm(&end);
    if (end_stamp == -1 && errno != 0) {
        throw std::runtime_error(std::string("Could not set up start time: ") + strerror(errno));
    }

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<std::time_t> distribution(start_stamp, end_stamp);
    for (int trial = 0; trial < 1000000; ++trial) {
        check_stamp(distribution(e1));
    }
    return 0;
}

