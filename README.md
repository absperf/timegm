# timegm

timegm(3) implementation for platforms that don't have it (AIX is an example)

This is forked off https://github.com/lzsiga/timegm, but with some changes:

* Written for C++14 or later, using constexprs where appropriate.
* Uses [days_from_civil](https://howardhinnant.github.io/date_algorithms.html#days_from_civil) as a better day calculation without an arbitrary minimal date, allowing arbitrary year and date representation filling the entire int range.
* Added small randomized test suite comparing to the linux timegm.
* Added a const-taking variant that works as a constexpr function.
* Handle leap seconds on input.
