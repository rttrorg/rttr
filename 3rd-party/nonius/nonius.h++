// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// This file was automatically generated on 2015-03-04T15:20:13.056695Z
// Do not edit it directly
// slightly modified by Axel Menzel (fixed compiler warnings)

#ifndef NONIUS_SINGLE_INCLUDE_HPP
#define NONIUS_SINGLE_INCLUDE_HPP

// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Single header root

// #included from: nonius.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Main header

#ifndef NONIUS_HPP
#define NONIUS_HPP

// #included from: clock.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Clocks

#define NONIUS_CLOCK_HPP

#if defined(_MSC_VER) && (_MSC_VER < 1900) // MSVC <chrono> is borken and had little to no testing done before shipping (Dev14/VS15 CTP fixes it)
#include <boost/chrono.hpp>
#else
#include <chrono>
#include <ratio>
#endif

namespace nonius {
#if defined(_MSC_VER) && (_MSC_VER < 1900) // MSVC <chrono> is borken and had little to no testing done before shipping (Dev14/VS15 CTP fixes it)
    namespace chrono = boost::chrono;
    template <unsigned Num, unsigned Den = 1>
    using ratio = boost::ratio<Num, Den>;
#else
    namespace chrono = std::chrono;
    template <unsigned Num, unsigned Den = 1>
    using ratio = std::ratio<Num, Den>;
#endif
    using milli = ratio<1,       1000>;
    using micro = ratio<1,    1000000>;
    using nano  = ratio<1, 1000000000>;

    template <typename Clock>
    using Duration = typename Clock::duration;
    template <typename Clock>
    using FloatDuration = chrono::duration<double, typename Clock::period>;

    template <typename Clock>
    using TimePoint = typename Clock::time_point;

    using default_clock = chrono::high_resolution_clock;

    template <typename Clock>
    struct now {
        TimePoint<Clock> operator()() const {
            return Clock::now();
        }
    };

    using fp_seconds = chrono::duration<double, ratio<1>>;
} // namespace nonius

// #included from: benchmark.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Benchmark

#define NONIUS_BENCHMARK_HPP

// #included from: configuration.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Run configuration

#define NONIUS_CONFIGURATION_HPP

#include <string>

namespace nonius {
    struct configuration {
    public:
        int samples = 100;
        double confidence_interval = 0.95;
        int resamples = 100000;
        std::string title = "benchmarks";
        std::string output_file;
        std::string reporter;
        bool list_benchmarks = false;
        bool list_reporters = false;
        bool no_analysis = false;
        bool verbose = false;
        bool summary = false;
        bool help = false;
    };
} // namespace nonius

// #included from: environment.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Environment information

#define NONIUS_ENVIRONMENT_HPP

// #included from: outlier_classification.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Outlier information

#define NONIUS_OUTLIERS_HPP

namespace nonius {
    struct outlier_classification {
        int samples_seen = 0;
        int low_severe = 0;     // more than 3 times IQR below Q1
        int low_mild = 0;       // 1.5 to 3 times IQR below Q1
        int high_mild = 0;      // 1.5 to 3 times IQR above Q3
        int high_severe = 0;    // more than 3 times IQR above Q3

        int total() const {
            return low_severe + low_mild + high_mild + high_severe;
        }
    };
} // namespace nonius

namespace nonius {
    template <typename Duration>
    struct environment_estimate {
        Duration mean;
        outlier_classification outliers;

        template <typename Duration2>
        operator environment_estimate<Duration2>() const {
            return { mean, outliers };
        }
    };
    template <typename Clock = default_clock>
    struct environment {
        environment_estimate<FloatDuration<Clock>> clock_resolution;
        environment_estimate<FloatDuration<Clock>> clock_cost;
        //estimate function_cost;
    };
} // namespace nonius

// #included from: execution_plan.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Execution plan

#define NONIUS_EXECUTION_PLAN_HPP

namespace nonius {
    template <typename Duration>
    struct execution_plan {
        int iterations_per_sample;
        Duration estimated_duration;

        template <typename Duration2>
        operator execution_plan<Duration2>() const {
            return { iterations_per_sample, estimated_duration };
        }
    };
} // namespace nonius

// #included from: chronometer.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// User-facing chronometer

#define NONIUS_CHRONOMETER_HPP

// #included from: detail/complete_invoke.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Invoke with a special case for void

#define NONIUS_DETAIL_COMPLETE_INVOKE_HPP

#include <type_traits>
#include <utility>

namespace nonius {
    namespace detail {
        template <typename T>
        struct complete_type { using type = T; };
        template <>
        struct complete_type<void> { struct type {}; };

        template <typename T>
        using CompleteType = typename complete_type<T>::type;

        template <typename Result>
        struct complete_invoker {
            template <typename Fun, typename... Args>
            static Result invoke(Fun&& fun, Args&&... args) {
                return std::forward<Fun>(fun)(std::forward<Args>(args)...);
            }
        };
        template <>
        struct complete_invoker<void> {
            template <typename Fun, typename... Args>
            static CompleteType<void> invoke(Fun&& fun, Args&&... args) {
                std::forward<Fun>(fun)(std::forward<Args>(args)...);
                return {};
            }
        };
        template <typename Sig>
        using ResultOf = typename std::result_of<Sig>::type;

        // invoke and not return void :(
        template <typename Fun, typename... Args>
        CompleteType<ResultOf<Fun(Args...)>> complete_invoke(Fun&& fun, Args&&... args) {
            return complete_invoker<ResultOf<Fun(Args...)>>::invoke(std::forward<Fun>(fun), std::forward<Args>(args)...);
        }

        template <typename> struct always_true : std::true_type {};
        struct is_callable_tester {
            template <typename Fun, typename... Args>
            always_true<decltype(std::declval<Fun>()(std::declval<Args>()...))> static test(int);
            template <typename...>
            std::false_type static test(...);
        };
        template <typename T>
        struct is_callable;
        template <typename Fun, typename... Args>
        struct is_callable<Fun(Args...)> : decltype(is_callable_tester::test<Fun, Args...>(0)) {};
    } // namespace detail
} // namespace nonius

namespace nonius {
    namespace detail {
        struct chronometer_concept {
            virtual void start() = 0;
            virtual void finish() = 0;
            virtual ~chronometer_concept() = default;
        };
        template <typename Clock>
        struct chronometer_model final : public chronometer_concept {
            void start() override { started = Clock::now(); }
            void finish() override { finished = Clock::now(); }

            TimePoint<Clock> started;
            TimePoint<Clock> finished;
        };
    } // namespace detail

    struct chronometer {
    public:
        template <typename Fun>
        void measure(Fun&& fun) { measure(std::forward<Fun>(fun), detail::is_callable<Fun(int)>()); }

        int runs() const { return k; }

        chronometer(detail::chronometer_concept& meter, int k)
        : impl(&meter), k(k) {}

    private:
        template <typename Fun>
        void measure(Fun&& fun, std::false_type) {
            measure([&fun](int) { fun(); });
        }
        template <typename Fun>
        void measure(Fun&& fun, std::true_type) {
            impl->start();
            for(int i = 0; i < k; ++i) fun(i);
            impl->finish();
        }

        detail::chronometer_concept* impl;
        int k;
    };
} // namespace nonius

// #included from: detail/measure.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Measure

#define NONIUS_DETAIL_MEASURE_HPP

// #included from: detail/timing.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Timing

#define NONIUS_DETAIL_TIMING_HPP

#include <tuple>
#include <type_traits>

namespace nonius {
    template <typename Duration, typename Result>
    struct timing {
        Duration elapsed;
        Result result;
        int iterations;
    };
    template <typename Clock, typename Sig>
    using TimingOf = timing<Duration<Clock>, detail::CompleteType<detail::ResultOf<Sig>>>;
} // namespace nonius

#include <utility>

namespace nonius {
    namespace detail {
        template <typename Clock = default_clock, typename Fun, typename... Args>
        TimingOf<Clock, Fun(Args...)> measure(Fun&& fun, Args&&... args) {
            auto start = Clock::now();
            auto&& r = detail::complete_invoke(fun, std::forward<Args>(args)...);
            auto end = Clock::now();
            auto delta = end - start;
            return { delta, std::forward<decltype(r)>(r), 1 };
        }
    } // namespace detail
} // namespace nonius

// #included from: detail/benchmark_function.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Dumb std::function implementation for consistent call overhead

#define NONIUS_DETAIL_BENCHMARK_FUNCTION_HPP

#include <type_traits>
#include <utility>
#include <memory>

namespace nonius {
    namespace detail {
        template <typename T>
        using Decay = typename std::decay<T>::type;
        template <typename T, typename U>
        struct is_related
        : std::is_same<Decay<T>, Decay<U>> {};

        struct benchmark_function {
        private:
            struct concept {
                virtual void call(chronometer meter) const = 0;
                virtual concept* clone() const = 0;
                virtual ~concept() = default;
            };
            template <typename Fun>
            struct model : public concept {
                model(Fun&& fun) : fun(std::move(fun)) {}
                model(Fun const& fun) : fun(fun) {}

                model<Fun>* clone() const override { return new model<Fun>(*this); }

                void call(chronometer meter) const override {
                    call(meter, is_callable<Fun(chronometer)>());
                }
                void call(chronometer meter, std::true_type) const {
                    fun(meter);
                }
                void call(chronometer meter, std::false_type) const {
                    meter.measure(fun);
                }

                Fun fun;
            };
        public:
            template <typename Fun,
                      typename std::enable_if<!is_related<Fun, benchmark_function>::value, int>::type = 0>
            benchmark_function(Fun&& fun)
            : f(new model<typename std::decay<Fun>::type>(std::forward<Fun>(fun))) {}

            benchmark_function(benchmark_function&& that)
            : f(std::move(that.f)) {}

            benchmark_function(benchmark_function const& that)
            : f(that.f->clone()) {}

            benchmark_function& operator=(benchmark_function&& that) {
                f = std::move(that.f);
                return *this;
            }

            benchmark_function& operator=(benchmark_function const& that) {
                f.reset(that.f->clone());
                return *this;
            }

            void operator()(chronometer meter) const { f->call(meter); }
        private:
            std::unique_ptr<concept> f;
        };
    } // namespace detail
} // namespace nonius

// #included from: detail/repeat.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// repeat algorithm

#define NONIUS_DETAIL_REPEAT_HPP

#include <type_traits>
#include <utility>

namespace nonius {
    namespace detail {
        template <typename Fun>
        struct repeater {
            void operator()(int k) const {
                for(int i = 0; i < k; ++i) {
                    fun();
                }
            }
            Fun fun;
        };
        template <typename Fun>
        repeater<typename std::decay<Fun>::type> repeat(Fun&& fun) {
            return { std::forward<Fun>(fun) };
        }
    } // namespace detail
} // namespace nonius

// #included from: detail/run_for_at_least.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Run a function for a minimum amount of time

#define NONIUS_RUN_FOR_AT_LEAST_HPP

// #included from: timeout_error.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Exception to be thrown when a process takes too long to run

#define NONIUS_TIMEOUT_ERROR_HPP

// #included from: detail/noexcept.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Cross-compiler noexcept support

#define NONIUS_DETAIL_NOEXCEPT_HPP

#ifdef _MSC_VER
#define NONIUS_NOEXCEPT throw()
#else
#define NONIUS_NOEXCEPT noexcept
#endif // _MSC_VER

#include <sstream>
#include <string>
#include <exception>

namespace nonius {
    struct timeout_error : virtual std::exception {
    public:
        timeout_error(int seed, int iters) {
            std::ostringstream ss;
            ss << "took too long to run; seed: " << seed << ", iters: " << iters;
            message = ss.str();
        }

        char const* what() const NONIUS_NOEXCEPT override {
            return message.c_str();
        }

    private:
        std::string message;
    };
} // namespace nonius

#include <utility>

namespace nonius {
    namespace detail {
        template <typename Clock = default_clock, typename Fun>
        TimingOf<Clock, Fun(int)> run_for_at_least(Duration<Clock> how_long, int seed, Fun&& fun) {
            auto iters = seed;
            auto start = Clock::now();
            while(true) {
                auto now = Clock::now();
                if(now - start > how_long * 10) {
                    throw timeout_error(seed, iters);
                }
                auto r = detail::measure<Clock>(fun, iters);
                if(r.elapsed >= how_long) {
                    return { r.elapsed, std::move(r.result), iters };
                }
                iters *= 2;
            }
        }
    } // namespace detail
} // namespace nonius

// #included from: detail/unique_name.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Unique name generator macro

#define NONIUS_DETAIL_UNIQUE_NAME_HPP

#define NONIUS_DETAIL_UNIQUE_NAME_LINE_CAT(name, id) NONIUS_ ## name ## _ ## id
#define NONIUS_DETAIL_UNIQUE_NAME_LINE(name, id) NONIUS_DETAIL_UNIQUE_NAME_LINE_CAT(name, id)
#ifdef __COUNTER__
#define NONIUS_DETAIL_UNIQUE_NAME(name) NONIUS_DETAIL_UNIQUE_NAME_LINE(name, __COUNTER__)
#else // __COUNTER__
#define NONIUS_DETAIL_UNIQUE_NAME(name) NONIUS_DETAIL_UNIQUE_NAME_LINE(name, __LINE__)
#endif // __COUNTER__

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <cmath>

namespace nonius {
    namespace detail {
        const auto warmup_iterations = 10000;
        const auto warmup_time = chrono::milliseconds(100);
        const auto minimum_ticks = 1000;
    } // namespace detail

    struct benchmark {
        benchmark(std::string name, detail::benchmark_function fun)
        : name(std::move(name)), fun(std::move(fun)) {}

        void operator()(chronometer meter) const {
            fun(meter);
        }

        template <typename Clock>
        execution_plan<FloatDuration<Clock>> prepare(configuration cfg, environment<FloatDuration<Clock>> env) const {
            auto min_time = env.clock_resolution.mean * detail::minimum_ticks;
            auto run_time = std::min(min_time, decltype(min_time)(detail::warmup_time));
            auto&& test = detail::run_for_at_least<Clock>(chrono::duration_cast<Duration<Clock>>(run_time), 1, [this](int k) {
                detail::chronometer_model<Clock> model;
                (*this)(chronometer(model, k));
            });
            int new_iters = static_cast<int>(std::ceil(min_time * test.iterations / test.elapsed));
            return { new_iters, test.elapsed / test.iterations * new_iters * cfg.samples };
        }

        template <typename Clock>
        std::vector<FloatDuration<Clock>> run(configuration cfg, environment<FloatDuration<Clock>> env, execution_plan<FloatDuration<Clock>> plan) const {
            // warmup a bit
            detail::run_for_at_least<Clock>(chrono::duration_cast<Duration<Clock>>(detail::warmup_time), detail::warmup_iterations, detail::repeat(now<Clock>{}));

            std::vector<FloatDuration<Clock>> times;
            times.reserve(cfg.samples);
            std::generate_n(std::back_inserter(times), cfg.samples, [this, env, plan]{
                    detail::chronometer_model<Clock> model;
                    (*this)(chronometer(model, plan.iterations_per_sample));
                    auto elapsed = model.finished - model.started;
                    auto sample_time = elapsed - env.clock_cost.mean;
                    if(sample_time < FloatDuration<Clock>::zero()) sample_time = FloatDuration<Clock>::zero();
                    return (sample_time / plan.iterations_per_sample);
            });
            return times;
        }

        std::string name;
        detail::benchmark_function fun;
    };

    using benchmark_registry = std::vector<benchmark>;

    inline benchmark_registry& global_benchmark_registry() {
        static benchmark_registry registry;
        return registry;
    }

    struct benchmark_registrar {
        template <typename Fun>
        benchmark_registrar(benchmark_registry& registry, std::string name, Fun&& registrant) {
            registry.emplace_back(std::move(name), std::forward<Fun>(registrant));
        }
    };
} // namespace nonius

#define NONIUS_BENCHMARK(name, ...) \
    namespace { static ::nonius::benchmark_registrar NONIUS_DETAIL_UNIQUE_NAME(benchmark_registrar) (::nonius::global_benchmark_registry(), name, __VA_ARGS__); }

// #included from: constructor.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Eric Tremblay <eric_tremblay89@hotmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Constructor and destructor helpers

#define NONIUS_CONSTRUCTOR_HPP

#include <type_traits>

namespace nonius {
    namespace detail {
        template <typename T, bool Destruct>
        struct object_storage
        {
            typedef typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type TStorage;

            object_storage() : data() {}

            object_storage(const object_storage& other)
            {
                new(&data) T(other.stored_object());
            }

            object_storage(object_storage&& other)
            {
                new(&data) T(std::move(other.stored_object()));
            }

            ~object_storage() { destruct_on_exit<T>(); }

            template <typename... Args>
            void construct(Args&&... args)
            {
                new (&data) T(std::forward<Args>(args)...);
            }

            template <bool AllowManualDestruction = !Destruct>
            typename std::enable_if<AllowManualDestruction>::type destruct()
            {
                stored_object().~T();
            }

        private:
            // If this is a constructor benchmark, destruct the underlying object
            template <typename U>
            void destruct_on_exit(typename std::enable_if<Destruct, U>::type* = 0) { destruct<true>(); }
            // Otherwise, don't
            template <typename U>
            void destruct_on_exit(typename std::enable_if<!Destruct, U>::type* = 0) { }

            T& stored_object()
            {
                return *static_cast<T*>(static_cast<void*>(&data));
            }

            TStorage data;
        };
    }

    template <typename T>
    using storage_for = detail::object_storage<T, true>;

    template <typename T>
    using destructable_object = detail::object_storage<T, false>;
}

// #included from: go.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Runner entry point

#define NONIUS_GO_HPP

// #included from: reporter.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Reporter interface

#define NONIUS_REPORTER_HPP

// #included from: sample_analysis.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Benchmark results

#define NONIUS_BENCHMARK_RESULTS_HPP

// #included from: estimate.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Statistics estimates

#define NONIUS_ESTIMATE_HPP

namespace nonius {
    template <typename Duration>
    struct estimate {
        Duration point;
        Duration lower_bound;
        Duration upper_bound;
        double confidence_interval;

        template <typename Duration2>
        operator estimate<Duration2>() const {
            return { point, lower_bound, upper_bound, confidence_interval };
        }
    };
} // namespace nonius

#include <algorithm>
#include <vector>
#include <string>
#include <iterator>

namespace nonius {
    template <typename Duration>
    struct sample_analysis {
        std::vector<Duration> samples;
        estimate<Duration> mean;
        estimate<Duration> standard_deviation;
        outlier_classification outliers;
        double outlier_variance;

        template <typename Duration2>
        operator sample_analysis<Duration2>() const {
            std::vector<Duration2> samples2;
            samples2.reserve(samples.size());
            std::transform(samples.begin(), samples.end(), std::back_inserter(samples2), [](Duration d) { return Duration2(d); });
            return {
                std::move(samples2),
                mean,
                standard_deviation,
                outliers,
                outlier_variance,
            };
        }
    };
} // namespace nonius

#include <boost/variant.hpp>

#include <vector>
#include <string>
#include <ios>
#include <ostream>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <exception>

namespace nonius {
    struct bad_stream : virtual std::exception {
        char const* what() const NONIUS_NOEXCEPT override {
            return "failed to open file";
        }
    };

    struct reporter {
    public:
        virtual ~reporter() = default;

        void configure(configuration& cfg) {
            if(cfg.output_file.empty()) {
                os = &std::cout;
            } else {
                os = std::unique_ptr<std::ostream>(new std::ofstream(cfg.output_file));
            }
            report_stream().exceptions(std::ios::failbit);
            if(!report_stream()) throw bad_stream();
            do_configure(cfg);
        }

        void warmup_start() {
            do_warmup_start();
        }
        void warmup_end(int iterations) {
            do_warmup_end(iterations);
        }
        void estimate_clock_resolution_start() {
            do_estimate_clock_resolution_start();
        }
        void estimate_clock_resolution_complete(environment_estimate<fp_seconds> estimate) {
            do_estimate_clock_resolution_complete(estimate);
        }

        void estimate_clock_cost_start() {
            do_estimate_clock_cost_start();
        }
        void estimate_clock_cost_complete(environment_estimate<fp_seconds> estimate) {
            do_estimate_clock_cost_complete(estimate);
        }

        void suite_start() {
            do_suite_start();
        }
        void benchmark_start(std::string const& name) {
            do_benchmark_start(name);
        }

        void measurement_start(execution_plan<fp_seconds> plan) {
            do_measurement_start(plan);
        }
        void measurement_complete(std::vector<fp_seconds> const& samples) {
            do_measurement_complete(samples);
        }

        void analysis_start() {
            do_analysis_start();
        }
        void analysis_complete(sample_analysis<fp_seconds> const& analysis) {
            do_analysis_complete(analysis);
        }

        void benchmark_failure(std::exception_ptr error) {
            do_benchmark_failure(error);
        }
        void benchmark_complete() {
            do_benchmark_complete();
        }
        void suite_complete() {
            do_suite_complete();
        }

        virtual std::string description() = 0;

    private:
        virtual void do_configure(configuration& /*cfg*/) {}

        virtual void do_warmup_start() {}
        virtual void do_warmup_end(int /*iterations*/) {}

        virtual void do_estimate_clock_resolution_start() {}
        virtual void do_estimate_clock_resolution_complete(environment_estimate<fp_seconds> /*estimate*/) {}

        virtual void do_estimate_clock_cost_start() {}
        virtual void do_estimate_clock_cost_complete(environment_estimate<fp_seconds> /*estimate*/) {}

        virtual void do_suite_start() {}
        virtual void do_benchmark_start(std::string const& /*name*/) {}

        virtual void do_measurement_start(execution_plan<fp_seconds> /*plan*/) {}
        virtual void do_measurement_complete(std::vector<fp_seconds> const& /*samples*/) {}

        virtual void do_analysis_start() {} // TODO make generic?
        virtual void do_analysis_complete(sample_analysis<fp_seconds> const& /*analysis*/) {}

        virtual void do_benchmark_failure(std::exception_ptr /*error*/) {}
        virtual void do_benchmark_complete() {}
        virtual void do_suite_complete() {}

    private:
        struct stream_visitor : boost::static_visitor<std::ostream&> {
            std::ostream& operator()(std::ostream* os) const { return *os; }
            std::ostream& operator()(std::unique_ptr<std::ostream>& os) const { return *os; }
        };

    protected:
        std::ostream& progress_stream() {
            return std::cout;
        }
        std::ostream& error_stream() {
            return std::cerr;
        }
        std::ostream& report_stream() {
            return boost::apply_visitor(stream_visitor(), os);
        }

    private:
        boost::variant<std::ostream*, std::unique_ptr<std::ostream>> os;
    };

    using reporter_registry = std::unordered_map<std::string, std::unique_ptr<reporter>>;

    inline reporter_registry& global_reporter_registry() {
        static reporter_registry registry;
        return registry;
    }

    struct reporter_registrar {
        reporter_registrar(reporter_registry& registry, std::string name, reporter* registrant) {
            registry.emplace(std::move(name), std::unique_ptr<reporter>(registrant));
        }
    };
} // namespace nonius

#define NONIUS_REPORTER(name, ...) \
    namespace { static ::nonius::reporter_registrar NONIUS_DETAIL_UNIQUE_NAME(reporter_registrar) (::nonius::global_reporter_registry(), name, new __VA_ARGS__()); } \
    static_assert(true, "")

// #included from: reporters/standard_reporter.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Standard reporter

#define NONIUS_REPORTERS_STANDARD_REPORTER_HPP

// #included from: detail/pretty_print.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Pretty printing routines

#define NONIUS_PRETTY_PRINT_HPP

#include <ratio>
#include <ios>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>
#include <utility>

namespace nonius {
    namespace detail {
        inline double get_magnitude(fp_seconds secs) {
            if(secs.count() >= 2.e0) {
                return 1.e0;
            } else if(secs.count() >= 2.e-3) {
                return 1.e3;
            } else if(secs.count() >= 2.e-6) {
                return 1.e6;
            } else {
                return 1.e9;
            }
        }
        inline std::string units_for_magnitude(double magnitude) {
            if(magnitude <= 1.e0) return "s";
            else if(magnitude <= 1.e3) return "ms";
            else if(magnitude <= 1.e6) return "μs";
            else return "ns";
        }
        inline std::string pretty_duration(fp_seconds secs) {
            auto magnitude = get_magnitude(secs);
            auto units = units_for_magnitude(magnitude);
#ifdef _MSC_VER
            if(units == "μs") units = "us";
#endif
            std::ostringstream ss;
            ss << std::setprecision(ss.precision());
            ss << (secs.count() * magnitude) << ' ' << units;
            return ss.str();
        }
        inline std::string percentage(double d) {
            std::ostringstream ss;
            ss << std::setprecision(3);
            if(d != 0 && d < 1e-5) {
                ss << std::fixed;
                ss << 0.0001 << "%";
            } else {
                ss.unsetf(std::ios::floatfield);
                ss << (100. * d) << "%";
            }
            return ss.str();
        }
        inline std::string percentage_ratio(double part, double whole) {
            return percentage(part / whole);
        }
    } // namespace detail
} // namespace nonius

#include <ratio>
#include <ios>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <memory>

namespace nonius {
    struct standard_reporter : reporter {
    private:
        std::string description() override {
            return "the standard reporter";
        }

        void do_configure(configuration& cfg) override {
            n_samples = cfg.samples;
            verbose = cfg.verbose;
            summary = cfg.summary;
            n_resamples = cfg.resamples;
        }

        void do_warmup_start() override {
            if(verbose) report_stream() << "warming up\n";
        }
        void do_estimate_clock_resolution_start() override {
            if(verbose) report_stream() << "estimating clock resolution\n";
        }
        void do_estimate_clock_resolution_complete(environment_estimate<fp_seconds> estimate) override {
            if(!summary) {
                if(!verbose) report_stream() << "clock resolution: ";
                print_environment_estimate(estimate, estimate.outliers.samples_seen + 2);
            }
        }

        void do_estimate_clock_cost_start() override {
            if(verbose) report_stream() << "estimating cost of a clock call\n";
        }
        void do_estimate_clock_cost_complete(environment_estimate<fp_seconds> estimate) override {
            if(verbose) print_environment_estimate(estimate, estimate.outliers.samples_seen);
        }

        void do_benchmark_start(std::string const& name) override {
            report_stream() << '\n';
            if(!summary) report_stream() << "benchmarking ";
            report_stream() << name << "\n";
            current = name;
        }

        void do_measurement_start(execution_plan<fp_seconds> plan) override {
            report_stream() << std::setprecision(7);
            report_stream().unsetf(std::ios::floatfield);
            if(!summary) report_stream() << "collecting " << n_samples << " samples, " << plan.iterations_per_sample << " iterations each, in estimated " << detail::pretty_duration(plan.estimated_duration) << "\n";
        }
        void do_analysis_start() override {
            if(verbose) report_stream() << "bootstrapping with " << n_resamples << " resamples\n";
        }
        void do_benchmark_failure(std::exception_ptr) override {
            error_stream() << current << " failed to run successfully\n";
            report_stream() << "benchmark aborted\n";
        }
        void do_analysis_complete(sample_analysis<fp_seconds> const& analysis) override {
            print_statistic_estimate("mean", analysis.mean);
            print_statistic_estimate("std dev", analysis.standard_deviation);
            if(!summary) print_outliers(analysis.outliers);
            if(verbose) report_stream() << "variance introduced by outliers: " << detail::percentage(analysis.outlier_variance) << "\n";
            const char* effect;
            if(analysis.outlier_variance < 0.01) {
                effect = "unaffected";
            } else if(analysis.outlier_variance < 0.1) {
                effect = "slightly inflated";
            } else if(analysis.outlier_variance < 0.5) {
                effect = "moderately inflated";
            } else {
                effect = "severely inflated";
            }
            report_stream() << "variance is " << effect << " by outliers\n";
        }

        void print_environment_estimate(environment_estimate<fp_seconds> e, int iterations) {
            report_stream() << std::setprecision(7);
            report_stream().unsetf(std::ios::floatfield);
            report_stream() << "mean is " << detail::pretty_duration(e.mean) << " (" << iterations << " iterations)\n";
            if(verbose) print_outliers(e.outliers);
        }
        void print_outlier_count(const char* description, int count, int total) {
            if(count > 0) report_stream() << "  " << count << " (" << detail::percentage_ratio(count, total) << ") " << description << "\n";
        }
        void print_outliers(outlier_classification o) {
            report_stream() << "found " << o.total() << " outliers among " << o.samples_seen << " samples (" << detail::percentage_ratio(o.total(), o.samples_seen) << ")\n";
            if(verbose) {
                print_outlier_count("low severe", o.low_severe, o.samples_seen);
                print_outlier_count("low mild", o.low_mild, o.samples_seen);
                print_outlier_count("high mild", o.high_mild, o.samples_seen);
                print_outlier_count("high severe", o.high_severe, o.samples_seen);
            }
        }
        void print_statistic_estimate(const char* name, estimate<fp_seconds> estimate) {
            report_stream() << std::setprecision(7);
            report_stream().unsetf(std::ios::floatfield);
            report_stream() << name << ": " << detail::pretty_duration(estimate.point);
            if(!summary) {
                report_stream() << ", lb " << detail::pretty_duration(estimate.lower_bound)
                         << ", ub " << detail::pretty_duration(estimate.upper_bound)
                         << ", ci " << std::setprecision(3) << estimate.confidence_interval;
            }
            report_stream() << "\n";
        }

        int n_samples = 0;
        int n_resamples = 0;
        bool verbose = false;
        bool summary = false;

        std::string current;
    };

    NONIUS_REPORTER("", standard_reporter);
    NONIUS_REPORTER("standard", standard_reporter);
} // namespace nonius

// #included from: detail/estimate_clock.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Environment measurement

#define NONIUS_DETAIL_ENVIRONMENT_HPP

// #included from: detail/stats.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Statistical analysis tools

#define NONIUS_DETAIL_ANALYSIS_HPP

#include <boost/math/distributions/normal.hpp>

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <array>
#include <random>
#include <numeric>
#include <tuple>
#include <cmath>
#include <utility>
#include <future>
#include <cstddef>

namespace nonius {
    namespace detail {
        using sample = std::vector<double>;

        template <typename Iterator>
        double weighted_average_quantile(int k, int q, Iterator first, Iterator last) {
            auto count = last - first;
            double idx = (count - 1) * k /static_cast<double>(q);
            int j = static_cast<int>(idx);
            double g = idx - j;
            std::nth_element(first, first+j, last);
            auto xj = first[j];
            if(g == 0) return xj;

            auto xj1 = *std::min_element(first+(j+1), last);
            return xj + g * (xj1 - xj);
        }

        template <typename Iterator>
        outlier_classification classify_outliers(Iterator first, Iterator last) {
            std::vector<double> copy(first, last);

            auto q1 = weighted_average_quantile(1, 4, copy.begin(), copy.end());
            auto q3 = weighted_average_quantile(3, 4, copy.begin(), copy.end());
            auto iqr = q3 - q1;
            auto los = q1 - (iqr * 3.);
            auto lom = q1 - (iqr * 1.5);
            auto him = q3 + (iqr * 1.5);
            auto his = q3 + (iqr * 3.);

            outlier_classification o;
            for(; first != last; ++first) {
                auto&& t = *first;
                if(t < los) ++o.low_severe;
                else if(t < lom) ++o.low_mild;
                else if(t > his) ++o.high_severe;
                else if(t > him) ++o.high_mild;
                ++o.samples_seen;
            }
            return o;
        }

        template <typename Iterator>
        double mean(Iterator first, Iterator last) {
            auto count = last - first;
            double sum = std::accumulate(first, last, 0.);
            return sum / count;
        }

        template <typename Iterator>
        double standard_deviation(Iterator first, Iterator last) {
            auto m = mean(first, last);
            double variance = std::accumulate(first, last, 0., [m](double a, double b) {
                        double diff = b - m;
                        return a + diff*diff;
                    }) / (last - first);
            return std::sqrt(variance);
        }

        template <typename URng, typename Iterator, typename Estimator>
        sample resample(URng& rng, int resamples, Iterator first, Iterator last, Estimator& estimator) {
            auto n = last - first;
            std::uniform_int_distribution<decltype(n)> dist(0, n-1);

            sample out;
            out.reserve(resamples);
            std::generate_n(std::back_inserter(out), resamples, [n, first, &estimator, &dist, &rng] {
                std::vector<double> resampled;
                resampled.reserve(n);
                std::generate_n(std::back_inserter(resampled), n, [first, &dist, &rng] { return first[dist(rng)]; });
                return estimator(resampled.begin(), resampled.end());
            });
            std::sort(out.begin(), out.end());
            return out;
        }

        template <typename Estimator, typename Iterator>
        sample jackknife(Estimator&& estimator, Iterator first, Iterator last) {
            auto n = last - first;
            auto second = std::next(first);
            sample results;
            results.reserve(n);

            for(auto it = first; it != last; ++it) {
                std::iter_swap(it, first);
                results.push_back(estimator(second, last));
            }

            return results;
        }

        template <typename Iterator, typename Estimator>
        estimate<double> bootstrap(double confidence_level, Iterator first, Iterator last, sample const& resample, Estimator&& estimator) {
            namespace bm = boost::math;

            auto n_samples = last - first;

            double point = estimator(first, last);
            // Degenerate case with a single sample
            if(n_samples == 1) return { point, point, point, confidence_level };
            sample jack = jackknife(estimator, first, last);
            double jack_mean = mean(jack.begin(), jack.end());
            double sum_squares, sum_cubes;
            std::tie(sum_squares, sum_cubes) = std::accumulate(jack.begin(), jack.end(), std::make_pair(0., 0.), [jack_mean](std::pair<double, double> sqcb, double x) -> std::pair<double, double> {
                        auto d = jack_mean - x;
                        auto d2 = d * d;
                        auto d3 = d2 * d;
                        return { sqcb.first + d2, sqcb.second + d3 };
                    });

            double accel = sum_cubes / (6 * std::pow(sum_squares, 1.5));
            int n = static_cast<int>(resample.size());
            double prob_n = std::count_if(resample.begin(), resample.end(), [point](double x) { return x < point; }) /(double) n;
            // degenerate case with uniform samples
            if(prob_n == 0) return { point, point, point, confidence_level };
            double bias = bm::quantile(bm::normal{}, prob_n);
            double z1 = bm::quantile(bm::normal{}, (1. - confidence_level) / 2.);

            auto cumn = [n](double x) -> int { return std::lround(bm::cdf(bm::normal{}, x) * n); };
            auto a = [bias, accel](double b) { return bias + b / (1. - accel * b); };
            double b1 = bias + z1;
            double b2 = bias - z1;
            double a1 = a(b1);
            double a2 = a(b2);
            auto lo = std::max(cumn(a1), 0);
            auto hi = std::min(cumn(a2), n - 1);

            return { point, resample[lo], resample[hi], confidence_level };
        }

        inline double outlier_variance(estimate<double> mean, estimate<double> stddev, std::size_t n) {
            double sb = stddev.point;
            double mn = mean.point / n;
            double mg_min = mn / 2.;
            double sg = std::min(mg_min / 4., sb / std::sqrt(n));
            double sg2 = sg * sg;
            double sb2 = sb * sb;

            auto c_max = [n, mn, sb2, sg2](double x) -> double {
                double k = mn - x;
                double d = k * k;
                double nd = n * d;
                double k0 = (-1) * n * nd;
                double k1 = sb2 - n * sg2 + nd;
                double det = k1 * k1 - 4 * sg2 * k0;
                return (int)(-2. * k0 / (k1 + std::sqrt(det)));
            };

            auto var_out = [n, sb2, sg2](double c) {
                double nc = n - c;
                return (nc / n) * (sb2 - nc * sg2);
            };

            return std::min(var_out(1), var_out(std::min(c_max(0.), c_max(mg_min)))) / sb2;
        }

        struct bootstrap_analysis {
            estimate<double> mean;
            estimate<double> standard_deviation;
            double outlier_variance;
        };

        template <typename Iterator>
        bootstrap_analysis analyse_samples(double confidence_level, int n_resamples, Iterator first, Iterator last) {
            static std::random_device entropy;

            auto n = last - first;

            auto mean = &detail::mean<Iterator>;
            auto stddev = &detail::standard_deviation<Iterator>;

            auto estimate = [=](double(*f)(Iterator, Iterator)) {
                auto seed = entropy();
                return std::async(std::launch::async, [=]{
                    std::mt19937 rng(seed);
                    auto resampled = resample(rng, n_resamples, first, last, f);
                    return bootstrap(confidence_level, first, last, resampled, f);
                });
            };

            auto mean_future = estimate(mean);
            auto stddev_future = estimate(stddev);

            auto mean_estimate = mean_future.get();
            auto stddev_estimate = stddev_future.get();

            double outlier_variance = detail::outlier_variance(mean_estimate, stddev_estimate, n);

            return { mean_estimate, stddev_estimate, outlier_variance };
        }
    } // namespace detail
} // namespace nonius

#include <algorithm>
#include <iterator>
#include <tuple>
#include <vector>
#include <cmath>

namespace nonius {
    namespace detail {
        template <typename Clock>
        std::vector<double> resolution(int k) {
            std::vector<TimePoint<Clock>> times;
            times.reserve(k+1);
            std::generate_n(std::back_inserter(times), k+1, now<Clock>{});

            std::vector<double> deltas;
            deltas.reserve(k);
            std::transform(std::next(times.begin()), times.end(), times.begin(),
                              std::back_inserter(deltas),
                              [](TimePoint<Clock> a, TimePoint<Clock> b) { return static_cast<double>((a - b).count()); });

            return deltas;
        }

        const auto warmup_seed = 10000;
        const auto clock_resolution_estimation_time = chrono::milliseconds(500);
        const auto clock_cost_estimation_time_limit = chrono::seconds(1);
        const auto clock_cost_estimation_tick_limit = 100000;
        const auto clock_cost_estimation_time = chrono::milliseconds(10);
        const auto clock_cost_estimation_iterations = 10000;

        template <typename Clock>
        int warmup() {
            return run_for_at_least<Clock>(chrono::duration_cast<Duration<Clock>>(warmup_time), warmup_seed, &resolution<Clock>)
                    .iterations;
        }
        template <typename Clock>
        environment_estimate<FloatDuration<Clock>> estimate_clock_resolution(int iterations) {
            auto r = run_for_at_least<Clock>(chrono::duration_cast<Duration<Clock>>(clock_resolution_estimation_time), iterations, &resolution<Clock>)
                    .result;
            return {
                FloatDuration<Clock>(mean(r.begin(), r.end())),
                classify_outliers(r.begin(), r.end()),
            };
        }
        template <typename Clock>
        environment_estimate<FloatDuration<Clock>> estimate_clock_cost(FloatDuration<Clock> resolution) {
            auto time_limit = std::min(resolution * clock_cost_estimation_tick_limit, FloatDuration<Clock>(clock_cost_estimation_time_limit));
            auto time_clock = [](int k) {
                return detail::measure<Clock>([k]{
                    for(int i = 0; i < k; ++i) {
                        volatile auto ignored = Clock::now();
                        (void)ignored;
                    }
                }).elapsed;
            };
            time_clock(1);
            int iters = clock_cost_estimation_iterations;
            auto&& r = run_for_at_least<Clock>(chrono::duration_cast<Duration<Clock>>(clock_cost_estimation_time), iters, time_clock);
            std::vector<double> times;
            int nsamples = static_cast<int>(std::ceil(time_limit / r.elapsed));
            times.reserve(nsamples);
            std::generate_n(std::back_inserter(times), nsamples, [time_clock, &r]{
                        return (time_clock(r.iterations) / r.iterations).count();
                    });
            return {
                FloatDuration<Clock>(mean(times.begin(), times.end())),
                classify_outliers(times.begin(), times.end()),
            };
        }
    } // namespace detail
} // namespace nonius

// #included from: detail/analyse.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Run and analyse one benchmark

#define NONIUS_DETAIL_ANALYSE_HPP

#include <algorithm>
#include <iterator>
#include <vector>

namespace nonius {
    namespace detail {
        template <typename Duration, typename Iterator>
        sample_analysis<Duration> analyse(configuration cfg, environment<Duration>, Iterator first, Iterator last) {
            std::vector<double> samples;
            samples.reserve(last - first);
            std::transform(first, last, std::back_inserter(samples), [](Duration d) { return d.count(); });

            auto analysis = nonius::detail::analyse_samples(cfg.confidence_interval, cfg.resamples, samples.begin(), samples.end());
            auto outliers = nonius::detail::classify_outliers(samples.begin(), samples.end());

            auto wrap_estimate = [](estimate<double> e) {
                return estimate<Duration> {
                    Duration(e.point),
                    Duration(e.lower_bound),
                    Duration(e.upper_bound),
                    e.confidence_interval,
                };
            };
            std::vector<Duration> samples2;
            samples2.reserve(samples.size());
            std::transform(samples.begin(), samples.end(), std::back_inserter(samples2), [](double d) { return Duration(d); });
            return {
                std::move(samples2),
                wrap_estimate(analysis.mean),
                wrap_estimate(analysis.standard_deviation),
                outliers,
                analysis.outlier_variance,
            };
        }
    } // namespace detail
} // namespace nonius

#include <set>
#include <exception>
#include <iostream>
#include <utility>

namespace nonius {
    namespace detail {
        template <typename Clock>
        environment<FloatDuration<Clock>> measure_environment(reporter& rep) {
            rep.warmup_start();
            auto iters = detail::warmup<Clock>();
            rep.warmup_end(iters);

            rep.estimate_clock_resolution_start();
            auto resolution = detail::estimate_clock_resolution<Clock>(iters);
            rep.estimate_clock_resolution_complete(resolution);

            rep.estimate_clock_cost_start();
            auto cost = detail::estimate_clock_cost<Clock>(resolution.mean);
            rep.estimate_clock_cost_complete(cost);

            return { resolution, cost };
        }
    } // namespace detail

    struct benchmark_user_error : virtual std::exception {
        char const* what() const NONIUS_NOEXCEPT override {
            return "a benchmark failed to run successfully";
        }
    };

    template <typename Fun>
    detail::CompleteType<detail::ResultOf<Fun()>> user_code(reporter& rep, Fun&& fun) {
        try {
            return detail::complete_invoke(std::forward<Fun>(fun));
        } catch(...) {
            rep.benchmark_failure(std::current_exception());
            throw benchmark_user_error();
        }
    }

    template <typename Clock = default_clock, typename Iterator>
    void go(configuration cfg, Iterator first, Iterator last, reporter& rep) {
        rep.configure(cfg);

        auto env = detail::measure_environment<Clock>(rep);

        rep.suite_start();

        for(; first != last; ++first) {
            try {
                rep.benchmark_start(first->name);

                auto plan = user_code(rep, [&first, &cfg, &env]{ return first->template prepare<Clock>(cfg, env); });
                rep.measurement_start(plan);
                auto samples = user_code(rep, [&first, &cfg, &env, &plan]{ return first->template run<Clock>(cfg, env, plan); });
                rep.measurement_complete(std::vector<fp_seconds>(samples.begin(), samples.end()));

                if(!cfg.no_analysis) {
                    rep.analysis_start();
                    auto analysis = detail::analyse(cfg, env, samples.begin(), samples.end());
                    rep.analysis_complete(analysis);
                }

                rep.benchmark_complete();
            } catch(benchmark_user_error const&) {
                continue;
            }
        }

        rep.suite_complete();
    }
    struct duplicate_benchmarks : virtual std::exception {
        char const* what() const NONIUS_NOEXCEPT override {
            return "two or more benchmarks with the same name were registered";
        }
    };
    template <typename Clock = default_clock, typename Iterator>
    void validate_benchmarks(Iterator first, Iterator last) {
        struct strings_lt_through_pointer {
            bool operator()(std::string* a, std::string* b) const { return *a < *b; };
        };
        std::set<std::string*, strings_lt_through_pointer> names;
        for(; first != last; ++first) {
            if(!names.insert(&first->name).second)
                throw duplicate_benchmarks();
        }
    }
    template <typename Clock = default_clock, typename Iterator>
    void go(configuration cfg, Iterator first, Iterator last, reporter&& rep) {
        go(cfg, first, last, rep);
    }
    struct no_such_reporter : virtual std::exception {
        char const* what() const NONIUS_NOEXCEPT override {
            return "reporter could not be found";
        }
    };
    template <typename Clock = default_clock>
    void go(configuration cfg, benchmark_registry& benchmarks = global_benchmark_registry(), reporter_registry& reporters = global_reporter_registry()) {
        auto it = reporters.find(cfg.reporter);
        if(it == reporters.end()) throw no_such_reporter();
        validate_benchmarks(benchmarks.begin(), benchmarks.end());
        go(cfg, benchmarks.begin(), benchmarks.end(), *it->second);
    }
} // namespace nonius

// #included from: reporters/csv_reporter.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// CSV raw data reporter

#define NONIUS_REPORTERS_CSV_REPORTER_HPP

#include <ios>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <vector>

namespace nonius {
    struct csv_reporter : reporter {
    private:
        std::string description() override {
            return "outputs samples to a CSV file";
        }

        void do_configure(configuration& cfg) override {
            cfg.no_analysis = true;
            n_samples = cfg.samples;
            verbose = cfg.verbose;
        }

        void do_warmup_start() override {
            if(verbose) progress_stream() << "warming up\n";
        }
        void do_estimate_clock_resolution_start() override {
            if(verbose) progress_stream() << "estimating clock resolution\n";
        }
        void do_estimate_clock_cost_start() override {
            if(verbose) progress_stream() << "estimating cost of a clock call\n";
        }

        void do_benchmark_start(std::string const& name) override {
            if(verbose) progress_stream() << "\nbenchmarking " << name << "\n";
            current = name;
        }

        void do_measurement_start(execution_plan<fp_seconds> plan) override {
            report_stream() << std::setprecision(7);
            report_stream().unsetf(std::ios::floatfield);
            if(verbose) progress_stream() << "collecting " << n_samples << " samples, " << plan.iterations_per_sample << " iterations each, in estimated " << detail::pretty_duration(plan.estimated_duration) << "\n";
        }
        void do_measurement_complete(std::vector<fp_seconds> const& samples) override {
            data[current] = samples;
        }

        void do_benchmark_failure(std::exception_ptr) override {
            error_stream() << current << " failed to run successfully\n";
        }

        void do_suite_complete() override {
            if(verbose) progress_stream() << "\ngenerating CSV report\n";
            report_stream() << std::fixed;
            report_stream().precision(std::numeric_limits<double>::digits10);
            bool first = true;
            for(auto&& kv : data) {
                if(!first) report_stream() << ",";
                report_stream() << "\"" << escape(kv.first) << "\""; // TODO escape
                first = false;
            }
            report_stream() << "\n";
            for(int i = 0; i < n_samples; ++i) {
                first = true;
                for(auto&& kv : data) {
                    if(!first) report_stream() << ",";
                    report_stream() << kv.second[i].count();
                    first = false;
                }
                report_stream() << "\n";
            }
            if(verbose) progress_stream() << "done\n";
        }

    private:
        static std::string escape(std::string const& source) {
            auto first = source.begin();
            auto last = source.end();

            auto quotes = std::count(first, last, '"');
            if(quotes == 0) return source;

            std::string escaped;
            escaped.reserve(source.size() + quotes);

            while(first != last) {
                auto next_quote = std::find(first, last, '"');
                std::copy(first, next_quote, std::back_inserter(escaped));
                first = next_quote;
                if(first != last) {
                    ++first;
                    escaped.push_back('"');
                    escaped.push_back('"');
                }
            }

            return escaped;
        }

        int n_samples;
        bool verbose;
        std::string current;
        std::unordered_map<std::string, std::vector<fp_seconds>> data;
    };

    NONIUS_REPORTER("csv", csv_reporter);
} // namespace nonius

// #included from: reporters/junit_reporter.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// JUnit reporter

#define NONIUS_REPORTERS_JUNIT_REPORTER_HPP

// #included from: detail/escape.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// General escaping routines

#define NONIUS_DETAIL_ESCAPE_HPP

#include <string>
#include <algorithm>
#include <iterator>
#include <utility>

namespace nonius {
    namespace detail {
        inline std::string escape(std::string const& source, std::unordered_map<char, std::string> const& escapes) {
            std::string magic;
            magic.reserve(escapes.size());
            std::transform(escapes.begin(), escapes.end(), std::back_inserter(magic), [](std::pair<char const, std::string> const& p) { return p.first; });

            auto first = source.begin();
            auto last = source.end();

            auto n_magic = std::count_if(first, last, [&magic](char c) { return magic.find(c) != std::string::npos; });

            std::string escaped;
            escaped.reserve(source.size() + n_magic*6);

            while(first != last) {
                auto next_magic = std::find_first_of(first, last, magic.begin(), magic.end());
                std::copy(first, next_magic, std::back_inserter(escaped));
                first = next_magic;
                if(first != last) {
                    auto it = escapes.find(*first);
                    if(it != escapes.end()) {
                        escaped += it->second;
                    }
                    ++first;
                }
            }
            return escaped;
        }
    } // namespace detail
} // namespace nonius

#include <ios>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <vector>
#include <exception>

namespace nonius {
    struct junit_reporter : reporter {
    private:
        std::string description() override {
            return "outputs results to a JUnit-compatible XML file";
        }

        void do_configure(configuration& cfg) override {
            n_samples = cfg.samples;
            confidence_interval = cfg.confidence_interval;
            resamples = cfg.resamples;
            verbose = cfg.verbose;
            title = cfg.title;
        }

        struct result {
            sample_analysis<fp_seconds> analysis;
            std::exception_ptr failure;
        };

        void do_warmup_start() override {
            if(verbose) progress_stream() << "warming up\n";
        }
        void do_estimate_clock_resolution_start() override {
            if(verbose) progress_stream() << "estimating clock resolution\n";
        }
        void do_estimate_clock_cost_start() override {
            if(verbose) progress_stream() << "estimating cost of a clock call\n";
        }

        void do_benchmark_start(std::string const& name) override {
            if(verbose) progress_stream() << "\nbenchmarking " << name << "\n";
            current = name;
        }

        void do_measurement_start(execution_plan<fp_seconds> plan) override {
            report_stream() << std::setprecision(7);
            report_stream().unsetf(std::ios::floatfield);
            if(verbose) progress_stream() << "collecting " << n_samples << " samples, " << plan.iterations_per_sample << " iterations each, in estimated " << detail::pretty_duration(plan.estimated_duration) << "\n";
        }

        void do_analysis_start() override {
            if(verbose) report_stream() << "analysing samples\n";
        }
        void do_analysis_complete(sample_analysis<fp_seconds> const& analysis) override {
            data[current] = { analysis, nullptr };
        }

        void do_benchmark_failure(std::exception_ptr e) override {
            data[current] = { sample_analysis<fp_seconds>(), e };
            error_stream() << current << " failed to run successfully\n";
        }

        void do_suite_complete() override {
            if(verbose) progress_stream() << "\ngenerating JUnit report\n";

            report_stream() << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            report_stream() << "<testsuite name=\"" << escape(title) << "\" tests=\"" << data.size() << "\"";
            auto failures = std::count_if(data.begin(), data.end(),
                    [](std::pair<std::string const&, result> const& p) {
                        return static_cast<bool>(p.second.failure);
                    });
            if(failures > 0) report_stream() << " errors=\"" << failures << "\"";
            report_stream() << ">\n";

            report_stream() << " <properties>\n";
            report_stream() << "  <property name=\"samples\" value=\"" << n_samples << "\">\n";
            report_stream() << "  <property name=\"confidence_interval\" value=\"" << std::setprecision(3) << confidence_interval << "\">\n";
            report_stream() << "  <property name=\"resamples\" value=\"" << resamples << "\">\n";
            report_stream() << " </properties>\n";

            for(auto tc : data) {
                report_stream() << " <testcase name=\"" << escape(tc.first) << "\"";
                if(tc.second.failure) {
                    report_stream() << ">\n";
                    try {
                        std::rethrow_exception(tc.second.failure);
                    } catch(std::exception const& e) {
                        report_stream() << "  <error message=\"" << escape(e.what()) << "\" />\n";
                    } catch(...) {
                        report_stream() << "  <error message=\"unknown error\" />\n";
                    }
                    report_stream() << " </testcase>\n";
                } else {
                    report_stream() << std::fixed;
                    report_stream().precision(std::numeric_limits<double>::digits10);
                    report_stream() << " time=\"" << tc.second.analysis.mean.point.count() << "\" />\n";
                }
            }

            report_stream() << "</testsuite>\n";

            report_stream() << std::flush;

            if(verbose) progress_stream() << "done\n";
        }

        static std::string escape(std::string const& source) {
            static const std::unordered_map<char, std::string> escapes {
                { '\'', "&apos;" },
                { '"',  "&quot;" },
                { '<',  "&lt;"   },
                { '>',  "&gt;"   },
                { '&',  "&amp;"  },
            };
            return detail::escape(source, escapes);
        }

        int n_samples;
        double confidence_interval;
        int resamples;
        bool verbose;
        std::string title;

        std::string current;
        std::unordered_map<std::string, result> data;
    };

    NONIUS_REPORTER("junit", junit_reporter);
} // namespace nonius

// #included from: reporters/html_reporter.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// HTML single-chart reporter

#define NONIUS_REPORTERS_HTML_ALL_REPORTER_HPP

// #included from: detail/cpptempl.h
// cpptempl
// =================
// This is a template engine for C++.
//
// Syntax
// =================
// Variables: {$variable_name}
// Loops: {% for person in people %}Name: {$person.name}{% endfor %}
// If: {% for person.name == "Bob" %}Full name: Robert{% endif %}
//
// Copyright
// ==================
// Copyright (c) Ryan Ginstrom
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// Modified by: Martinho Fernandes
//
// Usage
// =======================
//     std::string text = "{% if item %}{$item}{% endif %}\n"
//      "{% if thing %}{$thing}{% endif %}" ;
//  cpptempl::data_map data ;
//  data["item"] = cpptempl::make_data("aaa") ;
//  data["thing"] = cpptempl::make_data("bbb") ;
//
//     std::string result = cpptempl::parse(text, data) ;
//
// Handy Functions
// ========================
// make_data() : Feed it a string, data_map, or data_list to create a data entry.
// Example:
//  data_map person ;
//  person["name"] = make_data("Bob") ;
//  person["occupation"] = make_data("Plumber") ;
//  data_map data ;
//  data["person"] = make_data(person) ;
//     std::string result = parse(templ_text, data) ;

#ifndef CPPTEMPL_H
#define CPPTEMPL_H

#ifdef _MSC_VER
#pragma warning( disable : 4996 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#pragma warning( disable : 4512 ) // 'std::copy': Function call with parameters that may be unsafe - this call relies on the caller to check that the passed values are correct. To disable this warning, use -D_SCL_SECURE_NO_WARNINGS. See documentation on how to use Visual C++ 'Checked Iterators'
#endif

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <unordered_map>
#include <boost/lexical_cast.hpp>

#include <ostream>

#include <sstream>
#include <boost/algorithm/string.hpp>

namespace cpptempl
{
    // various typedefs

    // data classes
    class Data ;
    class DataValue ;
    class DataList ;
    class DataMap ;

    class data_ptr {
    public:
        data_ptr() {}
        template<typename T> data_ptr(const T& data) {
            this->operator =(data);
        }
        data_ptr(DataValue* data);
        data_ptr(DataList* data);
        data_ptr(DataMap* data);
        data_ptr(const data_ptr& data) {
            ptr = data.ptr;
        }
        template<typename T> void operator = (const T& data);
        void push_back(const data_ptr& data);
        virtual ~data_ptr() {}
        Data* operator ->() {
            return ptr.get();
        }
    private:
        std::shared_ptr<Data> ptr;
    };
    typedef std::vector<data_ptr> data_list ;

    class data_map {
    public:
        data_ptr& operator [](const std::string& key);
        bool empty();
        bool has(const std::string& key);
    private:
        std::unordered_map<std::string, data_ptr> data;
    };

    template<> inline void data_ptr::operator = (const data_ptr& data);
    template<> void data_ptr::operator = (const std::string& data);
    template<> void data_ptr::operator = (const std::string& data);
    template<> void data_ptr::operator = (const data_map& data);
    template<typename T>
    void data_ptr::operator = (const T& data) {
        std::string data_str = boost::lexical_cast<std::string>(data);
        this->operator =(data_str);
    }

    // token classes
    class Token ;
    typedef std::shared_ptr<Token> token_ptr ;
    typedef std::vector<token_ptr> token_vector ;

    // Custom exception class for library errors
    class TemplateException : public std::exception
    {
    public:
        TemplateException(std::string reason) : m_reason(std::move(reason)){}
        char const* what() const NONIUS_NOEXCEPT override {
            return m_reason.c_str();
        }
    private:
        std::string m_reason;
    };

    // Data types used in templates
    class Data
    {
    public:
        virtual bool empty() = 0 ;
        virtual std::string getvalue();
        virtual data_list& getlist();
        virtual data_map& getmap() ;
    };

    class DataValue : public Data
    {
        std::string m_value ;
    public:
        DataValue(std::string value) : m_value(value){}
        std::string getvalue();
        bool empty();
    };

    class DataList : public Data
    {
        data_list m_items ;
    public:
        DataList(const data_list &items) : m_items(items){}
        data_list& getlist() ;
        bool empty();
    };

    class DataMap : public Data
    {
        data_map m_items ;
    public:
        DataMap(const data_map &items) : m_items(items){}
        data_map& getmap();
        bool empty();
    };

    inline data_ptr::data_ptr(DataValue* data) : ptr(data) {}
    inline data_ptr::data_ptr(DataList* data) : ptr(data) {}
    inline data_ptr::data_ptr(DataMap* data) : ptr(data) {}

    // convenience functions for making data objects
    inline data_ptr make_data(std::string val)
    {
        return data_ptr(new DataValue(val)) ;
    }
    inline data_ptr make_data(data_list &val)
    {
        return data_ptr(new DataList(val)) ;
    }
    inline data_ptr make_data(data_map &val)
    {
        return data_ptr(new DataMap(val)) ;
    }
    // get a data value from a data map
    // e.g. foo.bar => data["foo"]["bar"]
    data_ptr parse_val(std::string key, data_map &data) ;

    typedef enum
    {
        TOKEN_TYPE_NONE,
        TOKEN_TYPE_TEXT,
        TOKEN_TYPE_VAR,
        TOKEN_TYPE_IF,
        TOKEN_TYPE_FOR,
        TOKEN_TYPE_ENDIF,
        TOKEN_TYPE_ENDFOR,
    } TokenType;

    // Template tokens
    // base class for all token types
    class Token
    {
    public:
        virtual TokenType gettype() = 0 ;
        virtual void gettext(std::ostream &stream, data_map &data) = 0 ;
        virtual void set_children(token_vector &children);
        virtual token_vector & get_children();
    };

    // normal text
    class TokenText : public Token
    {
        std::string m_text ;
    public:
        TokenText(std::string text) : m_text(text){}
        TokenType gettype();
        void gettext(std::ostream &stream, data_map &data);
    };

    // variable
    class TokenVar : public Token
    {
        std::string m_key ;
    public:
        TokenVar(std::string key) : m_key(key){}
        TokenType gettype();
        void gettext(std::ostream &stream, data_map &data);
    };

    // for block
    class TokenFor : public Token
    {
    public:
        std::string m_key ;
        std::string m_val ;
        token_vector m_children ;
        TokenFor(std::string expr);
        TokenType gettype();
        void gettext(std::ostream &stream, data_map &data);
        void set_children(token_vector &children);
        token_vector &get_children();
    };

    // if block
    class TokenIf : public Token
    {
    public:
        std::string m_expr ;
        token_vector m_children ;
        TokenIf(std::string expr) : m_expr(expr){}
        TokenType gettype();
        void gettext(std::ostream &stream, data_map &data);
        bool is_true(std::string expr, data_map &data);
        void set_children(token_vector &children);
        token_vector &get_children();
    };

    // end of block
    class TokenEnd : public Token // end of control block
    {
        std::string m_type ;
    public:
        TokenEnd(std::string text) : m_type(text){}
        TokenType gettype();
        void gettext(std::ostream &stream, data_map &data);
    };

    std::string gettext(token_ptr token, data_map &data) ;

    void parse_tree(token_vector &tokens, token_vector &tree, TokenType until=TOKEN_TYPE_NONE) ;
    token_vector & tokenize(std::string text, token_vector &tokens) ;

    // The big daddy. Pass in the template and data,
    // and get out a completed doc.
    void parse(std::ostream &stream, std::string templ_text, data_map &data) ;
    std::string parse(std::string templ_text, data_map &data);
    std::string parse(std::string templ_text, data_map &data);

// *********** Implementation ************

    //////////////////////////////////////////////////////////////////////////
    // Data classes
    //////////////////////////////////////////////////////////////////////////

    // data_map
    inline data_ptr& data_map::operator [](const std::string& key) {
        return data[key];
    }
    inline bool data_map::empty() {
        return data.empty();
    }
    inline bool data_map::has(const std::string& key) {
        return data.find(key) != data.end();
    }

    // data_ptr
    template<>
    inline void data_ptr::operator = (const data_ptr& data) {
        ptr = data.ptr;
    }

    template<>
    inline void data_ptr::operator = (const std::string& data) {
        ptr.reset(new DataValue(data));
    }

    template<>
    inline void data_ptr::operator = (const data_map& data) {
        ptr.reset(new DataMap(data));
    }

    inline void data_ptr::push_back(const data_ptr& data) {
        if (!ptr) {
            ptr.reset(new DataList(data_list()));
        }
        data_list& list = ptr->getlist();
        list.push_back(data);
    }

    // base data
    inline std::string Data::getvalue()
    {
        throw TemplateException("Data item is not a value") ;
    }

    inline data_list& Data::getlist()
    {
        throw TemplateException("Data item is not a list") ;
    }
    inline data_map& Data::getmap()
    {
        throw TemplateException("Data item is not a dictionary") ;
    }
    // data value
    inline std::string DataValue::getvalue()
    {
        return m_value ;
    }
    inline bool DataValue::empty()
    {
        return m_value.empty();
    }
    // data list
    inline data_list& DataList::getlist()
    {
        return m_items ;
    }

    inline bool DataList::empty()
    {
        return m_items.empty();
    }
    // data map
    inline data_map& DataMap:: getmap()
    {
        return m_items ;
    }
    inline bool DataMap::empty()
    {
        return m_items.empty();
    }
    //////////////////////////////////////////////////////////////////////////
    // parse_val
    //////////////////////////////////////////////////////////////////////////
    inline data_ptr parse_val(std::string key, data_map &data)
    {
        // quoted string
        if (key[0] == '\"')
        {
            return make_data(boost::trim_copy_if(key, [](char c){ return c == '"'; }));
        }
        // check for dotted notation, i.e [foo.bar]
        size_t index = key.find(".") ;
        if (index == std::string::npos)
        {
            if (!data.has(key))
            {
                return make_data("{$" + key + "}") ;
            }
            return data[key] ;
        }

        std::string sub_key = key.substr(0, index) ;
        if (!data.has(sub_key))
        {
            return make_data("{$" + key + "}") ;
        }
        data_ptr item = data[sub_key] ;
        return parse_val(key.substr(index+1), item->getmap()) ;
    }

    //////////////////////////////////////////////////////////////////////////
    // Token classes
    //////////////////////////////////////////////////////////////////////////

    // defaults, overridden by subclasses with children
    inline void Token::set_children( token_vector & )
    {
        throw TemplateException("This token type cannot have children") ;
    }

    inline token_vector & Token::get_children()
    {
        throw TemplateException("This token type cannot have children") ;
    }

    // TokenText
    inline TokenType TokenText::gettype()
    {
        return TOKEN_TYPE_TEXT ;
    }

    inline void TokenText::gettext( std::ostream &stream, data_map & )
    {
        stream << m_text ;
    }

    // TokenVar
    inline TokenType TokenVar::gettype()
    {
        return TOKEN_TYPE_VAR ;
    }

    inline void TokenVar::gettext( std::ostream &stream, data_map &data )
    {
        stream << parse_val(m_key, data)->getvalue() ;
    }

    // TokenFor
    inline TokenFor::TokenFor(std::string expr)
    {
        std::vector<std::string> elements ;
        boost::split(elements, expr, boost::is_space()) ;
        if (elements.size() != 4u)
        {
            throw TemplateException("Invalid syntax in for statement") ;
        }
        m_val = elements[1] ;
        m_key = elements[3] ;
    }

    inline TokenType TokenFor::gettype()
    {
        return TOKEN_TYPE_FOR ;
    }

    inline void TokenFor::gettext( std::ostream &stream, data_map &data )
    {
        data_ptr value = parse_val(m_key, data) ;
        data_list &items = value->getlist() ;
        for (size_t i = 0 ; i < items.size() ; ++i)
        {
            data_map loop ;
            loop["index"] = make_data(boost::lexical_cast<std::string>(i+1)) ;
            loop["index0"] = make_data(boost::lexical_cast<std::string>(i)) ;
            data["loop"] = make_data(loop);
            data[m_val] = items[i] ;
            for(size_t j = 0 ; j < m_children.size() ; ++j)
            {
                m_children[j]->gettext(stream, data) ;
            }
        }
    }

    inline void TokenFor::set_children( token_vector &children )
    {
        m_children.assign(children.begin(), children.end()) ;
    }

    inline token_vector & TokenFor::get_children()
    {
        return m_children;
    }

    // TokenIf
    inline TokenType TokenIf::gettype()
    {
        return TOKEN_TYPE_IF ;
    }

    inline void TokenIf::gettext( std::ostream &stream, data_map &data )
    {
        if (is_true(m_expr, data))
        {
            for(size_t j = 0 ; j < m_children.size() ; ++j)
            {
                m_children[j]->gettext(stream, data) ;
            }
        }
    }

    inline bool TokenIf::is_true( std::string expr, data_map &data )
    {
        std::vector<std::string> elements ;
        boost::split(elements, expr, boost::is_space()) ;

        if (elements[1] == "not")
        {
            return parse_val(elements[2], data)->empty() ;
        }
        if (elements.size() == 2)
        {
            return ! parse_val(elements[1], data)->empty() ;
        }
        data_ptr lhs = parse_val(elements[1], data) ;
        data_ptr rhs = parse_val(elements[3], data) ;
        if (elements[2] == "==")
        {
            return lhs->getvalue() == rhs->getvalue() ;
        }
        return lhs->getvalue() != rhs->getvalue() ;
    }

    inline void TokenIf::set_children( token_vector &children )
    {
        m_children.assign(children.begin(), children.end()) ;
    }

    inline token_vector & TokenIf::get_children()
    {
        return m_children;
    }

    // TokenEnd
    inline TokenType TokenEnd::gettype()
    {
        return m_type == "endfor" ? TOKEN_TYPE_ENDFOR : TOKEN_TYPE_ENDIF ;
    }

    inline void TokenEnd::gettext( std::ostream &, data_map &)
    {
        throw TemplateException("End-of-control statements have no associated text") ;
    }

    // gettext
    // generic helper for getting text from tokens.

    inline std::string gettext(token_ptr token, data_map &data)
    {
        std::ostringstream stream ;
        token->gettext(stream, data) ;
        return stream.str() ;
    }
    //////////////////////////////////////////////////////////////////////////
    // parse_tree
    // recursively parses list of tokens into a tree
    //////////////////////////////////////////////////////////////////////////
    inline void parse_tree(token_vector &tokens, token_vector &tree, TokenType until)
    {
        while(! tokens.empty())
        {
            // 'pops' first item off list
            token_ptr token = tokens[0] ;
            tokens.erase(tokens.begin()) ;

            if (token->gettype() == TOKEN_TYPE_FOR)
            {
                token_vector children ;
                parse_tree(tokens, children, TOKEN_TYPE_ENDFOR) ;
                token->set_children(children) ;
            }
            else if (token->gettype() == TOKEN_TYPE_IF)
            {
                token_vector children ;
                parse_tree(tokens, children, TOKEN_TYPE_ENDIF) ;
                token->set_children(children) ;
            }
            else if (token->gettype() == until)
            {
                return ;
            }
            tree.push_back(token) ;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // tokenize
    // parses a template into tokens (text, for, if, variable)
    //////////////////////////////////////////////////////////////////////////
    inline token_vector & tokenize(std::string text, token_vector &tokens)
    {
        while(! text.empty())
        {
            size_t pos = text.find("{") ;
            if (pos == std::string::npos)
            {
                if (! text.empty())
                {
                    tokens.push_back(token_ptr(new TokenText(text))) ;
                }
                return tokens ;
            }
            std::string pre_text = text.substr(0, pos) ;
            if (! pre_text.empty())
            {
                tokens.push_back(token_ptr(new TokenText(pre_text))) ;
            }
            text = text.substr(pos+1) ;
            if (text.empty())
            {
                tokens.push_back(token_ptr(new TokenText("{"))) ;
                return tokens ;
            }

            // variable
            if (text[0] == '$')
            {
                pos = text.find("}") ;
                if (pos != std::string::npos)
                {
                    tokens.push_back(token_ptr (new TokenVar(text.substr(1, pos-1)))) ;
                    text = text.substr(pos+1) ;
                }
            }
            // control statement
            else if (text[0] == '%')
            {
                pos = text.find("}") ;
                if (pos != std::string::npos)
                {
                    std::string expression = boost::trim_copy(text.substr(1, pos-2)) ;
                    text = text.substr(pos+1) ;
                    if (boost::starts_with(expression, "for"))
                    {
                        tokens.push_back(token_ptr (new TokenFor(expression))) ;
                    }
                    else if (boost::starts_with(expression, "if"))
                    {
                        tokens.push_back(token_ptr (new TokenIf(expression))) ;
                    }
                    else
                    {
                        tokens.push_back(token_ptr (new TokenEnd(boost::trim_copy(expression)))) ;
                    }
                }
            }
            else
            {
                tokens.push_back(token_ptr(new TokenText("{"))) ;
            }
        }
        return tokens ;
    }

    inline std::string parse(std::string templ_text, data_map &data)
    {
        std::ostringstream stream ;
        parse(stream, templ_text, data) ;
        return stream.str() ;
    }
    inline void parse(std::ostream &stream, std::string templ_text, data_map &data)
    {
        token_vector tokens ;
        tokenize(templ_text, tokens) ;
        token_vector tree ;
        parse_tree(tokens, tree) ;

        for (size_t i = 0 ; i < tree.size() ; ++i)
        {
            // Recursively calls gettext on each node in the tree.
            // gettext returns the appropriate text for that node.
            // for text, itself;
            // for variable, substitution;
            // for control statement, recursively gets kids
            tree[i]->gettext(stream, data) ;
        }
    }
}

#endif // CPPTEMPL_H
#include <ios>
#include <iomanip>
#include <algorithm>
#include <string>
#include <iterator>
#include <limits>
#include <unordered_map>
#include <vector>

#include <fstream>

namespace nonius {
    struct html_reporter : reporter {
    private:
        static std::string const& template_string() {
            static char const* template_parts[] = {
// generated content broken into pieces because MSVC is in the 1990s.
// #included from: detail/html_report_template.g.h++
"<!DOCTYPE html>\n"
"<html>\n"
" <head>\n"
"  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\n"
"  <title>{$title} - nonius report</title>\n"
"  <style type=\"text/css\">\n"
"#all-together {\n"
"  min-width: 310px;\n"
"  height: 400px;\n"
"  margin: 0 auto;\n"
"}\n"
"  </style>\n"
"  <script type=\"text/javascript\">\n"
"/*! jQuery v1.11.0 | (c) 2005, 2014 jQuery Foundation, Inc. | jquery.org/license */\n"
,
"!function(a,b){\"object\"==typeof module&&\"object\"==typeof module.exports?module.exports=a.document?b(a,!0):function(a){if(!a.document)throw new Error(\"jQuery requires a window with a document\");return b(a)}:b(a)}(\"undefined\"!=typeof window?window:this,function(a,b){var c=[],d=c.slice,e=c.concat,f=c.push,g=c.indexOf,h={},i=h.toString,j=h.hasOwnProperty,k=\"\".trim,l={},m=\"1.11.0\",n=function(a,b){return new n.fn.init(a,b)},o=/^[\\s\\uFEFF\\xA0]+|[\\s\\uFEFF\\xA0]+$/g,p=/^-ms-/,q=/-([\\da-z])/gi,r=function(a,b){return b.toUpperCase()};n.fn=n.prototype={jquery:m,constructor:n,selector:\"\",length:0,toArray:function(){return d.call(this)},get:function(a){return null!=a?0>a?this[a+this.length]:this[a]:d.call(this)},pushStack:function(a){var b=n.merge(this.constructor(),a);return b.prevObject=this,b.context=this.context,b},each:function(a,b){return n.each(this,a,b)},map:function(a){return this.pushStack(n.map(this,function(b,c){return a.call(b,c,b)}))},slice:function(){return this.pushStack(d.apply(this,arguments))},first:function(){return this.eq(0)},last:function(){return this.eq(-1)},eq:function(a){var b=this.length,c=+a+(0>a?b:0);return this.pushStack(c>=0&&b>c?[this[c]]:[])},end:function(){return this.prevObject||this.constructor(null)},push:f,sort:c.sort,splice:c.splice},n.extend=n.fn.extend=function(){var a,b,c,d,e,f,g=arguments[0]||{},h=1,i=arguments.length,j=!1;for(\"boolean\"==typeof g&&(j=g,g=arguments[h]||{},h++),\"object\"==typeof g||n.isFunction(g)||(g={}),h===i&&(g=this,h--);i>h;h++)if(null!=(e=arguments[h]))for(d in e)a=g[d],c=e[d],g!==c&&(j&&c&&(n.isPlainObject(c)||(b=n.isArray(c)))?(b?(b=!1,f=a&&n.isArray(a)?a:[]):f=a&&n.isPlainObject(a)?a:{},g[d]=n.extend(j,f,c)):void 0!==c&&(g[d]=c));return g},n.extend({expando:\"jQuery\"+(m+Math.random()).replace(/\\D/g,\"\"),isReady:!0,error:function(a){throw new Error(a)},noop:function(){},isFunction:function(a){return\"function\"===n.type(a)},isArray:Array.isArray||function(a){return\"array\"===n.type(a)},isWindow:function(a){return null!=a&&a==a.window},isNumeric:function(a){return a-parseFloat(a)>=0},isEmptyObject:function(a){var b;for(b in a)return!1;return!0},isPlainObject:function(a){var b;if(!a||\"object\"!==n.type(a)||a.nodeType||n.isWindow(a))return!1;try{if(a.constructor&&!j.call(a,\"constructor\")&&!j.call(a.constructor.prototype,\"isPrototypeOf\"))return!1}catch(c){return!1}if(l.ownLast)for(b in a)return j.call(a,b);for(b in a);return void 0===b||j.call(a,b)},type:function(a){return null==a?a+\"\":\"object\"==typeof a||\"function\"==typeof a?h[i.call(a)]||\"object\":typeof a},globalEval:function(b){b&&n.trim(b)&&(a.execScript||function(b){a.eval.call(a,b)})(b)},camelCase:function(a){return a.replace(p,\"ms-\").replace(q,r)},nodeName:function(a,b){return a.nodeName&&a.nodeName.toLowerCase()===b.toLowerCase()},each:function(a,b,c){var d,e=0,f=a.length,g=s(a);if(c){if(g){for(;f>e;e++)if(d=b.apply(a[e],c),d===!1)break}else for(e in a)if(d=b.apply(a[e],c),d===!1)break}else if(g){for(;f>e;e++)if(d=b.call(a[e],e,a[e]),d===!1)break}else for(e in a)if(d=b.call(a[e],e,a[e]),d===!1)break;return a},trim:k&&!k.call(\"\\ufeff\\xa0\")?function(a){return null==a?\"\":k.call(a)}:function(a){return null==a?\"\":(a+\"\").replace(o,\"\")},makeArray:function(a,b){var c=b||[];return null!=a&&(s(Object(a))?n.merge(c,\"string\"==typeof a?[a]:a):f.call(c,a)),c},inArray:function(a,b,c){var d;if(b){if(g)return g.call(b,a,c);for(d=b.length,c=c?0>c?Math.max(0,d+c):c:0;d>c;c++)if(c in b&&b[c]===a)return c}return-1},merge:function(a,b){var c=+b.length,d=0,e=a.length;while(c>d)a[e++]=b[d++];if(c!==c)while(void 0!==b[d])a[e++]=b[d++];return a.length=e,a},grep:function(a,b,c){for(var d,e=[],f=0,g=a.length,h=!c;g>f;f++)d=!b(a[f],f),d!==h&&e.push(a[f]);return e},map:function(a,b,c){var d,f=0,g=a.length,h=s(a),i=[];if(h)for(;g>f;f++)d=b(a[f],f,c),null!=d&&i.push(d);else for(f in a)d=b(a[f],f,c),null!=d&&i.push(d);return e.apply([],i)},guid:1,proxy:function(a,b){var c,e,f;return\"string\"==typeof b&&(f=a[b],b=a,a=f),n.isFunction(a)?(c=d.call(arguments,2),e=function(){return a.apply(b||this,c.concat(d.call(arguments)))},e.guid=a.guid=a.guid||n.guid++,e):void 0},now:function(){return+new Date},support:l}),n.each(\"Boolean Number String Function Array Date RegExp Object Error\".split(\" \"),function(a,b){h[\"[object \"+b+\"]\"]=b.toLowerCase()});function s(a){var b=a.length,c=n.type(a);return\"function\"===c||n.isWindow(a)?!1:1===a.nodeType&&b?!0:\"array\"===c||0===b||\"number\"==typeof b&&b>0&&b-1 in a}var t=function(a){var b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s=\"sizzle\"+-new Date,t=a.document,u=0,v=0,w=eb(),x=eb(),y=eb(),z=function(a,b){return a===b&&(j=!0),0},A=\"undefined\",B=1<<31,C={}.hasOwnProperty,D=[],E=D.pop,F=D.push,G=D.push,H=D.slice,I=D.indexOf||function(a){for(var b=0,c=this.length;c>b;b++)if(this[b]===a)return b;return-1},J=\"checked|selected|async|autofocus|autoplay|controls|defer|disabled|hidden|ismap|loop|multiple|open|readonly|required|scoped\",K=\"[\\\\x20\\\\t\\\\r\\\\n\\\\f]\",L=\"(?:\\\\\\\\.|[\\\\w-]|[^\\\\x00-\\\\xa0])+\",M=L.replace(\"w\",\"w#\"),N=\"\\\\[\"+K+\"*(\"+L+\")\"+K+\"*(?:([*^$|!~]?=)\"+K+\"*(?:(['\\\"])((?:\\\\\\\\.|[^\\\\\\\\])*?)\\\\3|(\"+M+\")|)|)\"+K+\"*\\\\]\",O=\":(\"+L+\")(?:\\\\(((['\\\"])((?:\\\\\\\\.|[^\\\\\\\\])*?)\\\\3|((?:\\\\\\\\.|[^\\\\\\\\()[\\\\]]|\"+N.replace(3,8)+\")*)|.*)\\\\)|)\",P=new RegExp(\"^\"+K+\"+|((?:^|[^\\\\\\\\])(?:\\\\\\\\.)*)\"+K+\"+$\",\"g\"),Q=new RegExp(\"^\"+K+\"*,\"+K+\"*\"),R=new RegExp(\"^\"+K+\"*([>+~]|\"+K+\")\"+K+\"*\"),S=new RegExp(\"=\"+K+\"*([^\\\\]'\\\"]*?)\"+K+\"*\\\\]\",\"g\"),T=new RegExp(O),U=new RegExp(\"^\"+M+\"$\"),V={ID:new RegExp(\"^#(\"+L+\")\"),CLASS:new RegExp(\"^\\\\.(\"+L+\")\"),TAG:new RegExp(\"^(\"+L.replace(\"w\",\"w*\")+\")\"),ATTR:new RegExp(\"^\"+N),PSEUDO:new RegExp(\"^\"+O),CHILD:new RegExp(\"^:(only|first|last|nth|nth-last)-(child|of-type)(?:\\\\(\"+K+\"*(even|odd|(([+-]|)(\\\\d*)n|)\"+K+\"*(?:([+-]|)\"+K+\"*(\\\\d+)|))\"+K+\"*\\\\)|)\",\"i\"),bool:new RegExp(\"^(?:\"+J+\")$\",\"i\"),needsContext:new RegExp(\"^\"+K+\"*[>+~]|:(even|odd|eq|gt|lt|nth|first|last)(?:\\\\(\"+K+\"*((?:-\\\\d)?\\\\d*)\"+K+\"*\\\\)|)(?=[^-]|$)\",\"i\")},W=/^(?:input|select|textarea|button)$/i,X=/^h\\d$/i,Y=/^[^{]+\\{\\s*\\[native \\w/,Z=/^(?:#([\\w-]+)|(\\w+)|\\.([\\w-]+))$/,$=/[+~]/,_=/'|\\\\/g,ab=new RegExp(\"\\\\\\\\([\\\\da-f]{1,6}\"+K+\"?|(\"+K+\")|.)\",\"ig\"),bb=function(a,b,c){var d=\"0x\"+b-65536;return d!==d||c?b:0>d?String.fromCharCode(d+65536):String.fromCharCode(d>>10|55296,1023&d|56320)};try{G.apply(D=H.call(t.childNodes),t.childNodes),D[t.childNodes.length].nodeType}catch(cb){G={apply:D.length?function(a,b){F.apply(a,H.call(b))}:function(a,b){var c=a.length,d=0;while(a[c++]=b[d++]);a.length=c-1}}}function db(a,b,d,e){var f,g,h,i,j,m,p,q,u,v;if((b?b.ownerDocument||b:t)!==l&&k(b),b=b||l,d=d||[],!a||\"string\"!=typeof a)return d;if(1!==(i=b.nodeType)&&9!==i)return[];if(n&&!e){if(f=Z.exec(a))if(h=f[1]){if(9===i){if(g=b.getElementById(h),!g||!g.parentNode)return d;if(g.id===h)return d.push(g),d}else if(b.ownerDocument&&(g=b.ownerDocument.getElementById(h))&&r(b,g)&&g.id===h)return d.push(g),d}else{if(f[2])return G.apply(d,b.getElementsByTagName(a)),d;if((h=f[3])&&c.getElementsByClassName&&b.getElementsByClassName)return G.apply(d,b.getElementsByClassName(h)),d}if(c.qsa&&(!o||!o.test(a))){if(q=p=s,u=b,v=9===i&&a,1===i&&\"object\"!==b.nodeName.toLowerCase()){m=ob(a),(p=b.getAttribute(\"id\"))?q=p.replace(_,\"\\\\$&\"):b.setAttribute(\"id\",q),q=\"[id='\"+q+\"'] \",j=m.length;while(j--)m[j]=q+pb(m[j]);u=$.test(a)&&mb(b.parentNode)||b,v=m.join(\",\")}if(v)try{return G.apply(d,u.querySelectorAll(v)),d}catch(w){}finally{p||b.removeAttribute(\"id\")}}}return xb(a.replace(P,\"$1\"),b,d,e)}function eb(){var a=[];function b(c,e){return a.push(c+\" \")>d.cacheLength&&delete b[a.shift()],b[c+\" \"]=e}return b}function fb(a){return a[s]=!0,a}function gb(a){var b=l.createElement(\"div\");try{return!!a(b)}catch(c){return!1}finally{b.parentNode&&b.parentNode.removeChild(b),b=null}}function hb(a,b){var c=a.split(\"|\"),e=a.length;while(e--)d.attrHandle[c[e]]=b}function ib(a,b){var c=b&&a,d=c&&1===a.nodeType&&1===b.nodeType&&(~b.sourceIndex||B)-(~a.sourceIndex||B);if(d)return d;if(c)while(c=c.nextSibling)if(c===b)return-1;return a?1:-1}function jb(a){return function(b){var c=b.nodeName.toLowerCase();return\"input\"===c&&b.type===a}}function kb(a){return function(b){var c=b.nodeName.toLowerCase();return(\"input\"===c||\"button\"===c)&&b.type===a}}function lb(a){return fb(function(b){return b=+b,fb(function(c,d){var e,f=a([],c.length,b),g=f.length;while(g--)c[e=f[g]]&&(c[e]=!(d[e]=c[e]))})})}function mb(a){return a&&typeof a.getElementsByTagName!==A&&a}c=db.support={},f=db.isXML=function(a){var b=a&&(a.ownerDocument||a).documentElement;return b?\"HTML\"!==b.nodeName:!1},k=db.setDocument=function(a){var b,e=a?a.ownerDocument||a:t,g=e.defaultView;return e!==l&&9===e.nodeType&&e.documentElement?(l=e,m=e.documentElement,n=!f(e),g&&g!==g.top&&(g.addEventListener?g.addEventListener(\"unload\",function(){k()},!1):g.attachEvent&&g.attachEvent(\"onunload\",function(){k()})),c.attributes=gb(function(a){return a.className=\"i\",!a.getAttribute(\"className\")}),c.getElementsByTagName=gb(function(a){return a.appendChild(e.createComment(\"\")),!a.getElementsByTagName(\"*\").length}),c.getElementsByClassName=Y.test(e.getElementsByClassName)&&gb(function(a){return a.innerHTML=\"<div class='a'></div><div class='a i'></div>\",a.firstChild.className=\"i\",2===a.getElementsByClassName(\"i\").length}),c.getById=gb(function(a){return m.appendChild(a).id=s,!e.getElementsByName||!e.getElementsByName(s).length}),c.getById?(d.find.ID=function(a,b){if(typeof b.getElementById!==A&&n){var c=b.getElementById(a);return c&&c.parentNode?[c]:[]}},d.filter.ID=function(a){var b=a.replace(ab,bb);return function(a){return a.getAttribute(\"id\")===b}}):(delete d.find.ID,d.filter.ID=function(a){var b=a.replace(ab,bb);return function(a){var c=typeof a.getAttributeNode!==A&&a.getAttributeNode(\"id\");return c&&c.value===b}}),d.find.TAG=c.getElementsByTagName?function(a,b){return typeof b.getElementsByTagName!==A?b.getElementsByTagName(a):void 0}:function(a,b){var c,d=[],e=0,f=b.getElementsByTagName(a);if(\"*\"===a){while(c=f[e++])1===c.nodeType&&d.push(c);return d}return f},d.find.CLASS=c.getElementsByClassName&&function(a"
,
",b){return typeof b.getElementsByClassName!==A&&n?b.getElementsByClassName(a):void 0},p=[],o=[],(c.qsa=Y.test(e.querySelectorAll))&&(gb(function(a){a.innerHTML=\"<select t=''><option selected=''></option></select>\",a.querySelectorAll(\"[t^='']\").length&&o.push(\"[*^$]=\"+K+\"*(?:''|\\\"\\\")\"),a.querySelectorAll(\"[selected]\").length||o.push(\"\\\\[\"+K+\"*(?:value|\"+J+\")\"),a.querySelectorAll(\":checked\").length||o.push(\":checked\")}),gb(function(a){var b=e.createElement(\"input\");b.setAttribute(\"type\",\"hidden\"),a.appendChild(b).setAttribute(\"name\",\"D\"),a.querySelectorAll(\"[name=d]\").length&&o.push(\"name\"+K+\"*[*^$|!~]?=\"),a.querySelectorAll(\":enabled\").length||o.push(\":enabled\",\":disabled\"),a.querySelectorAll(\"*,:x\"),o.push(\",.*:\")})),(c.matchesSelector=Y.test(q=m.webkitMatchesSelector||m.mozMatchesSelector||m.oMatchesSelector||m.msMatchesSelector))&&gb(function(a){c.disconnectedMatch=q.call(a,\"div\"),q.call(a,\"[s!='']:x\"),p.push(\"!=\",O)}),o=o.length&&new RegExp(o.join(\"|\")),p=p.length&&new RegExp(p.join(\"|\")),b=Y.test(m.compareDocumentPosition),r=b||Y.test(m.contains)?function(a,b){var c=9===a.nodeType?a.documentElement:a,d=b&&b.parentNode;return a===d||!(!d||1!==d.nodeType||!(c.contains?c.contains(d):a.compareDocumentPosition&&16&a.compareDocumentPosition(d)))}:function(a,b){if(b)while(b=b.parentNode)if(b===a)return!0;return!1},z=b?function(a,b){if(a===b)return j=!0,0;var d=!a.compareDocumentPosition-!b.compareDocumentPosition;return d?d:(d=(a.ownerDocument||a)===(b.ownerDocument||b)?a.compareDocumentPosition(b):1,1&d||!c.sortDetached&&b.compareDocumentPosition(a)===d?a===e||a.ownerDocument===t&&r(t,a)?-1:b===e||b.ownerDocument===t&&r(t,b)?1:i?I.call(i,a)-I.call(i,b):0:4&d?-1:1)}:function(a,b){if(a===b)return j=!0,0;var c,d=0,f=a.parentNode,g=b.parentNode,h=[a],k=[b];if(!f||!g)return a===e?-1:b===e?1:f?-1:g?1:i?I.call(i,a)-I.call(i,b):0;if(f===g)return ib(a,b);c=a;while(c=c.parentNode)h.unshift(c);c=b;while(c=c.parentNode)k.unshift(c);while(h[d]===k[d])d++;return d?ib(h[d],k[d]):h[d]===t?-1:k[d]===t?1:0},e):l},db.matches=function(a,b){return db(a,null,null,b)},db.matchesSelector=function(a,b){if((a.ownerDocument||a)!==l&&k(a),b=b.replace(S,\"='$1']\"),!(!c.matchesSelector||!n||p&&p.test(b)||o&&o.test(b)))try{var d=q.call(a,b);if(d||c.disconnectedMatch||a.document&&11!==a.document.nodeType)return d}catch(e){}return db(b,l,null,[a]).length>0},db.contains=function(a,b){return(a.ownerDocument||a)!==l&&k(a),r(a,b)},db.attr=function(a,b){(a.ownerDocument||a)!==l&&k(a);var e=d.attrHandle[b.toLowerCase()],f=e&&C.call(d.attrHandle,b.toLowerCase())?e(a,b,!n):void 0;return void 0!==f?f:c.attributes||!n?a.getAttribute(b):(f=a.getAttributeNode(b))&&f.specified?f.value:null},db.error=function(a){throw new Error(\"Syntax error, unrecognized expression: \"+a)},db.uniqueSort=function(a){var b,d=[],e=0,f=0;if(j=!c.detectDuplicates,i=!c.sortStable&&a.slice(0),a.sort(z),j){while(b=a[f++])b===a[f]&&(e=d.push(f));while(e--)a.splice(d[e],1)}return i=null,a},e=db.getText=function(a){var b,c=\"\",d=0,f=a.nodeType;if(f){if(1===f||9===f||11===f){if(\"string\"==typeof a.textContent)return a.textContent;for(a=a.firstChild;a;a=a.nextSibling)c+=e(a)}else if(3===f||4===f)return a.nodeValue}else while(b=a[d++])c+=e(b);return c},d=db.selectors={cacheLength:50,createPseudo:fb,match:V,attrHandle:{},find:{},relative:{\">\":{dir:\"parentNode\",first:!0},\" \":{dir:\"parentNode\"},\"+\":{dir:\"previousSibling\",first:!0},\"~\":{dir:\"previousSibling\"}},preFilter:{ATTR:function(a){return a[1]=a[1].replace(ab,bb),a[3]=(a[4]||a[5]||\"\").replace(ab,bb),\"~=\"===a[2]&&(a[3]=\" \"+a[3]+\" \"),a.slice(0,4)},CHILD:function(a){return a[1]=a[1].toLowerCase(),\"nth\"===a[1].slice(0,3)?(a[3]||db.error(a[0]),a[4]=+(a[4]?a[5]+(a[6]||1):2*(\"even\"===a[3]||\"odd\"===a[3])),a[5]=+(a[7]+a[8]||\"odd\"===a[3])):a[3]&&db.error(a[0]),a},PSEUDO:function(a){var b,c=!a[5]&&a[2];return V.CHILD.test(a[0])?null:(a[3]&&void 0!==a[4]?a[2]=a[4]:c&&T.test(c)&&(b=ob(c,!0))&&(b=c.indexOf(\")\",c.length-b)-c.length)&&(a[0]=a[0].slice(0,b),a[2]=c.slice(0,b)),a.slice(0,3))}},filter:{TAG:function(a){var b=a.replace(ab,bb).toLowerCase();return\"*\"===a?function(){return!0}:function(a){return a.nodeName&&a.nodeName.toLowerCase()===b}},CLASS:function(a){var b=w[a+\" \"];return b||(b=new RegExp(\"(^|\"+K+\")\"+a+\"(\"+K+\"|$)\"))&&w(a,function(a){return b.test(\"string\"==typeof a.className&&a.className||typeof a.getAttribute!==A&&a.getAttribute(\"class\")||\"\")})},ATTR:function(a,b,c){return function(d){var e=db.attr(d,a);return null==e?\"!=\"===b:b?(e+=\"\",\"=\"===b?e===c:\"!=\"===b?e!==c:\"^=\"===b?c&&0===e.indexOf(c):\"*=\"===b?c&&e.indexOf(c)>-1:\"$=\"===b?c&&e.slice(-c.length)===c:\"~=\"===b?(\" \"+e+\" \").indexOf(c)>-1:\"|=\"===b?e===c||e.slice(0,c.length+1)===c+\"-\":!1):!0}},CHILD:function(a,b,c,d,e){var f=\"nth\"!==a.slice(0,3),g=\"last\"!==a.slice(-4),h=\"of-type\"===b;return 1===d&&0===e?function(a){return!!a.parentNode}:function(b,c,i){var j,k,l,m,n,o,p=f!==g?\"nextSibling\":\"previousSibling\",q=b.parentNode,r=h&&b.nodeName.toLowerCase(),t=!i&&!h;if(q){if(f){while(p){l=b;while(l=l[p])if(h?l.nodeName.toLowerCase()===r:1===l.nodeType)return!1;o=p=\"only\"===a&&!o&&\"nextSibling\"}return!0}if(o=[g?q.firstChild:q.lastChild],g&&t){k=q[s]||(q[s]={}),j=k[a]||[],n=j[0]===u&&j[1],m=j[0]===u&&j[2],l=n&&q.childNodes[n];while(l=++n&&l&&l[p]||(m=n=0)||o.pop())if(1===l.nodeType&&++m&&l===b){k[a]=[u,n,m];break}}else if(t&&(j=(b[s]||(b[s]={}))[a])&&j[0]===u)m=j[1];else while(l=++n&&l&&l[p]||(m=n=0)||o.pop())if((h?l.nodeName.toLowerCase()===r:1===l.nodeType)&&++m&&(t&&((l[s]||(l[s]={}))[a]=[u,m]),l===b))break;return m-=e,m===d||m%d===0&&m/d>=0}}},PSEUDO:function(a,b){var c,e=d.pseudos[a]||d.setFilters[a.toLowerCase()]||db.error(\"unsupported pseudo: \"+a);return e[s]?e(b):e.length>1?(c=[a,a,\"\",b],d.setFilters.hasOwnProperty(a.toLowerCase())?fb(function(a,c){var d,f=e(a,b),g=f.length;while(g--)d=I.call(a,f[g]),a[d]=!(c[d]=f[g])}):function(a){return e(a,0,c)}):e}},pseudos:{not:fb(function(a){var b=[],c=[],d=g(a.replace(P,\"$1\"));return d[s]?fb(function(a,b,c,e){var f,g=d(a,null,e,[]),h=a.length;while(h--)(f=g[h])&&(a[h]=!(b[h]=f))}):function(a,e,f){return b[0]=a,d(b,null,f,c),!c.pop()}}),has:fb(function(a){return function(b){return db(a,b).length>0}}),contains:fb(function(a){return function(b){return(b.textContent||b.innerText||e(b)).indexOf(a)>-1}}),lang:fb(function(a){return U.test(a||\"\")||db.error(\"unsupported lang: \"+a),a=a.replace(ab,bb).toLowerCase(),function(b){var c;do if(c=n?b.lang:b.getAttribute(\"xml:lang\")||b.getAttribute(\"lang\"))return c=c.toLowerCase(),c===a||0===c.indexOf(a+\"-\");while((b=b.parentNode)&&1===b.nodeType);return!1}}),target:function(b){var c=a.location&&a.location.hash;return c&&c.slice(1)===b.id},root:function(a){return a===m},focus:function(a){return a===l.activeElement&&(!l.hasFocus||l.hasFocus())&&!!(a.type||a.href||~a.tabIndex)},enabled:function(a){return a.disabled===!1},disabled:function(a){return a.disabled===!0},checked:function(a){var b=a.nodeName.toLowerCase();return\"input\"===b&&!!a.checked||\"option\"===b&&!!a.selected},selected:function(a){return a.parentNode&&a.parentNode.selectedIndex,a.selected===!0},empty:function(a){for(a=a.firstChild;a;a=a.nextSibling)if(a.nodeType<6)return!1;return!0},parent:function(a){return!d.pseudos.empty(a)},header:function(a){return X.test(a.nodeName)},input:function(a){return W.test(a.nodeName)},button:function(a){var b=a.nodeName.toLowerCase();return\"input\"===b&&\"button\"===a.type||\"button\"===b},text:function(a){var b;return\"input\"===a.nodeName.toLowerCase()&&\"text\"===a.type&&(null==(b=a.getAttribute(\"type\"))||\"text\"===b.toLowerCase())},first:lb(function(){return[0]}),last:lb(function(a,b){return[b-1]}),eq:lb(function(a,b,c){return[0>c?c+b:c]}),even:lb(function(a,b){for(var c=0;b>c;c+=2)a.push(c);return a}),odd:lb(function(a,b){for(var c=1;b>c;c+=2)a.push(c);return a}),lt:lb(function(a,b,c){for(var d=0>c?c+b:c;--d>=0;)a.push(d);return a}),gt:lb(function(a,b,c){for(var d=0>c?c+b:c;++d<b;)a.push(d);return a})}},d.pseudos.nth=d.pseudos.eq;for(b in{radio:!0,checkbox:!0,file:!0,password:!0,image:!0})d.pseudos[b]=jb(b);for(b in{submit:!0,reset:!0})d.pseudos[b]=kb(b);function nb(){}nb.prototype=d.filters=d.pseudos,d.setFilters=new nb;function ob(a,b){var c,e,f,g,h,i,j,k=x[a+\" \"];if(k)return b?0:k.slice(0);h=a,i=[],j=d.preFilter;while(h){(!c||(e=Q.exec(h)))&&(e&&(h=h.slice(e[0].length)||h),i.push(f=[])),c=!1,(e=R.exec(h))&&(c=e.shift(),f.push({value:c,type:e[0].replace(P,\" \")}),h=h.slice(c.length));for(g in d.filter)!(e=V[g].exec(h))||j[g]&&!(e=j[g](e))||(c=e.shift(),f.push({value:c,type:g,matches:e}),h=h.slice(c.length));if(!c)break}return b?h.length:h?db.error(a):x(a,i).slice(0)}function pb(a){for(var b=0,c=a.length,d=\"\";c>b;b++)d+=a[b].value;return d}function qb(a,b,c){var d=b.dir,e=c&&\"parentNode\"===d,f=v++;return b.first?function(b,c,f){while(b=b[d])if(1===b.nodeType||e)return a(b,c,f)}:function(b,c,g){var h,i,j=[u,f];if(g){while(b=b[d])if((1===b.nodeType||e)&&a(b,c,g))return!0}else while(b=b[d])if(1===b.nodeType||e){if(i=b[s]||(b[s]={}),(h=i[d])&&h[0]===u&&h[1]===f)return j[2]=h[2];if(i[d]=j,j[2]=a(b,c,g))return!0}}}function rb(a){return a.length>1?function(b,c,d){var e=a.length;while(e--)if(!a[e](b,c,d))return!1;return!0}:a[0]}function sb(a,b,c,d,e){for(var f,g=[],h=0,i=a.length,j=null!=b;i>h;h++)(f=a[h])&&(!c||c(f,d,e))&&(g.push(f),j&&b.push(h));return g}function tb(a,b,c,d,e,f){return d&&!d[s]&&(d=tb(d)),e&&!e[s]&&(e=tb(e,f)),fb(function(f,g,h,i){var j,k,l,m=[],n=[],o=g.length,p=f||wb(b||\"*\",h.nodeType?[h]:h,[]),q=!a||!f&&b?p:sb(p,m,a,h,i),r=c?e||(f?a:o||d)?[]:g:q;if(c&&c(q,r,h,i),d){j=sb(r,n),d(j,[],h,i),k=j.length;while(k--)(l=j[k])&&(r[n[k]]=!(q[n[k]]=l))}if(f){if(e||a){if(e){j=[],k=r.length;while(k--)(l=r[k])&&j.push(q[k]=l);e(null,r=[],j,i)}k=r.length;while(k--)(l=r[k])&&(j=e?I.call(f,l):m[k])>-1&&(f[j]=!(g[j]=l))}}else r=sb(r===g?r.splice(o,r.length):r),e?e(null,g,r,i):G.apply(g,r)})}function ub(a){for(var b,c,e,f=a.length,g=d.relative[a[0].type],i=g||d.relative[\" \"],j=g?1:0,k=qb(function(a){return a===b},i,!0),l=qb(function(a){"
,
"return I.call(b,a)>-1},i,!0),m=[function(a,c,d){return!g&&(d||c!==h)||((b=c).nodeType?k(a,c,d):l(a,c,d))}];f>j;j++)if(c=d.relative[a[j].type])m=[qb(rb(m),c)];else{if(c=d.filter[a[j].type].apply(null,a[j].matches),c[s]){for(e=++j;f>e;e++)if(d.relative[a[e].type])break;return tb(j>1&&rb(m),j>1&&pb(a.slice(0,j-1).concat({value:\" \"===a[j-2].type?\"*\":\"\"})).replace(P,\"$1\"),c,e>j&&ub(a.slice(j,e)),f>e&&ub(a=a.slice(e)),f>e&&pb(a))}m.push(c)}return rb(m)}function vb(a,b){var c=b.length>0,e=a.length>0,f=function(f,g,i,j,k){var m,n,o,p=0,q=\"0\",r=f&&[],s=[],t=h,v=f||e&&d.find.TAG(\"*\",k),w=u+=null==t?1:Math.random()||.1,x=v.length;for(k&&(h=g!==l&&g);q!==x&&null!=(m=v[q]);q++){if(e&&m){n=0;while(o=a[n++])if(o(m,g,i)){j.push(m);break}k&&(u=w)}c&&((m=!o&&m)&&p--,f&&r.push(m))}if(p+=q,c&&q!==p){n=0;while(o=b[n++])o(r,s,g,i);if(f){if(p>0)while(q--)r[q]||s[q]||(s[q]=E.call(j));s=sb(s)}G.apply(j,s),k&&!f&&s.length>0&&p+b.length>1&&db.uniqueSort(j)}return k&&(u=w,h=t),r};return c?fb(f):f}g=db.compile=function(a,b){var c,d=[],e=[],f=y[a+\" \"];if(!f){b||(b=ob(a)),c=b.length;while(c--)f=ub(b[c]),f[s]?d.push(f):e.push(f);f=y(a,vb(e,d))}return f};function wb(a,b,c){for(var d=0,e=b.length;e>d;d++)db(a,b[d],c);return c}function xb(a,b,e,f){var h,i,j,k,l,m=ob(a);if(!f&&1===m.length){if(i=m[0]=m[0].slice(0),i.length>2&&\"ID\"===(j=i[0]).type&&c.getById&&9===b.nodeType&&n&&d.relative[i[1].type]){if(b=(d.find.ID(j.matches[0].replace(ab,bb),b)||[])[0],!b)return e;a=a.slice(i.shift().value.length)}h=V.needsContext.test(a)?0:i.length;while(h--){if(j=i[h],d.relative[k=j.type])break;if((l=d.find[k])&&(f=l(j.matches[0].replace(ab,bb),$.test(i[0].type)&&mb(b.parentNode)||b))){if(i.splice(h,1),a=f.length&&pb(i),!a)return G.apply(e,f),e;break}}}return g(a,m)(f,b,!n,e,$.test(a)&&mb(b.parentNode)||b),e}return c.sortStable=s.split(\"\").sort(z).join(\"\")===s,c.detectDuplicates=!!j,k(),c.sortDetached=gb(function(a){return 1&a.compareDocumentPosition(l.createElement(\"div\"))}),gb(function(a){return a.innerHTML=\"<a href='#'></a>\",\"#\"===a.firstChild.getAttribute(\"href\")})||hb(\"type|href|height|width\",function(a,b,c){return c?void 0:a.getAttribute(b,\"type\"===b.toLowerCase()?1:2)}),c.attributes&&gb(function(a){return a.innerHTML=\"<input/>\",a.firstChild.setAttribute(\"value\",\"\"),\"\"===a.firstChild.getAttribute(\"value\")})||hb(\"value\",function(a,b,c){return c||\"input\"!==a.nodeName.toLowerCase()?void 0:a.defaultValue}),gb(function(a){return null==a.getAttribute(\"disabled\")})||hb(J,function(a,b,c){var d;return c?void 0:a[b]===!0?b.toLowerCase():(d=a.getAttributeNode(b))&&d.specified?d.value:null}),db}(a);n.find=t,n.expr=t.selectors,n.expr[\":\"]=n.expr.pseudos,n.unique=t.uniqueSort,n.text=t.getText,n.isXMLDoc=t.isXML,n.contains=t.contains;var u=n.expr.match.needsContext,v=/^<(\\w+)\\s*\\/?>(?:<\\/\\1>|)$/,w=/^.[^:#\\[\\.,]*$/;function x(a,b,c){if(n.isFunction(b))return n.grep(a,function(a,d){return!!b.call(a,d,a)!==c});if(b.nodeType)return n.grep(a,function(a){return a===b!==c});if(\"string\"==typeof b){if(w.test(b))return n.filter(b,a,c);b=n.filter(b,a)}return n.grep(a,function(a){return n.inArray(a,b)>=0!==c})}n.filter=function(a,b,c){var d=b[0];return c&&(a=\":not(\"+a+\")\"),1===b.length&&1===d.nodeType?n.find.matchesSelector(d,a)?[d]:[]:n.find.matches(a,n.grep(b,function(a){return 1===a.nodeType}))},n.fn.extend({find:function(a){var b,c=[],d=this,e=d.length;if(\"string\"!=typeof a)return this.pushStack(n(a).filter(function(){for(b=0;e>b;b++)if(n.contains(d[b],this))return!0}));for(b=0;e>b;b++)n.find(a,d[b],c);return c=this.pushStack(e>1?n.unique(c):c),c.selector=this.selector?this.selector+\" \"+a:a,c},filter:function(a){return this.pushStack(x(this,a||[],!1))},not:function(a){return this.pushStack(x(this,a||[],!0))},is:function(a){return!!x(this,\"string\"==typeof a&&u.test(a)?n(a):a||[],!1).length}});var y,z=a.document,A=/^(?:\\s*(<[\\w\\W]+>)[^>]*|#([\\w-]*))$/,B=n.fn.init=function(a,b){var c,d;if(!a)return this;if(\"string\"==typeof a){if(c=\"<\"===a.charAt(0)&&\">\"===a.charAt(a.length-1)&&a.length>=3?[null,a,null]:A.exec(a),!c||!c[1]&&b)return!b||b.jquery?(b||y).find(a):this.constructor(b).find(a);if(c[1]){if(b=b instanceof n?b[0]:b,n.merge(this,n.parseHTML(c[1],b&&b.nodeType?b.ownerDocument||b:z,!0)),v.test(c[1])&&n.isPlainObject(b))for(c in b)n.isFunction(this[c])?this[c](b[c]):this.attr(c,b[c]);return this}if(d=z.getElementById(c[2]),d&&d.parentNode){if(d.id!==c[2])return y.find(a);this.length=1,this[0]=d}return this.context=z,this.selector=a,this}return a.nodeType?(this.context=this[0]=a,this.length=1,this):n.isFunction(a)?\"undefined\"!=typeof y.ready?y.ready(a):a(n):(void 0!==a.selector&&(this.selector=a.selector,this.context=a.context),n.makeArray(a,this))};B.prototype=n.fn,y=n(z);var C=/^(?:parents|prev(?:Until|All))/,D={children:!0,contents:!0,next:!0,prev:!0};n.extend({dir:function(a,b,c){var d=[],e=a[b];while(e&&9!==e.nodeType&&(void 0===c||1!==e.nodeType||!n(e).is(c)))1===e.nodeType&&d.push(e),e=e[b];return d},sibling:function(a,b){for(var c=[];a;a=a.nextSibling)1===a.nodeType&&a!==b&&c.push(a);return c}}),n.fn.extend({has:function(a){var b,c=n(a,this),d=c.length;return this.filter(function(){for(b=0;d>b;b++)if(n.contains(this,c[b]))return!0})},closest:function(a,b){for(var c,d=0,e=this.length,f=[],g=u.test(a)||\"string\"!=typeof a?n(a,b||this.context):0;e>d;d++)for(c=this[d];c&&c!==b;c=c.parentNode)if(c.nodeType<11&&(g?g.index(c)>-1:1===c.nodeType&&n.find.matchesSelector(c,a))){f.push(c);break}return this.pushStack(f.length>1?n.unique(f):f)},index:function(a){return a?\"string\"==typeof a?n.inArray(this[0],n(a)):n.inArray(a.jquery?a[0]:a,this):this[0]&&this[0].parentNode?this.first().prevAll().length:-1},add:function(a,b){return this.pushStack(n.unique(n.merge(this.get(),n(a,b))))},addBack:function(a){return this.add(null==a?this.prevObject:this.prevObject.filter(a))}});function E(a,b){do a=a[b];while(a&&1!==a.nodeType);return a}n.each({parent:function(a){var b=a.parentNode;return b&&11!==b.nodeType?b:null},parents:function(a){return n.dir(a,\"parentNode\")},parentsUntil:function(a,b,c){return n.dir(a,\"parentNode\",c)},next:function(a){return E(a,\"nextSibling\")},prev:function(a){return E(a,\"previousSibling\")},nextAll:function(a){return n.dir(a,\"nextSibling\")},prevAll:function(a){return n.dir(a,\"previousSibling\")},nextUntil:function(a,b,c){return n.dir(a,\"nextSibling\",c)},prevUntil:function(a,b,c){return n.dir(a,\"previousSibling\",c)},siblings:function(a){return n.sibling((a.parentNode||{}).firstChild,a)},children:function(a){return n.sibling(a.firstChild)},contents:function(a){return n.nodeName(a,\"iframe\")?a.contentDocument||a.contentWindow.document:n.merge([],a.childNodes)}},function(a,b){n.fn[a]=function(c,d){var e=n.map(this,b,c);return\"Until\"!==a.slice(-5)&&(d=c),d&&\"string\"==typeof d&&(e=n.filter(d,e)),this.length>1&&(D[a]||(e=n.unique(e)),C.test(a)&&(e=e.reverse())),this.pushStack(e)}});var F=/\\S+/g,G={};function H(a){var b=G[a]={};return n.each(a.match(F)||[],function(a,c){b[c]=!0}),b}n.Callbacks=function(a){a=\"string\"==typeof a?G[a]||H(a):n.extend({},a);var b,c,d,e,f,g,h=[],i=!a.once&&[],j=function(l){for(c=a.memory&&l,d=!0,f=g||0,g=0,e=h.length,b=!0;h&&e>f;f++)if(h[f].apply(l[0],l[1])===!1&&a.stopOnFalse){c=!1;break}b=!1,h&&(i?i.length&&j(i.shift()):c?h=[]:k.disable())},k={add:function(){if(h){var d=h.length;!function f(b){n.each(b,function(b,c){var d=n.type(c);\"function\"===d?a.unique&&k.has(c)||h.push(c):c&&c.length&&\"string\"!==d&&f(c)})}(arguments),b?e=h.length:c&&(g=d,j(c))}return this},remove:function(){return h&&n.each(arguments,function(a,c){var d;while((d=n.inArray(c,h,d))>-1)h.splice(d,1),b&&(e>=d&&e--,f>=d&&f--)}),this},has:function(a){return a?n.inArray(a,h)>-1:!(!h||!h.length)},empty:function(){return h=[],e=0,this},disable:function(){return h=i=c=void 0,this},disabled:function(){return!h},lock:function(){return i=void 0,c||k.disable(),this},locked:function(){return!i},fireWith:function(a,c){return!h||d&&!i||(c=c||[],c=[a,c.slice?c.slice():c],b?i.push(c):j(c)),this},fire:function(){return k.fireWith(this,arguments),this},fired:function(){return!!d}};return k},n.extend({Deferred:function(a){var b=[[\"resolve\",\"done\",n.Callbacks(\"once memory\"),\"resolved\"],[\"reject\",\"fail\",n.Callbacks(\"once memory\"),\"rejected\"],[\"notify\",\"progress\",n.Callbacks(\"memory\")]],c=\"pending\",d={state:function(){return c},always:function(){return e.done(arguments).fail(arguments),this},then:function(){var a=arguments;return n.Deferred(function(c){n.each(b,function(b,f){var g=n.isFunction(a[b])&&a[b];e[f[1]](function(){var a=g&&g.apply(this,arguments);a&&n.isFunction(a.promise)?a.promise().done(c.resolve).fail(c.reject).progress(c.notify):c[f[0]+\"With\"](this===d?c.promise():this,g?[a]:arguments)})}),a=null}).promise()},promise:function(a){return null!=a?n.extend(a,d):d}},e={};return d.pipe=d.then,n.each(b,function(a,f){var g=f[2],h=f[3];d[f[1]]=g.add,h&&g.add(function(){c=h},b[1^a][2].disable,b[2][2].lock),e[f[0]]=function(){return e[f[0]+\"With\"](this===e?d:this,arguments),this},e[f[0]+\"With\"]=g.fireWith}),d.promise(e),a&&a.call(e,e),e},when:function(a){var b=0,c=d.call(arguments),e=c.length,f=1!==e||a&&n.isFunction(a.promise)?e:0,g=1===f?a:n.Deferred(),h=function(a,b,c){return function(e){b[a]=this,c[a]=arguments.length>1?d.call(arguments):e,c===i?g.notifyWith(b,c):--f||g.resolveWith(b,c)}},i,j,k;if(e>1)for(i=new Array(e),j=new Array(e),k=new Array(e);e>b;b++)c[b]&&n.isFunction(c[b].promise)?c[b].promise().done(h(b,k,c)).fail(g.reject).progress(h(b,j,i)):--f;return f||g.resolveWith(k,c),g.promise()}});var I;n.fn.ready=function(a){return n.ready.promise().done(a),this},n.extend({isReady:!1,readyWait:1,holdReady:function(a){a?n.readyWait++:n.ready(!0)},ready:function(a){if(a===!0?!--n.readyWait:!n.isReady){if(!z.body)return setTimeout(n.ready);n.isReady=!0,a!==!0&&--n.readyWait>0||(I.resolveWith(z,[n]),n.fn.trigger&&n(z).trigger(\"ready\").off(\"ready\"))}}});function J(){z.addEventListener?(z.removeEventListener(\"DOMContentLoaded\",K,!1),a.removeEventListener(\"load\",K,!1)):(z.detachEvent(\"onreadystatec"
,
"hange\",K),a.detachEvent(\"onload\",K))}function K(){(z.addEventListener||\"load\"===event.type||\"complete\"===z.readyState)&&(J(),n.ready())}n.ready.promise=function(b){if(!I)if(I=n.Deferred(),\"complete\"===z.readyState)setTimeout(n.ready);else if(z.addEventListener)z.addEventListener(\"DOMContentLoaded\",K,!1),a.addEventListener(\"load\",K,!1);else{z.attachEvent(\"onreadystatechange\",K),a.attachEvent(\"onload\",K);var c=!1;try{c=null==a.frameElement&&z.documentElement}catch(d){}c&&c.doScroll&&!function e(){if(!n.isReady){try{c.doScroll(\"left\")}catch(a){return setTimeout(e,50)}J(),n.ready()}}()}return I.promise(b)};var L=\"undefined\",M;for(M in n(l))break;l.ownLast=\"0\"!==M,l.inlineBlockNeedsLayout=!1,n(function(){var a,b,c=z.getElementsByTagName(\"body\")[0];c&&(a=z.createElement(\"div\"),a.style.cssText=\"border:0;width:0;height:0;position:absolute;top:0;left:-9999px;margin-top:1px\",b=z.createElement(\"div\"),c.appendChild(a).appendChild(b),typeof b.style.zoom!==L&&(b.style.cssText=\"border:0;margin:0;width:1px;padding:1px;display:inline;zoom:1\",(l.inlineBlockNeedsLayout=3===b.offsetWidth)&&(c.style.zoom=1)),c.removeChild(a),a=b=null)}),function(){var a=z.createElement(\"div\");if(null==l.deleteExpando){l.deleteExpando=!0;try{delete a.test}catch(b){l.deleteExpando=!1}}a=null}(),n.acceptData=function(a){var b=n.noData[(a.nodeName+\" \").toLowerCase()],c=+a.nodeType||1;return 1!==c&&9!==c?!1:!b||b!==!0&&a.getAttribute(\"classid\")===b};var N=/^(?:\\{[\\w\\W]*\\}|\\[[\\w\\W]*\\])$/,O=/([A-Z])/g;function P(a,b,c){if(void 0===c&&1===a.nodeType){var d=\"data-\"+b.replace(O,\"-$1\").toLowerCase();if(c=a.getAttribute(d),\"string\"==typeof c){try{c=\"true\"===c?!0:\"false\"===c?!1:\"null\"===c?null:+c+\"\"===c?+c:N.test(c)?n.parseJSON(c):c}catch(e){}n.data(a,b,c)}else c=void 0}return c}function Q(a){var b;for(b in a)if((\"data\"!==b||!n.isEmptyObject(a[b]))&&\"toJSON\"!==b)return!1;return!0}function R(a,b,d,e){if(n.acceptData(a)){var f,g,h=n.expando,i=a.nodeType,j=i?n.cache:a,k=i?a[h]:a[h]&&h;if(k&&j[k]&&(e||j[k].data)||void 0!==d||\"string\"!=typeof b)return k||(k=i?a[h]=c.pop()||n.guid++:h),j[k]||(j[k]=i?{}:{toJSON:n.noop}),(\"object\"==typeof b||\"function\"==typeof b)&&(e?j[k]=n.extend(j[k],b):j[k].data=n.extend(j[k].data,b)),g=j[k],e||(g.data||(g.data={}),g=g.data),void 0!==d&&(g[n.camelCase(b)]=d),\"string\"==typeof b?(f=g[b],null==f&&(f=g[n.camelCase(b)])):f=g,f\n"
,
"}}function S(a,b,c){if(n.acceptData(a)){var d,e,f=a.nodeType,g=f?n.cache:a,h=f?a[n.expando]:n.expando;if(g[h]){if(b&&(d=c?g[h]:g[h].data)){n.isArray(b)?b=b.concat(n.map(b,n.camelCase)):b in d?b=[b]:(b=n.camelCase(b),b=b in d?[b]:b.split(\" \")),e=b.length;while(e--)delete d[b[e]];if(c?!Q(d):!n.isEmptyObject(d))return}(c||(delete g[h].data,Q(g[h])))&&(f?n.cleanData([a],!0):l.deleteExpando||g!=g.window?delete g[h]:g[h]=null)}}}n.extend({cache:{},noData:{\"applet \":!0,\"embed \":!0,\"object \":\"clsid:D27CDB6E-AE6D-11cf-96B8-444553540000\"},hasData:function(a){return a=a.nodeType?n.cache[a[n.expando]]:a[n.expando],!!a&&!Q(a)},data:function(a,b,c){return R(a,b,c)},removeData:function(a,b){return S(a,b)},_data:function(a,b,c){return R(a,b,c,!0)},_removeData:function(a,b){return S(a,b,!0)}}),n.fn.extend({data:function(a,b){var c,d,e,f=this[0],g=f&&f.attributes;if(void 0===a){if(this.length&&(e=n.data(f),1===f.nodeType&&!n._data(f,\"parsedAttrs\"))){c=g.length;while(c--)d=g[c].name,0===d.indexOf(\"data-\")&&(d=n.camelCase(d.slice(5)),P(f,d,e[d]));n._data(f,\"parsedAttrs\",!0)}return e}return\"object\"==typeof a?this.each(function(){n.data(this,a)}):arguments.length>1?this.each(function(){n.data(this,a,b)}):f?P(f,a,n.data(f,a)):void 0},removeData:function(a){return this.each(function(){n.removeData(this,a)})}}),n.extend({queue:function(a,b,c){var d;return a?(b=(b||\"fx\")+\"queue\",d=n._data(a,b),c&&(!d||n.isArray(c)?d=n._data(a,b,n.makeArray(c)):d.push(c)),d||[]):void 0},dequeue:function(a,b){b=b||\"fx\";var c=n.queue(a,b),d=c.length,e=c.shift(),f=n._queueHooks(a,b),g=function(){n.dequeue(a,b)};\"inprogress\"===e&&(e=c.shift(),d--),e&&(\"fx\"===b&&c.unshift(\"inprogress\"),delete f.stop,e.call(a,g,f)),!d&&f&&f.empty.fire()},_queueHooks:function(a,b){var c=b+\"queueHooks\";return n._data(a,c)||n._data(a,c,{empty:n.Callbacks(\"once memory\").add(function(){n._removeData(a,b+\"queue\"),n._removeData(a,c)})})}}),n.fn.extend({queue:function(a,b){var c=2;return\"string\"!=typeof a&&(b=a,a=\"fx\",c--),arguments.length<c?n.queue(this[0],a):void 0===b?this:this.each(function(){var c=n.queue(this,a,b);n._queueHooks(this,a),\"fx\"===a&&\"inprogress\"!==c[0]&&n.dequeue(this,a)})},dequeue:function(a){return this.each(function(){n.dequeue(this,a)})},clearQueue:function(a){return this.queue(a||\"fx\",[])},promise:function(a,b){var c,d=1,e=n.Deferred(),f=this,g=this.length,h=function(){--d||e.resolveWith(f,[f])};\"string\"!=typeof a&&(b=a,a=void 0),a=a||\"fx\";while(g--)c=n._data(f[g],a+\"queueHooks\"),c&&c.empty&&(d++,c.empty.add(h));return h(),e.promise(b)}});var T=/[+-]?(?:\\d*\\.|)\\d+(?:[eE][+-]?\\d+|)/.source,U=[\"Top\",\"Right\",\"Bottom\",\"Left\"],V=function(a,b){return a=b||a,\"none\"===n.css(a,\"display\")||!n.contains(a.ownerDocument,a)},W=n.access=function(a,b,c,d,e,f,g){var h=0,i=a.length,j=null==c;if(\"object\"===n.type(c)){e=!0;for(h in c)n.access(a,b,h,c[h],!0,f,g)}else if(void 0!==d&&(e=!0,n.isFunction(d)||(g=!0),j&&(g?(b.call(a,d),b=null):(j=b,b=function(a,b,c){return j.call(n(a),c)})),b))for(;i>h;h++)b(a[h],c,g?d:d.call(a[h],h,b(a[h],c)));return e?a:j?b.call(a):i?b(a[0],c):f},X=/^(?:checkbox|radio)$/i;!function(){var a=z.createDocumentFragment(),b=z.createElement(\"div\"),c=z.createElement(\"input\");if(b.setAttribute(\"className\",\"t\"),b.innerHTML=\"  <link/><table></table><a href='/a'>a</a>\",l.leadingWhitespace=3===b.firstChild.nodeType,l.tbody=!b.getElementsByTagName(\"tbody\").length,l.htmlSerialize=!!b.getElementsByTagName(\"link\").length,l.html5Clone=\"<:nav></:nav>\"!==z.createElement(\"nav\").cloneNode(!0).outerHTML,c.type=\"checkbox\",c.checked=!0,a.appendChild(c),l.appendChecked=c.checked,b.innerHTML=\"<textarea>x</textarea>\",l.noCloneChecked=!!b.cloneNode(!0).lastChild.defaultValue,a.appendChild(b),b.innerHTML=\"<input type='radio' checked='checked' name='t'/>\",l.checkClone=b.cloneNode(!0).cloneNode(!0).lastChild.checked,l.noCloneEvent=!0,b.attachEvent&&(b.attachEvent(\"onclick\",function(){l.noCloneEvent=!1}),b.cloneNode(!0).click()),null==l.deleteExpando){l.deleteExpando=!0;try{delete b.test}catch(d){l.deleteExpando=!1}}a=b=c=null}(),function(){var b,c,d=z.createElement(\"div\");for(b in{submit:!0,change:!0,focusin:!0})c=\"on\"+b,(l[b+\"Bubbles\"]=c in a)||(d.setAttribute(c,\"t\"),l[b+\"Bubbles\"]=d.attributes[c].expando===!1);d=null}();var Y=/^(?:input|select|textarea)$/i,Z=/^key/,$=/^(?:mouse|contextmenu)|click/,_=/^(?:focusinfocus|focusoutblur)$/,ab=/^([^.]*)(?:\\.(.+)|)$/;function bb(){return!0}function cb(){return!1}function db(){try{return z.activeElement}catch(a){}}n.event={global:{},add:function(a,b,c,d,e){var f,g,h,i,j,k,l,m,o,p,q,r=n._data(a);if(r){c.handler&&(i=c,c=i.handler,e=i.selector),c.guid||(c.guid=n.guid++),(g=r.events)||(g=r.events={}),(k=r.handle)||(k=r.handle=function(a){return typeof n===L||a&&n.event.triggered===a.type?void 0:n.event.dispatch.apply(k.elem,arguments)},k.elem=a),b=(b||\"\").match(F)||[\"\"],h=b.length;while(h--)f=ab.exec(b[h])||[],o=q=f[1],p=(f[2]||\"\").split(\".\").sort(),o&&(j=n.event.special[o]||{},o=(e?j.delegateType:j.bindType)||o,j=n.event.special[o]||{},l=n.extend({type:o,origType:q,data:d,handler:c,guid:c.guid,selector:e,needsContext:e&&n.expr.match.needsContext.test(e),namespace:p.join(\".\")},i),(m=g[o])||(m=g[o]=[],m.delegateCount=0,j.setup&&j.setup.call(a,d,p,k)!==!1||(a.addEventListener?a.addEventListener(o,k,!1):a.attachEvent&&a.attachEvent(\"on\"+o,k))),j.add&&(j.add.call(a,l),l.handler.guid||(l.handler.guid=c.guid)),e?m.splice(m.delegateCount++,0,l):m.push(l),n.event.global[o]=!0);a=null}},remove:function(a,b,c,d,e){var f,g,h,i,j,k,l,m,o,p,q,r=n.hasData(a)&&n._data(a);if(r&&(k=r.events)){b=(b||\"\").match(F)||[\"\"],j=b.length;while(j--)if(h=ab.exec(b[j])||[],o=q=h[1],p=(h[2]||\"\").split(\".\").sort(),o){l=n.event.special[o]||{},o=(d?l.delegateType:l.bindType)||o,m=k[o]||[],h=h[2]&&new RegExp(\"(^|\\\\.)\"+p.join(\"\\\\.(?:.*\\\\.|)\")+\"(\\\\.|$)\"),i=f=m.length;while(f--)g=m[f],!e&&q!==g.origType||c&&c.guid!==g.guid||h&&!h.test(g.namespace)||d&&d!==g.selector&&(\"**\"!==d||!g.selector)||(m.splice(f,1),g.selector&&m.delegateCount--,l.remove&&l.remove.call(a,g));i&&!m.length&&(l.teardown&&l.teardown.call(a,p,r.handle)!==!1||n.removeEvent(a,o,r.handle),delete k[o])}else for(o in k)n.event.remove(a,o+b[j],c,d,!0);n.isEmptyObject(k)&&(delete r.handle,n._removeData(a,\"events\"))}},trigger:function(b,c,d,e){var f,g,h,i,k,l,m,o=[d||z],p=j.call(b,\"type\")?b.type:b,q=j.call(b,\"namespace\")?b.namespace.split(\".\"):[];if(h=l=d=d||z,3!==d.nodeType&&8!==d.nodeType&&!_.test(p+n.event.triggered)&&(p.indexOf(\".\")>=0&&(q=p.split(\".\"),p=q.shift(),q.sort()),g=p.indexOf(\":\")<0&&\"on\"+p,b=b[n.expando]?b:new n.Event(p,\"object\"==typeof b&&b),b.isTrigger=e?2:3,b.namespace=q.join(\".\"),b.namespace_re=b.namespace?new RegExp(\"(^|\\\\.)\"+q.join(\"\\\\.(?:.*\\\\.|)\")+\"(\\\\.|$)\"):null,b.result=void 0,b.target||(b.target=d),c=null==c?[b]:n.makeArray(c,[b]),k=n.event.special[p]||{},e||!k.trigger||k.trigger.apply(d,c)!==!1)){if(!e&&!k.noBubble&&!n.isWindow(d)){for(i=k.delegateType||p,_.test(i+p)||(h=h.parentNode);h;h=h.parentNode)o.push(h),l=h;l===(d.ownerDocument||z)&&o.push(l.defaultView||l.parentWindow||a)}m=0;while((h=o[m++])&&!b.isPropagationStopped())b.type=m>1?i:k.bindType||p,f=(n._data(h,\"events\")||{})[b.type]&&n._data(h,\"handle\"),f&&f.apply(h,c),f=g&&h[g],f&&f.apply&&n.acceptData(h)&&(b.result=f.apply(h,c),b.result===!1&&b.preventDefault());if(b.type=p,!e&&!b.isDefaultPrevented()&&(!k._default||k._default.apply(o.pop(),c)===!1)&&n.acceptData(d)&&g&&d[p]&&!n.isWindow(d)){l=d[g],l&&(d[g]=null),n.event.triggered=p;try{d[p]()}catch(r){}n.event.triggered=void 0,l&&(d[g]=l)}return b.result}},dispatch:function(a){a=n.event.fix(a);var b,c,e,f,g,h=[],i=d.call(arguments),j=(n._data(this,\"events\")||{})[a.type]||[],k=n.event.special[a.type]||{};if(i[0]=a,a.delegateTarget=this,!k.preDispatch||k.preDispatch.call(this,a)!==!1){h=n.event.handlers.call(this,a,j),b=0;while((f=h[b++])&&!a.isPropagationStopped()){a.currentTarget=f.elem,g=0;while((e=f.handlers[g++])&&!a.isImmediatePropagationStopped())(!a.namespace_re||a.namespace_re.test(e.namespace))&&(a.handleObj=e,a.data=e.data,c=((n.event.special[e.origType]||{}).handle||e.handler).apply(f.elem,i),void 0!==c&&(a.result=c)===!1&&(a.preventDefault(),a.stopPropagation()))}return k.postDispatch&&k.postDispatch.call(this,a),a.result}},handlers:function(a,b){var c,d,e,f,g=[],h=b.delegateCount,i=a.target;if(h&&i.nodeType&&(!a.button||\"click\"!==a.type))for(;i!=this;i=i.parentNode||this)if(1===i.nodeType&&(i.disabled!==!0||\"click\"!==a.type)){for(e=[],f=0;h>f;f++)d=b[f],c=d.selector+\" \",void 0===e[c]&&(e[c]=d.needsContext?n(c,this).index(i)>=0:n.find(c,this,null,[i]).length),e[c]&&e.push(d);e.length&&g.push({elem:i,handlers:e})}return h<b.length&&g.push({elem:this,handlers:b.slice(h)}),g},fix:function(a){if(a[n.expando])return a;var b,c,d,e=a.type,f=a,g=this.fixHooks[e];g||(this.fixHooks[e]=g=$.test(e)?this.mouseHooks:Z.test(e)?this.keyHooks:{}),d=g.props?this.props.concat(g.props):this.props,a=new n.Event(f),b=d.length;while(b--)c=d[b],a[c]=f[c];return a.target||(a.target=f.srcElement||z),3===a.target.nodeType&&(a.target=a.target.parentNode),a.metaKey=!!a.metaKey,g.filter?g.filter(a,f):a},props:\"altKey bubbles cancelable ctrlKey currentTarget eventPhase metaKey relatedTarget shiftKey target timeStamp view which\".split(\" \"),fixHooks:{},keyHooks:{props:\"char charCode key keyCode\".split(\" \"),filter:function(a,b){return null==a.which&&(a.which=null!=b.charCode?b.charCode:b.keyCode),a}},mouseHooks:{props:\"button buttons clientX clientY fromElement offsetX offsetY pageX pageY screenX screenY toElement\".split(\" \"),filter:function(a,b){var c,d,e,f=b.button,g=b.fromElement;return null==a.pageX&&null!=b.clientX&&(d=a.target.ownerDocument||z,e=d.documentElement,c=d.body,a.pageX=b.clientX+(e&&e.scrollLeft||c&&c.scrollLeft||0)-(e&&e.clientLeft||c&&c.clientLeft||0),a.pageY=b.clientY+(e&&e.scrollTop||c&&c.scrollTop||0)-(e&&e.clientTop||c&&c.clientTop||0)),!a.relatedTarget&&g&&(a.relatedTarget=g===a.target?b.toElement:g),a.which||void 0===f||(a.which=1&f?1:2&f?3:4&f?2:0),a}},special:{load:{noBubble:!0},f"
,
"ocus:{trigger:function(){if(this!==db()&&this.focus)try{return this.focus(),!1}catch(a){}},delegateType:\"focusin\"},blur:{trigger:function(){return this===db()&&this.blur?(this.blur(),!1):void 0},delegateType:\"focusout\"},click:{trigger:function(){return n.nodeName(this,\"input\")&&\"checkbox\"===this.type&&this.click?(this.click(),!1):void 0},_default:function(a){return n.nodeName(a.target,\"a\")}},beforeunload:{postDispatch:function(a){void 0!==a.result&&(a.originalEvent.returnValue=a.result)}}},simulate:function(a,b,c,d){var e=n.extend(new n.Event,c,{type:a,isSimulated:!0,originalEvent:{}});d?n.event.trigger(e,null,b):n.event.dispatch.call(b,e),e.isDefaultPrevented()&&c.preventDefault()}},n.removeEvent=z.removeEventListener?function(a,b,c){a.removeEventListener&&a.removeEventListener(b,c,!1)}:function(a,b,c){var d=\"on\"+b;a.detachEvent&&(typeof a[d]===L&&(a[d]=null),a.detachEvent(d,c))},n.Event=function(a,b){return this instanceof n.Event?(a&&a.type?(this.originalEvent=a,this.type=a.type,this.isDefaultPrevented=a.defaultPrevented||void 0===a.defaultPrevented&&(a.returnValue===!1||a.getPreventDefault&&a.getPreventDefault())?bb:cb):this.type=a,b&&n.extend(this,b),this.timeStamp=a&&a.timeStamp||n.now(),void(this[n.expando]=!0)):new n.Event(a,b)},n.Event.prototype={isDefaultPrevented:cb,isPropagationStopped:cb,isImmediatePropagationStopped:cb,preventDefault:function(){var a=this.originalEvent;this.isDefaultPrevented=bb,a&&(a.preventDefault?a.preventDefault():a.returnValue=!1)},stopPropagation:function(){var a=this.originalEvent;this.isPropagationStopped=bb,a&&(a.stopPropagation&&a.stopPropagation(),a.cancelBubble=!0)},stopImmediatePropagation:function(){this.isImmediatePropagationStopped=bb,this.stopPropagation()}},n.each({mouseenter:\"mouseover\",mouseleave:\"mouseout\"},function(a,b){n.event.special[a]={delegateType:b,bindType:b,handle:function(a){var c,d=this,e=a.relatedTarget,f=a.handleObj;return(!e||e!==d&&!n.contains(d,e))&&(a.type=f.origType,c=f.handler.apply(this,arguments),a.type=b),c}}}),l.submitBubbles||(n.event.special.submit={setup:function(){return n.nodeName(this,\"form\")?!1:void n.event.add(this,\"click._submit keypress._submit\",function(a){var b=a.target,c=n.nodeName(b,\"input\")||n.nodeName(b,\"button\")?b.form:void 0;c&&!n._data(c,\"submitBubbles\")&&(n.event.add(c,\"submit._submit\",function(a){a._submit_bubble=!0}),n._data(c,\"submitBubbles\",!0))})},postDispatch:function(a){a._submit_bubble&&(delete a._submit_bubble,this.parentNode&&!a.isTrigger&&n.event.simulate(\"submit\",this.parentNode,a,!0))},teardown:function(){return n.nodeName(this,\"form\")?!1:void n.event.remove(this,\"._submit\")}}),l.changeBubbles||(n.event.special.change={setup:function(){return Y.test(this.nodeName)?((\"checkbox\"===this.type||\"radio\"===this.type)&&(n.event.add(this,\"propertychange._change\",function(a){\"checked\"===a.originalEvent.propertyName&&(this._just_changed=!0)}),n.event.add(this,\"click._change\",function(a){this._just_changed&&!a.isTrigger&&(this._just_changed=!1),n.event.simulate(\"change\",this,a,!0)})),!1):void n.event.add(this,\"beforeactivate._change\",function(a){var b=a.target;Y.test(b.nodeName)&&!n._data(b,\"changeBubbles\")&&(n.event.add(b,\"change._change\",function(a){!this.parentNode||a.isSimulated||a.isTrigger||n.event.simulate(\"change\",this.parentNode,a,!0)}),n._data(b,\"changeBubbles\",!0))})},handle:function(a){var b=a.target;return this!==b||a.isSimulated||a.isTrigger||\"radio\"!==b.type&&\"checkbox\"!==b.type?a.handleObj.handler.apply(this,arguments):void 0},teardown:function(){return n.event.remove(this,\"._change\"),!Y.test(this.nodeName)}}),l.focusinBubbles||n.each({focus:\"focusin\",blur:\"focusout\"},function(a,b){var c=function(a){n.event.simulate(b,a.target,n.event.fix(a),!0)};n.event.special[b]={setup:function(){var d=this.ownerDocument||this,e=n._data(d,b);e||d.addEventListener(a,c,!0),n._data(d,b,(e||0)+1)},teardown:function(){var d=this.ownerDocument||this,e=n._data(d,b)-1;e?n._data(d,b,e):(d.removeEventListener(a,c,!0),n._removeData(d,b))}}}),n.fn.extend({on:function(a,b,c,d,e){var f,g;if(\"object\"==typeof a){\"string\"!=typeof b&&(c=c||b,b=void 0);for(f in a)this.on(f,b,c,a[f],e);return this}if(null==c&&null==d?(d=b,c=b=void 0):null==d&&(\"string\"==typeof b?(d=c,c=void 0):(d=c,c=b,b=void 0)),d===!1)d=cb;else if(!d)return this;return 1===e&&(g=d,d=function(a){return n().off(a),g.apply(this,arguments)},d.guid=g.guid||(g.guid=n.guid++)),this.each(function(){n.event.add(this,a,d,c,b)})},one:function(a,b,c,d){return this.on(a,b,c,d,1)},off:function(a,b,c){var d,e;if(a&&a.preventDefault&&a.handleObj)return d=a.handleObj,n(a.delegateTarget).off(d.namespace?d.origType+\".\"+d.namespace:d.origType,d.selector,d.handler),this;if(\"object\"==typeof a){for(e in a)this.off(e,b,a[e]);return this}return(b===!1||\"function\"==typeof b)&&(c=b,b=void 0),c===!1&&(c=cb),this.each(function(){n.event.remove(this,a,c,b)})},trigger:function(a,b){return this.each(function(){n.event.trigger(a,b,this)})},triggerHandler:function(a,b){var c=this[0];return c?n.event.trigger(a,b,c,!0):void 0}});function eb(a){var b=fb.split(\"|\"),c=a.createDocumentFragment();if(c.createElement)while(b.length)c.createElement(b.pop());return c}var fb=\"abbr|article|aside|audio|bdi|canvas|data|datalist|details|figcaption|figure|footer|header|hgroup|mark|meter|nav|output|progress|section|summary|time|video\",gb=/ jQuery\\d+=\"(?:null|\\d+)\"/g,hb=new RegExp(\"<(?:\"+fb+\")[\\\\s/>]\",\"i\"),ib=/^\\s+/,jb=/<(?!area|br|col|embed|hr|img|input|link|meta|param)(([\\w:]+)[^>]*)\\/>/gi,kb=/<([\\w:]+)/,lb=/<tbody/i,mb=/<|&#?\\w+;/,nb=/<(?:script|style|link)/i,ob=/checked\\s*(?:[^=]|=\\s*.checked.)/i,pb=/^$|\\/(?:java|ecma)script/i,qb=/^true\\/(.*)/,rb=/^\\s*<!(?:\\[CDATA\\[|--)|(?:\\]\\]|--)>\\s*$/g,sb={option:[1,\"<select multiple='multiple'>\",\"</select>\"],legend:[1,\"<fieldset>\",\"</fieldset>\"],area:[1,\"<map>\",\"</map>\"],param:[1,\"<object>\",\"</object>\"],thead:[1,\"<table>\",\"</table>\"],tr:[2,\"<table><tbody>\",\"</tbody></table>\"],col:[2,\"<table><tbody></tbody><colgroup>\",\"</colgroup></table>\"],td:[3,\"<table><tbody><tr>\",\"</tr></tbody></table>\"],_default:l.htmlSerialize?[0,\"\",\"\"]:[1,\"X<div>\",\"</div>\"]},tb=eb(z),ub=tb.appendChild(z.createElement(\"div\"));sb.optgroup=sb.option,sb.tbody=sb.tfoot=sb.colgroup=sb.caption=sb.thead,sb.th=sb.td;function vb(a,b){var c,d,e=0,f=typeof a.getElementsByTagName!==L?a.getElementsByTagName(b||\"*\"):typeof a.querySelectorAll!==L?a.querySelectorAll(b||\"*\"):void 0;if(!f)for(f=[],c=a.childNodes||a;null!=(d=c[e]);e++)!b||n.nodeName(d,b)?f.push(d):n.merge(f,vb(d,b));return void 0===b||b&&n.nodeName(a,b)?n.merge([a],f):f}function wb(a){X.test(a.type)&&(a.defaultChecked=a.checked)}function xb(a,b){return n.nodeName(a,\"table\")&&n.nodeName(11!==b.nodeType?b:b.firstChild,\"tr\")?a.getElementsByTagName(\"tbody\")[0]||a.appendChild(a.ownerDocument.createElement(\"tbody\")):a}function yb(a){return a.type=(null!==n.find.attr(a,\"type\"))+\"/\"+a.type,a}function zb(a){var b=qb.exec(a.type);return b?a.type=b[1]:a.removeAttribute(\"type\"),a}function Ab(a,b){for(var c,d=0;null!=(c=a[d]);d++)n._data(c,\"globalEval\",!b||n._data(b[d],\"globalEval\"))}function Bb(a,b){if(1===b.nodeType&&n.hasData(a)){var c,d,e,f=n._data(a),g=n._data(b,f),h=f.events;if(h){delete g.handle,g.events={};for(c in h)for(d=0,e=h[c].length;e>d;d++)n.event.add(b,c,h[c][d])}g.data&&(g.data=n.extend({},g.data))}}function Cb(a,b){var c,d,e;if(1===b.nodeType){if(c=b.nodeName.toLowerCase(),!l.noCloneEvent&&b[n.expando]){e=n._data(b);for(d in e.events)n.removeEvent(b,d,e.handle);b.removeAttribute(n.expando)}\"script\"===c&&b.text!==a.text?(yb(b).text=a.text,zb(b)):\"object\"===c?(b.parentNode&&(b.outerHTML=a.outerHTML),l.html5Clone&&a.innerHTML&&!n.trim(b.innerHTML)&&(b.innerHTML=a.innerHTML)):\"input\"===c&&X.test(a.type)?(b.defaultChecked=b.checked=a.checked,b.value!==a.value&&(b.value=a.value)):\"option\"===c?b.defaultSelected=b.selected=a.defaultSelected:(\"input\"===c||\"textarea\"===c)&&(b.defaultValue=a.defaultValue)}}n.extend({clone:function(a,b,c){var d,e,f,g,h,i=n.contains(a.ownerDocument,a);if(l.html5Clone||n.isXMLDoc(a)||!hb.test(\"<\"+a.nodeName+\">\")?f=a.cloneNode(!0):(ub.innerHTML=a.outerHTML,ub.removeChild(f=ub.firstChild)),!(l.noCloneEvent&&l.noCloneChecked||1!==a.nodeType&&11!==a.nodeType||n.isXMLDoc(a)))for(d=vb(f),h=vb(a),g=0;null!=(e=h[g]);++g)d[g]&&Cb(e,d[g]);if(b)if(c)for(h=h||vb(a),d=d||vb(f),g=0;null!=(e=h[g]);g++)Bb(e,d[g]);else Bb(a,f);return d=vb(f,\"script\"),d.length>0&&Ab(d,!i&&vb(a,\"script\")),d=h=e=null,f},buildFragment:function(a,b,c,d){for(var e,f,g,h,i,j,k,m=a.length,o=eb(b),p=[],q=0;m>q;q++)if(f=a[q],f||0===f)if(\"object\"===n.type(f))n.merge(p,f.nodeType?[f]:f);else if(mb.test(f)){h=h||o.appendChild(b.createElement(\"div\")),i=(kb.exec(f)||[\"\",\"\"])[1].toLowerCase(),k=sb[i]||sb._default,h.innerHTML=k[1]+f.replace(jb,\"<$1></$2>\")+k[2],e=k[0];while(e--)h=h.lastChild;if(!l.leadingWhitespace&&ib.test(f)&&p.push(b.createTextNode(ib.exec(f)[0])),!l.tbody){f=\"table\"!==i||lb.test(f)?\"<table>\"!==k[1]||lb.test(f)?0:h:h.firstChild,e=f&&f.childNodes.length;while(e--)n.nodeName(j=f.childNodes[e],\"tbody\")&&!j.childNodes.length&&f.removeChild(j)}n.merge(p,h.childNodes),h.textContent=\"\";while(h.firstChild)h.removeChild(h.firstChild);h=o.lastChild}else p.push(b.createTextNode(f));h&&o.removeChild(h),l.appendChecked||n.grep(vb(p,\"input\"),wb),q=0;while(f=p[q++])if((!d||-1===n.inArray(f,d))&&(g=n.contains(f.ownerDocument,f),h=vb(o.appendChild(f),\"script\"),g&&Ab(h),c)){e=0;while(f=h[e++])pb.test(f.type||\"\")&&c.push(f)}return h=null,o},cleanData:function(a,b){for(var d,e,f,g,h=0,i=n.expando,j=n.cache,k=l.deleteExpando,m=n.event.special;null!=(d=a[h]);h++)if((b||n.acceptData(d))&&(f=d[i],g=f&&j[f])){if(g.events)for(e in g.events)m[e]?n.event.remove(d,e):n.removeEvent(d,e,g.handle);j[f]&&(delete j[f],k?delete d[i]:typeof d.removeAttribute!==L?d.removeAttribute(i):d[i]=null,c.push(f))}}}),n.fn.extend({text:function(a){return W(this,function(a){return void 0===a?n.text(this):this.empty().append((this[0]&&this[0].ownerDocument||z).createTextNode(a))},null,a,arguments.length)},app"
,
"end:function(){return this.domManip(arguments,function(a){if(1===this.nodeType||11===this.nodeType||9===this.nodeType){var b=xb(this,a);b.appendChild(a)}})},prepend:function(){return this.domManip(arguments,function(a){if(1===this.nodeType||11===this.nodeType||9===this.nodeType){var b=xb(this,a);b.insertBefore(a,b.firstChild)}})},before:function(){return this.domManip(arguments,function(a){this.parentNode&&this.parentNode.insertBefore(a,this)})},after:function(){return this.domManip(arguments,function(a){this.parentNode&&this.parentNode.insertBefore(a,this.nextSibling)})},remove:function(a,b){for(var c,d=a?n.filter(a,this):this,e=0;null!=(c=d[e]);e++)b||1!==c.nodeType||n.cleanData(vb(c)),c.parentNode&&(b&&n.contains(c.ownerDocument,c)&&Ab(vb(c,\"script\")),c.parentNode.removeChild(c));return this},empty:function(){for(var a,b=0;null!=(a=this[b]);b++){1===a.nodeType&&n.cleanData(vb(a,!1));while(a.firstChild)a.removeChild(a.firstChild);a.options&&n.nodeName(a,\"select\")&&(a.options.length=0)}return this},clone:function(a,b){return a=null==a?!1:a,b=null==b?a:b,this.map(function(){return n.clone(this,a,b)})},html:function(a){return W(this,function(a){var b=this[0]||{},c=0,d=this.length;if(void 0===a)return 1===b.nodeType?b.innerHTML.replace(gb,\"\"):void 0;if(!(\"string\"!=typeof a||nb.test(a)||!l.htmlSerialize&&hb.test(a)||!l.leadingWhitespace&&ib.test(a)||sb[(kb.exec(a)||[\"\",\"\"])[1].toLowerCase()])){a=a.replace(jb,\"<$1></$2>\");try{for(;d>c;c++)b=this[c]||{},1===b.nodeType&&(n.cleanData(vb(b,!1)),b.innerHTML=a);b=0}catch(e){}}b&&this.empty().append(a)},null,a,arguments.length)},replaceWith:function(){var a=arguments[0];return this.domManip(arguments,function(b){a=this.parentNode,n.cleanData(vb(this)),a&&a.replaceChild(b,this)}),a&&(a.length||a.nodeType)?this:this.remove()},detach:function(a){return this.remove(a,!0)},domManip:function(a,b){a=e.apply([],a);var c,d,f,g,h,i,j=0,k=this.length,m=this,o=k-1,p=a[0],q=n.isFunction(p);if(q||k>1&&\"string\"==typeof p&&!l.checkClone&&ob.test(p))return this.each(function(c){var d=m.eq(c);q&&(a[0]=p.call(this,c,d.html())),d.domManip(a,b)});if(k&&(i=n.buildFragment(a,this[0].ownerDocument,!1,this),c=i.firstChild,1===i.childNodes.length&&(i=c),c)){for(g=n.map(vb(i,\"script\"),yb),f=g.length;k>j;j++)d=i,j!==o&&(d=n.clone(d,!0,!0),f&&n.merge(g,vb(d,\"script\"))),b.call(this[j],d,j);if(f)for(h=g[g.length-1].ownerDocument,n.map(g,zb),j=0;f>j;j++)d=g[j],pb.test(d.type||\"\")&&!n._data(d,\"globalEval\")&&n.contains(h,d)&&(d.src?n._evalUrl&&n._evalUrl(d.src):n.globalEval((d.text||d.textContent||d.innerHTML||\"\").replace(rb,\"\")));i=c=null}return this}}),n.each({appendTo:\"append\",prependTo:\"prepend\",insertBefore:\"before\",insertAfter:\"after\",replaceAll:\"replaceWith\"},function(a,b){n.fn[a]=function(a){for(var c,d=0,e=[],g=n(a),h=g.length-1;h>=d;d++)c=d===h?this:this.clone(!0),n(g[d])[b](c),f.apply(e,c.get());return this.pushStack(e)}});var Db,Eb={};function Fb(b,c){var d=n(c.createElement(b)).appendTo(c.body),e=a.getDefaultComputedStyle?a.getDefaultComputedStyle(d[0]).display:n.css(d[0],\"display\");return d.detach(),e}function Gb(a){var b=z,c=Eb[a];return c||(c=Fb(a,b),\"none\"!==c&&c||(Db=(Db||n(\"<iframe frameborder='0' width='0' height='0'/>\")).appendTo(b.documentElement),b=(Db[0].contentWindow||Db[0].contentDocument).document,b.write(),b.close(),c=Fb(a,b),Db.detach()),Eb[a]=c),c}!function(){var a,b,c=z.createElement(\"div\"),d=\"-webkit-box-sizing:content-box;-moz-box-sizing:content-box;box-sizing:content-box;display:block;padding:0;margin:0;border:0\";c.innerHTML=\"  <link/><table></table><a href='/a'>a</a><input type='checkbox'/>\",a=c.getElementsByTagName(\"a\")[0],a.style.cssText=\"float:left;opacity:.5\",l.opacity=/^0.5/.test(a.style.opacity),l.cssFloat=!!a.style.cssFloat,c.style.backgroundClip=\"content-box\",c.cloneNode(!0).style.backgroundClip=\"\",l.clearCloneStyle=\"content-box\"===c.style.backgroundClip,a=c=null,l.shrinkWrapBlocks=function(){var a,c,e,f;if(null==b){if(a=z.getElementsByTagName(\"body\")[0],!a)return;f=\"border:0;width:0;height:0;position:absolute;top:0;left:-9999px\",c=z.createElement(\"div\"),e=z.createElement(\"div\"),a.appendChild(c).appendChild(e),b=!1,typeof e.style.zoom!==L&&(e.style.cssText=d+\";width:1px;padding:1px;zoom:1\",e.innerHTML=\"<div></div>\",e.firstChild.style.width=\"5px\",b=3!==e.offsetWidth),a.removeChild(c),a=c=e=null}return b}}();var Hb=/^margin/,Ib=new RegExp(\"^(\"+T+\")(?!px)[a-z%]+$\",\"i\"),Jb,Kb,Lb=/^(top|right|bottom|left)$/;a.getComputedStyle?(Jb=function(a){return a.ownerDocument.defaultView.getComputedStyle(a,null)},Kb=function(a,b,c){var d,e,f,g,h=a.style;return c=c||Jb(a),g=c?c.getPropertyValue(b)||c[b]:void 0,c&&(\"\"!==g||n.contains(a.ownerDocument,a)||(g=n.style(a,b)),Ib.test(g)&&Hb.test(b)&&(d=h.width,e=h.minWidth,f=h.maxWidth,h.minWidth=h.maxWidth=h.width=g,g=c.width,h.width=d,h.minWidth=e,h.maxWidth=f)),void 0===g?g:g+\"\"}):z.documentElement.currentStyle&&(Jb=function(a){return a.currentStyle},Kb=function(a,b,c){var d,e,f,g,h=a.style;return c=c||Jb(a),g=c?c[b]:void 0,null==g&&h&&h[b]&&(g=h[b]),Ib.test(g)&&!Lb.test(b)&&(d=h.left,e=a.runtimeStyle,f=e&&e.left,f&&(e.left=a.currentStyle.left),h.left=\"fontSize\"===b?\"1em\":g,g=h.pixelLeft+\"px\",h.left=d,f&&(e.left=f)),void 0===g?g:g+\"\"||\"auto\"});function Mb(a,b){return{get:function(){var c=a();if(null!=c)return c?void delete this.get:(this.get=b).apply(this,arguments)}}}!function(){var b,c,d,e,f,g,h=z.createElement(\"div\"),i=\"border:0;width:0;height:0;position:absolute;top:0;left:-9999px\",j=\"-webkit-box-sizing:content-box;-moz-box-sizing:content-box;box-sizing:content-box;display:block;padding:0;margin:0;border:0\";h.innerHTML=\"  <link/><table></table><a href='/a'>a</a><input type='checkbox'/>\",b=h.getElementsByTagName(\"a\")[0],b.style.cssText=\"float:left;opacity:.5\",l.opacity=/^0.5/.test(b.style.opacity),l.cssFloat=!!b.style.cssFloat,h.style.backgroundClip=\"content-box\",h.cloneNode(!0).style.backgroundClip=\"\",l.clearCloneStyle=\"content-box\"===h.style.backgroundClip,b=h=null,n.extend(l,{reliableHiddenOffsets:function(){if(null!=c)return c;var a,b,d,e=z.createElement(\"div\"),f=z.getElementsByTagName(\"body\")[0];if(f)return e.setAttribute(\"className\",\"t\"),e.innerHTML=\"  <link/><table></table><a href='/a'>a</a><input type='checkbox'/>\",a=z.createElement(\"div\"),a.style.cssText=i,f.appendChild(a).appendChild(e),e.innerHTML=\"<table><tr><td></td><td>t</td></tr></table>\",b=e.getElementsByTagName(\"td\"),b[0].style.cssText=\"padding:0;margin:0;border:0;display:none\",d=0===b[0].offsetHeight,b[0].style.display=\"\",b[1].style.display=\"none\",c=d&&0===b[0].offsetHeight,f.removeChild(a),e=f=null,c},boxSizing:function(){return null==d&&k(),d},boxSizingReliable:function(){return null==e&&k(),e},pixelPosition:function(){return null==f&&k(),f},reliableMarginRight:function(){var b,c,d,e;if(null==g&&a.getComputedStyle){if(b=z.getElementsByTagName(\"body\")[0],!b)return;c=z.createElement(\"div\"),d=z.createElement(\"div\"),c.style.cssText=i,b.appendChild(c).appendChild(d),e=d.appendChild(z.createElement(\"div\")),e.style.cssText=d.style.cssText=j,e.style.marginRight=e.style.width=\"0\",d.style.width=\"1px\",g=!parseFloat((a.getComputedStyle(e,null)||{}).marginRight),b.removeChild(c)}return g}});function k(){var b,c,h=z.getElementsByTagName(\"body\")[0];h&&(b=z.createElement(\"div\"),c=z.createElement(\"div\"),b.style.cssText=i,h.appendChild(b).appendChild(c),c.style.cssText=\"-webkit-box-sizing:border-box;-moz-box-sizing:border-box;box-sizing:border-box;position:absolute;display:block;padding:1px;border:1px;width:4px;margin-top:1%;top:1%\",n.swap(h,null!=h.style.zoom?{zoom:1}:{},function(){d=4===c.offsetWidth}),e=!0,f=!1,g=!0,a.getComputedStyle&&(f=\"1%\"!==(a.getComputedStyle(c,null)||{}).top,e=\"4px\"===(a.getComputedStyle(c,null)||{width:\"4px\"}).width),h.removeChild(b),c=h=null)}}(),n.swap=function(a,b,c,d){var e,f,g={};for(f in b)g[f]=a.style[f],a.style[f]=b[f];e=c.apply(a,d||[]);for(f in b)a.style[f]=g[f];return e};var Nb=/alpha\\([^)]*\\)/i,Ob=/opacity\\s*=\\s*([^)]*)/,Pb=/^(none|table(?!-c[ea]).+)/,Qb=new RegExp(\"^(\"+T+\")(.*)$\",\"i\"),Rb=new RegExp(\"^([+-])=(\"+T+\")\",\"i\"),Sb={position:\"absolute\",visibility:\"hidden\",display:\"block\"},Tb={letterSpacing:0,fontWeight:400},Ub=[\"Webkit\",\"O\",\"Moz\",\"ms\"];function Vb(a,b){if(b in a)return b;var c=b.charAt(0).toUpperCase()+b.slice(1),d=b,e=Ub.length;while(e--)if(b=Ub[e]+c,b in a)return b;return d}function Wb(a,b){for(var c,d,e,f=[],g=0,h=a.length;h>g;g++)d=a[g],d.style&&(f[g]=n._data(d,\"olddisplay\"),c=d.style.display,b?(f[g]||\"none\"!==c||(d.style.display=\"\"),\"\"===d.style.display&&V(d)&&(f[g]=n._data(d,\"olddisplay\",Gb(d.nodeName)))):f[g]||(e=V(d),(c&&\"none\"!==c||!e)&&n._data(d,\"olddisplay\",e?c:n.css(d,\"display\"))));for(g=0;h>g;g++)d=a[g],d.style&&(b&&\"none\"!==d.style.display&&\"\"!==d.style.display||(d.style.display=b?f[g]||\"\":\"none\"));return a}function Xb(a,b,c){var d=Qb.exec(b);return d?Math.max(0,d[1]-(c||0))+(d[2]||\"px\"):b}function Yb(a,b,c,d,e){for(var f=c===(d?\"border\":\"content\")?4:\"width\"===b?1:0,g=0;4>f;f+=2)\"margin\"===c&&(g+=n.css(a,c+U[f],!0,e)),d?(\"content\"===c&&(g-=n.css(a,\"padding\"+U[f],!0,e)),\"margin\"!==c&&(g-=n.css(a,\"border\"+U[f]+\"Width\",!0,e))):(g+=n.css(a,\"padding\"+U[f],!0,e),\"padding\"!==c&&(g+=n.css(a,\"border\"+U[f]+\"Width\",!0,e)));return g}function Zb(a,b,c){var d=!0,e=\"width\"===b?a.offsetWidth:a.offsetHeight,f=Jb(a),g=l.boxSizing()&&\"border-box\"===n.css(a,\"boxSizing\",!1,f);if(0>=e||null==e){if(e=Kb(a,b,f),(0>e||null==e)&&(e=a.style[b]),Ib.test(e))return e;d=g&&(l.boxSizingReliable()||e===a.style[b]),e=parseFloat(e)||0}return e+Yb(a,b,c||(g?\"border\":\"content\"),d,f)+\"px\"}n.extend({cssHooks:{opacity:{get:function(a,b){if(b){var c=Kb(a,\"opacity\");return\"\"===c?\"1\":c}}}},cssNumber:{columnCount:!0,fillOpacity:!0,fontWeight:!0,lineHeight:!0,opacity:!0,order:!0,orphans:!0,widows:!0,zIndex:!0,zoom:!0},cssProps:{\"float\":l.cssFloat?\"cssFloat\":\"styleFloat\"},style:function(a,b,c,d){if(a&&3!==a.nodeType&&8!==a.nodeType&&a.style){var e,f,g,h=n.camelCase(b),i=a.style;if(b=n.cssProps[h]||(n.cssProps[h]=Vb(i,h)),g=n.cssHooks[b]||n.cssHooks[h],void 0=="
,
"=c)return g&&\"get\"in g&&void 0!==(e=g.get(a,!1,d))?e:i[b];if(f=typeof c,\"string\"===f&&(e=Rb.exec(c))&&(c=(e[1]+1)*e[2]+parseFloat(n.css(a,b)),f=\"number\"),null!=c&&c===c&&(\"number\"!==f||n.cssNumber[h]||(c+=\"px\"),l.clearCloneStyle||\"\"!==c||0!==b.indexOf(\"background\")||(i[b]=\"inherit\"),!(g&&\"set\"in g&&void 0===(c=g.set(a,c,d)))))try{i[b]=\"\",i[b]=c}catch(j){}}},css:function(a,b,c,d){var e,f,g,h=n.camelCase(b);return b=n.cssProps[h]||(n.cssProps[h]=Vb(a.style,h)),g=n.cssHooks[b]||n.cssHooks[h],g&&\"get\"in g&&(f=g.get(a,!0,c)),void 0===f&&(f=Kb(a,b,d)),\"normal\"===f&&b in Tb&&(f=Tb[b]),\"\"===c||c?(e=parseFloat(f),c===!0||n.isNumeric(e)?e||0:f):f}}),n.each([\"height\",\"width\"],function(a,b){n.cssHooks[b]={get:function(a,c,d){return c?0===a.offsetWidth&&Pb.test(n.css(a,\"display\"))?n.swap(a,Sb,function(){return Zb(a,b,d)}):Zb(a,b,d):void 0},set:function(a,c,d){var e=d&&Jb(a);return Xb(a,c,d?Yb(a,b,d,l.boxSizing()&&\"border-box\"===n.css(a,\"boxSizing\",!1,e),e):0)}}}),l.opacity||(n.cssHooks.opacity={get:function(a,b){return Ob.test((b&&a.currentStyle?a.currentStyle.filter:a.style.filter)||\"\")?.01*parseFloat(RegExp.$1)+\"\":b?\"1\":\"\"},set:function(a,b){var c=a.style,d=a.currentStyle,e=n.isNumeric(b)?\"alpha(opacity=\"+100*b+\")\":\"\",f=d&&d.filter||c.filter||\"\";c.zoom=1,(b>=1||\"\"===b)&&\"\"===n.trim(f.replace(Nb,\"\"))&&c.removeAttribute&&(c.removeAttribute(\"filter\"),\"\"===b||d&&!d.filter)||(c.filter=Nb.test(f)?f.replace(Nb,e):f+\" \"+e)}}),n.cssHooks.marginRight=Mb(l.reliableMarginRight,function(a,b){return b?n.swap(a,{display:\"inline-block\"},Kb,[a,\"marginRight\"]):void 0}),n.each({margin:\"\",padding:\"\",border:\"Width\"},function(a,b){n.cssHooks[a+b]={expand:function(c){for(var d=0,e={},f=\"string\"==typeof c?c.split(\" \"):[c];4>d;d++)e[a+U[d]+b]=f[d]||f[d-2]||f[0];return e}},Hb.test(a)||(n.cssHooks[a+b].set=Xb)}),n.fn.extend({css:function(a,b){return W(this,function(a,b,c){var d,e,f={},g=0;if(n.isArray(b)){for(d=Jb(a),e=b.length;e>g;g++)f[b[g]]=n.css(a,b[g],!1,d);return f}return void 0!==c?n.style(a,b,c):n.css(a,b)\n"
,
"},a,b,arguments.length>1)},show:function(){return Wb(this,!0)},hide:function(){return Wb(this)},toggle:function(a){return\"boolean\"==typeof a?a?this.show():this.hide():this.each(function(){V(this)?n(this).show():n(this).hide()})}});function $b(a,b,c,d,e){return new $b.prototype.init(a,b,c,d,e)}n.Tween=$b,$b.prototype={constructor:$b,init:function(a,b,c,d,e,f){this.elem=a,this.prop=c,this.easing=e||\"swing\",this.options=b,this.start=this.now=this.cur(),this.end=d,this.unit=f||(n.cssNumber[c]?\"\":\"px\")},cur:function(){var a=$b.propHooks[this.prop];return a&&a.get?a.get(this):$b.propHooks._default.get(this)},run:function(a){var b,c=$b.propHooks[this.prop];return this.pos=b=this.options.duration?n.easing[this.easing](a,this.options.duration*a,0,1,this.options.duration):a,this.now=(this.end-this.start)*b+this.start,this.options.step&&this.options.step.call(this.elem,this.now,this),c&&c.set?c.set(this):$b.propHooks._default.set(this),this}},$b.prototype.init.prototype=$b.prototype,$b.propHooks={_default:{get:function(a){var b;return null==a.elem[a.prop]||a.elem.style&&null!=a.elem.style[a.prop]?(b=n.css(a.elem,a.prop,\"\"),b&&\"auto\"!==b?b:0):a.elem[a.prop]},set:function(a){n.fx.step[a.prop]?n.fx.step[a.prop](a):a.elem.style&&(null!=a.elem.style[n.cssProps[a.prop]]||n.cssHooks[a.prop])?n.style(a.elem,a.prop,a.now+a.unit):a.elem[a.prop]=a.now}}},$b.propHooks.scrollTop=$b.propHooks.scrollLeft={set:function(a){a.elem.nodeType&&a.elem.parentNode&&(a.elem[a.prop]=a.now)}},n.easing={linear:function(a){return a},swing:function(a){return.5-Math.cos(a*Math.PI)/2}},n.fx=$b.prototype.init,n.fx.step={};var _b,ac,bc=/^(?:toggle|show|hide)$/,cc=new RegExp(\"^(?:([+-])=|)(\"+T+\")([a-z%]*)$\",\"i\"),dc=/queueHooks$/,ec=[jc],fc={\"*\":[function(a,b){var c=this.createTween(a,b),d=c.cur(),e=cc.exec(b),f=e&&e[3]||(n.cssNumber[a]?\"\":\"px\"),g=(n.cssNumber[a]||\"px\"!==f&&+d)&&cc.exec(n.css(c.elem,a)),h=1,i=20;if(g&&g[3]!==f){f=f||g[3],e=e||[],g=+d||1;do h=h||\".5\",g/=h,n.style(c.elem,a,g+f);while(h!==(h=c.cur()/d)&&1!==h&&--i)}return e&&(g=c.start=+g||+d||0,c.unit=f,c.end=e[1]?g+(e[1]+1)*e[2]:+e[2]),c}]};function gc(){return setTimeout(function(){_b=void 0}),_b=n.now()}function hc(a,b){var c,d={height:a},e=0;for(b=b?1:0;4>e;e+=2-b)c=U[e],d[\"margin\"+c]=d[\"padding\"+c]=a;return b&&(d.opacity=d.width=a),d}function ic(a,b,c){for(var d,e=(fc[b]||[]).concat(fc[\"*\"]),f=0,g=e.length;g>f;f++)if(d=e[f].call(c,b,a))return d}function jc(a,b,c){var d,e,f,g,h,i,j,k,m=this,o={},p=a.style,q=a.nodeType&&V(a),r=n._data(a,\"fxshow\");c.queue||(h=n._queueHooks(a,\"fx\"),null==h.unqueued&&(h.unqueued=0,i=h.empty.fire,h.empty.fire=function(){h.unqueued||i()}),h.unqueued++,m.always(function(){m.always(function(){h.unqueued--,n.queue(a,\"fx\").length||h.empty.fire()})})),1===a.nodeType&&(\"height\"in b||\"width\"in b)&&(c.overflow=[p.overflow,p.overflowX,p.overflowY],j=n.css(a,\"display\"),k=Gb(a.nodeName),\"none\"===j&&(j=k),\"inline\"===j&&\"none\"===n.css(a,\"float\")&&(l.inlineBlockNeedsLayout&&\"inline\"!==k?p.zoom=1:p.display=\"inline-block\")),c.overflow&&(p.overflow=\"hidden\",l.shrinkWrapBlocks()||m.always(function(){p.overflow=c.overflow[0],p.overflowX=c.overflow[1],p.overflowY=c.overflow[2]}));for(d in b)if(e=b[d],bc.exec(e)){if(delete b[d],f=f||\"toggle\"===e,e===(q?\"hide\":\"show\")){if(\"show\"!==e||!r||void 0===r[d])continue;q=!0}o[d]=r&&r[d]||n.style(a,d)}if(!n.isEmptyObject(o)){r?\"hidden\"in r&&(q=r.hidden):r=n._data(a,\"fxshow\",{}),f&&(r.hidden=!q),q?n(a).show():m.done(function(){n(a).hide()}),m.done(function(){var b;n._removeData(a,\"fxshow\");for(b in o)n.style(a,b,o[b])});for(d in o)g=ic(q?r[d]:0,d,m),d in r||(r[d]=g.start,q&&(g.end=g.start,g.start=\"width\"===d||\"height\"===d?1:0))}}function kc(a,b){var c,d,e,f,g;for(c in a)if(d=n.camelCase(c),e=b[d],f=a[c],n.isArray(f)&&(e=f[1],f=a[c]=f[0]),c!==d&&(a[d]=f,delete a[c]),g=n.cssHooks[d],g&&\"expand\"in g){f=g.expand(f),delete a[d];for(c in f)c in a||(a[c]=f[c],b[c]=e)}else b[d]=e}function lc(a,b,c){var d,e,f=0,g=ec.length,h=n.Deferred().always(function(){delete i.elem}),i=function(){if(e)return!1;for(var b=_b||gc(),c=Math.max(0,j.startTime+j.duration-b),d=c/j.duration||0,f=1-d,g=0,i=j.tweens.length;i>g;g++)j.tweens[g].run(f);return h.notifyWith(a,[j,f,c]),1>f&&i?c:(h.resolveWith(a,[j]),!1)},j=h.promise({elem:a,props:n.extend({},b),opts:n.extend(!0,{specialEasing:{}},c),originalProperties:b,originalOptions:c,startTime:_b||gc(),duration:c.duration,tweens:[],createTween:function(b,c){var d=n.Tween(a,j.opts,b,c,j.opts.specialEasing[b]||j.opts.easing);return j.tweens.push(d),d},stop:function(b){var c=0,d=b?j.tweens.length:0;if(e)return this;for(e=!0;d>c;c++)j.tweens[c].run(1);return b?h.resolveWith(a,[j,b]):h.rejectWith(a,[j,b]),this}}),k=j.props;for(kc(k,j.opts.specialEasing);g>f;f++)if(d=ec[f].call(j,a,k,j.opts))return d;return n.map(k,ic,j),n.isFunction(j.opts.start)&&j.opts.start.call(a,j),n.fx.timer(n.extend(i,{elem:a,anim:j,queue:j.opts.queue})),j.progress(j.opts.progress).done(j.opts.done,j.opts.complete).fail(j.opts.fail).always(j.opts.always)}n.Animation=n.extend(lc,{tweener:function(a,b){n.isFunction(a)?(b=a,a=[\"*\"]):a=a.split(\" \");for(var c,d=0,e=a.length;e>d;d++)c=a[d],fc[c]=fc[c]||[],fc[c].unshift(b)},prefilter:function(a,b){b?ec.unshift(a):ec.push(a)}}),n.speed=function(a,b,c){var d=a&&\"object\"==typeof a?n.extend({},a):{complete:c||!c&&b||n.isFunction(a)&&a,duration:a,easing:c&&b||b&&!n.isFunction(b)&&b};return d.duration=n.fx.off?0:\"number\"==typeof d.duration?d.duration:d.duration in n.fx.speeds?n.fx.speeds[d.duration]:n.fx.speeds._default,(null==d.queue||d.queue===!0)&&(d.queue=\"fx\"),d.old=d.complete,d.complete=function(){n.isFunction(d.old)&&d.old.call(this),d.queue&&n.dequeue(this,d.queue)},d},n.fn.extend({fadeTo:function(a,b,c,d){return this.filter(V).css(\"opacity\",0).show().end().animate({opacity:b},a,c,d)},animate:function(a,b,c,d){var e=n.isEmptyObject(a),f=n.speed(b,c,d),g=function(){var b=lc(this,n.extend({},a),f);(e||n._data(this,\"finish\"))&&b.stop(!0)};return g.finish=g,e||f.queue===!1?this.each(g):this.queue(f.queue,g)},stop:function(a,b,c){var d=function(a){var b=a.stop;delete a.stop,b(c)};return\"string\"!=typeof a&&(c=b,b=a,a=void 0),b&&a!==!1&&this.queue(a||\"fx\",[]),this.each(function(){var b=!0,e=null!=a&&a+\"queueHooks\",f=n.timers,g=n._data(this);if(e)g[e]&&g[e].stop&&d(g[e]);else for(e in g)g[e]&&g[e].stop&&dc.test(e)&&d(g[e]);for(e=f.length;e--;)f[e].elem!==this||null!=a&&f[e].queue!==a||(f[e].anim.stop(c),b=!1,f.splice(e,1));(b||!c)&&n.dequeue(this,a)})},finish:function(a){return a!==!1&&(a=a||\"fx\"),this.each(function(){var b,c=n._data(this),d=c[a+\"queue\"],e=c[a+\"queueHooks\"],f=n.timers,g=d?d.length:0;for(c.finish=!0,n.queue(this,a,[]),e&&e.stop&&e.stop.call(this,!0),b=f.length;b--;)f[b].elem===this&&f[b].queue===a&&(f[b].anim.stop(!0),f.splice(b,1));for(b=0;g>b;b++)d[b]&&d[b].finish&&d[b].finish.call(this);delete c.finish})}}),n.each([\"toggle\",\"show\",\"hide\"],function(a,b){var c=n.fn[b];n.fn[b]=function(a,d,e){return null==a||\"boolean\"==typeof a?c.apply(this,arguments):this.animate(hc(b,!0),a,d,e)}}),n.each({slideDown:hc(\"show\"),slideUp:hc(\"hide\"),slideToggle:hc(\"toggle\"),fadeIn:{opacity:\"show\"},fadeOut:{opacity:\"hide\"},fadeToggle:{opacity:\"toggle\"}},function(a,b){n.fn[a]=function(a,c,d){return this.animate(b,a,c,d)}}),n.timers=[],n.fx.tick=function(){var a,b=n.timers,c=0;for(_b=n.now();c<b.length;c++)a=b[c],a()||b[c]!==a||b.splice(c--,1);b.length||n.fx.stop(),_b=void 0},n.fx.timer=function(a){n.timers.push(a),a()?n.fx.start():n.timers.pop()},n.fx.interval=13,n.fx.start=function(){ac||(ac=setInterval(n.fx.tick,n.fx.interval))},n.fx.stop=function(){clearInterval(ac),ac=null},n.fx.speeds={slow:600,fast:200,_default:400},n.fn.delay=function(a,b){return a=n.fx?n.fx.speeds[a]||a:a,b=b||\"fx\",this.queue(b,function(b,c){var d=setTimeout(b,a);c.stop=function(){clearTimeout(d)}})},function(){var a,b,c,d,e=z.createElement(\"div\");e.setAttribute(\"className\",\"t\"),e.innerHTML=\"  <link/><table></table><a href='/a'>a</a><input type='checkbox'/>\",a=e.getElementsByTagName(\"a\")[0],c=z.createElement(\"select\"),d=c.appendChild(z.createElement(\"option\")),b=e.getElementsByTagName(\"input\")[0],a.style.cssText=\"top:1px\",l.getSetAttribute=\"t\"!==e.className,l.style=/top/.test(a.getAttribute(\"style\")),l.hrefNormalized=\"/a\"===a.getAttribute(\"href\"),l.checkOn=!!b.value,l.optSelected=d.selected,l.enctype=!!z.createElement(\"form\").enctype,c.disabled=!0,l.optDisabled=!d.disabled,b=z.createElement(\"input\"),b.setAttribute(\"value\",\"\"),l.input=\"\"===b.getAttribute(\"value\"),b.value=\"t\",b.setAttribute(\"type\",\"radio\"),l.radioValue=\"t\"===b.value,a=b=c=d=e=null}();var mc=/\\r/g;n.fn.extend({val:function(a){var b,c,d,e=this[0];{if(arguments.length)return d=n.isFunction(a),this.each(function(c){var e;1===this.nodeType&&(e=d?a.call(this,c,n(this).val()):a,null==e?e=\"\":\"number\"==typeof e?e+=\"\":n.isArray(e)&&(e=n.map(e,function(a){return null==a?\"\":a+\"\"})),b=n.valHooks[this.type]||n.valHooks[this.nodeName.toLowerCase()],b&&\"set\"in b&&void 0!==b.set(this,e,\"value\")||(this.value=e))});if(e)return b=n.valHooks[e.type]||n.valHooks[e.nodeName.toLowerCase()],b&&\"get\"in b&&void 0!==(c=b.get(e,\"value\"))?c:(c=e.value,\"string\"==typeof c?c.replace(mc,\"\"):null==c?\"\":c)}}}),n.extend({valHooks:{option:{get:function(a){var b=n.find.attr(a,\"value\");return null!=b?b:n.text(a)}},select:{get:function(a){for(var b,c,d=a.options,e=a.selectedIndex,f=\"select-one\"===a.type||0>e,g=f?null:[],h=f?e+1:d.length,i=0>e?h:f?e:0;h>i;i++)if(c=d[i],!(!c.selected&&i!==e||(l.optDisabled?c.disabled:null!==c.getAttribute(\"disabled\"))||c.parentNode.disabled&&n.nodeName(c.parentNode,\"optgroup\"))){if(b=n(c).val(),f)return b;g.push(b)}return g},set:function(a,b){var c,d,e=a.options,f=n.makeArray(b),g=e.length;while(g--)if(d=e[g],n.inArray(n.valHooks.option.get(d),f)>=0)try{d.selected=c=!0}catch(h){d.scrollHeight}else d.selected=!1;return c||(a.selectedIndex=-1),e}}}}),n.each([\"radio\",\"checkbox\"],function(){n.valHooks[this]={set:function(a,b){return n.isArray(b)?a.checked=n.inArray(n(a).val(),b)>=0:void 0}},l.checkO"
,
"n||(n.valHooks[this].get=function(a){return null===a.getAttribute(\"value\")?\"on\":a.value})});var nc,oc,pc=n.expr.attrHandle,qc=/^(?:checked|selected)$/i,rc=l.getSetAttribute,sc=l.input;n.fn.extend({attr:function(a,b){return W(this,n.attr,a,b,arguments.length>1)},removeAttr:function(a){return this.each(function(){n.removeAttr(this,a)})}}),n.extend({attr:function(a,b,c){var d,e,f=a.nodeType;if(a&&3!==f&&8!==f&&2!==f)return typeof a.getAttribute===L?n.prop(a,b,c):(1===f&&n.isXMLDoc(a)||(b=b.toLowerCase(),d=n.attrHooks[b]||(n.expr.match.bool.test(b)?oc:nc)),void 0===c?d&&\"get\"in d&&null!==(e=d.get(a,b))?e:(e=n.find.attr(a,b),null==e?void 0:e):null!==c?d&&\"set\"in d&&void 0!==(e=d.set(a,c,b))?e:(a.setAttribute(b,c+\"\"),c):void n.removeAttr(a,b))},removeAttr:function(a,b){var c,d,e=0,f=b&&b.match(F);if(f&&1===a.nodeType)while(c=f[e++])d=n.propFix[c]||c,n.expr.match.bool.test(c)?sc&&rc||!qc.test(c)?a[d]=!1:a[n.camelCase(\"default-\"+c)]=a[d]=!1:n.attr(a,c,\"\"),a.removeAttribute(rc?c:d)},attrHooks:{type:{set:function(a,b){if(!l.radioValue&&\"radio\"===b&&n.nodeName(a,\"input\")){var c=a.value;return a.setAttribute(\"type\",b),c&&(a.value=c),b}}}}}),oc={set:function(a,b,c){return b===!1?n.removeAttr(a,c):sc&&rc||!qc.test(c)?a.setAttribute(!rc&&n.propFix[c]||c,c):a[n.camelCase(\"default-\"+c)]=a[c]=!0,c}},n.each(n.expr.match.bool.source.match(/\\w+/g),function(a,b){var c=pc[b]||n.find.attr;pc[b]=sc&&rc||!qc.test(b)?function(a,b,d){var e,f;return d||(f=pc[b],pc[b]=e,e=null!=c(a,b,d)?b.toLowerCase():null,pc[b]=f),e}:function(a,b,c){return c?void 0:a[n.camelCase(\"default-\"+b)]?b.toLowerCase():null}}),sc&&rc||(n.attrHooks.value={set:function(a,b,c){return n.nodeName(a,\"input\")?void(a.defaultValue=b):nc&&nc.set(a,b,c)}}),rc||(nc={set:function(a,b,c){var d=a.getAttributeNode(c);return d||a.setAttributeNode(d=a.ownerDocument.createAttribute(c)),d.value=b+=\"\",\"value\"===c||b===a.getAttribute(c)?b:void 0}},pc.id=pc.name=pc.coords=function(a,b,c){var d;return c?void 0:(d=a.getAttributeNode(b))&&\"\"!==d.value?d.value:null},n.valHooks.button={get:function(a,b){var c=a.getAttributeNode(b);return c&&c.specified?c.value:void 0},set:nc.set},n.attrHooks.contenteditable={set:function(a,b,c){nc.set(a,\"\"===b?!1:b,c)}},n.each([\"width\",\"height\"],function(a,b){n.attrHooks[b]={set:function(a,c){return\"\"===c?(a.setAttribute(b,\"auto\"),c):void 0}}})),l.style||(n.attrHooks.style={get:function(a){return a.style.cssText||void 0},set:function(a,b){return a.style.cssText=b+\"\"}});var tc=/^(?:input|select|textarea|button|object)$/i,uc=/^(?:a|area)$/i;n.fn.extend({prop:function(a,b){return W(this,n.prop,a,b,arguments.length>1)},removeProp:function(a){return a=n.propFix[a]||a,this.each(function(){try{this[a]=void 0,delete this[a]}catch(b){}})}}),n.extend({propFix:{\"for\":\"htmlFor\",\"class\":\"className\"},prop:function(a,b,c){var d,e,f,g=a.nodeType;if(a&&3!==g&&8!==g&&2!==g)return f=1!==g||!n.isXMLDoc(a),f&&(b=n.propFix[b]||b,e=n.propHooks[b]),void 0!==c?e&&\"set\"in e&&void 0!==(d=e.set(a,c,b))?d:a[b]=c:e&&\"get\"in e&&null!==(d=e.get(a,b))?d:a[b]},propHooks:{tabIndex:{get:function(a){var b=n.find.attr(a,\"tabindex\");return b?parseInt(b,10):tc.test(a.nodeName)||uc.test(a.nodeName)&&a.href?0:-1}}}}),l.hrefNormalized||n.each([\"href\",\"src\"],function(a,b){n.propHooks[b]={get:function(a){return a.getAttribute(b,4)}}}),l.optSelected||(n.propHooks.selected={get:function(a){var b=a.parentNode;return b&&(b.selectedIndex,b.parentNode&&b.parentNode.selectedIndex),null}}),n.each([\"tabIndex\",\"readOnly\",\"maxLength\",\"cellSpacing\",\"cellPadding\",\"rowSpan\",\"colSpan\",\"useMap\",\"frameBorder\",\"contentEditable\"],function(){n.propFix[this.toLowerCase()]=this}),l.enctype||(n.propFix.enctype=\"encoding\");var vc=/[\\t\\r\\n\\f]/g;n.fn.extend({addClass:function(a){var b,c,d,e,f,g,h=0,i=this.length,j=\"string\"==typeof a&&a;if(n.isFunction(a))return this.each(function(b){n(this).addClass(a.call(this,b,this.className))});if(j)for(b=(a||\"\").match(F)||[];i>h;h++)if(c=this[h],d=1===c.nodeType&&(c.className?(\" \"+c.className+\" \").replace(vc,\" \"):\" \")){f=0;while(e=b[f++])d.indexOf(\" \"+e+\" \")<0&&(d+=e+\" \");g=n.trim(d),c.className!==g&&(c.className=g)}return this},removeClass:function(a){var b,c,d,e,f,g,h=0,i=this.length,j=0===arguments.length||\"string\"==typeof a&&a;if(n.isFunction(a))return this.each(function(b){n(this).removeClass(a.call(this,b,this.className))});if(j)for(b=(a||\"\").match(F)||[];i>h;h++)if(c=this[h],d=1===c.nodeType&&(c.className?(\" \"+c.className+\" \").replace(vc,\" \"):\"\")){f=0;while(e=b[f++])while(d.indexOf(\" \"+e+\" \")>=0)d=d.replace(\" \"+e+\" \",\" \");g=a?n.trim(d):\"\",c.className!==g&&(c.className=g)}return this},toggleClass:function(a,b){var c=typeof a;return\"boolean\"==typeof b&&\"string\"===c?b?this.addClass(a):this.removeClass(a):this.each(n.isFunction(a)?function(c){n(this).toggleClass(a.call(this,c,this.className,b),b)}:function(){if(\"string\"===c){var b,d=0,e=n(this),f=a.match(F)||[];while(b=f[d++])e.hasClass(b)?e.removeClass(b):e.addClass(b)}else(c===L||\"boolean\"===c)&&(this.className&&n._data(this,\"__className__\",this.className),this.className=this.className||a===!1?\"\":n._data(this,\"__className__\")||\"\")})},hasClass:function(a){for(var b=\" \"+a+\" \",c=0,d=this.length;d>c;c++)if(1===this[c].nodeType&&(\" \"+this[c].className+\" \").replace(vc,\" \").indexOf(b)>=0)return!0;return!1}}),n.each(\"blur focus focusin focusout load resize scroll unload click dblclick mousedown mouseup mousemove mouseover mouseout mouseenter mouseleave change select submit keydown keypress keyup error contextmenu\".split(\" \"),function(a,b){n.fn[b]=function(a,c){return arguments.length>0?this.on(b,null,a,c):this.trigger(b)}}),n.fn.extend({hover:function(a,b){return this.mouseenter(a).mouseleave(b||a)},bind:function(a,b,c){return this.on(a,null,b,c)},unbind:function(a,b){return this.off(a,null,b)},delegate:function(a,b,c,d){return this.on(b,a,c,d)},undelegate:function(a,b,c){return 1===arguments.length?this.off(a,\"**\"):this.off(b,a||\"**\",c)}});var wc=n.now(),xc=/\\?/,yc=/(,)|(\\[|{)|(}|])|\"(?:[^\"\\\\\\r\\n]|\\\\[\"\\\\\\/bfnrt]|\\\\u[\\da-fA-F]{4})*\"\\s*:?|true|false|null|-?(?!0\\d)\\d+(?:\\.\\d+|)(?:[eE][+-]?\\d+|)/g;n.parseJSON=function(b){if(a.JSON&&a.JSON.parse)return a.JSON.parse(b+\"\");var c,d=null,e=n.trim(b+\"\");return e&&!n.trim(e.replace(yc,function(a,b,e,f){return c&&b&&(d=0),0===d?a:(c=e||b,d+=!f-!e,\"\")}))?Function(\"return \"+e)():n.error(\"Invalid JSON: \"+b)},n.parseXML=function(b){var c,d;if(!b||\"string\"!=typeof b)return null;try{a.DOMParser?(d=new DOMParser,c=d.parseFromString(b,\"text/xml\")):(c=new ActiveXObject(\"Microsoft.XMLDOM\"),c.async=\"false\",c.loadXML(b))}catch(e){c=void 0}return c&&c.documentElement&&!c.getElementsByTagName(\"parsererror\").length||n.error(\"Invalid XML: \"+b),c};var zc,Ac,Bc=/#.*$/,Cc=/([?&])_=[^&]*/,Dc=/^(.*?):[ \\t]*([^\\r\\n]*)\\r?$/gm,Ec=/^(?:about|app|app-storage|.+-extension|file|res|widget):$/,Fc=/^(?:GET|HEAD)$/,Gc=/^\\/\\//,Hc=/^([\\w.+-]+:)(?:\\/\\/(?:[^\\/?#]*@|)([^\\/?#:]*)(?::(\\d+)|)|)/,Ic={},Jc={},Kc=\"*/\".concat(\"*\");try{Ac=location.href}catch(Lc){Ac=z.createElement(\"a\"),Ac.href=\"\",Ac=Ac.href}zc=Hc.exec(Ac.toLowerCase())||[];function Mc(a){return function(b,c){\"string\"!=typeof b&&(c=b,b=\"*\");var d,e=0,f=b.toLowerCase().match(F)||[];if(n.isFunction(c))while(d=f[e++])\"+\"===d.charAt(0)?(d=d.slice(1)||\"*\",(a[d]=a[d]||[]).unshift(c)):(a[d]=a[d]||[]).push(c)}}function Nc(a,b,c,d){var e={},f=a===Jc;function g(h){var i;return e[h]=!0,n.each(a[h]||[],function(a,h){var j=h(b,c,d);return\"string\"!=typeof j||f||e[j]?f?!(i=j):void 0:(b.dataTypes.unshift(j),g(j),!1)}),i}return g(b.dataTypes[0])||!e[\"*\"]&&g(\"*\")}function Oc(a,b){var c,d,e=n.ajaxSettings.flatOptions||{};for(d in b)void 0!==b[d]&&((e[d]?a:c||(c={}))[d]=b[d]);return c&&n.extend(!0,a,c),a}function Pc(a,b,c){var d,e,f,g,h=a.contents,i=a.dataTypes;while(\"*\"===i[0])i.shift(),void 0===e&&(e=a.mimeType||b.getResponseHeader(\"Content-Type\"));if(e)for(g in h)if(h[g]&&h[g].test(e)){i.unshift(g);break}if(i[0]in c)f=i[0];else{for(g in c){if(!i[0]||a.converters[g+\" \"+i[0]]){f=g;break}d||(d=g)}f=f||d}return f?(f!==i[0]&&i.unshift(f),c[f]):void 0}function Qc(a,b,c,d){var e,f,g,h,i,j={},k=a.dataTypes.slice();if(k[1])for(g in a.converters)j[g.toLowerCase()]=a.converters[g];f=k.shift();while(f)if(a.responseFields[f]&&(c[a.responseFields[f]]=b),!i&&d&&a.dataFilter&&(b=a.dataFilter(b,a.dataType)),i=f,f=k.shift())if(\"*\"===f)f=i;else if(\"*\"!==i&&i!==f){if(g=j[i+\" \"+f]||j[\"* \"+f],!g)for(e in j)if(h=e.split(\" \"),h[1]===f&&(g=j[i+\" \"+h[0]]||j[\"* \"+h[0]])){g===!0?g=j[e]:j[e]!==!0&&(f=h[0],k.unshift(h[1]));break}if(g!==!0)if(g&&a[\"throws\"])b=g(b);else try{b=g(b)}catch(l){return{state:\"parsererror\",error:g?l:\"No conversion from \"+i+\" to \"+f}}}return{state:\"success\",data:b}}n.extend({active:0,lastModified:{},etag:{},ajaxSettings:{url:Ac,type:\"GET\",isLocal:Ec.test(zc[1]),global:!0,processData:!0,async:!0,contentType:\"application/x-www-form-urlencoded; charset=UTF-8\",accepts:{\"*\":Kc,text:\"text/plain\",html:\"text/html\",xml:\"application/xml, text/xml\",json:\"application/json, text/javascript\"},contents:{xml:/xml/,html:/html/,json:/json/},responseFields:{xml:\"responseXML\",text:\"responseText\",json:\"responseJSON\"},converters:{\"* text\":String,\"text html\":!0,\"text json\":n.parseJSON,\"text xml\":n.parseXML},flatOptions:{url:!0,context:!0}},ajaxSetup:function(a,b){return b?Oc(Oc(a,n.ajaxSettings),b):Oc(n.ajaxSettings,a)},ajaxPrefilter:Mc(Ic),ajaxTransport:Mc(Jc),ajax:function(a,b){\"object\"==typeof a&&(b=a,a=void 0),b=b||{};var c,d,e,f,g,h,i,j,k=n.ajaxSetup({},b),l=k.context||k,m=k.context&&(l.nodeType||l.jquery)?n(l):n.event,o=n.Deferred(),p=n.Callbacks(\"once memory\"),q=k.statusCode||{},r={},s={},t=0,u=\"canceled\",v={readyState:0,getResponseHeader:function(a){var b;if(2===t){if(!j){j={};while(b=Dc.exec(f))j[b[1].toLowerCase()]=b[2]}b=j[a.toLowerCase()]}return null==b?null:b},getAllResponseHeaders:function(){return 2===t?f:null},setRequestHeader:function(a,b){var c=a.toLowerCase();return t||(a=s[c]=s[c]||a,r[a]=b),this},overrideMimeType:function(a){return t||(k.mimeType=a),this},statusCode:function(a){var b;if(a)if(2>t)for(b in"
,
" a)q[b]=[q[b],a[b]];else v.always(a[v.status]);return this},abort:function(a){var b=a||u;return i&&i.abort(b),x(0,b),this}};if(o.promise(v).complete=p.add,v.success=v.done,v.error=v.fail,k.url=((a||k.url||Ac)+\"\").replace(Bc,\"\").replace(Gc,zc[1]+\"//\"),k.type=b.method||b.type||k.method||k.type,k.dataTypes=n.trim(k.dataType||\"*\").toLowerCase().match(F)||[\"\"],null==k.crossDomain&&(c=Hc.exec(k.url.toLowerCase()),k.crossDomain=!(!c||c[1]===zc[1]&&c[2]===zc[2]&&(c[3]||(\"http:\"===c[1]?\"80\":\"443\"))===(zc[3]||(\"http:\"===zc[1]?\"80\":\"443\")))),k.data&&k.processData&&\"string\"!=typeof k.data&&(k.data=n.param(k.data,k.traditional)),Nc(Ic,k,b,v),2===t)return v;h=k.global,h&&0===n.active++&&n.event.trigger(\"ajaxStart\"),k.type=k.type.toUpperCase(),k.hasContent=!Fc.test(k.type),e=k.url,k.hasContent||(k.data&&(e=k.url+=(xc.test(e)?\"&\":\"?\")+k.data,delete k.data),k.cache===!1&&(k.url=Cc.test(e)?e.replace(Cc,\"$1_=\"+wc++):e+(xc.test(e)?\"&\":\"?\")+\"_=\"+wc++)),k.ifModified&&(n.lastModified[e]&&v.setRequestHeader(\"If-Modified-Since\",n.lastModified[e]),n.etag[e]&&v.setRequestHeader(\"If-None-Match\",n.etag[e])),(k.data&&k.hasContent&&k.contentType!==!1||b.contentType)&&v.setRequestHeader(\"Content-Type\",k.contentType),v.setRequestHeader(\"Accept\",k.dataTypes[0]&&k.accepts[k.dataTypes[0]]?k.accepts[k.dataTypes[0]]+(\"*\"!==k.dataTypes[0]?\", \"+Kc+\"; q=0.01\":\"\"):k.accepts[\"*\"]);for(d in k.headers)v.setRequestHeader(d,k.headers[d]);if(k.beforeSend&&(k.beforeSend.call(l,v,k)===!1||2===t))return v.abort();u=\"abort\";for(d in{success:1,error:1,complete:1})v[d](k[d]);if(i=Nc(Jc,k,b,v)){v.readyState=1,h&&m.trigger(\"ajaxSend\",[v,k]),k.async&&k.timeout>0&&(g=setTimeout(function(){v.abort(\"timeout\")},k.timeout));try{t=1,i.send(r,x)}catch(w){if(!(2>t))throw w;x(-1,w)}}else x(-1,\"No Transport\");function x(a,b,c,d){var j,r,s,u,w,x=b;2!==t&&(t=2,g&&clearTimeout(g),i=void 0,f=d||\"\",v.readyState=a>0?4:0,j=a>=200&&300>a||304===a,c&&(u=Pc(k,v,c)),u=Qc(k,u,v,j),j?(k.ifModified&&(w=v.getResponseHeader(\"Last-Modified\"),w&&(n.lastModified[e]=w),w=v.getResponseHeader(\"etag\"),w&&(n.etag[e]=w)),204===a||\"HEAD\"===k.type?x=\"nocontent\":304===a?x=\"notmodified\":(x=u.state,r=u.data,s=u.error,j=!s)):(s=x,(a||!x)&&(x=\"error\",0>a&&(a=0))),v.status=a,v.statusText=(b||x)+\"\",j?o.resolveWith(l,[r,x,v]):o.rejectWith(l,[v,x,s]),v.statusCode(q),q=void 0,h&&m.trigger(j?\"ajaxSuccess\":\"ajaxError\",[v,k,j?r:s]),p.fireWith(l,[v,x]),h&&(m.trigger(\"ajaxComplete\",[v,k]),--n.active||n.event.trigger(\"ajaxStop\")))}return v},getJSON:function(a,b,c){return n.get(a,b,c,\"json\")},getScript:function(a,b){return n.get(a,void 0,b,\"script\")}}),n.each([\"get\",\"post\"],function(a,b){n[b]=function(a,c,d,e){return n.isFunction(c)&&(e=e||d,d=c,c=void 0),n.ajax({url:a,type:b,dataType:e,data:c,success:d})}}),n.each([\"ajaxStart\",\"ajaxStop\",\"ajaxComplete\",\"ajaxError\",\"ajaxSuccess\",\"ajaxSend\"],function(a,b){n.fn[b]=function(a){return this.on(b,a)}}),n._evalUrl=function(a){return n.ajax({url:a,type:\"GET\",dataType:\"script\",async:!1,global:!1,\"throws\":!0})},n.fn.extend({wrapAll:function(a){if(n.isFunction(a))return this.each(function(b){n(this).wrapAll(a.call(this,b))});if(this[0]){var b=n(a,this[0].ownerDocument).eq(0).clone(!0);this[0].parentNode&&b.insertBefore(this[0]),b.map(function(){var a=this;while(a.firstChild&&1===a.firstChild.nodeType)a=a.firstChild;return a}).append(this)}return this},wrapInner:function(a){return this.each(n.isFunction(a)?function(b){n(this).wrapInner(a.call(this,b))}:function(){var b=n(this),c=b.contents();c.length?c.wrapAll(a):b.append(a)})},wrap:function(a){var b=n.isFunction(a);return this.each(function(c){n(this).wrapAll(b?a.call(this,c):a)})},unwrap:function(){return this.parent().each(function(){n.nodeName(this,\"body\")||n(this).replaceWith(this.childNodes)}).end()}}),n.expr.filters.hidden=function(a){return a.offsetWidth<=0&&a.offsetHeight<=0||!l.reliableHiddenOffsets()&&\"none\"===(a.style&&a.style.display||n.css(a,\"display\"))},n.expr.filters.visible=function(a){return!n.expr.filters.hidden(a)};var Rc=/%20/g,Sc=/\\[\\]$/,Tc=/\\r?\\n/g,Uc=/^(?:submit|button|image|reset|file)$/i,Vc=/^(?:input|select|textarea|keygen)/i;function Wc(a,b,c,d){var e;if(n.isArray(b))n.each(b,function(b,e){c||Sc.test(a)?d(a,e):Wc(a+\"[\"+(\"object\"==typeof e?b:\"\")+\"]\",e,c,d)});else if(c||\"object\"!==n.type(b))d(a,b);else for(e in b)Wc(a+\"[\"+e+\"]\",b[e],c,d)}n.param=function(a,b){var c,d=[],e=function(a,b){b=n.isFunction(b)?b():null==b?\"\":b,d[d.length]=encodeURIComponent(a)+\"=\"+encodeURIComponent(b)};if(void 0===b&&(b=n.ajaxSettings&&n.ajaxSettings.traditional),n.isArray(a)||a.jquery&&!n.isPlainObject(a))n.each(a,function(){e(this.name,this.value)});else for(c in a)Wc(c,a[c],b,e);return d.join(\"&\").replace(Rc,\"+\")},n.fn.extend({serialize:function(){return n.param(this.serializeArray())},serializeArray:function(){return this.map(function(){var a=n.prop(this,\"elements\");return a?n.makeArray(a):this}).filter(function(){var a=this.type;return this.name&&!n(this).is(\":disabled\")&&Vc.test(this.nodeName)&&!Uc.test(a)&&(this.checked||!X.test(a))}).map(function(a,b){var c=n(this).val();return null==c?null:n.isArray(c)?n.map(c,function(a){return{name:b.name,value:a.replace(Tc,\"\\r\\n\")}}):{name:b.name,value:c.replace(Tc,\"\\r\\n\")}}).get()}}),n.ajaxSettings.xhr=void 0!==a.ActiveXObject?function(){return!this.isLocal&&/^(get|post|head|put|delete|options)$/i.test(this.type)&&$c()||_c()}:$c;var Xc=0,Yc={},Zc=n.ajaxSettings.xhr();a.ActiveXObject&&n(a).on(\"unload\",function(){for(var a in Yc)Yc[a](void 0,!0)}),l.cors=!!Zc&&\"withCredentials\"in Zc,Zc=l.ajax=!!Zc,Zc&&n.ajaxTransport(function(a){if(!a.crossDomain||l.cors){var b;return{send:function(c,d){var e,f=a.xhr(),g=++Xc;if(f.open(a.type,a.url,a.async,a.username,a.password),a.xhrFields)for(e in a.xhrFields)f[e]=a.xhrFields[e];a.mimeType&&f.overrideMimeType&&f.overrideMimeType(a.mimeType),a.crossDomain||c[\"X-Requested-With\"]||(c[\"X-Requested-With\"]=\"XMLHttpRequest\");for(e in c)void 0!==c[e]&&f.setRequestHeader(e,c[e]+\"\");f.send(a.hasContent&&a.data||null),b=function(c,e){var h,i,j;if(b&&(e||4===f.readyState))if(delete Yc[g],b=void 0,f.onreadystatechange=n.noop,e)4!==f.readyState&&f.abort();else{j={},h=f.status,\"string\"==typeof f.responseText&&(j.text=f.responseText);try{i=f.statusText}catch(k){i=\"\"}h||!a.isLocal||a.crossDomain?1223===h&&(h=204):h=j.text?200:404}j&&d(h,i,j,f.getAllResponseHeaders())},a.async?4===f.readyState?setTimeout(b):f.onreadystatechange=Yc[g]=b:b()},abort:function(){b&&b(void 0,!0)}}}});function $c(){try{return new a.XMLHttpRequest}catch(b){}}function _c(){try{return new a.ActiveXObject(\"Microsoft.XMLHTTP\")}catch(b){}}n.ajaxSetup({accepts:{script:\"text/javascript, application/javascript, application/ecmascript, application/x-ecmascript\"},contents:{script:/(?:java|ecma)script/},converters:{\"text script\":function(a){return n.globalEval(a),a}}}),n.ajaxPrefilter(\"script\",function(a){void 0===a.cache&&(a.cache=!1),a.crossDomain&&(a.type=\"GET\",a.global=!1)}),n.ajaxTransport(\"script\",function(a){if(a.crossDomain){var b,c=z.head||n(\"head\")[0]||z.documentElement;return{send:function(d,e){b=z.createElement(\"script\"),b.async=!0,a.scriptCharset&&(b.charset=a.scriptCharset),b.src=a.url,b.onload=b.onreadystatechange=function(a,c){(c||!b.readyState||/loaded|complete/.test(b.readyState))&&(b.onload=b.onreadystatechange=null,b.parentNode&&b.parentNode.removeChild(b),b=null,c||e(200,\"success\"))},c.insertBefore(b,c.firstChild)},abort:function(){b&&b.onload(void 0,!0)}}}});var ad=[],bd=/(=)\\?(?=&|$)|\\?\\?/;n.ajaxSetup({jsonp:\"callback\",jsonpCallback:function(){var a=ad.pop()||n.expando+\"_\"+wc++;return this[a]=!0,a}}),n.ajaxPrefilter(\"json jsonp\",function(b,c,d){var e,f,g,h=b.jsonp!==!1&&(bd.test(b.url)?\"url\":\"string\"==typeof b.data&&!(b.contentType||\"\").indexOf(\"application/x-www-form-urlencoded\")&&bd.test(b.data)&&\"data\");return h||\"jsonp\"===b.dataTypes[0]?(e=b.jsonpCallback=n.isFunction(b.jsonpCallback)?b.jsonpCallback():b.jsonpCallback,h?b[h]=b[h].replace(bd,\"$1\"+e):b.jsonp!==!1&&(b.url+=(xc.test(b.url)?\"&\":\"?\")+b.jsonp+\"=\"+e),b.converters[\"script json\"]=function(){return g||n.error(e+\" was not called\"),g[0]},b.dataTypes[0]=\"json\",f=a[e],a[e]=function(){g=arguments},d.always(function(){a[e]=f,b[e]&&(b.jsonpCallback=c.jsonpCallback,ad.push(e)),g&&n.isFunction(f)&&f(g[0]),g=f=void 0}),\"script\"):void 0}),n.parseHTML=function(a,b,c){if(!a||\"string\"!=typeof a)return null;\"boolean\"==typeof b&&(c=b,b=!1),b=b||z;var d=v.exec(a),e=!c&&[];return d?[b.createElement(d[1])]:(d=n.buildFragment([a],b,e),e&&e.length&&n(e).remove(),n.merge([],d.childNodes))};var cd=n.fn.load;n.fn.load=function(a,b,c){if(\"string\"!=typeof a&&cd)return cd.apply(this,arguments);var d,e,f,g=this,h=a.indexOf(\" \");return h>=0&&(d=a.slice(h,a.length),a=a.slice(0,h)),n.isFunction(b)?(c=b,b=void 0):b&&\"object\"==typeof b&&(f=\"POST\"),g.length>0&&n.ajax({url:a,type:f,dataType:\"html\",data:b}).done(function(a){e=arguments,g.html(d?n(\"<div>\").append(n.parseHTML(a)).find(d):a)}).complete(c&&function(a,b){g.each(c,e||[a.responseText,b,a])}),this},n.expr.filters.animated=function(a){return n.grep(n.timers,function(b){return a===b.elem}).length};var dd=a.document.documentElement;function ed(a){return n.isWindow(a)?a:9===a.nodeType?a.defaultView||a.parentWindow:!1}n.offset={setOffset:function(a,b,c){var d,e,f,g,h,i,j,k=n.css(a,\"position\"),l=n(a),m={};\"static\"===k&&(a.style.position=\"relative\"),h=l.offset(),f=n.css(a,\"top\"),i=n.css(a,\"left\"),j=(\"absolute\"===k||\"fixed\"===k)&&n.inArray(\"auto\",[f,i])>-1,j?(d=l.position(),g=d.top,e=d.left):(g=parseFloat(f)||0,e=parseFloat(i)||0),n.isFunction(b)&&(b=b.call(a,c,h)),null!=b.top&&(m.top=b.top-h.top+g),null!=b.left&&(m.left=b.left-h.left+e),\"using\"in b?b.using.call(a,m):l.css(m)}},n.fn.extend({offset:function(a){if(arguments.length)return void 0===a?this:this.each(function(b){n.offset.setOffset(this,a,b)});var b,c,d={top:0,left:0},e=this[0],f=e&&e.ownerDocument;if(f)return b=f.documentElement,n.contains(b,e)?(typeof e.getBoundingClientRect!==L&&(d=e.getBoundingClientRect()),c=ed(f),{top:d.top+(c.pageYOffs"
,
"et||b.scrollTop)-(b.clientTop||0),left:d.left+(c.pageXOffset||b.scrollLeft)-(b.clientLeft||0)}):d},position:function(){if(this[0]){var a,b,c={top:0,left:0},d=this[0];return\"fixed\"===n.css(d,\"position\")?b=d.getBoundingClientRect():(a=this.offsetParent(),b=this.offset(),n.nodeName(a[0],\"html\")||(c=a.offset()),c.top+=n.css(a[0],\"borderTopWidth\",!0),c.left+=n.css(a[0],\"borderLeftWidth\",!0)),{top:b.top-c.top-n.css(d,\"marginTop\",!0),left:b.left-c.left-n.css(d,\"marginLeft\",!0)}}},offsetParent:function(){return this.map(function(){var a=this.offsetParent||dd;while(a&&!n.nodeName(a,\"html\")&&\"static\"===n.css(a,\"position\"))a=a.offsetParent;return a||dd})}}),n.each({scrollLeft:\"pageXOffset\",scrollTop:\"pageYOffset\"},function(a,b){var c=/Y/.test(b);n.fn[a]=function(d){return W(this,function(a,d,e){var f=ed(a);return void 0===e?f?b in f?f[b]:f.document.documentElement[d]:a[d]:void(f?f.scrollTo(c?n(f).scrollLeft():e,c?e:n(f).scrollTop()):a[d]=e)},a,d,arguments.length,null)}}),n.each([\"top\",\"left\"],function(a,b){n.cssHooks[b]=Mb(l.pixelPosition,function(a,c){return c?(c=Kb(a,b),Ib.test(c)?n(a).position()[b]+\"px\":c):void 0})}),n.each({Height:\"height\",Width:\"width\"},function(a,b){n.each({padding:\"inner\"+a,content:b,\"\":\"outer\"+a},function(c,d){n.fn[d]=function(d,e){var f=arguments.length&&(c||\"boolean\"!=typeof d),g=c||(d===!0||e===!0?\"margin\":\"border\");return W(this,function(b,c,d){var e;return n.isWindow(b)?b.document.documentElement[\"client\"+a]:9===b.nodeType?(e=b.documentElement,Math.max(b.body[\"scroll\"+a],e[\"scroll\"+a],b.body[\"offset\"+a],e[\"offset\"+a],e[\"client\"+a])):void 0===d?n.css(b,c,g):n.style(b,c,d,g)},b,f?d:void 0,f,null)}})}),n.fn.size=function(){return this.length},n.fn.andSelf=n.fn.addBack,\"function\"==typeof define&&define.amd&&define(\"jquery\",[],function(){return n});var fd=a.jQuery,gd=a.$;return n.noConflict=function(b){return a.$===n&&(a.$=gd),b&&a.jQuery===n&&(a.jQuery=fd),n},typeof b===L&&(a.jQuery=a.$=n),n});\n"
"  </script>\n"
"  <script type=\"text/javascript\">\n"
"/*\n"
" Highcharts JS v3.0.9 (2014-01-15)\n"
"\n"
" (c) 2009-2014 Torstein Honsi\n"
"\n"
" License: www.highcharts.com/license\n"
"*/\n"
"(function(){function r(a,b){var c;a||(a={});for(c in b)a[c]=b[c];return a}function x(){var a,b=arguments,c,d={},e=function(a,b){var c,d;typeof a!==\"object\"&&(a={});for(d in b)b.hasOwnProperty(d)&&(c=b[d],a[d]=c&&typeof c===\"object\"&&Object.prototype.toString.call(c)!==\"[object Array]\"&&typeof c.nodeType!==\"number\"?e(a[d]||{},c):b[d]);return a};b[0]===!0&&(d=b[1],b=Array.prototype.slice.call(b,2));c=b.length;for(a=0;a<c;a++)d=e(d,b[a]);return d}function z(a,b){return parseInt(a,b||10)}function fa(a){return typeof a===\n"
"\"string\"}function S(a){return typeof a===\"object\"}function Ka(a){return Object.prototype.toString.call(a)===\"[object Array]\"}function wa(a){return typeof a===\"number\"}function xa(a){return P.log(a)/P.LN10}function ga(a){return P.pow(10,a)}function ha(a,b){for(var c=a.length;c--;)if(a[c]===b){a.splice(c,1);break}}function t(a){return a!==u&&a!==null}function v(a,b,c){var d,e;if(fa(b))t(c)?a.setAttribute(b,c):a&&a.getAttribute&&(e=a.getAttribute(b));else if(t(b)&&S(b))for(d in b)a.setAttribute(d,b[d]);\n"
"return e}function ja(a){return Ka(a)?a:[a]}function n(){var a=arguments,b,c,d=a.length;for(b=0;b<d;b++)if(c=a[b],typeof c!==\"undefined\"&&c!==null)return c}function D(a,b){if(ya&&b&&b.opacity!==u)b.filter=\"alpha(opacity=\"+b.opacity*100+\")\";r(a.style,b)}function T(a,b,c,d,e){a=y.createElement(a);b&&r(a,b);e&&D(a,{padding:0,border:Q,margin:0});c&&D(a,c);d&&d.appendChild(a);return a}function ia(a,b){var c=function(){};c.prototype=new a;r(c.prototype,b);return c}function Da(a,b,c,d){var e=G.lang,a=+a||\n"
"0,f=b===-1?(a.toString().split(\".\")[1]||\"\").length:isNaN(b=M(b))?2:b,b=c===void 0?e.decimalPoint:c,d=d===void 0?e.thousandsSep:d,e=a<0?\"-\":\"\",c=String(z(a=M(a).toFixed(f))),g=c.length>3?c.length%3:0;return e+(g?c.substr(0,g)+d:\"\")+c.substr(g).replace(/(\\d{3})(?=\\d)/g,\"$1\"+d)+(f?b+M(a-c).toFixed(f).slice(2):\"\")}function Ea(a,b){return Array((b||2)+1-String(a).length).join(0)+a}function Va(a,b,c){var d=a[b];a[b]=function(){var a=Array.prototype.slice.call(arguments);a.unshift(d);return c.apply(this,\n"
"a)}}function Fa(a,b){for(var c=\"{\",d=!1,e,f,g,h,i,j=[];(c=a.indexOf(c))!==-1;){e=a.slice(0,c);if(d){f=e.split(\":\");g=f.shift().split(\".\");i=g.length;e=b;for(h=0;h<i;h++)e=e[g[h]];if(f.length)f=f.join(\":\"),g=/\\.([0-9])/,h=G.lang,i=void 0,/f$/.test(f)?(i=(i=f.match(g))?i[1]:-1,e=Da(e,i,h.decimalPoint,f.indexOf(\",\")>-1?h.thousandsSep:\"\")):e=ab(f,e)}j.push(e);a=a.slice(c+1);c=(d=!d)?\"}\":\"{\"}j.push(a);return j.join(\"\")}function mb(a){return P.pow(10,N(P.log(a)/P.LN10))}function nb(a,b,c,d){var e,c=n(c,\n"
"1);e=a/c;b||(b=[1,2,2.5,5,10],d&&d.allowDecimals===!1&&(c===1?b=[1,2,5,10]:c<=0.1&&(b=[1/c])));for(d=0;d<b.length;d++)if(a=b[d],e<=(b[d]+(b[d+1]||b[d]))/2)break;a*=c;return a}function Ab(){this.symbol=this.color=0}function ob(a,b){var c=a.length,d,e;for(e=0;e<c;e++)a[e].ss_i=e;a.sort(function(a,c){d=b(a,c);return d===0?a.ss_i-c.ss_i:d});for(e=0;e<c;e++)delete a[e].ss_i}function La(a){for(var b=a.length,c=a[0];b--;)a[b]<c&&(c=a[b]);return c}function za(a){for(var b=a.length,c=a[0];b--;)a[b]>c&&(c=\n"
"a[b]);return c}function Ma(a,b){for(var c in a)a[c]&&a[c]!==b&&a[c].destroy&&a[c].destroy(),delete a[c]}function Na(a){bb||(bb=T(Ga));a&&bb.appendChild(a);bb.innerHTML=\"\"}function ka(a,b){var c=\"Highcharts error #\"+a+\": www.highcharts.com/errors/\"+a;if(b)throw c;else C.console&&console.log(c)}function aa(a){return parseFloat(a.toPrecision(14))}function Oa(a,b){oa=n(a,b.animation)}function Bb(){var a=G.global.useUTC,b=a?\"getUTC\":\"get\",c=a?\"setUTC\":\"set\";Pa=(a&&G.global.timezoneOffset||0)*6E4;cb=a?\n"
"Date.UTC:function(a,b,c,g,h,i){return(new Date(a,b,n(c,1),n(g,0),n(h,0),n(i,0))).getTime()};pb=b+\"Minutes\";qb=b+\"Hours\";rb=b+\"Day\";Wa=b+\"Date\";db=b+\"Month\";eb=b+\"FullYear\";Cb=c+\"Minutes\";Db=c+\"Hours\";sb=c+\"Date\";Eb=c+\"Month\";Fb=c+\"FullYear\"}function pa(){}function Qa(a,b,c,d){this.axis=a;this.pos=b;this.type=c||\"\";this.isNew=!0;!c&&!d&&this.addLabel()}function qa(){this.init.apply(this,arguments)}function Gb(a,b,c,d,e,f){var g=a.chart.inverted;this.axis=a;this.isNegative=c;this.options=b;this.x=d;\n"
"this.total=null;this.points={};this.stack=e;this.percent=f===\"percent\";this.alignOptions={align:b.align||(g?c?\"left\":\"right\":\"center\"),verticalAlign:b.verticalAlign||(g?\"middle\":c?\"bottom\":\"top\"),y:n(b.y,g?4:c?14:-6),x:n(b.x,g?c?-6:6:0)};this.textAlign=b.textAlign||(g?c?\"right\":\"left\":\"center\")}function tb(){this.init.apply(this,arguments)}function fb(){this.init.apply(this,arguments)}var u,y=document,C=window,P=Math,w=P.round,N=P.floor,Ha=P.ceil,s=P.max,I=P.min,M=P.abs,U=P.cos,ba=P.sin,Aa=P.PI,Ba=\n"
"Aa*2/360,ra=navigator.userAgent,Hb=C.opera,ya=/msie/i.test(ra)&&!Hb,gb=y.documentMode===8,hb=/AppleWebKit/.test(ra),Xa=/Firefox/.test(ra),Ib=/(Mobile|Android|Windows Phone)/.test(ra),Ca=\"http://www.w3.org/2000/svg\",V=!!y.createElementNS&&!!y.createElementNS(Ca,\"svg\").createSVGRect,Nb=Xa&&parseInt(ra.split(\"Firefox/\")[1],10)<4,da=!V&&!ya&&!!y.createElement(\"canvas\").getContext,Ya,ib=y.documentElement.ontouchstart!==u,Jb={},ub=0,bb,G,ab,oa,vb,E,la=function(){},Ia=[],Ga=\"div\",Q=\"none\",Ob=/^[0-9]+$/,\n"
"Kb=\"rgba(192,192,192,\"+(V?1.0E-4:0.002)+\")\",Lb=\"stroke-width\",cb,Pa,pb,qb,rb,Wa,db,eb,Cb,Db,sb,Eb,Fb,L={};C.Highcharts=C.Highcharts?ka(16,!0):{};ab=function(a,b,c){if(!t(b)||isNaN(b))return\"Invalid date\";var a=n(a,\"%Y-%m-%d %H:%M:%S\"),d=new Date(b-Pa),e,f=d[qb](),g=d[rb](),h=d[Wa](),i=d[db](),j=d[eb](),k=G.lang,l=k.weekdays,d=r({a:l[g].substr(0,3),A:l[g],d:Ea(h),e:h,b:k.shortMonths[i],B:k.months[i],m:Ea(i+1),y:j.toString().substr(2,2),Y:j,H:Ea(f),I:Ea(f%12||12),l:f%12||12,M:Ea(d[pb]()),p:f<12?\"AM\":\n"
"\"PM\",P:f<12?\"am\":\"pm\",S:Ea(d.getSeconds()),L:Ea(w(b%1E3),3)},Highcharts.dateFormats);for(e in d)for(;a.indexOf(\"%\"+e)!==-1;)a=a.replace(\"%\"+e,typeof d[e]===\"function\"?d[e](b):d[e]);return c?a.substr(0,1).toUpperCase()+a.substr(1):a};Ab.prototype={wrapColor:function(a){if(this.color>=a)this.color=0},wrapSymbol:function(a){if(this.symbol>=a)this.symbol=0}};E=function(){for(var a=0,b=arguments,c=b.length,d={};a<c;a++)d[b[a++]]=b[a];return d}(\"millisecond\",1,\"second\",1E3,\"minute\",6E4,\"hour\",36E5,\"day\",\n"
"864E5,\"week\",6048E5,\"month\",26784E5,\"year\",31556952E3);vb={init:function(a,b,c){var b=b||\"\",d=a.shift,e=b.indexOf(\"C\")>-1,f=e?7:3,g,b=b.split(\" \"),c=[].concat(c),h,i,j=function(a){for(g=a.length;g--;)a[g]===\"M\"&&a.splice(g+1,0,a[g+1],a[g+2],a[g+1],a[g+2])};e&&(j(b),j(c));a.isArea&&(h=b.splice(b.length-6,6),i=c.splice(c.length-6,6));if(d<=c.length/f&&b.length===c.length)for(;d--;)c=[].concat(c).splice(0,f).concat(c);a.shift=0;if(b.length)for(a=c.length;b.length<a;)d=[].concat(b).splice(b.length-f,\n"
"f),e&&(d[f-6]=d[f-2],d[f-5]=d[f-1]),b=b.concat(d);h&&(b=b.concat(h),c=c.concat(i));return[b,c]},step:function(a,b,c,d){var e=[],f=a.length;if(c===1)e=d;else if(f===b.length&&c<1)for(;f--;)d=parseFloat(a[f]),e[f]=isNaN(d)?a[f]:c*parseFloat(b[f]-d)+d;else e=b;return e}};(function(a){C.HighchartsAdapter=C.HighchartsAdapter||a&&{init:function(b){var c=a.fx,d=c.step,e,f=a.Tween,g=f&&f.propHooks;e=a.cssHooks.opacity;a.extend(a.easing,{easeOutQuad:function(a,b,c,d,e){return-d*(b/=e)*(b-2)+c}});a.each([\"cur\",\n"
,
"\"_default\",\"width\",\"height\",\"opacity\"],function(a,b){var e=d,k;b===\"cur\"?e=c.prototype:b===\"_default\"&&f&&(e=g[b],b=\"set\");(k=e[b])&&(e[b]=function(c){var d,c=a?c:this;if(c.prop!==\"align\")return d=c.elem,d.attr?d.attr(c.prop,b===\"cur\"?u:c.now):k.apply(this,arguments)})});Va(e,\"get\",function(a,b,c){return b.attr?b.opacity||0:a.call(this,b,c)});e=function(a){var c=a.elem,d;if(!a.started)d=b.init(c,c.d,c.toD),a.start=d[0],a.end=d[1],a.started=!0;c.attr(\"d\",b.step(a.start,a.end,a.pos,c.toD))};f?g.d={set:e}:\n"
"d.d=e;this.each=Array.prototype.forEach?function(a,b){return Array.prototype.forEach.call(a,b)}:function(a,b){for(var c=0,d=a.length;c<d;c++)if(b.call(a[c],a[c],c,a)===!1)return c};a.fn.highcharts=function(){var a=\"Chart\",b=arguments,c,d;fa(b[0])&&(a=b[0],b=Array.prototype.slice.call(b,1));c=b[0];if(c!==u)c.chart=c.chart||{},c.chart.renderTo=this[0],new Highcharts[a](c,b[1]),d=this;c===u&&(d=Ia[v(this[0],\"data-highcharts-chart\")]);return d}},getScript:a.getScript,inArray:a.inArray,adapterRun:function(b,\n"
"c){return a(b)[c]()},grep:a.grep,map:function(a,c){for(var d=[],e=0,f=a.length;e<f;e++)d[e]=c.call(a[e],a[e],e,a);return d},offset:function(b){return a(b).offset()},addEvent:function(b,c,d){a(b).bind(c,d)},removeEvent:function(b,c,d){var e=y.removeEventListener?\"removeEventListener\":\"detachEvent\";y[e]&&b&&!b[e]&&(b[e]=function(){});a(b).unbind(c,d)},fireEvent:function(b,c,d,e){var f=a.Event(c),g=\"detached\"+c,h;!ya&&d&&(delete d.layerX,delete d.layerY);r(f,d);b[c]&&(b[g]=b[c],b[c]=null);a.each([\"preventDefault\",\n"
"\"stopPropagation\"],function(a,b){var c=f[b];f[b]=function(){try{c.call(f)}catch(a){b===\"preventDefault\"&&(h=!0)}}});a(b).trigger(f);b[g]&&(b[c]=b[g],b[g]=null);e&&!f.isDefaultPrevented()&&!h&&e(f)},washMouseEvent:function(a){var c=a.originalEvent||a;if(c.pageX===u)c.pageX=a.pageX,c.pageY=a.pageY;return c},animate:function(b,c,d){var e=a(b);if(!b.style)b.style={};if(c.d)b.toD=c.d,c.d=1;e.stop();c.opacity!==u&&b.attr&&(c.opacity+=\"px\");e.animate(c,d)},stop:function(b){a(b).stop()}}})(C.jQuery);var W=\n"
"C.HighchartsAdapter,J=W||{};W&&W.init.call(W,vb);var jb=J.adapterRun,Pb=J.getScript,sa=J.inArray,p=J.each,wb=J.grep,Qb=J.offset,Ra=J.map,F=J.addEvent,X=J.removeEvent,A=J.fireEvent,Rb=J.washMouseEvent,kb=J.animate,Za=J.stop,J={enabled:!0,x:0,y:15,style:{color:\"#666\",cursor:\"default\",fontSize:\"11px\"}};G={colors:\"#2f7ed8,#0d233a,#8bbc21,#910000,#1aadce,#492970,#f28f43,#77a1e5,#c42525,#a6c96a\".split(\",\"),symbols:[\"circle\",\"diamond\",\"square\",\"triangle\",\"triangle-down\"],lang:{loading:\"Loading...\",months:\"January,February,March,April,May,June,July,August,September,October,November,December\".split(\",\"),\n"
"shortMonths:\"Jan,Feb,Mar,Apr,May,Jun,Jul,Aug,Sep,Oct,Nov,Dec\".split(\",\"),weekdays:\"Sunday,Monday,Tuesday,Wednesday,Thursday,Friday,Saturday\".split(\",\"),decimalPoint:\".\",numericSymbols:\"k,M,G,T,P,E\".split(\",\"),resetZoom:\"Reset zoom\",resetZoomTitle:\"Reset zoom level 1:1\",thousandsSep:\",\"},global:{useUTC:!0,canvasToolsURL:\"http://code.highcharts.com/3.0.9/modules/canvas-tools.js\",VMLRadialGradientURL:\"http://code.highcharts.com/3.0.9/gfx/vml-radial-gradient.png\"},chart:{borderColor:\"#4572A7\",borderRadius:5,\n"
"defaultSeriesType:\"line\",ignoreHiddenSeries:!0,spacing:[10,10,15,10],style:{fontFamily:'\"Lucida Grande\", \"Lucida Sans Unicode\", Verdana, Arial, Helvetica, sans-serif',fontSize:\"12px\"},backgroundColor:\"#FFFFFF\",plotBorderColor:\"#C0C0C0\",resetZoomButton:{theme:{zIndex:20},position:{align:\"right\",x:-10,y:10}}},title:{text:\"Chart title\",align:\"center\",margin:15,style:{color:\"#274b6d\",fontSize:\"16px\"}},subtitle:{text:\"\",align:\"center\",style:{color:\"#4d759e\"}},plotOptions:{line:{allowPointSelect:!1,showCheckbox:!1,\n"
"animation:{duration:1E3},events:{},lineWidth:2,marker:{enabled:!0,lineWidth:0,radius:4,lineColor:\"#FFFFFF\",states:{hover:{enabled:!0},select:{fillColor:\"#FFFFFF\",lineColor:\"#000000\",lineWidth:2}}},point:{events:{}},dataLabels:x(J,{align:\"center\",enabled:!1,formatter:function(){return this.y===null?\"\":Da(this.y,-1)},verticalAlign:\"bottom\",y:0}),cropThreshold:300,pointRange:0,states:{hover:{marker:{}},select:{marker:{}}},stickyTracking:!0,turboThreshold:1E3}},labels:{style:{position:\"absolute\",color:\"#3E576F\"}},\n"
"legend:{enabled:!0,align:\"center\",layout:\"horizontal\",labelFormatter:function(){return this.name},borderWidth:1,borderColor:\"#909090\",borderRadius:5,navigation:{activeColor:\"#274b6d\",inactiveColor:\"#CCC\"},shadow:!1,itemStyle:{cursor:\"pointer\",color:\"#274b6d\",fontSize:\"12px\"},itemHoverStyle:{color:\"#000\"},itemHiddenStyle:{color:\"#CCC\"},itemCheckboxStyle:{position:\"absolute\",width:\"13px\",height:\"13px\"},symbolPadding:5,verticalAlign:\"bottom\",x:0,y:0,title:{style:{fontWeight:\"bold\"}}},loading:{labelStyle:{fontWeight:\"bold\",\n"
"position:\"relative\",top:\"1em\"},style:{position:\"absolute\",backgroundColor:\"white\",opacity:0.5,textAlign:\"center\"}},tooltip:{enabled:!0,animation:V,backgroundColor:\"rgba(255, 255, 255, .85)\",borderWidth:1,borderRadius:3,dateTimeLabelFormats:{millisecond:\"%A, %b %e, %H:%M:%S.%L\",second:\"%A, %b %e, %H:%M:%S\",minute:\"%A, %b %e, %H:%M\",hour:\"%A, %b %e, %H:%M\",day:\"%A, %b %e, %Y\",week:\"Week from %A, %b %e, %Y\",month:\"%B %Y\",year:\"%Y\"},headerFormat:'<span style=\"font-size: 10px\">{point.key}</span><br/>',\n"
"pointFormat:'<span style=\"color:{series.color}\">{series.name}</span>: <b>{point.y}</b><br/>',shadow:!0,snap:Ib?25:10,style:{color:\"#333333\",cursor:\"default\",fontSize:\"12px\",padding:\"8px\",whiteSpace:\"nowrap\"}},credits:{enabled:!0,text:\"Highcharts.com\",href:\"http://www.highcharts.com\",position:{align:\"right\",x:-10,verticalAlign:\"bottom\",y:-5},style:{cursor:\"pointer\",color:\"#909090\",fontSize:\"9px\"}}};var Y=G.plotOptions,W=Y.line;Bb();var Sb=/rgba\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]?(?:\\.[0-9]+)?)\\s*\\)/,\n"
"Tb=/#([a-fA-F0-9]{2})([a-fA-F0-9]{2})([a-fA-F0-9]{2})/,Ub=/rgb\\(\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*,\\s*([0-9]{1,3})\\s*\\)/,ta=function(a){var b=[],c,d;(function(a){a&&a.stops?d=Ra(a.stops,function(a){return ta(a[1])}):(c=Sb.exec(a))?b=[z(c[1]),z(c[2]),z(c[3]),parseFloat(c[4],10)]:(c=Tb.exec(a))?b=[z(c[1],16),z(c[2],16),z(c[3],16),1]:(c=Ub.exec(a))&&(b=[z(c[1]),z(c[2]),z(c[3]),1])})(a);return{get:function(c){var f;d?(f=x(a),f.stops=[].concat(f.stops),p(d,function(a,b){f.stops[b]=[f.stops[b][0],a.get(c)]})):\n"
"f=b&&!isNaN(b[0])?c===\"rgb\"?\"rgb(\"+b[0]+\",\"+b[1]+\",\"+b[2]+\")\":c===\"a\"?b[3]:\"rgba(\"+b.join(\",\")+\")\":a;return f},brighten:function(a){if(d)p(d,function(b){b.brighten(a)});else if(wa(a)&&a!==0){var c;for(c=0;c<3;c++)b[c]+=z(a*255),b[c]<0&&(b[c]=0),b[c]>255&&(b[c]=255)}return this},rgba:b,setOpacity:function(a){b[3]=a;return this}}};pa.prototype={init:function(a,b){this.element=b===\"span\"?T(b):y.createElementNS(Ca,b);this.renderer=a;this.attrSetters={}},opacity:1,animate:function(a,b,c){b=n(b,oa,!0);\n"
"Za(this);if(b){b=x(b);if(c)b.complete=c;kb(this,a,b)}else this.attr(a),c&&c()},attr:function(a,b){var c,d,e,f,g=this.element,h=g.nodeName.toLowerCase(),i=this.renderer,j,k=this.attrSetters,l=this.shadows,m,q,o=this;fa(a)&&t(b)&&(c=a,a={},a[c]=b);if(fa(a))c=a,h===\"circle\"?c={x:\"cx\",y:\"cy\"}[c]||c:c===\"strokeWidth\"&&(c=\"stroke-width\"),o=v(g,c)||this[c]||0,c!==\"d\"&&c!==\"visibility\"&&c!==\"fill\"&&(o=parseFloat(o));else{for(c in a)if(j=!1,d=a[c],e=k[c]&&k[c].call(this,d,c),e!==!1){e!==u&&(d=e);if(c===\"d\")d&&\n"
"d.join&&(d=d.join(\" \")),/(NaN| {2}|^$)/.test(d)&&(d=\"M 0 0\");else if(c===\"x\"&&h===\"text\")for(e=0;e<g.childNodes.length;e++)f=g.childNodes[e],v(f,\"x\")===v(g,\"x\")&&v(f,\"x\",d);else if(this.rotation&&(c===\"x\"||c===\"y\"))q=!0;else if(c===\"fill\")d=i.color(d,g,c);else if(h===\"circle\"&&(c===\"x\"||c===\"y\"))c={x:\"cx\",y:\"cy\"}[c]||c;else if(h===\"rect\"&&c===\"r\")v(g,{rx:d,ry:d}),j=!0;else if(c===\"translateX\"||c===\"translateY\"||c===\"rotation\"||c===\"verticalAlign\"||c===\"scaleX\"||c===\"scaleY\")j=q=!0;else if(c===\"stroke\")d=\n"
"i.color(d,g,c);else if(c===\"dashstyle\")if(c=\"stroke-dasharray\",d=d&&d.toLowerCase(),d===\"solid\")d=Q;else{if(d){d=d.replace(\"shortdashdotdot\",\"3,1,1,1,1,1,\").replace(\"shortdashdot\",\"3,1,1,1\").replace(\"shortdot\",\"1,1,\").replace(\"shortdash\",\"3,1,\").replace(\"longdash\",\"8,3,\").replace(/dot/g,\"1,3,\").replace(\"dash\",\"4,3,\").replace(/,$/,\"\").split(\",\");for(e=d.length;e--;)d[e]=z(d[e])*n(a[\"stroke-width\"],this[\"stroke-width\"]);d=d.join(\",\")}}else if(c===\"width\")d=z(d);else if(c===\"align\")c=\"text-anchor\",d=\n"
"{left:\"start\",center:\"middle\",right:\"end\"}[d];else if(c===\"title\")e=g.getElementsByTagName(\"title\")[0],e||(e=y.createElementNS(Ca,\"title\"),g.appendChild(e)),e.textContent=d;c===\"strokeWidth\"&&(c=\"stroke-width\");if(c===\"stroke-width\"||c===\"stroke\"){this[c]=d;if(this.stroke&&this[\"stroke-width\"])v(g,\"stroke\",this.stroke),v(g,\"stroke-width\",this[\"stroke-width\"]),this.hasStroke=!0;else if(c===\"stroke-width\"&&d===0&&this.hasStroke)g.removeAttribute(\"stroke\"),this.hasStroke=!1;j=!0}this.symbolName&&/^(x|y|width|height|r|start|end|innerR|anchorX|anchorY)/.test(c)&&\n"
"(m||(this.symbolAttr(a),m=!0),j=!0);if(l&&/^(width|height|visibility|x|y|d|transform|cx|cy|r)$/.test(c))for(e=l.length;e--;)v(l[e],c,c===\"height\"?s(d-(l[e].cutHeight||0),0):d);if((c===\"width\"||c===\"height\")&&h===\"rect\"&&d<0)d=0;this[c]=d;c===\"text\"?(d!==this.textStr&&delete this.bBox,this.textStr=d,this.added&&i.buildText(this)):j||v(g,c,d)}q&&this.updateTransform()}return o},addClass:function(a){var b=this.element,c=v(b,\"class\")||\"\";c.indexOf(a)===-1&&v(b,\"class\",c+\" \"+a);return this},symbolAttr:function(a){var b=\n"
,
"this;p(\"x,y,r,start,end,width,height,innerR,anchorX,anchorY\".split(\",\"),function(c){b[c]=n(a[c],b[c])});b.attr({d:b.renderer.symbols[b.symbolName](b.x,b.y,b.width,b.height,b)})},clip:function(a){return this.attr(\"clip-path\",a?\"url(\"+this.renderer.url+\"#\"+a.id+\")\":Q)},crisp:function(a,b,c,d,e){var f,g={},h={},i,a=a||this.strokeWidth||this.attr&&this.attr(\"stroke-width\")||0;i=w(a)%2/2;h.x=N(b||this.x||0)+i;h.y=N(c||this.y||0)+i;h.width=N((d||this.width||0)-2*i);h.height=N((e||this.height||0)-2*i);h.strokeWidth=\n"
"a;for(f in h)this[f]!==h[f]&&(this[f]=g[f]=h[f]);return g},css:function(a){var b=this.element,c=this.textWidth=a&&a.width&&b.nodeName.toLowerCase()===\"text\"&&z(a.width),d,e=\"\",f=function(a,b){return\"-\"+b.toLowerCase()};if(a&&a.color)a.fill=a.color;this.styles=a=r(this.styles,a);c&&delete a.width;if(ya&&!V)D(this.element,a);else{for(d in a)e+=d.replace(/([A-Z])/g,f)+\":\"+a[d]+\";\";v(b,\"style\",e)}c&&this.added&&this.renderer.buildText(this);return this},on:function(a,b){var c=this,d=c.element;ib&&a===\n"
"\"click\"?(d.ontouchstart=function(a){c.touchEventFired=Date.now();a.preventDefault();b.call(d,a)},d.onclick=function(a){(ra.indexOf(\"Android\")===-1||Date.now()-(c.touchEventFired||0)>1100)&&b.call(d,a)}):d[\"on\"+a]=b;return this},setRadialReference:function(a){this.element.radialReference=a;return this},translate:function(a,b){return this.attr({translateX:a,translateY:b})},invert:function(){this.inverted=!0;this.updateTransform();return this},updateTransform:function(){var a=this.translateX||0,b=this.translateY||\n"
"0,c=this.scaleX,d=this.scaleY,e=this.inverted,f=this.rotation;e&&(a+=this.attr(\"width\"),b+=this.attr(\"height\"));a=[\"translate(\"+a+\",\"+b+\")\"];e?a.push(\"rotate(90) scale(-1,1)\"):f&&a.push(\"rotate(\"+f+\" \"+(this.x||0)+\" \"+(this.y||0)+\")\");(t(c)||t(d))&&a.push(\"scale(\"+n(c,1)+\" \"+n(d,1)+\")\");a.length&&v(this.element,\"transform\",a.join(\" \"))},toFront:function(){var a=this.element;a.parentNode.appendChild(a);return this},align:function(a,b,c){var d,e,f,g,h={};e=this.renderer;f=e.alignedObjects;if(a){if(this.alignOptions=\n"
"a,this.alignByTranslate=b,!c||fa(c))this.alignTo=d=c||\"renderer\",ha(f,this),f.push(this),c=null}else a=this.alignOptions,b=this.alignByTranslate,d=this.alignTo;c=n(c,e[d],e);d=a.align;e=a.verticalAlign;f=(c.x||0)+(a.x||0);g=(c.y||0)+(a.y||0);if(d===\"right\"||d===\"center\")f+=(c.width-(a.width||0))/{right:1,center:2}[d];h[b?\"translateX\":\"x\"]=w(f);if(e===\"bottom\"||e===\"middle\")g+=(c.height-(a.height||0))/({bottom:1,middle:2}[e]||1);h[b?\"translateY\":\"y\"]=w(g);this[this.placed?\"animate\":\"attr\"](h);this.placed=\n"
"!0;this.alignAttr=h;return this},getBBox:function(){var a=this.bBox,b=this.renderer,c,d,e=this.rotation;c=this.element;var f=this.styles,g=e*Ba;d=this.textStr;var h;if(d===\"\"||Ob.test(d))h=d.length+\"|\"+f.fontSize+\"|\"+f.fontFamily,a=b.cache[h];if(!a){if(c.namespaceURI===Ca||b.forExport){try{a=c.getBBox?r({},c.getBBox()):{width:c.offsetWidth,height:c.offsetHeight}}catch(i){}if(!a||a.width<0)a={width:0,height:0}}else a=this.htmlGetBBox();if(b.isSVG){c=a.width;d=a.height;if(ya&&f&&f.fontSize===\"11px\"&&\n"
"d.toPrecision(3)===\"16.9\")a.height=d=14;if(e)a.width=M(d*ba(g))+M(c*U(g)),a.height=M(d*U(g))+M(c*ba(g))}this.bBox=a;h&&(b.cache[h]=a)}return a},show:function(){return this.attr({visibility:\"visible\"})},hide:function(){return this.attr({visibility:\"hidden\"})},fadeOut:function(a){var b=this;b.animate({opacity:0},{duration:a||150,complete:function(){b.hide()}})},add:function(a){var b=this.renderer,c=a||b,d=c.element||b.box,e=d.childNodes,f=this.element,g=v(f,\"zIndex\"),h;if(a)this.parentGroup=a;this.parentInverted=\n"
"a&&a.inverted;this.textStr!==void 0&&b.buildText(this);if(g)c.handleZ=!0,g=z(g);if(c.handleZ)for(c=0;c<e.length;c++)if(a=e[c],b=v(a,\"zIndex\"),a!==f&&(z(b)>g||!t(g)&&t(b))){d.insertBefore(f,a);h=!0;break}h||d.appendChild(f);this.added=!0;A(this,\"add\");return this},safeRemoveChild:function(a){var b=a.parentNode;b&&b.removeChild(a)},destroy:function(){var a=this,b=a.element||{},c=a.shadows,d=a.renderer.isSVG&&b.nodeName===\"SPAN\"&&a.parentGroup,e,f;b.onclick=b.onmouseout=b.onmouseover=b.onmousemove=b.point=\n"
"null;Za(a);if(a.clipPath)a.clipPath=a.clipPath.destroy();if(a.stops){for(f=0;f<a.stops.length;f++)a.stops[f]=a.stops[f].destroy();a.stops=null}a.safeRemoveChild(b);for(c&&p(c,function(b){a.safeRemoveChild(b)});d&&d.div.childNodes.length===0;)b=d.parentGroup,a.safeRemoveChild(d.div),delete d.div,d=b;a.alignTo&&ha(a.renderer.alignedObjects,a);for(e in a)delete a[e];return null},shadow:function(a,b,c){var d=[],e,f,g=this.element,h,i,j,k;if(a){i=n(a.width,3);j=(a.opacity||0.15)/i;k=this.parentInverted?\n"
"\"(-1,-1)\":\"(\"+n(a.offsetX,1)+\", \"+n(a.offsetY,1)+\")\";for(e=1;e<=i;e++){f=g.cloneNode(0);h=i*2+1-2*e;v(f,{isShadow:\"true\",stroke:a.color||\"black\",\"stroke-opacity\":j*e,\"stroke-width\":h,transform:\"translate\"+k,fill:Q});if(c)v(f,\"height\",s(v(f,\"height\")-h,0)),f.cutHeight=h;b?b.element.appendChild(f):g.parentNode.insertBefore(f,g);d.push(f)}this.shadows=d}return this}};var ua=function(){this.init.apply(this,arguments)};ua.prototype={Element:pa,init:function(a,b,c,d){var e=location,f,g;f=this.createElement(\"svg\").attr({version:\"1.1\"});\n"
"g=f.element;a.appendChild(g);a.innerHTML.indexOf(\"xmlns\")===-1&&v(g,\"xmlns\",Ca);this.isSVG=!0;this.box=g;this.boxWrapper=f;this.alignedObjects=[];this.url=(Xa||hb)&&y.getElementsByTagName(\"base\").length?e.href.replace(/#.*?$/,\"\").replace(/([\\('\\)])/g,\"\\\\$1\").replace(/ /g,\"%20\"):\"\";this.createElement(\"desc\").add().element.appendChild(y.createTextNode(\"Created with Highcharts 3.0.9\"));this.defs=this.createElement(\"defs\").add();this.forExport=d;this.gradients={};this.cache={};this.setSize(b,c,!1);var h;\n"
"if(Xa&&a.getBoundingClientRect)this.subPixelFix=b=function(){D(a,{left:0,top:0});h=a.getBoundingClientRect();D(a,{left:Ha(h.left)-h.left+\"px\",top:Ha(h.top)-h.top+\"px\"})},b(),F(C,\"resize\",b)},isHidden:function(){return!this.boxWrapper.getBBox().width},destroy:function(){var a=this.defs;this.box=null;this.boxWrapper=this.boxWrapper.destroy();Ma(this.gradients||{});this.gradients=null;if(a)this.defs=a.destroy();this.subPixelFix&&X(C,\"resize\",this.subPixelFix);return this.alignedObjects=null},createElement:function(a){var b=\n"
"new this.Element;b.init(this,a);return b},draw:function(){},buildText:function(a){for(var b=a.element,c=this,d=c.forExport,e=n(a.textStr,\"\").toString().replace(/<(b|strong)>/g,'<span style=\"font-weight:bold\">').replace(/<(i|em)>/g,'<span style=\"font-style:italic\">').replace(/<a/g,\"<span\").replace(/<\\/(b|strong|i|em|a)>/g,\"</span>\").split(/<br.*?>/g),f=b.childNodes,g=/style=\"([^\"]+)\"/,h=/href=\"(http[^\"]+)\"/,i=v(b,\"x\"),j=a.styles,k=a.textWidth,l=j&&j.lineHeight,m=f.length,q=function(a){return l?z(l):\n"
"c.fontMetrics(/px$/.test(a&&a.style.fontSize)?a.style.fontSize:j.fontSize||11).h};m--;)b.removeChild(f[m]);k&&!a.added&&this.box.appendChild(b);e[e.length-1]===\"\"&&e.pop();p(e,function(e,f){var l,m=0,e=e.replace(/<span/g,\"|||<span\").replace(/<\\/span>/g,\"</span>|||\");l=e.split(\"|||\");p(l,function(e){if(e!==\"\"||l.length===1){var o={},n=y.createElementNS(Ca,\"tspan\"),p;g.test(e)&&(p=e.match(g)[1].replace(/(;| |^)color([ :])/,\"$1fill$2\"),v(n,\"style\",p));h.test(e)&&!d&&(v(n,\"onclick\",'location.href=\"'+\n"
"e.match(h)[1]+'\"'),D(n,{cursor:\"pointer\"}));e=(e.replace(/<(.|\\n)*?>/g,\"\")||\" \").replace(/&lt;/g,\"<\").replace(/&gt;/g,\">\");if(e!==\" \"&&(n.appendChild(y.createTextNode(e)),m?o.dx=0:o.x=i,v(n,o),!m&&f&&(!V&&d&&D(n,{display:\"block\"}),v(n,\"dy\",q(n),hb&&n.offsetHeight)),b.appendChild(n),m++,k))for(var e=e.replace(/([^\\^])-/g,\"$1- \").split(\" \"),o=e.length>1&&j.whiteSpace!==\"nowrap\",t,s,w=a._clipHeight,u=[],r=q(),$=1;o&&(e.length||u.length);)delete a.bBox,t=a.getBBox(),s=t.width,!V&&c.forExport&&(s=c.measureSpanWidth(n.firstChild.data,\n"
"a.styles)),t=s>k,!t||e.length===1?(e=u,u=[],e.length&&($++,w&&$*r>w?(e=[\"...\"],a.attr(\"title\",a.textStr)):(n=y.createElementNS(Ca,\"tspan\"),v(n,{dy:r,x:i}),p&&v(n,\"style\",p),b.appendChild(n),s>k&&(k=s)))):(n.removeChild(n.firstChild),u.unshift(e.pop())),e.length&&n.appendChild(y.createTextNode(e.join(\" \").replace(/- /g,\"-\")))}})})},button:function(a,b,c,d,e,f,g,h,i){var j=this.label(a,b,c,i,null,null,null,null,\"button\"),k=0,l,m,q,o,n,p,a={x1:0,y1:0,x2:0,y2:1},e=x({\"stroke-width\":1,stroke:\"#CCCCCC\",\n"
"fill:{linearGradient:a,stops:[[0,\"#FEFEFE\"],[1,\"#F6F6F6\"]]},r:2,padding:5,style:{color:\"black\"}},e);q=e.style;delete e.style;f=x(e,{stroke:\"#68A\",fill:{linearGradient:a,stops:[[0,\"#FFF\"],[1,\"#ACF\"]]}},f);o=f.style;delete f.style;g=x(e,{stroke:\"#68A\",fill:{linearGradient:a,stops:[[0,\"#9BD\"],[1,\"#CDF\"]]}},g);n=g.style;delete g.style;h=x(e,{style:{color:\"#CCC\"}},h);p=h.style;delete h.style;F(j.element,ya?\"mouseover\":\"mouseenter\",function(){k!==3&&j.attr(f).css(o)});F(j.element,ya?\"mouseout\":\"mouseleave\",\n"
"function(){k!==3&&(l=[e,f,g][k],m=[q,o,n][k],j.attr(l).css(m))});j.setState=function(a){(j.state=k=a)?a===2?j.attr(g).css(n):a===3&&j.attr(h).css(p):j.attr(e).css(q)};return j.on(\"click\",function(){k!==3&&d.call(j)}).attr(e).css(r({cursor:\"default\"},q))},crispLine:function(a,b){a[1]===a[4]&&(a[1]=a[4]=w(a[1])-b%2/2);a[2]===a[5]&&(a[2]=a[5]=w(a[2])+b%2/2);return a},path:function(a){var b={fill:Q};Ka(a)?b.d=a:S(a)&&r(b,a);return this.createElement(\"path\").attr(b)},circle:function(a,b,c){a=S(a)?a:{x:a,\n"
,
"y:b,r:c};return this.createElement(\"circle\").attr(a)},arc:function(a,b,c,d,e,f){if(S(a))b=a.y,c=a.r,d=a.innerR,e=a.start,f=a.end,a=a.x;a=this.symbol(\"arc\",a||0,b||0,c||0,c||0,{innerR:d||0,start:e||0,end:f||0});a.r=c;return a},rect:function(a,b,c,d,e,f){e=S(a)?a.r:e;e=this.createElement(\"rect\").attr({rx:e,ry:e,fill:Q});return e.attr(S(a)?a:e.crisp(f,a,b,s(c,0),s(d,0)))},setSize:function(a,b,c){var d=this.alignedObjects,e=d.length;this.width=a;this.height=b;for(this.boxWrapper[n(c,!0)?\"animate\":\"attr\"]({width:a,\n"
"height:b});e--;)d[e].align()},g:function(a){var b=this.createElement(\"g\");return t(a)?b.attr({\"class\":\"highcharts-\"+a}):b},image:function(a,b,c,d,e){var f={preserveAspectRatio:Q};arguments.length>1&&r(f,{x:b,y:c,width:d,height:e});f=this.createElement(\"image\").attr(f);f.element.setAttributeNS?f.element.setAttributeNS(\"http://www.w3.org/1999/xlink\",\"href\",a):f.element.setAttribute(\"hc-svg-href\",a);return f},symbol:function(a,b,c,d,e,f){var g,h=this.symbols[a],h=h&&h(w(b),w(c),d,e,f),i=/^url\\((.*?)\\)$/,\n"
"j,k;if(h)g=this.path(h),r(g,{symbolName:a,x:b,y:c,width:d,height:e}),f&&r(g,f);else if(i.test(a))k=function(a,b){a.element&&(a.attr({width:b[0],height:b[1]}),a.alignByTranslate||a.translate(w((d-b[0])/2),w((e-b[1])/2)))},j=a.match(i)[1],a=Jb[j],g=this.image(j).attr({x:b,y:c}),g.isImg=!0,a?k(g,a):(g.attr({width:0,height:0}),T(\"img\",{onload:function(){k(g,Jb[j]=[this.width,this.height])},src:j}));return g},symbols:{circle:function(a,b,c,d){var e=0.166*c;return[\"M\",a+c/2,b,\"C\",a+c+e,b,a+c+e,b+d,a+c/\n"
"2,b+d,\"C\",a-e,b+d,a-e,b,a+c/2,b,\"Z\"]},square:function(a,b,c,d){return[\"M\",a,b,\"L\",a+c,b,a+c,b+d,a,b+d,\"Z\"]},triangle:function(a,b,c,d){return[\"M\",a+c/2,b,\"L\",a+c,b+d,a,b+d,\"Z\"]},\"triangle-down\":function(a,b,c,d){return[\"M\",a,b,\"L\",a+c,b,a+c/2,b+d,\"Z\"]},diamond:function(a,b,c,d){return[\"M\",a+c/2,b,\"L\",a+c,b+d/2,a+c/2,b+d,a,b+d/2,\"Z\"]},arc:function(a,b,c,d,e){var f=e.start,c=e.r||c||d,g=e.end-0.001,d=e.innerR,h=e.open,i=U(f),j=ba(f),k=U(g),g=ba(g),e=e.end-f<Aa?0:1;return[\"M\",a+c*i,b+c*j,\"A\",c,c,0,e,\n"
"1,a+c*k,b+c*g,h?\"M\":\"L\",a+d*k,b+d*g,\"A\",d,d,0,e,0,a+d*i,b+d*j,h?\"\":\"Z\"]}},clipRect:function(a,b,c,d){var e=\"highcharts-\"+ub++,f=this.createElement(\"clipPath\").attr({id:e}).add(this.defs),a=this.rect(a,b,c,d,0).add(f);a.id=e;a.clipPath=f;return a},color:function(a,b,c){var d=this,e,f=/^rgba/,g,h,i,j,k,l,m,q=[];a&&a.linearGradient?g=\"linearGradient\":a&&a.radialGradient&&(g=\"radialGradient\");if(g){c=a[g];h=d.gradients;j=a.stops;b=b.radialReference;Ka(c)&&(a[g]=c={x1:c[0],y1:c[1],x2:c[2],y2:c[3],gradientUnits:\"userSpaceOnUse\"});\n"
"g===\"radialGradient\"&&b&&!t(c.gradientUnits)&&(c=x(c,{cx:b[0]-b[2]/2+c.cx*b[2],cy:b[1]-b[2]/2+c.cy*b[2],r:c.r*b[2],gradientUnits:\"userSpaceOnUse\"}));for(m in c)m!==\"id\"&&q.push(m,c[m]);for(m in j)q.push(j[m]);q=q.join(\",\");h[q]?a=h[q].id:(c.id=a=\"highcharts-\"+ub++,h[q]=i=d.createElement(g).attr(c).add(d.defs),i.stops=[],p(j,function(a){f.test(a[1])?(e=ta(a[1]),k=e.get(\"rgb\"),l=e.get(\"a\")):(k=a[1],l=1);a=d.createElement(\"stop\").attr({offset:a[0],\"stop-color\":k,\"stop-opacity\":l}).add(i);i.stops.push(a)}));\n"
"return\"url(\"+d.url+\"#\"+a+\")\"}else return f.test(a)?(e=ta(a),v(b,c+\"-opacity\",e.get(\"a\")),e.get(\"rgb\")):(b.removeAttribute(c+\"-opacity\"),a)},text:function(a,b,c,d){var e=G.chart.style,f=da||!V&&this.forExport;if(d&&!this.forExport)return this.html(a,b,c);b=w(n(b,0));c=w(n(c,0));a=this.createElement(\"text\").attr({x:b,y:c,text:a}).css({fontFamily:e.fontFamily,fontSize:e.fontSize});f&&a.css({position:\"absolute\"});a.x=b;a.y=c;return a},fontMetrics:function(a){var a=z(a||11),a=a<24?a+4:w(a*1.2),b=w(a*0.8);\n"
"return{h:a,b:b}},label:function(a,b,c,d,e,f,g,h,i){function j(){var a,b;a=n.element.style;va=(Z===void 0||Sa===void 0||o.styles.textAlign)&&n.getBBox();o.width=(Z||va.width||0)+2*ea+lb;o.height=(Sa||va.height||0)+2*ea;$=ea+q.fontMetrics(a&&a.fontSize).b;if(y){if(!H)a=w(-s*ea),b=h?-$:0,o.box=H=d?q.symbol(d,a,b,o.width,o.height,v):q.rect(a,b,o.width,o.height,0,v[Lb]),H.add(o);H.isImg||H.attr(x({width:o.width,height:o.height},v));v=null}}function k(){var a=o.styles,a=a&&a.textAlign,b=lb+ea*(1-s),c;c=\n"
"h?0:$;if(t(Z)&&(a===\"center\"||a===\"right\"))b+={center:0.5,right:1}[a]*(Z-va.width);(b!==n.x||c!==n.y)&&n.attr({x:b,y:c});n.x=b;n.y=c}function l(a,b){H?H.attr(a,b):v[a]=b}function m(){n.add(o);o.attr({text:a,x:b,y:c});H&&t(e)&&o.attr({anchorX:e,anchorY:f})}var q=this,o=q.g(i),n=q.text(\"\",0,0,g).attr({zIndex:1}),H,va,s=0,ea=3,lb=0,Z,Sa,Ta,K,B=0,v={},$,g=o.attrSetters,y;F(o,\"add\",m);g.width=function(a){Z=a;return!1};g.height=function(a){Sa=a;return!1};g.padding=function(a){t(a)&&a!==ea&&(ea=a,k());return!1};\n"
"g.paddingLeft=function(a){t(a)&&a!==lb&&(lb=a,k());return!1};g.align=function(a){s={left:0,center:0.5,right:1}[a];return!1};g.text=function(a,b){n.attr(b,a);j();k();return!1};g[Lb]=function(a,b){y=!0;B=a%2/2;l(b,a);return!1};g.stroke=g.fill=g.r=function(a,b){b===\"fill\"&&(y=!0);l(b,a);return!1};g.anchorX=function(a,b){e=a;l(b,a+B-Ta);return!1};g.anchorY=function(a,b){f=a;l(b,a-K);return!1};g.x=function(a){o.x=a;a-=s*((Z||va.width)+ea);Ta=w(a);o.attr(\"translateX\",Ta);return!1};g.y=function(a){K=o.y=\n"
"w(a);o.attr(\"translateY\",K);return!1};var z=o.css;return r(o,{css:function(a){if(a){var b={},a=x(a);p(\"fontSize,fontWeight,fontFamily,color,lineHeight,width,textDecoration,textShadow\".split(\",\"),function(c){a[c]!==u&&(b[c]=a[c],delete a[c])});n.css(b)}return z.call(o,a)},getBBox:function(){return{width:va.width+2*ea,height:va.height+2*ea,x:va.x-ea,y:va.y-ea}},shadow:function(a){H&&H.shadow(a);return o},destroy:function(){X(o,\"add\",m);X(o.element,\"mouseenter\");X(o.element,\"mouseleave\");n&&(n=n.destroy());\n"
"H&&(H=H.destroy());pa.prototype.destroy.call(o);o=q=j=k=l=m=null}})}};Ya=ua;r(pa.prototype,{htmlCss:function(a){var b=this.element;if(b=a&&b.tagName===\"SPAN\"&&a.width)delete a.width,this.textWidth=b,this.updateTransform();this.styles=r(this.styles,a);D(this.element,a);return this},htmlGetBBox:function(){var a=this.element,b=this.bBox;if(!b){if(a.nodeName===\"text\")a.style.position=\"absolute\";b=this.bBox={x:a.offsetLeft,y:a.offsetTop,width:a.offsetWidth,height:a.offsetHeight}}return b},htmlUpdateTransform:function(){if(this.added){var a=\n"
"this.renderer,b=this.element,c=this.translateX||0,d=this.translateY||0,e=this.x||0,f=this.y||0,g=this.textAlign||\"left\",h={left:0,center:0.5,right:1}[g],i=this.shadows;D(b,{marginLeft:c,marginTop:d});i&&p(i,function(a){D(a,{marginLeft:c+1,marginTop:d+1})});this.inverted&&p(b.childNodes,function(c){a.invertChild(c,b)});if(b.tagName===\"SPAN\"){var j=this.rotation,k,l=z(this.textWidth),m=[j,g,b.innerHTML,this.textWidth].join(\",\");if(m!==this.cTT){k=a.fontMetrics(b.style.fontSize).b;t(j)&&this.setSpanRotation(j,\n"
"h,k);i=n(this.elemWidth,b.offsetWidth);if(i>l&&/[ \\-]/.test(b.textContent||b.innerText))D(b,{width:l+\"px\",display:\"block\",whiteSpace:\"normal\"}),i=l;this.getSpanCorrection(i,k,h,j,g)}D(b,{left:e+(this.xCorr||0)+\"px\",top:f+(this.yCorr||0)+\"px\"});if(hb)k=b.offsetHeight;this.cTT=m}}else this.alignOnAdd=!0},setSpanRotation:function(a,b,c){var d={},e=ya?\"-ms-transform\":hb?\"-webkit-transform\":Xa?\"MozTransform\":Hb?\"-o-transform\":\"\";d[e]=d.transform=\"rotate(\"+a+\"deg)\";d[e+(Xa?\"Origin\":\"-origin\")]=b*100+\"% \"+\n"
"c+\"px\";D(this.element,d)},getSpanCorrection:function(a,b,c){this.xCorr=-a*c;this.yCorr=-b}});r(ua.prototype,{html:function(a,b,c){var d=G.chart.style,e=this.createElement(\"span\"),f=e.attrSetters,g=e.element,h=e.renderer;f.text=function(a){a!==g.innerHTML&&delete this.bBox;g.innerHTML=a;return!1};f.x=f.y=f.align=f.rotation=function(a,b){b===\"align\"&&(b=\"textAlign\");e[b]=a;e.htmlUpdateTransform();return!1};e.attr({text:a,x:w(b),y:w(c)}).css({position:\"absolute\",whiteSpace:\"nowrap\",fontFamily:d.fontFamily,\n"
"fontSize:d.fontSize});e.css=e.htmlCss;if(h.isSVG)e.add=function(a){var b,c=h.box.parentNode,d=[];if(this.parentGroup=a){if(b=a.div,!b){for(;a;)d.push(a),a=a.parentGroup;p(d.reverse(),function(a){var d;b=a.div=a.div||T(Ga,{className:v(a.element,\"class\")},{position:\"absolute\",left:(a.translateX||0)+\"px\",top:(a.translateY||0)+\"px\"},b||c);d=b.style;r(a.attrSetters,{translateX:function(a){d.left=a+\"px\"},translateY:function(a){d.top=a+\"px\"},visibility:function(a,b){d[b]=a}})})}}else b=c;b.appendChild(g);\n"
"e.added=!0;e.alignOnAdd&&e.htmlUpdateTransform();return e};return e}});var R;if(!V&&!da){Highcharts.VMLElement=R={init:function(a,b){var c=[\"<\",b,' filled=\"f\" stroked=\"f\"'],d=[\"position: \",\"absolute\",\";\"],e=b===Ga;(b===\"shape\"||e)&&d.push(\"left:0;top:0;width:1px;height:1px;\");d.push(\"visibility: \",e?\"hidden\":\"visible\");c.push(' style=\"',d.join(\"\"),'\"/>');if(b)c=e||b===\"span\"||b===\"img\"?c.join(\"\"):a.prepVML(c),this.element=T(c);this.renderer=a;this.attrSetters={}},add:function(a){var b=this.renderer,\n"
"c=this.element,d=b.box,d=a?a.element||a:d;a&&a.inverted&&b.invertChild(c,d);d.appendChild(c);this.added=!0;this.alignOnAdd&&!this.deferUpdateTransform&&this.updateTransform();A(this,\"add\");return this},updateTransform:pa.prototype.htmlUpdateTransform,setSpanRotation:function(){var a=this.rotation,b=U(a*Ba),c=ba(a*Ba);D(this.element,{filter:a?[\"progid:DXImageTransform.Microsoft.Matrix(M11=\",b,\", M12=\",-c,\", M21=\",c,\", M22=\",b,\", sizingMethod='auto expand')\"].join(\"\"):Q})},getSpanCorrection:function(a,\n"
,
"b,c,d,e){var f=d?U(d*Ba):1,g=d?ba(d*Ba):0,h=n(this.elemHeight,this.element.offsetHeight),i;this.xCorr=f<0&&-a;this.yCorr=g<0&&-h;i=f*g<0;this.xCorr+=g*b*(i?1-c:c);this.yCorr-=f*b*(d?i?c:1-c:1);e&&e!==\"left\"&&(this.xCorr-=a*c*(f<0?-1:1),d&&(this.yCorr-=h*c*(g<0?-1:1)),D(this.element,{textAlign:e}))},pathToVML:function(a){for(var b=a.length,c=[];b--;)if(wa(a[b]))c[b]=w(a[b]*10)-5;else if(a[b]===\"Z\")c[b]=\"x\";else if(c[b]=a[b],a.isArc&&(a[b]===\"wa\"||a[b]===\"at\"))c[b+5]===c[b+7]&&(c[b+7]+=a[b+7]>a[b+5]?\n"
"1:-1),c[b+6]===c[b+8]&&(c[b+8]+=a[b+8]>a[b+6]?1:-1);return c.join(\" \")||\"x\"},attr:function(a,b){var c,d,e,f=this.element||{},g=f.style,h=f.nodeName,i=this.renderer,j=this.symbolName,k,l=this.shadows,m,q=this.attrSetters,o=this;fa(a)&&t(b)&&(c=a,a={},a[c]=b);if(fa(a))c=a,o=c===\"strokeWidth\"||c===\"stroke-width\"?this.strokeweight:this[c];else for(c in a)if(d=a[c],m=!1,e=q[c]&&q[c].call(this,d,c),e!==!1&&d!==null){e!==u&&(d=e);if(j&&/^(x|y|r|start|end|width|height|innerR|anchorX|anchorY)/.test(c))k||\n"
"(this.symbolAttr(a),k=!0),m=!0;else if(c===\"d\"){d=d||[];this.d=d.join(\" \");f.path=d=this.pathToVML(d);if(l)for(e=l.length;e--;)l[e].path=l[e].cutOff?this.cutOffPath(d,l[e].cutOff):d;m=!0}else if(c===\"visibility\"){if(l)for(e=l.length;e--;)l[e].style[c]=d;h===\"DIV\"&&(d=d===\"hidden\"?\"-999em\":0,gb||(g[c]=d?\"visible\":\"hidden\"),c=\"top\");g[c]=d;m=!0}else if(c===\"zIndex\")d&&(g[c]=d),m=!0;else if(sa(c,[\"x\",\"y\",\"width\",\"height\"])!==-1)this[c]=d,c===\"x\"||c===\"y\"?c={x:\"left\",y:\"top\"}[c]:d=s(0,d),this.updateClipping?\n"
"(this[c]=d,this.updateClipping()):g[c]=d,m=!0;else if(c===\"class\"&&h===\"DIV\")f.className=d;else if(c===\"stroke\")d=i.color(d,f,c),c=\"strokecolor\";else if(c===\"stroke-width\"||c===\"strokeWidth\")f.stroked=d?!0:!1,c=\"strokeweight\",this[c]=d,wa(d)&&(d+=\"px\");else if(c===\"dashstyle\")(f.getElementsByTagName(\"stroke\")[0]||T(i.prepVML([\"<stroke/>\"]),null,null,f))[c]=d||\"solid\",this.dashstyle=d,m=!0;else if(c===\"fill\")if(h===\"SPAN\")g.color=d;else{if(h!==\"IMG\")f.filled=d!==Q?!0:!1,d=i.color(d,f,c,this),c=\"fillcolor\"}else if(c===\n"
"\"opacity\")m=!0;else if(h===\"shape\"&&c===\"rotation\")this[c]=f.style[c]=d,f.style.left=-w(ba(d*Ba)+1)+\"px\",f.style.top=w(U(d*Ba))+\"px\";else if(c===\"translateX\"||c===\"translateY\"||c===\"rotation\")this[c]=d,this.updateTransform(),m=!0;m||(gb?f[c]=d:v(f,c,d))}return o},clip:function(a){var b=this,c;a?(c=a.members,ha(c,b),c.push(b),b.destroyClip=function(){ha(c,b)},a=a.getCSS(b)):(b.destroyClip&&b.destroyClip(),a={clip:gb?\"inherit\":\"rect(auto)\"});return b.css(a)},css:pa.prototype.htmlCss,safeRemoveChild:function(a){a.parentNode&&\n"
"Na(a)},destroy:function(){this.destroyClip&&this.destroyClip();return pa.prototype.destroy.apply(this)},on:function(a,b){this.element[\"on\"+a]=function(){var a=C.event;a.target=a.srcElement;b(a)};return this},cutOffPath:function(a,b){var c,a=a.split(/[ ,]/);c=a.length;if(c===9||c===11)a[c-4]=a[c-2]=z(a[c-2])-10*b;return a.join(\" \")},shadow:function(a,b,c){var d=[],e,f=this.element,g=this.renderer,h,i=f.style,j,k=f.path,l,m,q,o;k&&typeof k.value!==\"string\"&&(k=\"x\");m=k;if(a){q=n(a.width,3);o=(a.opacity||\n"
"0.15)/q;for(e=1;e<=3;e++){l=q*2+1-2*e;c&&(m=this.cutOffPath(k.value,l+0.5));j=['<shape isShadow=\"true\" strokeweight=\"',l,'\" filled=\"false\" path=\"',m,'\" coordsize=\"10 10\" style=\"',f.style.cssText,'\" />'];h=T(g.prepVML(j),null,{left:z(i.left)+n(a.offsetX,1),top:z(i.top)+n(a.offsetY,1)});if(c)h.cutOff=l+1;j=['<stroke color=\"',a.color||\"black\",'\" opacity=\"',o*e,'\"/>'];T(g.prepVML(j),null,null,h);b?b.element.appendChild(h):f.parentNode.insertBefore(h,f);d.push(h)}this.shadows=d}return this}};R=ia(pa,R);\n"
"var xb={Element:R,isIE8:ra.indexOf(\"MSIE 8.0\")>-1,init:function(a,b,c){var d,e;this.alignedObjects=[];d=this.createElement(Ga);e=d.element;e.style.position=\"relative\";a.appendChild(d.element);this.isVML=!0;this.box=e;this.boxWrapper=d;this.cache={};this.setSize(b,c,!1);if(!y.namespaces.hcv){y.namespaces.add(\"hcv\",\"urn:schemas-microsoft-com:vml\");try{y.createStyleSheet().cssText=\"hcv\\\\:fill, hcv\\\\:path, hcv\\\\:shape, hcv\\\\:stroke{ behavior:url(#default#VML); display: inline-block; } \"}catch(f){y.styleSheets[0].cssText+=\n"
"\"hcv\\\\:fill, hcv\\\\:path, hcv\\\\:shape, hcv\\\\:stroke{ behavior:url(#default#VML); display: inline-block; } \"}}},isHidden:function(){return!this.box.offsetWidth},clipRect:function(a,b,c,d){var e=this.createElement(),f=S(a);return r(e,{members:[],left:(f?a.x:a)+1,top:(f?a.y:b)+1,width:(f?a.width:c)-1,height:(f?a.height:d)-1,getCSS:function(a){var b=a.element,c=b.nodeName,a=a.inverted,d=this.top-(c===\"shape\"?b.offsetTop:0),e=this.left,b=e+this.width,f=d+this.height,d={clip:\"rect(\"+w(a?e:d)+\"px,\"+w(a?f:\n"
"b)+\"px,\"+w(a?b:f)+\"px,\"+w(a?d:e)+\"px)\"};!a&&gb&&c===\"DIV\"&&r(d,{width:b+\"px\",height:f+\"px\"});return d},updateClipping:function(){p(e.members,function(a){a.css(e.getCSS(a))})}})},color:function(a,b,c,d){var e=this,f,g=/^rgba/,h,i,j=Q;a&&a.linearGradient?i=\"gradient\":a&&a.radialGradient&&(i=\"pattern\");if(i){var k,l,m=a.linearGradient||a.radialGradient,q,o,n,H,s,t=\"\",a=a.stops,u,w=[],r=function(){h=['<fill colors=\"'+w.join(\",\")+'\" opacity=\"',n,'\" o:opacity2=\"',o,'\" type=\"',i,'\" ',t,'focus=\"100%\" method=\"any\" />'];\n"
"T(e.prepVML(h),null,null,b)};q=a[0];u=a[a.length-1];q[0]>0&&a.unshift([0,q[1]]);u[0]<1&&a.push([1,u[1]]);p(a,function(a,b){g.test(a[1])?(f=ta(a[1]),k=f.get(\"rgb\"),l=f.get(\"a\")):(k=a[1],l=1);w.push(a[0]*100+\"% \"+k);b?(n=l,H=k):(o=l,s=k)});if(c===\"fill\")if(i===\"gradient\")c=m.x1||m[0]||0,a=m.y1||m[1]||0,q=m.x2||m[2]||0,m=m.y2||m[3]||0,t='angle=\"'+(90-P.atan((m-a)/(q-c))*180/Aa)+'\"',r();else{var j=m.r,Sa=j*2,Ta=j*2,v=m.cx,B=m.cy,x=b.radialReference,$,j=function(){x&&($=d.getBBox(),v+=(x[0]-$.x)/$.width-\n"
"0.5,B+=(x[1]-$.y)/$.height-0.5,Sa*=x[2]/$.width,Ta*=x[2]/$.height);t='src=\"'+G.global.VMLRadialGradientURL+'\" size=\"'+Sa+\",\"+Ta+'\" origin=\"0.5,0.5\" position=\"'+v+\",\"+B+'\" color2=\"'+s+'\" ';r()};d.added?j():F(d,\"add\",j);j=H}else j=k}else if(g.test(a)&&b.tagName!==\"IMG\")f=ta(a),h=[\"<\",c,' opacity=\"',f.get(\"a\"),'\"/>'],T(this.prepVML(h),null,null,b),j=f.get(\"rgb\");else{j=b.getElementsByTagName(c);if(j.length)j[0].opacity=1,j[0].type=\"solid\";j=a}return j},prepVML:function(a){var b=this.isIE8,a=a.join(\"\");\n"
"b?(a=a.replace(\"/>\",' xmlns=\"urn:schemas-microsoft-com:vml\" />'),a=a.indexOf('style=\"')===-1?a.replace(\"/>\",' style=\"display:inline-block;behavior:url(#default#VML);\" />'):a.replace('style=\"','style=\"display:inline-block;behavior:url(#default#VML);')):a=a.replace(\"<\",\"<hcv:\");return a},text:ua.prototype.html,path:function(a){var b={coordsize:\"10 10\"};Ka(a)?b.d=a:S(a)&&r(b,a);return this.createElement(\"shape\").attr(b)},circle:function(a,b,c){var d=this.symbol(\"circle\");if(S(a))c=a.r,b=a.y,a=a.x;d.isCircle=\n"
"!0;d.r=c;return d.attr({x:a,y:b})},g:function(a){var b;a&&(b={className:\"highcharts-\"+a,\"class\":\"highcharts-\"+a});return this.createElement(Ga).attr(b)},image:function(a,b,c,d,e){var f=this.createElement(\"img\").attr({src:a});arguments.length>1&&f.attr({x:b,y:c,width:d,height:e});return f},rect:function(a,b,c,d,e,f){var g=this.symbol(\"rect\");g.r=S(a)?a.r:e;return g.attr(S(a)?a:g.crisp(f,a,b,s(c,0),s(d,0)))},invertChild:function(a,b){var c=b.style;D(a,{flip:\"x\",left:z(c.width)-1,top:z(c.height)-1,rotation:-90})},\n"
"symbols:{arc:function(a,b,c,d,e){var f=e.start,g=e.end,h=e.r||c||d,c=e.innerR,d=U(f),i=ba(f),j=U(g),k=ba(g);if(g-f===0)return[\"x\"];f=[\"wa\",a-h,b-h,a+h,b+h,a+h*d,b+h*i,a+h*j,b+h*k];e.open&&!c&&f.push(\"e\",\"M\",a,b);f.push(\"at\",a-c,b-c,a+c,b+c,a+c*j,b+c*k,a+c*d,b+c*i,\"x\",\"e\");f.isArc=!0;return f},circle:function(a,b,c,d,e){e&&(c=d=2*e.r);e&&e.isCircle&&(a-=c/2,b-=d/2);return[\"wa\",a,b,a+c,b+d,a+c,b+d/2,a+c,b+d/2,\"e\"]},rect:function(a,b,c,d,e){var f=a+c,g=b+d,h;!t(e)||!e.r?f=ua.prototype.symbols.square.apply(0,\n"
"arguments):(h=I(e.r,c,d),f=[\"M\",a+h,b,\"L\",f-h,b,\"wa\",f-2*h,b,f,b+2*h,f-h,b,f,b+h,\"L\",f,g-h,\"wa\",f-2*h,g-2*h,f,g,f,g-h,f-h,g,\"L\",a+h,g,\"wa\",a,g-2*h,a+2*h,g,a+h,g,a,g-h,\"L\",a,b+h,\"wa\",a,b,a+2*h,b+2*h,a,b+h,a+h,b,\"x\",\"e\"]);return f}}};Highcharts.VMLRenderer=R=function(){this.init.apply(this,arguments)};R.prototype=x(ua.prototype,xb);Ya=R}ua.prototype.measureSpanWidth=function(a,b){var c=y.createElement(\"span\"),d;d=y.createTextNode(a);c.appendChild(d);D(c,b);this.box.appendChild(c);d=c.offsetWidth;Na(c);\n"
"return d};var Mb;if(da)Highcharts.CanVGRenderer=R=function(){Ca=\"http://www.w3.org/1999/xhtml\"},R.prototype.symbols={},Mb=function(){function a(){var a=b.length,d;for(d=0;d<a;d++)b[d]();b=[]}var b=[];return{push:function(c,d){b.length===0&&Pb(d,a);b.push(c)}}}(),Ya=R;Qa.prototype={addLabel:function(){var a=this.axis,b=a.options,c=a.chart,d=a.horiz,e=a.categories,f=a.names,g=this.pos,h=b.labels,i=a.tickPositions,d=d&&e&&!h.step&&!h.staggerLines&&!h.rotation&&c.plotWidth/i.length||!d&&(c.margin[3]||\n"
"c.chartWidth*0.33),j=g===i[0],k=g===i[i.length-1],l,f=e?n(e[g],f[g],g):g,e=this.label,m=i.info;a.isDatetimeAxis&&m&&(l=b.dateTimeLabelFormats[m.higherRanks[g]||m.unitName]);this.isFirst=j;this.isLast=k;b=a.labelFormatter.call({axis:a,chart:c,isFirst:j,isLast:k,dateTimeLabelFormat:l,value:a.isLog?aa(ga(f)):f});g=d&&{width:s(1,w(d-2*(h.padding||10)))+\"px\"};g=r(g,h.style);if(t(e))e&&e.attr({text:b}).css(g);else{l={align:a.labelAlign};if(wa(h.rotation))l.rotation=h.rotation;if(d&&h.ellipsis)l._clipHeight=\n"
,
"a.len/i.length;this.label=t(b)&&h.enabled?c.renderer.text(b,0,0,h.useHTML).attr(l).css(g).add(a.labelGroup):null}},getLabelSize:function(){var a=this.label,b=this.axis;return a?a.getBBox()[b.horiz?\"height\":\"width\"]:0},getLabelSides:function(){var a=this.label.getBBox(),b=this.axis,c=b.horiz,d=b.options.labels,a=c?a.width:a.height,b=c?a*{left:0,center:0.5,right:1}[b.labelAlign]-d.x:a;return[-b,a-b]},handleOverflow:function(a,b){var B;var c=!0,d=this.axis,e=this.isFirst,f=this.isLast,g=d.horiz?b.x:\n"
"b.y,h=d.reversed,i=d.tickPositions,j=this.getLabelSides(),k=j[0],j=j[1],l=d.pos,m=l+d.len,q=this.label.line||0,o=d.labelEdge,n=d.justifyLabels&&(e||f);o[q]===u||g+k>o[q]?o[q]=g+j:n||(c=!1);if(n)B=(d=d.ticks[i[a+(e?1:-1)]])&&d.label.xy&&d.label.xy.x+d.getLabelSides()[e?0:1],i=B,e&&!h||f&&h?g+k<l&&(g=l-k,d&&g+j>i&&(c=!1)):g+j>m&&(g=m-j,d&&g+k<i&&(c=!1)),b.x=g;return c},getPosition:function(a,b,c,d){var e=this.axis,f=e.chart,g=d&&f.oldChartHeight||f.chartHeight;return{x:a?e.translate(b+c,null,null,d)+\n"
"e.transB:e.left+e.offset+(e.opposite?(d&&f.oldChartWidth||f.chartWidth)-e.right-e.left:0),y:a?g-e.bottom+e.offset-(e.opposite?e.height:0):g-e.translate(b+c,null,null,d)-e.transB}},getLabelPosition:function(a,b,c,d,e,f,g,h){var i=this.axis,j=i.transA,k=i.reversed,l=i.staggerLines,m=i.chart.renderer.fontMetrics(e.style.fontSize).b,q=e.rotation,a=a+e.x-(f&&d?f*j*(k?-1:1):0),b=b+e.y-(f&&!d?f*j*(k?1:-1):0);q&&i.side===2&&(b-=m-m*U(q*Ba));!t(e.y)&&!q&&(b+=m-c.getBBox().height/2);if(l)c.line=g/(h||1)%l,\n"
"b+=c.line*(i.labelOffset/l);return{x:a,y:b}},getMarkPath:function(a,b,c,d,e,f){return f.crispLine([\"M\",a,b,\"L\",a+(e?0:-c),b+(e?c:0)],d)},render:function(a,b,c){var d=this.axis,e=d.options,f=d.chart.renderer,g=d.horiz,h=this.type,i=this.label,j=this.pos,k=e.labels,l=this.gridLine,m=h?h+\"Grid\":\"grid\",q=h?h+\"Tick\":\"tick\",o=e[m+\"LineWidth\"],p=e[m+\"LineColor\"],H=e[m+\"LineDashStyle\"],s=e[q+\"Length\"],m=e[q+\"Width\"]||0,t=e[q+\"Color\"],w=e[q+\"Position\"],q=this.mark,r=k.step,Z=!0,x=d.tickmarkOffset,v=this.getPosition(g,\n"
"j,x,b),y=v.x,v=v.y,B=g&&y===d.pos+d.len||!g&&v===d.pos?-1:1;this.isActive=!0;if(o){j=d.getPlotLinePath(j+x,o*B,b,!0);if(l===u){l={stroke:p,\"stroke-width\":o};if(H)l.dashstyle=H;if(!h)l.zIndex=1;if(b)l.opacity=0;this.gridLine=l=o?f.path(j).attr(l).add(d.gridGroup):null}if(!b&&l&&j)l[this.isNew?\"attr\":\"animate\"]({d:j,opacity:c})}if(m&&s)w===\"inside\"&&(s=-s),d.opposite&&(s=-s),h=this.getMarkPath(y,v,s,m*B,g,f),q?q.animate({d:h,opacity:c}):this.mark=f.path(h).attr({stroke:t,\"stroke-width\":m,opacity:c}).add(d.axisGroup);\n"
"if(i&&!isNaN(y))i.xy=v=this.getLabelPosition(y,v,i,g,k,x,a,r),this.isFirst&&!this.isLast&&!n(e.showFirstLabel,1)||this.isLast&&!this.isFirst&&!n(e.showLastLabel,1)?Z=!1:!d.isRadial&&!k.step&&!k.rotation&&!b&&c!==0&&(Z=this.handleOverflow(a,v)),r&&a%r&&(Z=!1),Z&&!isNaN(v.y)?(v.opacity=c,i[this.isNew?\"attr\":\"animate\"](v),this.isNew=!1):i.attr(\"y\",-9999)},destroy:function(){Ma(this,this.axis)}};var yb=function(a,b){this.axis=a;if(b)this.options=b,this.id=b.id};yb.prototype={render:function(){var a=this,\n"
"b=a.axis,c=b.horiz,d=(b.pointRange||0)/2,e=a.options,f=e.label,g=a.label,h=e.width,i=e.to,j=e.from,k=t(j)&&t(i),l=e.value,m=e.dashStyle,q=a.svgElem,o=[],p,H=e.color,w=e.zIndex,r=e.events,u=b.chart.renderer;b.isLog&&(j=xa(j),i=xa(i),l=xa(l));if(h){if(o=b.getPlotLinePath(l,h),d={stroke:H,\"stroke-width\":h},m)d.dashstyle=m}else if(k){if(j=s(j,b.min-d),i=I(i,b.max+d),o=b.getPlotBandPath(j,i,e),d={fill:H},e.borderWidth)d.stroke=e.borderColor,d[\"stroke-width\"]=e.borderWidth}else return;if(t(w))d.zIndex=\n"
"w;if(q)if(o)q.animate({d:o},null,q.onGetPath);else{if(q.hide(),q.onGetPath=function(){q.show()},g)a.label=g=g.destroy()}else if(o&&o.length&&(a.svgElem=q=u.path(o).attr(d).add(),r))for(p in e=function(b){q.on(b,function(c){r[b].apply(a,[c])})},r)e(p);if(f&&t(f.text)&&o&&o.length&&b.width>0&&b.height>0){f=x({align:c&&k&&\"center\",x:c?!k&&4:10,verticalAlign:!c&&k&&\"middle\",y:c?k?16:10:k?6:-4,rotation:c&&!k&&90},f);if(!g)a.label=g=u.text(f.text,0,0,f.useHTML).attr({align:f.textAlign||f.align,rotation:f.rotation,\n"
"zIndex:w}).css(f.style).add();b=[o[1],o[4],n(o[6],o[1])];o=[o[2],o[5],n(o[7],o[2])];c=La(b);k=La(o);g.align(f,!1,{x:c,y:k,width:za(b)-c,height:za(o)-k});g.show()}else g&&g.hide();return a},destroy:function(){ha(this.axis.plotLinesAndBands,this);delete this.axis;Ma(this)}};qa.prototype={defaultOptions:{dateTimeLabelFormats:{millisecond:\"%H:%M:%S.%L\",second:\"%H:%M:%S\",minute:\"%H:%M\",hour:\"%H:%M\",day:\"%e. %b\",week:\"%e. %b\",month:\"%b '%y\",year:\"%Y\"},endOnTick:!1,gridLineColor:\"#C0C0C0\",labels:J,lineColor:\"#C0D0E0\",\n"
"lineWidth:1,minPadding:0.01,maxPadding:0.01,minorGridLineColor:\"#E0E0E0\",minorGridLineWidth:1,minorTickColor:\"#A0A0A0\",minorTickLength:2,minorTickPosition:\"outside\",startOfWeek:1,startOnTick:!1,tickColor:\"#C0D0E0\",tickLength:5,tickmarkPlacement:\"between\",tickPixelInterval:100,tickPosition:\"outside\",tickWidth:1,title:{align:\"middle\",style:{color:\"#4d759e\",fontWeight:\"bold\"}},type:\"linear\"},defaultYAxisOptions:{endOnTick:!0,gridLineWidth:1,tickPixelInterval:72,showLastLabel:!0,labels:{x:-8,y:3},lineWidth:0,\n"
"maxPadding:0.05,minPadding:0.05,startOnTick:!0,tickWidth:0,title:{rotation:270,text:\"Values\"},stackLabels:{enabled:!1,formatter:function(){return Da(this.total,-1)},style:J.style}},defaultLeftAxisOptions:{labels:{x:-8,y:null},title:{rotation:270}},defaultRightAxisOptions:{labels:{x:8,y:null},title:{rotation:90}},defaultBottomAxisOptions:{labels:{x:0,y:14},title:{rotation:0}},defaultTopAxisOptions:{labels:{x:0,y:-5},title:{rotation:0}},init:function(a,b){var c=b.isX;this.horiz=a.inverted?!c:c;this.coll=\n"
"(this.isXAxis=c)?\"xAxis\":\"yAxis\";this.opposite=b.opposite;this.side=b.side||(this.horiz?this.opposite?0:2:this.opposite?1:3);this.setOptions(b);var d=this.options,e=d.type;this.labelFormatter=d.labels.formatter||this.defaultLabelFormatter;this.userOptions=b;this.minPixelPadding=0;this.chart=a;this.reversed=d.reversed;this.zoomEnabled=d.zoomEnabled!==!1;this.categories=d.categories||e===\"category\";this.names=[];this.isLog=e===\"logarithmic\";this.isDatetimeAxis=e===\"datetime\";this.isLinked=t(d.linkedTo);\n"
"this.tickmarkOffset=this.categories&&d.tickmarkPlacement===\"between\"?0.5:0;this.ticks={};this.labelEdge=[];this.minorTicks={};this.plotLinesAndBands=[];this.alternateBands={};this.len=0;this.minRange=this.userMinRange=d.minRange||d.maxZoom;this.range=d.range;this.offset=d.offset||0;this.stacks={};this.oldStacks={};this.stackExtremes={};this.min=this.max=null;this.crosshair=n(d.crosshair,ja(a.options.tooltip.crosshairs)[c?0:1],!1);var f,d=this.options.events;sa(this,a.axes)===-1&&(a.axes.push(this),\n"
"a[this.coll].push(this));this.series=this.series||[];if(a.inverted&&c&&this.reversed===u)this.reversed=!0;this.removePlotLine=this.removePlotBand=this.removePlotBandOrLine;for(f in d)F(this,f,d[f]);if(this.isLog)this.val2lin=xa,this.lin2val=ga},setOptions:function(a){this.options=x(this.defaultOptions,this.isXAxis?{}:this.defaultYAxisOptions,[this.defaultTopAxisOptions,this.defaultRightAxisOptions,this.defaultBottomAxisOptions,this.defaultLeftAxisOptions][this.side],x(G[this.coll],a))},defaultLabelFormatter:function(){var a=\n"
"this.axis,b=this.value,c=a.categories,d=this.dateTimeLabelFormat,e=G.lang.numericSymbols,f=e&&e.length,g,h=a.options.labels.format,a=a.isLog?b:a.tickInterval;if(h)g=Fa(h,this);else if(c)g=b;else if(d)g=ab(d,b);else if(f&&a>=1E3)for(;f--&&g===u;)c=Math.pow(1E3,f+1),a>=c&&e[f]!==null&&(g=Da(b/c,-1)+e[f]);g===u&&(g=b>=1E4?Da(b,0):Da(b,-1,u,\"\"));return g},getSeriesExtremes:function(){var a=this,b=a.chart;a.hasVisibleSeries=!1;a.dataMin=a.dataMax=null;a.stackExtremes={};a.buildStacks();p(a.series,function(c){if(c.visible||\n"
"!b.options.chart.ignoreHiddenSeries){var d;d=c.options.threshold;var e;a.hasVisibleSeries=!0;a.isLog&&d<=0&&(d=null);if(a.isXAxis){if(d=c.xData,d.length)a.dataMin=I(n(a.dataMin,d[0]),La(d)),a.dataMax=s(n(a.dataMax,d[0]),za(d))}else{c.getExtremes();e=c.dataMax;c=c.dataMin;if(t(c)&&t(e))a.dataMin=I(n(a.dataMin,c),c),a.dataMax=s(n(a.dataMax,e),e);if(t(d))if(a.dataMin>=d)a.dataMin=d,a.ignoreMinPadding=!0;else if(a.dataMax<d)a.dataMax=d,a.ignoreMaxPadding=!0}}})},translate:function(a,b,c,d,e,f){var g=\n"
"this.len,h=1,i=0,j=d?this.oldTransA:this.transA,d=d?this.oldMin:this.min,k=this.minPixelPadding,e=(this.options.ordinal||this.isLog&&e)&&this.lin2val;if(!j)j=this.transA;c&&(h*=-1,i=g);this.reversed&&(h*=-1,i-=h*g);b?(a=a*h+i,a-=k,a=a/j+d,e&&(a=this.lin2val(a))):(e&&(a=this.val2lin(a)),f===\"between\"&&(f=0.5),a=h*(a-d)*j+i+h*k+(wa(f)?j*f*this.pointRange:0));return a},toPixels:function(a,b){return this.translate(a,!1,!this.horiz,null,!0)+(b?0:this.pos)},toValue:function(a,b){return this.translate(a-\n"
"(b?0:this.pos),!0,!this.horiz,null,!0)},getPlotLinePath:function(a,b,c,d,e){var f=this.chart,g=this.left,h=this.top,i,j,k=c&&f.oldChartHeight||f.chartHeight,l=c&&f.oldChartWidth||f.chartWidth,m;i=this.transB;e=n(e,this.translate(a,null,null,c));a=c=w(e+i);i=j=w(k-e-i);if(isNaN(e))m=!0;else if(this.horiz){if(i=h,j=k-this.bottom,a<g||a>g+this.width)m=!0}else if(a=g,c=l-this.right,i<h||i>h+this.height)m=!0;return m&&!d?null:f.renderer.crispLine([\"M\",a,i,\"L\",c,j],b||1)},getLinearTickPositions:function(a,\n"
"b,c){for(var d,b=aa(N(b/a)*a),c=aa(Ha(c/a)*a),e=[];b<=c;){e.push(b);b=aa(b+a);if(b===d)break;d=b}return e},getMinorTickPositions:function(){var a=this.options,b=this.tickPositions,c=this.minorTickInterval,d=[],e;if(this.isLog){e=b.length;for(a=1;a<e;a++)d=d.concat(this.getLogTickPositions(c,b[a-1],b[a],!0))}else if(this.isDatetimeAxis&&a.minorTickInterval===\"auto\")d=d.concat(this.getTimeTicks(this.normalizeTimeTickInterval(c),this.min,this.max,a.startOfWeek)),d[0]<this.min&&d.shift();else for(b=this.min+\n"
,
"(b[0]-this.min)%c;b<=this.max;b+=c)d.push(b);return d},adjustForMinRange:function(){var a=this.options,b=this.min,c=this.max,d,e=this.dataMax-this.dataMin>=this.minRange,f,g,h,i,j;if(this.isXAxis&&this.minRange===u&&!this.isLog)t(a.min)||t(a.max)?this.minRange=null:(p(this.series,function(a){i=a.xData;for(g=j=a.xIncrement?1:i.length-1;g>0;g--)if(h=i[g]-i[g-1],f===u||h<f)f=h}),this.minRange=I(f*5,this.dataMax-this.dataMin));if(c-b<this.minRange){var k=this.minRange;d=(k-c+b)/2;d=[b-d,n(a.min,b-d)];\n"
"if(e)d[2]=this.dataMin;b=za(d);c=[b+k,n(a.max,b+k)];if(e)c[2]=this.dataMax;c=La(c);c-b<k&&(d[0]=c-k,d[1]=n(a.min,c-k),b=za(d))}this.min=b;this.max=c},setAxisTranslation:function(a){var b=this.max-this.min,c=0,d,e=0,f=0,g=this.linkedParent,h=!!this.categories,i=this.transA;if(this.isXAxis||h)g?(e=g.minPointOffset,f=g.pointRangePadding):p(this.series,function(a){var g=s(a.pointRange,+h),i=a.options.pointPlacement,m=a.closestPointRange;g>b&&(g=0);c=s(c,g);e=s(e,fa(i)?0:g/2);f=s(f,i===\"on\"?0:g);!a.noSharedTooltip&&\n"
"t(m)&&(d=t(d)?I(d,m):m)}),g=this.ordinalSlope&&d?this.ordinalSlope/d:1,this.minPointOffset=e*=g,this.pointRangePadding=f*=g,this.pointRange=I(c,b),this.closestPointRange=d;if(a)this.oldTransA=i;this.translationSlope=this.transA=i=this.len/(b+f||1);this.transB=this.horiz?this.left:this.bottom;this.minPixelPadding=i*e},setTickPositions:function(a){var b=this,c=b.chart,d=b.options,e=b.isLog,f=b.isDatetimeAxis,g=b.isXAxis,h=b.isLinked,i=b.options.tickPositioner,j=d.maxPadding,k=d.minPadding,l=d.tickInterval,\n"
"m=d.minTickInterval,q=d.tickPixelInterval,o,ma=b.categories;h?(b.linkedParent=c[b.coll][d.linkedTo],c=b.linkedParent.getExtremes(),b.min=n(c.min,c.dataMin),b.max=n(c.max,c.dataMax),d.type!==b.linkedParent.options.type&&ka(11,1)):(b.min=n(b.userMin,d.min,b.dataMin),b.max=n(b.userMax,d.max,b.dataMax));if(e)!a&&I(b.min,n(b.dataMin,b.min))<=0&&ka(10,1),b.min=aa(xa(b.min)),b.max=aa(xa(b.max));if(b.range&&t(b.max))b.userMin=b.min=s(b.min,b.max-b.range),b.userMax=b.max,b.range=null;b.beforePadding&&b.beforePadding();\n"
"b.adjustForMinRange();if(!ma&&!b.usePercentage&&!h&&t(b.min)&&t(b.max)&&(c=b.max-b.min)){if(!t(d.min)&&!t(b.userMin)&&k&&(b.dataMin<0||!b.ignoreMinPadding))b.min-=c*k;if(!t(d.max)&&!t(b.userMax)&&j&&(b.dataMax>0||!b.ignoreMaxPadding))b.max+=c*j}b.min===b.max||b.min===void 0||b.max===void 0?b.tickInterval=1:h&&!l&&q===b.linkedParent.options.tickPixelInterval?b.tickInterval=b.linkedParent.tickInterval:(b.tickInterval=n(l,ma?1:(b.max-b.min)*q/s(b.len,q)),!t(l)&&b.len<q&&!this.isRadial&&!ma&&d.startOnTick&&\n"
"d.endOnTick&&(o=!0,b.tickInterval/=4));g&&!a&&p(b.series,function(a){a.processData(b.min!==b.oldMin||b.max!==b.oldMax)});b.setAxisTranslation(!0);b.beforeSetTickPositions&&b.beforeSetTickPositions();if(b.postProcessTickInterval)b.tickInterval=b.postProcessTickInterval(b.tickInterval);if(b.pointRange)b.tickInterval=s(b.pointRange,b.tickInterval);if(!l&&b.tickInterval<m)b.tickInterval=m;if(!f&&!e&&!l)b.tickInterval=nb(b.tickInterval,null,mb(b.tickInterval),d);b.minorTickInterval=d.minorTickInterval===\n"
"\"auto\"&&b.tickInterval?b.tickInterval/5:d.minorTickInterval;b.tickPositions=a=d.tickPositions?[].concat(d.tickPositions):i&&i.apply(b,[b.min,b.max]);if(!a)!b.ordinalPositions&&(b.max-b.min)/b.tickInterval>s(2*b.len,200)&&ka(19,!0),a=f?b.getTimeTicks(b.normalizeTimeTickInterval(b.tickInterval,d.units),b.min,b.max,d.startOfWeek,b.ordinalPositions,b.closestPointRange,!0):e?b.getLogTickPositions(b.tickInterval,b.min,b.max):b.getLinearTickPositions(b.tickInterval,b.min,b.max),o&&a.splice(1,a.length-2),\n"
"b.tickPositions=a;if(!h)e=a[0],f=a[a.length-1],h=b.minPointOffset||0,d.startOnTick?b.min=e:b.min-h>e&&a.shift(),d.endOnTick?b.max=f:b.max+h<f&&a.pop(),a.length===1&&(b.min-=0.001,b.max+=0.001)},setMaxTicks:function(){var a=this.chart,b=a.maxTicks||{},c=this.tickPositions,d=this._maxTicksKey=[this.coll,this.pos,this.len].join(\"-\");if(!this.isLinked&&!this.isDatetimeAxis&&c&&c.length>(b[d]||0)&&this.options.alignTicks!==!1)b[d]=c.length;a.maxTicks=b},adjustTickAmount:function(){var a=this._maxTicksKey,\n"
"b=this.tickPositions,c=this.chart.maxTicks;if(c&&c[a]&&!this.isDatetimeAxis&&!this.categories&&!this.isLinked&&this.options.alignTicks!==!1&&this.min!==u){var d=this.tickAmount,e=b.length;this.tickAmount=a=c[a];if(e<a){for(;b.length<a;)b.push(aa(b[b.length-1]+this.tickInterval));this.transA*=(e-1)/(a-1);this.max=b[b.length-1]}if(t(d)&&a!==d)this.isDirty=!0}},setScale:function(){var a=this.stacks,b,c,d,e;this.oldMin=this.min;this.oldMax=this.max;this.oldAxisLength=this.len;this.setAxisSize();e=this.len!==\n"
"this.oldAxisLength;p(this.series,function(a){if(a.isDirtyData||a.isDirty||a.xAxis.isDirty)d=!0});if(e||d||this.isLinked||this.forceRedraw||this.userMin!==this.oldUserMin||this.userMax!==this.oldUserMax){if(!this.isXAxis)for(b in a)for(c in a[b])a[b][c].total=null,a[b][c].cum=0;this.forceRedraw=!1;this.getSeriesExtremes();this.setTickPositions();this.oldUserMin=this.userMin;this.oldUserMax=this.userMax;if(!this.isDirty)this.isDirty=e||this.min!==this.oldMin||this.max!==this.oldMax}else if(!this.isXAxis){if(this.oldStacks)a=\n"
"this.stacks=this.oldStacks;for(b in a)for(c in a[b])a[b][c].cum=a[b][c].total}this.setMaxTicks()},setExtremes:function(a,b,c,d,e){var f=this,g=f.chart,c=n(c,!0),e=r(e,{min:a,max:b});A(f,\"setExtremes\",e,function(){f.userMin=a;f.userMax=b;f.eventArgs=e;f.isDirtyExtremes=!0;c&&g.redraw(d)})},zoom:function(a,b){this.allowZoomOutside||(t(this.dataMin)&&a<=this.dataMin&&(a=u),t(this.dataMax)&&b>=this.dataMax&&(b=u));this.displayBtn=a!==u||b!==u;this.setExtremes(a,b,!1,u,{trigger:\"zoom\"});return!0},setAxisSize:function(){var a=\n"
"this.chart,b=this.options,c=b.offsetLeft||0,d=b.offsetRight||0,e=this.horiz,f,g;this.left=g=n(b.left,a.plotLeft+c);this.top=f=n(b.top,a.plotTop);this.width=c=n(b.width,a.plotWidth-c+d);this.height=b=n(b.height,a.plotHeight);this.bottom=a.chartHeight-b-f;this.right=a.chartWidth-c-g;this.len=s(e?c:b,0);this.pos=e?g:f},getExtremes:function(){var a=this.isLog;return{min:a?aa(ga(this.min)):this.min,max:a?aa(ga(this.max)):this.max,dataMin:this.dataMin,dataMax:this.dataMax,userMin:this.userMin,userMax:this.userMax}},\n"
"getThreshold:function(a){var b=this.isLog,c=b?ga(this.min):this.min,b=b?ga(this.max):this.max;c>a||a===null?a=c:b<a&&(a=b);return this.translate(a,0,1,0,1)},autoLabelAlign:function(a){a=(n(a,0)-this.side*90+720)%360;return a>15&&a<165?\"right\":a>195&&a<345?\"left\":\"center\"},getOffset:function(){var a=this,b=a.chart,c=b.renderer,d=a.options,e=a.tickPositions,f=a.ticks,g=a.horiz,h=a.side,i=b.inverted?[1,0,3,2][h]:h,j,k=0,l,m=0,q=d.title,o=d.labels,ma=0,H=b.axisOffset,w=b.clipOffset,r=[-1,1,1,-1][h],v,\n"
"x=1,Z=n(o.maxStaggerLines,5),y,z,K,B;a.hasData=j=a.hasVisibleSeries||t(a.min)&&t(a.max)&&!!e;a.showAxis=b=j||n(d.showEmpty,!0);a.staggerLines=a.horiz&&o.staggerLines;if(!a.axisGroup)a.gridGroup=c.g(\"grid\").attr({zIndex:d.gridZIndex||1}).add(),a.axisGroup=c.g(\"axis\").attr({zIndex:d.zIndex||2}).add(),a.labelGroup=c.g(\"axis-labels\").attr({zIndex:o.zIndex||7}).add();if(j||a.isLinked){a.labelAlign=n(o.align||a.autoLabelAlign(o.rotation));p(e,function(b){f[b]?f[b].addLabel():f[b]=new Qa(a,b)});if(a.horiz&&\n"
"!a.staggerLines&&Z&&!o.rotation){for(v=a.reversed?[].concat(e).reverse():e;x<Z;){j=[];y=!1;for(o=0;o<v.length;o++)z=v[o],K=(K=f[z].label&&f[z].label.getBBox())?K.width:0,B=o%x,K&&(z=a.translate(z),j[B]!==u&&z<j[B]&&(y=!0),j[B]=z+K);if(y)x++;else break}if(x>1)a.staggerLines=x}p(e,function(b){if(h===0||h===2||{1:\"left\",3:\"right\"}[h]===a.labelAlign)ma=s(f[b].getLabelSize(),ma)});if(a.staggerLines)ma*=a.staggerLines,a.labelOffset=ma}else for(v in f)f[v].destroy(),delete f[v];if(q&&q.text&&q.enabled!==\n"
"!1){if(!a.axisTitle)a.axisTitle=c.text(q.text,0,0,q.useHTML).attr({zIndex:7,rotation:q.rotation||0,align:q.textAlign||{low:\"left\",middle:\"center\",high:\"right\"}[q.align]}).css(q.style).add(a.axisGroup),a.axisTitle.isNew=!0;if(b)k=a.axisTitle.getBBox()[g?\"height\":\"width\"],m=n(q.margin,g?5:10),l=q.offset;a.axisTitle[b?\"show\":\"hide\"]()}a.offset=r*n(d.offset,H[h]);a.axisTitleMargin=n(l,ma+m+(h!==2&&ma&&r*d.labels[g?\"y\":\"x\"]));H[h]=s(H[h],a.axisTitleMargin+k+r*a.offset);w[i]=s(w[i],N(d.lineWidth/2)*2)},\n"
"getLinePath:function(a){var b=this.chart,c=this.opposite,d=this.offset,e=this.horiz,f=this.left+(c?this.width:0)+d,d=b.chartHeight-this.bottom-(c?this.height:0)+d;c&&(a*=-1);return b.renderer.crispLine([\"M\",e?this.left:f,e?d:this.top,\"L\",e?b.chartWidth-this.right:f,e?d:b.chartHeight-this.bottom],a)},getTitlePosition:function(){var a=this.horiz,b=this.left,c=this.top,d=this.len,e=this.options.title,f=a?b:c,g=this.opposite,h=this.offset,i=z(e.style.fontSize||12),d={low:f+(a?0:d),middle:f+d/2,high:f+\n"
"(a?d:0)}[e.align],b=(a?c+this.height:b)+(a?1:-1)*(g?-1:1)*this.axisTitleMargin+(this.side===2?i:0);return{x:a?d:b+(g?this.width:0)+h+(e.x||0),y:a?b-(g?this.height:0)+h:d+(e.y||0)}},render:function(){var a=this,b=a.horiz,c=a.reversed,d=a.chart,e=d.renderer,f=a.options,g=a.isLog,h=a.isLinked,i=a.tickPositions,j,k=a.axisTitle,l=a.stacks,m=a.ticks,q=a.minorTicks,o=a.alternateBands,n=f.stackLabels,H=f.alternateGridColor,s=a.tickmarkOffset,r=f.lineWidth,w=d.hasRendered&&t(a.oldMin)&&!isNaN(a.oldMin),v=\n"
"a.hasData,x=a.showAxis,y,z=a.justifyLabels=!a.staggerLines&&b&&f.labels.overflow===\"justify\",K;a.labelEdge.length=0;p([m,q,o],function(a){for(var b in a)a[b].isActive=!1});if(v||h)if(a.minorTickInterval&&!a.categories&&p(a.getMinorTickPositions(),function(b){q[b]||(q[b]=new Qa(a,b,\"minor\"));w&&q[b].isNew&&q[b].render(null,!0);q[b].render(null,!1,1)}),i.length&&(j=i.slice(),(b&&c||!b&&!c)&&j.reverse(),z&&(j=j.slice(1).concat([j[0]])),p(j,function(b,c){z&&(c=c===j.length-1?0:c+1);if(!h||b>=a.min&&b<=\n"
,
"a.max)m[b]||(m[b]=new Qa(a,b)),w&&m[b].isNew&&m[b].render(c,!0,0.1),m[b].render(c,!1,1)}),s&&a.min===0&&(m[-1]||(m[-1]=new Qa(a,-1,null,!0)),m[-1].render(-1))),H&&p(i,function(b,c){if(c%2===0&&b<a.max)o[b]||(o[b]=new yb(a)),y=b+s,K=i[c+1]!==u?i[c+1]+s:a.max,o[b].options={from:g?ga(y):y,to:g?ga(K):K,color:H},o[b].render(),o[b].isActive=!0}),!a._addedPlotLB)p((f.plotLines||[]).concat(f.plotBands||[]),function(b){a.addPlotBandOrLine(b)}),a._addedPlotLB=!0;p([m,q,o],function(a){var b,c,e=[],f=oa?oa.duration||\n"
"500:0,g=function(){for(c=e.length;c--;)a[e[c]]&&!a[e[c]].isActive&&(a[e[c]].destroy(),delete a[e[c]])};for(b in a)if(!a[b].isActive)a[b].render(b,!1,0),a[b].isActive=!1,e.push(b);a===o||!d.hasRendered||!f?g():f&&setTimeout(g,f)});if(r)b=a.getLinePath(r),a.axisLine?a.axisLine.animate({d:b}):a.axisLine=e.path(b).attr({stroke:f.lineColor,\"stroke-width\":r,zIndex:7}).add(a.axisGroup),a.axisLine[x?\"show\":\"hide\"]();if(k&&x)k[k.isNew?\"attr\":\"animate\"](a.getTitlePosition()),k.isNew=!1;if(n&&n.enabled){var B,\n"
"A,f=a.stackTotalGroup;if(!f)a.stackTotalGroup=f=e.g(\"stack-labels\").attr({visibility:\"visible\",zIndex:6}).add();f.translate(d.plotLeft,d.plotTop);for(B in l)for(A in e=l[B],e)e[A].render(f)}a.isDirty=!1},redraw:function(){var a=this.chart.pointer;a.reset&&a.reset(!0);this.render();p(this.plotLinesAndBands,function(a){a.render()});p(this.series,function(a){a.isDirty=!0})},buildStacks:function(){var a=this.series,b=a.length;if(!this.isXAxis){for(;b--;)a[b].setStackedPoints();if(this.usePercentage)for(b=\n"
"0;b<a.length;b++)a[b].setPercentStacks()}},destroy:function(a){var b=this,c=b.stacks,d,e=b.plotLinesAndBands;a||X(b);for(d in c)Ma(c[d]),c[d]=null;p([b.ticks,b.minorTicks,b.alternateBands],function(a){Ma(a)});for(a=e.length;a--;)e[a].destroy();p(\"stackTotalGroup,axisLine,axisTitle,axisGroup,cross,gridGroup,labelGroup\".split(\",\"),function(a){b[a]&&(b[a]=b[a].destroy())});this.cross&&this.cross.destroy()},drawCrosshair:function(a,b){if(this.crosshair)if((t(b)||!n(this.crosshair.snap,!0))===!1)this.hideCrosshair();\n"
"else{var c,d=this.crosshair,e=d.animation;n(d.snap,!0)?t(b)&&(c=this.chart.inverted!=this.horiz?b.plotX:this.len-b.plotY):c=this.horiz?a.chartX-this.pos:this.len-a.chartY+this.pos;c=this.isRadial?this.getPlotLinePath(this.isXAxis?b.x:n(b.stackY,b.y)):this.getPlotLinePath(null,null,null,null,c);if(c===null)this.hideCrosshair();else if(this.cross)this.cross.attr({visibility:\"visible\"})[e?\"animate\":\"attr\"]({d:c},e);else{e={\"stroke-width\":d.width||1,stroke:d.color||\"#C0C0C0\",zIndex:d.zIndex||2};if(d.dashStyle)e.dashstyle=\n"
"d.dashStyle;this.cross=this.chart.renderer.path(c).attr(e).add()}}},hideCrosshair:function(){this.cross&&this.cross.hide()}};r(qa.prototype,{getPlotBandPath:function(a,b){var c=this.getPlotLinePath(b),d=this.getPlotLinePath(a);d&&c?d.push(c[4],c[5],c[1],c[2]):d=null;return d},addPlotBand:function(a){this.addPlotBandOrLine(a,\"plotBands\")},addPlotLine:function(a){this.addPlotBandOrLine(a,\"plotLines\")},addPlotBandOrLine:function(a,b){var c=(new yb(this,a)).render(),d=this.userOptions;c&&(b&&(d[b]=d[b]||\n"
"[],d[b].push(a)),this.plotLinesAndBands.push(c));return c},removePlotBandOrLine:function(a){for(var b=this.plotLinesAndBands,c=this.options,d=this.userOptions,e=b.length;e--;)b[e].id===a&&b[e].destroy();p([c.plotLines||[],d.plotLines||[],c.plotBands||[],d.plotBands||[]],function(b){for(e=b.length;e--;)b[e].id===a&&ha(b,b[e])})}});qa.prototype.getLogTickPositions=function(a,b,c,d){var e=this.options,f=this.len,g=[];if(!d)this._minorAutoInterval=null;if(a>=0.5)a=w(a),g=this.getLinearTickPositions(a,\n"
"b,c);else if(a>=0.08)for(var f=N(b),h,i,j,k,l,e=a>0.3?[1,2,4]:a>0.15?[1,2,4,6,8]:[1,2,3,4,5,6,7,8,9];f<c+1&&!l;f++){i=e.length;for(h=0;h<i&&!l;h++)j=xa(ga(f)*e[h]),j>b&&(!d||k<=c)&&g.push(k),k>c&&(l=!0),k=j}else if(b=ga(b),c=ga(c),a=e[d?\"minorTickInterval\":\"tickInterval\"],a=n(a===\"auto\"?null:a,this._minorAutoInterval,(c-b)*(e.tickPixelInterval/(d?5:1))/((d?f/this.tickPositions.length:f)||1)),a=nb(a,null,mb(a)),g=Ra(this.getLinearTickPositions(a,b,c),xa),!d)this._minorAutoInterval=a/5;if(!d)this.tickInterval=\n"
"a;return g};qa.prototype.getTimeTicks=function(a,b,c,d){var e=[],f={},g=G.global.useUTC,h,i=new Date(b-Pa),j=a.unitRange,k=a.count;if(t(b)){j>=E.second&&(i.setMilliseconds(0),i.setSeconds(j>=E.minute?0:k*N(i.getSeconds()/k)));if(j>=E.minute)i[Cb](j>=E.hour?0:k*N(i[pb]()/k));if(j>=E.hour)i[Db](j>=E.day?0:k*N(i[qb]()/k));if(j>=E.day)i[sb](j>=E.month?1:k*N(i[Wa]()/k));j>=E.month&&(i[Eb](j>=E.year?0:k*N(i[db]()/k)),h=i[eb]());j>=E.year&&(h-=h%k,i[Fb](h));if(j===E.week)i[sb](i[Wa]()-i[rb]()+n(d,1));b=\n"
"1;Pa&&(i=new Date(i.getTime()+Pa));h=i[eb]();for(var d=i.getTime(),l=i[db](),m=i[Wa](),q=g?Pa:(864E5+i.getTimezoneOffset()*6E4)%864E5;d<c;)e.push(d),j===E.year?d=cb(h+b*k,0):j===E.month?d=cb(h,l+b*k):!g&&(j===E.day||j===E.week)?d=cb(h,l,m+b*k*(j===E.day?1:7)):d+=j*k,b++;e.push(d);p(wb(e,function(a){return j<=E.hour&&a%E.day===q}),function(a){f[a]=\"day\"})}e.info=r(a,{higherRanks:f,totalRange:j*k});return e};qa.prototype.normalizeTimeTickInterval=function(a,b){var c=b||[[\"millisecond\",[1,2,5,10,20,\n"
"25,50,100,200,500]],[\"second\",[1,2,5,10,15,30]],[\"minute\",[1,2,5,10,15,30]],[\"hour\",[1,2,3,4,6,8,12]],[\"day\",[1,2]],[\"week\",[1,2]],[\"month\",[1,2,3,4,6]],[\"year\",null]],d=c[c.length-1],e=E[d[0]],f=d[1],g;for(g=0;g<c.length;g++)if(d=c[g],e=E[d[0]],f=d[1],c[g+1]&&a<=(e*f[f.length-1]+E[c[g+1][0]])/2)break;e===E.year&&a<5*e&&(f=[1,2,5]);c=nb(a/e,f,d[0]===\"year\"?s(mb(a/e),1):1);return{unitRange:e,count:c,unitName:d[0]}};Gb.prototype={destroy:function(){Ma(this,this.axis)},render:function(a){var b=this.options,\n"
"c=b.format,c=c?Fa(c,this):b.formatter.call(this);this.label?this.label.attr({text:c,visibility:\"hidden\"}):this.label=this.axis.chart.renderer.text(c,0,0,b.useHTML).css(b.style).attr({align:this.textAlign,rotation:b.rotation,visibility:\"hidden\"}).add(a)},setOffset:function(a,b){var c=this.axis,d=c.chart,e=d.inverted,f=this.isNegative,g=c.translate(this.percent?100:this.total,0,0,0,1),c=c.translate(0),c=M(g-c),h=d.xAxis[0].translate(this.x)+a,i=d.plotHeight,f={x:e?f?g:g-c:h,y:e?i-h-b:f?i-g-c:i-g,width:e?\n"
"c:b,height:e?b:c};if(e=this.label)e.align(this.alignOptions,null,f),f=e.alignAttr,e.attr({visibility:this.options.crop===!1||d.isInsidePlot(f.x,f.y)?V?\"inherit\":\"visible\":\"hidden\"})}};tb.prototype={init:function(a,b){var c=b.borderWidth,d=b.style,e=z(d.padding);this.chart=a;this.options=b;this.crosshairs=[];this.now={x:0,y:0};this.isHidden=!0;this.label=a.renderer.label(\"\",0,0,b.shape,null,null,b.useHTML,null,\"tooltip\").attr({padding:e,fill:b.backgroundColor,\"stroke-width\":c,r:b.borderRadius,zIndex:8}).css(d).css({padding:0}).add().attr({y:-999});\n"
"da||this.label.shadow(b.shadow);this.shared=b.shared},destroy:function(){if(this.label)this.label=this.label.destroy();clearTimeout(this.hideTimer);clearTimeout(this.tooltipTimeout)},move:function(a,b,c,d){var e=this,f=e.now,g=e.options.animation!==!1&&!e.isHidden;r(f,{x:g?(2*f.x+a)/3:a,y:g?(f.y+b)/2:b,anchorX:g?(2*f.anchorX+c)/3:c,anchorY:g?(f.anchorY+d)/2:d});e.label.attr(f);if(g&&(M(a-f.x)>1||M(b-f.y)>1))clearTimeout(this.tooltipTimeout),this.tooltipTimeout=setTimeout(function(){e&&e.move(a,b,\n"
"c,d)},32)},hide:function(){var a=this,b;clearTimeout(this.hideTimer);if(!this.isHidden)b=this.chart.hoverPoints,this.hideTimer=setTimeout(function(){a.label.fadeOut();a.isHidden=!0},n(this.options.hideDelay,500)),b&&p(b,function(a){a.setState()}),this.chart.hoverPoints=null},getAnchor:function(a,b){var c,d=this.chart,e=d.inverted,f=d.plotTop,g=0,h=0,i,a=ja(a);c=a[0].tooltipPos;this.followPointer&&b&&(b.chartX===u&&(b=d.pointer.normalize(b)),c=[b.chartX-d.plotLeft,b.chartY-f]);c||(p(a,function(a){i=\n"
"a.series.yAxis;g+=a.plotX;h+=(a.plotLow?(a.plotLow+a.plotHigh)/2:a.plotY)+(!e&&i?i.top-f:0)}),g/=a.length,h/=a.length,c=[e?d.plotWidth-h:g,this.shared&&!e&&a.length>1&&b?b.chartY-f:e?d.plotHeight-g:h]);return Ra(c,w)},getPosition:function(a,b,c){var d=this.chart,e=d.plotLeft,f=d.plotTop,g=d.plotWidth,h=d.plotHeight,i=n(this.options.distance,12),j=c.plotX,c=c.plotY,d=j+e+(d.inverted?i:-a-i),k=c-b+f+15,l;d<7&&(d=e+s(j,0)+i);d+a>e+g&&(d-=d+a-(e+g),k=c-b+f-i,l=!0);k<f+5&&(k=f+5,l&&c>=k&&c<=k+b&&(k=c+\n"
"f+i));k+b>f+h&&(k=s(f,f+h-b-i));return{x:d,y:k}},defaultFormatter:function(a){var b=this.points||ja(this),c=b[0].series,d;d=[c.tooltipHeaderFormatter(b[0])];p(b,function(a){c=a.series;d.push(c.tooltipFormatter&&c.tooltipFormatter(a)||a.point.tooltipFormatter(c.tooltipOptions.pointFormat))});d.push(a.options.footerFormat||\"\");return d.join(\"\")},refresh:function(a,b){var c=this.chart,d=this.label,e=this.options,f,g,h={},i,j=[];i=e.formatter||this.defaultFormatter;var h=c.hoverPoints,k,l=this.shared;\n"
"clearTimeout(this.hideTimer);this.followPointer=ja(a)[0].series.tooltipOptions.followPointer;g=this.getAnchor(a,b);f=g[0];g=g[1];l&&(!a.series||!a.series.noSharedTooltip)?(c.hoverPoints=a,h&&p(h,function(a){a.setState()}),p(a,function(a){a.setState(\"hover\");j.push(a.getLabelConfig())}),h={x:a[0].category,y:a[0].y},h.points=j,a=a[0]):h=a.getLabelConfig();i=i.call(h,this);h=a.series;i===!1?this.hide():(this.isHidden&&(Za(d),d.attr(\"opacity\",1).show()),d.attr({text:i}),k=e.borderColor||a.color||h.color||\n"
"\"#606060\",d.attr({stroke:k}),this.updatePosition({plotX:f,plotY:g}),this.isHidden=!1);A(c,\"tooltipRefresh\",{text:i,x:f+c.plotLeft,y:g+c.plotTop,borderColor:k})},updatePosition:function(a){var b=this.chart,c=this.label,c=(this.options.positioner||this.getPosition).call(this,c.width,c.height,a);this.move(w(c.x),w(c.y),a.plotX+b.plotLeft,a.plotY+b.plotTop)}};var $a=Highcharts.Pointer=function(a,b){this.init(a,b)};$a.prototype={init:function(a,b){var c=b.chart,d=c.events,e=da?\"\":c.zoomType,c=a.inverted,\n"
,
"f;this.options=b;this.chart=a;this.zoomX=f=/x/.test(e);this.zoomY=e=/y/.test(e);this.zoomHor=f&&!c||e&&c;this.zoomVert=e&&!c||f&&c;this.runChartClick=d&&!!d.click;this.pinchDown=[];this.lastValidTouch={};if(b.tooltip.enabled)a.tooltip=new tb(a,b.tooltip);this.setDOMEvents()},normalize:function(a,b){var c,d,a=a||C.event;if(!a.target)a.target=a.srcElement;a=Rb(a);d=a.touches?a.touches.item(0):a;if(!b)this.chartPosition=b=Qb(this.chart.container);d.pageX===u?(c=s(a.x,a.clientX-b.left),d=a.y):(c=d.pageX-\n"
"b.left,d=d.pageY-b.top);return r(a,{chartX:w(c),chartY:w(d)})},getCoordinates:function(a){var b={xAxis:[],yAxis:[]};p(this.chart.axes,function(c){b[c.isXAxis?\"xAxis\":\"yAxis\"].push({axis:c,value:c.toValue(a[c.horiz?\"chartX\":\"chartY\"])})});return b},getIndex:function(a){var b=this.chart;return b.inverted?b.plotHeight+b.plotTop-a.chartY:a.chartX-b.plotLeft},runPointActions:function(a){var b=this,c=b.chart,d=c.series,e=c.tooltip,f,g,h=c.hoverPoint,i=c.hoverSeries,j,k,l=c.chartWidth,m=b.getIndex(a);if(e&&\n"
"b.options.tooltip.shared&&(!i||!i.noSharedTooltip)){g=[];j=d.length;for(k=0;k<j;k++)if(d[k].visible&&d[k].options.enableMouseTracking!==!1&&!d[k].noSharedTooltip&&d[k].tooltipPoints.length&&(f=d[k].tooltipPoints[m])&&f.series)f._dist=M(m-f.clientX),l=I(l,f._dist),g.push(f);for(j=g.length;j--;)g[j]._dist>l&&g.splice(j,1);if(g.length&&g[0].clientX!==b.hoverX)e.refresh(g,a),b.hoverX=g[0].clientX}if(i&&i.tracker){if((f=i.tooltipPoints[m])&&f!==h)f.onMouseOver(a)}else e&&e.followPointer&&!e.isHidden&&\n"
"(d=e.getAnchor([{}],a),e.updatePosition({plotX:d[0],plotY:d[1]}));if(e&&!b._onDocumentMouseMove)b._onDocumentMouseMove=function(a){b.onDocumentMouseMove(a)},F(y,\"mousemove\",b._onDocumentMouseMove);p(c.axes,function(b){b.drawCrosshair(a,n(f,h))})},reset:function(a){var b=this.chart,c=b.hoverSeries,d=b.hoverPoint,e=b.tooltip,f=e&&e.shared?b.hoverPoints:d;(a=a&&e&&f)&&ja(f)[0].plotX===u&&(a=!1);if(a)e.refresh(f),d&&d.setState(d.state,!0);else{if(d)d.onMouseOut();if(c)c.onMouseOut();e&&e.hide();if(this._onDocumentMouseMove)X(y,\n"
"\"mousemove\",this._onDocumentMouseMove),this._onDocumentMouseMove=null;p(b.axes,function(a){a.hideCrosshair()});this.hoverX=null}},scaleGroups:function(a,b){var c=this.chart,d;p(c.series,function(e){d=a||e.getPlotBox();e.xAxis&&e.xAxis.zoomEnabled&&(e.group.attr(d),e.markerGroup&&(e.markerGroup.attr(d),e.markerGroup.clip(b?c.clipRect:null)),e.dataLabelsGroup&&e.dataLabelsGroup.attr(d))});c.clipRect.attr(b||c.clipBox)},pinchTranslate:function(a,b,c,d,e,f,g,h){a&&this.pinchTranslateDirection(!0,c,d,\n"
"e,f,g,h);b&&this.pinchTranslateDirection(!1,c,d,e,f,g,h)},pinchTranslateDirection:function(a,b,c,d,e,f,g,h){var i=this.chart,j=a?\"x\":\"y\",k=a?\"X\":\"Y\",l=\"chart\"+k,m=a?\"width\":\"height\",q=i[\"plot\"+(a?\"Left\":\"Top\")],o,n,p=h||1,s=i.inverted,t=i.bounds[a?\"h\":\"v\"],r=b.length===1,w=b[0][l],u=c[0][l],v=!r&&b[1][l],x=!r&&c[1][l],y,c=function(){!r&&M(w-v)>20&&(p=h||M(u-x)/M(w-v));n=(q-u)/p+w;o=i[\"plot\"+(a?\"Width\":\"Height\")]/p};c();b=n;b<t.min?(b=t.min,y=!0):b+o>t.max&&(b=t.max-o,y=!0);y?(u-=0.8*(u-g[j][0]),r||\n"
"(x-=0.8*(x-g[j][1])),c()):g[j]=[u,x];s||(f[j]=n-q,f[m]=o);f=s?1/p:p;e[m]=o;e[j]=b;d[s?a?\"scaleY\":\"scaleX\":\"scale\"+k]=p;d[\"translate\"+k]=f*q+(u-f*w)},pinch:function(a){var b=this,c=b.chart,d=b.pinchDown,e=c.tooltip&&c.tooltip.options.followTouchMove,f=a.touches,g=f.length,h=b.lastValidTouch,i=b.zoomHor||b.pinchHor,j=b.zoomVert||b.pinchVert,k=i||j,l=b.selectionMarker,m={},q=g===1&&(b.inClass(a.target,\"highcharts-tracker\")&&c.runTrackerClick||c.runChartClick),o={};(k||e)&&!q&&a.preventDefault();Ra(f,\n"
"function(a){return b.normalize(a)});if(a.type===\"touchstart\")p(f,function(a,b){d[b]={chartX:a.chartX,chartY:a.chartY}}),h.x=[d[0].chartX,d[1]&&d[1].chartX],h.y=[d[0].chartY,d[1]&&d[1].chartY],p(c.axes,function(a){if(a.zoomEnabled){var b=c.bounds[a.horiz?\"h\":\"v\"],d=a.minPixelPadding,e=a.toPixels(a.dataMin),f=a.toPixels(a.dataMax),g=I(e,f),e=s(e,f);b.min=I(a.pos,g-d);b.max=s(a.pos+a.len,e+d)}});else if(d.length){if(!l)b.selectionMarker=l=r({destroy:la},c.plotBox);b.pinchTranslate(i,j,d,f,m,l,o,h);b.hasPinched=\n"
"k;b.scaleGroups(m,o);!k&&e&&g===1&&this.runPointActions(b.normalize(a))}},dragStart:function(a){var b=this.chart;b.mouseIsDown=a.type;b.cancelClick=!1;b.mouseDownX=this.mouseDownX=a.chartX;b.mouseDownY=this.mouseDownY=a.chartY},drag:function(a){var b=this.chart,c=b.options.chart,d=a.chartX,e=a.chartY,f=this.zoomHor,g=this.zoomVert,h=b.plotLeft,i=b.plotTop,j=b.plotWidth,k=b.plotHeight,l,m=this.mouseDownX,q=this.mouseDownY;d<h?d=h:d>h+j&&(d=h+j);e<i?e=i:e>i+k&&(e=i+k);this.hasDragged=Math.sqrt(Math.pow(m-\n"
"d,2)+Math.pow(q-e,2));if(this.hasDragged>10){l=b.isInsidePlot(m-h,q-i);if(b.hasCartesianSeries&&(this.zoomX||this.zoomY)&&l&&!this.selectionMarker)this.selectionMarker=b.renderer.rect(h,i,f?1:j,g?1:k,0).attr({fill:c.selectionMarkerFill||\"rgba(69,114,167,0.25)\",zIndex:7}).add();this.selectionMarker&&f&&(d-=m,this.selectionMarker.attr({width:M(d),x:(d>0?0:d)+m}));this.selectionMarker&&g&&(d=e-q,this.selectionMarker.attr({height:M(d),y:(d>0?0:d)+q}));l&&!this.selectionMarker&&c.panning&&b.pan(a,c.panning)}},\n"
"drop:function(a){var b=this.chart,c=this.hasPinched;if(this.selectionMarker){var d={xAxis:[],yAxis:[],originalEvent:a.originalEvent||a},e=this.selectionMarker,f=e.x,g=e.y,h;if(this.hasDragged||c)p(b.axes,function(a){if(a.zoomEnabled){var b=a.horiz,c=a.toValue(b?f:g),b=a.toValue(b?f+e.width:g+e.height);!isNaN(c)&&!isNaN(b)&&(d[a.coll].push({axis:a,min:I(c,b),max:s(c,b)}),h=!0)}}),h&&A(b,\"selection\",d,function(a){b.zoom(r(a,c?{animation:!1}:null))});this.selectionMarker=this.selectionMarker.destroy();\n"
"c&&this.scaleGroups()}if(b)D(b.container,{cursor:b._cursor}),b.cancelClick=this.hasDragged>10,b.mouseIsDown=this.hasDragged=this.hasPinched=!1,this.pinchDown=[]},onContainerMouseDown:function(a){a=this.normalize(a);a.preventDefault&&a.preventDefault();this.dragStart(a)},onDocumentMouseUp:function(a){this.drop(a)},onDocumentMouseMove:function(a){var b=this.chart,c=this.chartPosition,d=b.hoverSeries,a=this.normalize(a,c);c&&d&&!this.inClass(a.target,\"highcharts-tracker\")&&!b.isInsidePlot(a.chartX-b.plotLeft,\n"
"a.chartY-b.plotTop)&&this.reset()},onContainerMouseLeave:function(){this.reset();this.chartPosition=null},onContainerMouseMove:function(a){var b=this.chart,a=this.normalize(a);b.mouseIsDown===\"mousedown\"&&this.drag(a);(this.inClass(a.target,\"highcharts-tracker\")||b.isInsidePlot(a.chartX-b.plotLeft,a.chartY-b.plotTop))&&!b.openMenu&&this.runPointActions(a)},inClass:function(a,b){for(var c;a;){if(c=v(a,\"class\"))if(c.indexOf(b)!==-1)return!0;else if(c.indexOf(\"highcharts-container\")!==-1)return!1;a=\n"
"a.parentNode}},onTrackerMouseOut:function(a){var b=this.chart.hoverSeries,c=(a=a.relatedTarget||a.toElement)&&a.point&&a.point.series;if(b&&!b.options.stickyTracking&&!this.inClass(a,\"highcharts-tooltip\")&&c!==b)b.onMouseOut()},onContainerClick:function(a){var b=this.chart,c=b.hoverPoint,d=b.plotLeft,e=b.plotTop,f=b.inverted,g,h,i,a=this.normalize(a);a.cancelBubble=!0;if(!b.cancelClick)c&&this.inClass(a.target,\"highcharts-tracker\")?(g=this.chartPosition,h=c.plotX,i=c.plotY,r(c,{pageX:g.left+d+(f?\n"
"b.plotWidth-i:h),pageY:g.top+e+(f?b.plotHeight-h:i)}),A(c.series,\"click\",r(a,{point:c})),b.hoverPoint&&c.firePointEvent(\"click\",a)):(r(a,this.getCoordinates(a)),b.isInsidePlot(a.chartX-d,a.chartY-e)&&A(b,\"click\",a))},onContainerTouchStart:function(a){var b=this.chart;a.touches.length===1?(a=this.normalize(a),b.isInsidePlot(a.chartX-b.plotLeft,a.chartY-b.plotTop)?(this.runPointActions(a),this.pinch(a)):this.reset()):a.touches.length===2&&this.pinch(a)},onContainerTouchMove:function(a){(a.touches.length===\n"
"1||a.touches.length===2)&&this.pinch(a)},onDocumentTouchEnd:function(a){this.drop(a)},setDOMEvents:function(){var a=this,b=a.chart.container,c;this._events=c=[[b,\"onmousedown\",\"onContainerMouseDown\"],[b,\"onmousemove\",\"onContainerMouseMove\"],[b,\"onclick\",\"onContainerClick\"],[b,\"mouseleave\",\"onContainerMouseLeave\"],[y,\"mouseup\",\"onDocumentMouseUp\"]];ib&&c.push([b,\"ontouchstart\",\"onContainerTouchStart\"],[b,\"ontouchmove\",\"onContainerTouchMove\"],[y,\"touchend\",\"onDocumentTouchEnd\"]);p(c,function(b){a[\"_\"+\n"
"b[2]]=function(c){a[b[2]](c)};b[1].indexOf(\"on\")===0?b[0][b[1]]=a[\"_\"+b[2]]:F(b[0],b[1],a[\"_\"+b[2]])})},destroy:function(){var a=this;p(a._events,function(b){b[1].indexOf(\"on\")===0?b[0][b[1]]=null:X(b[0],b[1],a[\"_\"+b[2]])});delete a._events;clearInterval(a.tooltipTimeout)}};J=Highcharts.TrackerMixin={drawTrackerPoint:function(){var a=this,b=a.chart,c=b.pointer,d=a.options.cursor,e=d&&{cursor:d},f=function(c){var d=c.target,e;if(b.hoverSeries!==a)a.onMouseOver();for(;d&&!e;)e=d.point,d=d.parentNode;\n"
"if(e!==u&&e!==b.hoverPoint)e.onMouseOver(c)};p(a.points,function(a){if(a.graphic)a.graphic.element.point=a;if(a.dataLabel)a.dataLabel.element.point=a});if(!a._hasTracking)p(a.trackerGroups,function(b){if(a[b]&&(a[b].addClass(\"highcharts-tracker\").on(\"mouseover\",f).on(\"mouseout\",function(a){c.onTrackerMouseOut(a)}).css(e),ib))a[b].on(\"touchstart\",f)}),a._hasTracking=!0},drawTrackerGraph:function(){var a=this,b=a.options,c=b.trackByArea,d=[].concat(c?a.areaPath:a.graphPath),e=d.length,f=a.chart,g=f.pointer,\n"
"h=f.renderer,i=f.options.tooltip.snap,j=a.tracker,k=b.cursor,l=k&&{cursor:k},k=a.singlePoints,m,q=function(){if(f.hoverSeries!==a)a.onMouseOver()};if(e&&!c)for(m=e+1;m--;)d[m]===\"M\"&&d.splice(m+1,0,d[m+1]-i,d[m+2],\"L\"),(m&&d[m]===\"M\"||m===e)&&d.splice(m,0,\"L\",d[m-2]+i,d[m-1]);for(m=0;m<k.length;m++)e=k[m],d.push(\"M\",e.plotX-i,e.plotY,\"L\",e.plotX+i,e.plotY);j?j.attr({d:d}):(a.tracker=h.path(d).attr({\"stroke-linejoin\":\"round\",visibility:a.visible?\"visible\":\"hidden\",stroke:Kb,fill:c?Kb:Q,\"stroke-width\":b.lineWidth+\n"
,
"(c?0:2*i),zIndex:2}).add(a.group),p([a.tracker,a.markerGroup],function(a){a.addClass(\"highcharts-tracker\").on(\"mouseover\",q).on(\"mouseout\",function(a){g.onTrackerMouseOut(a)}).css(l);if(ib)a.on(\"touchstart\",q)}))}};if(C.PointerEvent||C.MSPointerEvent){var na={};$a.prototype.getWebkitTouches=function(){var a,b=[];b.item=function(a){return this[a]};for(a in na)na.hasOwnProperty(a)&&b.push({pageX:na[a].pageX,pageY:na[a].pageY,target:na[a].target});return b};Va($a.prototype,\"init\",function(a,b,c){b.container.style[\"-ms-touch-action\"]=\n"
"b.container.style[\"touch-action\"]=\"none\";a.call(this,b,c)});Va($a.prototype,\"setDOMEvents\",function(a){var b=this;a.apply(this,Array.prototype.slice.call(arguments,1));p([[this.chart.container,\"PointerDown\",\"touchstart\",\"onContainerTouchStart\",function(a){na[a.pointerId]={pageX:a.pageX,pageY:a.pageY,target:a.currentTarget}}],[this.chart.container,\"PointerMove\",\"touchmove\",\"onContainerTouchMove\",function(a){na[a.pointerId]={pageX:a.pageX,pageY:a.pageY};if(!na[a.pointerId].target)na[a.pointerId].target=\n"
"a.currentTarget}],[document,\"PointerUp\",\"touchend\",\"onDocumentTouchEnd\",function(a){delete na[a.pointerId]}]],function(a){F(a[0],window.PointerEvent?a[1].toLowerCase():\"MS\"+a[1],function(d){d=d.originalEvent;if(d.pointerType===\"touch\"||d.pointerType===d.MSPOINTER_TYPE_TOUCH)a[4](d),b[a[3]]({type:a[2],target:d.currentTarget,preventDefault:la,touches:b.getWebkitTouches()})})})})}var zb=Highcharts.Legend=function(a,b){this.init(a,b)};zb.prototype={init:function(a,b){var c=this,d=b.itemStyle,e=n(b.padding,\n"
"8),f=b.itemMarginTop||0;this.options=b;if(b.enabled)c.baseline=z(d.fontSize)+3+f,c.itemStyle=d,c.itemHiddenStyle=x(d,b.itemHiddenStyle),c.itemMarginTop=f,c.padding=e,c.initialItemX=e,c.initialItemY=e-5,c.maxItemWidth=0,c.chart=a,c.itemHeight=0,c.lastLineHeight=0,c.symbolWidth=n(b.symbolWidth,16),c.pages=[],c.render(),F(c.chart,\"endResize\",function(){c.positionCheckboxes()})},colorizeItem:function(a,b){var c=this.options,d=a.legendItem,e=a.legendLine,f=a.legendSymbol,g=this.itemHiddenStyle.color,c=\n"
"b?c.itemStyle.color:g,h=b?a.legendColor||a.color:g,g=a.options&&a.options.marker,i={stroke:h,fill:h},j;d&&d.css({fill:c,color:c});e&&e.attr({stroke:h});if(f){if(g&&f.isMarker)for(j in g=a.convertAttribs(g),g)d=g[j],d!==u&&(i[j]=d);f.attr(i)}},positionItem:function(a){var b=this.options,c=b.symbolPadding,b=!b.rtl,d=a._legendItemPos,e=d[0],d=d[1],f=a.checkbox;a.legendGroup&&a.legendGroup.translate(b?e:this.legendWidth-e-2*c-4,d);if(f)f.x=e,f.y=d},destroyItem:function(a){var b=a.checkbox;p([\"legendItem\",\n"
"\"legendLine\",\"legendSymbol\",\"legendGroup\"],function(b){a[b]&&(a[b]=a[b].destroy())});b&&Na(a.checkbox)},destroy:function(){var a=this.group,b=this.box;if(b)this.box=b.destroy();if(a)this.group=a.destroy()},positionCheckboxes:function(a){var b=this.group.alignAttr,c,d=this.clipHeight||this.legendHeight;if(b)c=b.translateY,p(this.allItems,function(e){var f=e.checkbox,g;f&&(g=c+f.y+(a||0)+3,D(f,{left:b.translateX+e.legendItemWidth+f.x-20+\"px\",top:g+\"px\",display:g>c-6&&g<c+d-6?\"\":Q}))})},renderTitle:function(){var a=\n"
"this.padding,b=this.options.title,c=0;if(b.text){if(!this.title)this.title=this.chart.renderer.label(b.text,a-3,a-4,null,null,null,null,null,\"legend-title\").attr({zIndex:1}).css(b.style).add(this.group);a=this.title.getBBox();c=a.height;this.offsetWidth=a.width;this.contentGroup.attr({translateY:c})}this.titleHeight=c},renderItem:function(a){var B;var b=this,c=b.chart,d=c.renderer,e=b.options,f=e.layout===\"horizontal\",g=b.symbolWidth,h=e.symbolPadding,i=b.itemStyle,j=b.itemHiddenStyle,k=b.padding,\n"
"l=f?n(e.itemDistance,8):0,m=!e.rtl,q=e.width,o=e.itemMarginBottom||0,p=b.itemMarginTop,t=b.initialItemX,r=a.legendItem,u=a.series&&a.series.drawLegendSymbol?a.series:a,v=u.options,v=v&&v.showCheckbox,y=e.useHTML;if(!r&&(a.legendGroup=d.g(\"legend-item\").attr({zIndex:1}).add(b.scrollGroup),u.drawLegendSymbol(b,a),a.legendItem=r=d.text(e.labelFormat?Fa(e.labelFormat,a):e.labelFormatter.call(a),m?g+h:-h,b.baseline,y).css(x(a.visible?i:j)).attr({align:m?\"left\":\"right\",zIndex:2}).add(a.legendGroup),(y?\n"
"r:a.legendGroup).on(\"mouseover\",function(){a.setState(\"hover\");r.css(b.options.itemHoverStyle)}).on(\"mouseout\",function(){r.css(a.visible?i:j);a.setState()}).on(\"click\",function(b){var c=function(){a.setVisible()},b={browserEvent:b};a.firePointEvent?a.firePointEvent(\"legendItemClick\",b,c):A(a,\"legendItemClick\",b,c)}),b.colorizeItem(a,a.visible),v))a.checkbox=T(\"input\",{type:\"checkbox\",checked:a.selected,defaultChecked:a.selected},e.itemCheckboxStyle,c.container),F(a.checkbox,\"click\",function(b){A(a,\n"
"\"checkboxClick\",{checked:b.target.checked},function(){a.select()})});d=r.getBBox();B=a.legendItemWidth=e.itemWidth||a.legendItemWidth||g+h+d.width+l+(v?20:0),e=B;b.itemHeight=g=w(a.legendItemHeight||d.height);if(f&&b.itemX-t+e>(q||c.chartWidth-2*k-t))b.itemX=t,b.itemY+=p+b.lastLineHeight+o,b.lastLineHeight=0;b.maxItemWidth=s(b.maxItemWidth,e);b.lastItemY=p+b.itemY+o;b.lastLineHeight=s(g,b.lastLineHeight);a._legendItemPos=[b.itemX,b.itemY];f?b.itemX+=e:(b.itemY+=p+g+o,b.lastLineHeight=g);b.offsetWidth=\n"
"q||s((f?b.itemX-t-l:e)+k,b.offsetWidth)},getAllItems:function(){var a=[];p(this.chart.series,function(b){var c=b.options;if(n(c.showInLegend,!t(c.linkedTo)?u:!1,!0))a=a.concat(b.legendItems||(c.legendType===\"point\"?b.data:b))});return a},render:function(){var a=this,b=a.chart,c=b.renderer,d=a.group,e,f,g,h,i=a.box,j=a.options,k=a.padding,l=j.borderWidth,m=j.backgroundColor;a.itemX=a.initialItemX;a.itemY=a.initialItemY;a.offsetWidth=0;a.lastItemY=0;if(!d)a.group=d=c.g(\"legend\").attr({zIndex:7}).add(),\n"
"a.contentGroup=c.g().attr({zIndex:1}).add(d),a.scrollGroup=c.g().add(a.contentGroup);a.renderTitle();e=a.getAllItems();ob(e,function(a,b){return(a.options&&a.options.legendIndex||0)-(b.options&&b.options.legendIndex||0)});j.reversed&&e.reverse();a.allItems=e;a.display=f=!!e.length;p(e,function(b){a.renderItem(b)});g=j.width||a.offsetWidth;h=a.lastItemY+a.lastLineHeight+a.titleHeight;h=a.handleOverflow(h);if(l||m){g+=k;h+=k;if(i){if(g>0&&h>0)i[i.isNew?\"attr\":\"animate\"](i.crisp(null,null,null,g,h)),\n"
"i.isNew=!1}else a.box=i=c.rect(0,0,g,h,j.borderRadius,l||0).attr({stroke:j.borderColor,\"stroke-width\":l||0,fill:m||Q}).add(d).shadow(j.shadow),i.isNew=!0;i[f?\"show\":\"hide\"]()}a.legendWidth=g;a.legendHeight=h;p(e,function(b){a.positionItem(b)});f&&d.align(r({width:g,height:h},j),!0,\"spacingBox\");b.isResizing||this.positionCheckboxes()},handleOverflow:function(a){var b=this,c=this.chart,d=c.renderer,e=this.options,f=e.y,f=c.spacingBox.height+(e.verticalAlign===\"top\"?-f:f)-this.padding,g=e.maxHeight,\n"
"h,i=this.clipRect,j=e.navigation,k=n(j.animation,!0),l=j.arrowSize||12,m=this.nav,q=this.pages,o,s=this.allItems;e.layout===\"horizontal\"&&(f/=2);g&&(f=I(f,g));q.length=0;if(a>f&&!e.useHTML){this.clipHeight=h=f-20-this.titleHeight-this.padding;this.currentPage=n(this.currentPage,1);this.fullHeight=a;p(s,function(a,b){var c=a._legendItemPos[1],d=w(a.legendItem.bBox.height),e=q.length;if(!e||c-q[e-1]>h)q.push(o||c);b===s.length-1&&c+d-q[e-1]>h&&q.push(c);c!==o&&(o=c)});if(!i)i=b.clipRect=d.clipRect(0,\n"
"this.padding,9999,0),b.contentGroup.clip(i);i.attr({height:h});if(!m)this.nav=m=d.g().attr({zIndex:1}).add(this.group),this.up=d.symbol(\"triangle\",0,0,l,l).on(\"click\",function(){b.scroll(-1,k)}).add(m),this.pager=d.text(\"\",15,10).css(j.style).add(m),this.down=d.symbol(\"triangle-down\",0,0,l,l).on(\"click\",function(){b.scroll(1,k)}).add(m);b.scroll(0);a=f}else if(m)i.attr({height:c.chartHeight}),m.hide(),this.scrollGroup.attr({translateY:1}),this.clipHeight=0;return a},scroll:function(a,b){var c=this.pages,\n"
"d=c.length,e=this.currentPage+a,f=this.clipHeight,g=this.options.navigation,h=g.activeColor,g=g.inactiveColor,i=this.pager,j=this.padding;e>d&&(e=d);if(e>0)b!==u&&Oa(b,this.chart),this.nav.attr({translateX:j,translateY:f+this.padding+7+this.titleHeight,visibility:\"visible\"}),this.up.attr({fill:e===1?g:h}).css({cursor:e===1?\"default\":\"pointer\"}),i.attr({text:e+\"/\"+d}),this.down.attr({x:18+this.pager.getBBox().width,fill:e===d?g:h}).css({cursor:e===d?\"default\":\"pointer\"}),c=-c[e-1]+this.initialItemY,\n"
"this.scrollGroup.animate({translateY:c}),this.currentPage=e,this.positionCheckboxes(c)}};R=Highcharts.LegendSymbolMixin={drawRectangle:function(a,b){var c=a.options.symbolHeight||12;b.legendSymbol=this.chart.renderer.rect(0,a.baseline-5-c/2,a.symbolWidth,c,n(a.options.symbolRadius,2)).attr({zIndex:3}).add(b.legendGroup)},drawLineMarker:function(a){var b=this.options,c=b.marker,d;d=a.symbolWidth;var e=this.chart.renderer,f=this.legendGroup,a=a.baseline-w(e.fontMetrics(a.options.itemStyle.fontSize).b*\n"
"0.3),g;if(b.lineWidth){g={\"stroke-width\":b.lineWidth};if(b.dashStyle)g.dashstyle=b.dashStyle;this.legendLine=e.path([\"M\",0,a,\"L\",d,a]).attr(g).add(f)}if(c&&c.enabled)b=c.radius,this.legendSymbol=d=e.symbol(this.symbol,d/2-b,a-b,2*b,2*b).add(f),d.isMarker=!0}};/Trident\\/7\\.0/.test(ra)&&Va(zb.prototype,\"positionItem\",function(a,b){var c=this,d=function(){b._legendItemPos&&a.call(c,b)};c.chart.renderer.forExport?d():setTimeout(d)});fb.prototype={init:function(a,b){var c,d=a.series;a.series=null;c=x(G,\n"
"a);c.series=a.series=d;this.userOptions=a;d=c.chart;this.margin=this.splashArray(\"margin\",d);this.spacing=this.splashArray(\"spacing\",d);var e=d.events;this.bounds={h:{},v:{}};this.callback=b;this.isResizing=0;this.options=c;this.axes=[];this.series=[];this.hasCartesianSeries=d.showAxes;var f=this,g;f.index=Ia.length;Ia.push(f);d.reflow!==!1&&F(f,\"load\",function(){f.initReflow()});if(e)for(g in e)F(f,g,e[g]);f.xAxis=[];f.yAxis=[];f.animation=da?!1:n(d.animation,!0);f.pointCount=0;f.counters=new Ab;\n"
,
"f.firstRender()},initSeries:function(a){var b=this.options.chart;(b=L[a.type||b.type||b.defaultSeriesType])||ka(17,!0);b=new b;b.init(this,a);return b},isInsidePlot:function(a,b,c){var d=c?b:a,a=c?a:b;return d>=0&&d<=this.plotWidth&&a>=0&&a<=this.plotHeight},adjustTickAmounts:function(){this.options.chart.alignTicks!==!1&&p(this.axes,function(a){a.adjustTickAmount()});this.maxTicks=null},redraw:function(a){var b=this.axes,c=this.series,d=this.pointer,e=this.legend,f=this.isDirtyLegend,g,h,i=this.isDirtyBox,\n"
"j=c.length,k=j,l=this.renderer,m=l.isHidden(),q=[];Oa(a,this);m&&this.cloneRenderTo();for(this.layOutTitles();k--;)if(a=c[k],a.options.stacking&&(g=!0,a.isDirty)){h=!0;break}if(h)for(k=j;k--;)if(a=c[k],a.options.stacking)a.isDirty=!0;p(c,function(a){a.isDirty&&a.options.legendType===\"point\"&&(f=!0)});if(f&&e.options.enabled)e.render(),this.isDirtyLegend=!1;g&&this.getStacks();if(this.hasCartesianSeries){if(!this.isResizing)this.maxTicks=null,p(b,function(a){a.setScale()});this.adjustTickAmounts();\n"
"this.getMargins();p(b,function(a){a.isDirty&&(i=!0)});p(b,function(a){if(a.isDirtyExtremes)a.isDirtyExtremes=!1,q.push(function(){A(a,\"afterSetExtremes\",r(a.eventArgs,a.getExtremes()));delete a.eventArgs});(i||g)&&a.redraw()})}i&&this.drawChartBox();p(c,function(a){a.isDirty&&a.visible&&(!a.isCartesian||a.xAxis)&&a.redraw()});d&&d.reset&&d.reset(!0);l.draw();A(this,\"redraw\");m&&this.cloneRenderTo(!0);p(q,function(a){a.call()})},get:function(a){var b=this.axes,c=this.series,d,e;for(d=0;d<b.length;d++)if(b[d].options.id===\n"
"a)return b[d];for(d=0;d<c.length;d++)if(c[d].options.id===a)return c[d];for(d=0;d<c.length;d++){e=c[d].points||[];for(b=0;b<e.length;b++)if(e[b].id===a)return e[b]}return null},getAxes:function(){var a=this,b=this.options,c=b.xAxis=ja(b.xAxis||{}),b=b.yAxis=ja(b.yAxis||{});p(c,function(a,b){a.index=b;a.isX=!0});p(b,function(a,b){a.index=b});c=c.concat(b);p(c,function(b){new qa(a,b)});a.adjustTickAmounts()},getSelectedPoints:function(){var a=[];p(this.series,function(b){a=a.concat(wb(b.points||[],\n"
"function(a){return a.selected}))});return a},getSelectedSeries:function(){return wb(this.series,function(a){return a.selected})},getStacks:function(){var a=this;p(a.yAxis,function(a){if(a.stacks&&a.hasVisibleSeries)a.oldStacks=a.stacks});p(a.series,function(b){if(b.options.stacking&&(b.visible===!0||a.options.chart.ignoreHiddenSeries===!1))b.stackKey=b.type+n(b.options.stack,\"\")})},showResetZoom:function(){var a=this,b=G.lang,c=a.options.chart.resetZoomButton,d=c.theme,e=d.states,f=c.relativeTo===\n"
"\"chart\"?null:\"plotBox\";this.resetZoomButton=a.renderer.button(b.resetZoom,null,null,function(){a.zoomOut()},d,e&&e.hover).attr({align:c.position.align,title:b.resetZoomTitle}).add().align(c.position,!1,f)},zoomOut:function(){var a=this;A(a,\"selection\",{resetSelection:!0},function(){a.zoom()})},zoom:function(a){var b,c=this.pointer,d=!1,e;!a||a.resetSelection?p(this.axes,function(a){b=a.zoom()}):p(a.xAxis.concat(a.yAxis),function(a){var e=a.axis,h=e.isXAxis;if(c[h?\"zoomX\":\"zoomY\"]||c[h?\"pinchX\":\"pinchY\"])b=\n"
"e.zoom(a.min,a.max),e.displayBtn&&(d=!0)});e=this.resetZoomButton;if(d&&!e)this.showResetZoom();else if(!d&&S(e))this.resetZoomButton=e.destroy();b&&this.redraw(n(this.options.chart.animation,a&&a.animation,this.pointCount<100))},pan:function(a,b){var c=this,d=c.hoverPoints,e;d&&p(d,function(a){a.setState()});p(b===\"xy\"?[1,0]:[1],function(b){var d=a[b?\"chartX\":\"chartY\"],h=c[b?\"xAxis\":\"yAxis\"][0],i=c[b?\"mouseDownX\":\"mouseDownY\"],j=(h.pointRange||0)/2,k=h.getExtremes(),l=h.toValue(i-d,!0)+j,i=h.toValue(i+\n"
"c[b?\"plotWidth\":\"plotHeight\"]-d,!0)-j;h.series.length&&l>I(k.dataMin,k.min)&&i<s(k.dataMax,k.max)&&(h.setExtremes(l,i,!1,!1,{trigger:\"pan\"}),e=!0);c[b?\"mouseDownX\":\"mouseDownY\"]=d});e&&c.redraw(!1);D(c.container,{cursor:\"move\"})},setTitle:function(a,b){var f;var c=this,d=c.options,e;e=d.title=x(d.title,a);f=d.subtitle=x(d.subtitle,b),d=f;p([[\"title\",a,e],[\"subtitle\",b,d]],function(a){var b=a[0],d=c[b],e=a[1],a=a[2];d&&e&&(c[b]=d=d.destroy());a&&a.text&&!d&&(c[b]=c.renderer.text(a.text,0,0,a.useHTML).attr({align:a.align,\n"
"\"class\":\"highcharts-\"+b,zIndex:a.zIndex||4}).css(a.style).add())});c.layOutTitles()},layOutTitles:function(){var a=0,b=this.title,c=this.subtitle,d=this.options,e=d.title,d=d.subtitle,f=this.spacingBox.width-44;if(b&&(b.css({width:(e.width||f)+\"px\"}).align(r({y:15},e),!1,\"spacingBox\"),!e.floating&&!e.verticalAlign))a=b.getBBox().height,a>=18&&a<=25&&(a=15);c&&(c.css({width:(d.width||f)+\"px\"}).align(r({y:a+e.margin},d),!1,\"spacingBox\"),!d.floating&&!d.verticalAlign&&(a=Ha(a+c.getBBox().height)));this.titleOffset=\n"
"a},getChartSize:function(){var a=this.options.chart,b=this.renderToClone||this.renderTo;this.containerWidth=jb(b,\"width\");this.containerHeight=jb(b,\"height\");this.chartWidth=s(0,a.width||this.containerWidth||600);this.chartHeight=s(0,n(a.height,this.containerHeight>19?this.containerHeight:400))},cloneRenderTo:function(a){var b=this.renderToClone,c=this.container;a?b&&(this.renderTo.appendChild(c),Na(b),delete this.renderToClone):(c&&c.parentNode===this.renderTo&&this.renderTo.removeChild(c),this.renderToClone=\n"
"b=this.renderTo.cloneNode(0),D(b,{position:\"absolute\",top:\"-9999px\",display:\"block\"}),y.body.appendChild(b),c&&b.appendChild(c))},getContainer:function(){var a,b=this.options.chart,c,d,e;this.renderTo=a=b.renderTo;e=\"highcharts-\"+ub++;if(fa(a))this.renderTo=a=y.getElementById(a);a||ka(13,!0);c=z(v(a,\"data-highcharts-chart\"));!isNaN(c)&&Ia[c]&&Ia[c].destroy();v(a,\"data-highcharts-chart\",this.index);a.innerHTML=\"\";a.offsetWidth||this.cloneRenderTo();this.getChartSize();c=this.chartWidth;d=this.chartHeight;\n"
"this.container=a=T(Ga,{className:\"highcharts-container\"+(b.className?\" \"+b.className:\"\"),id:e},r({position:\"relative\",overflow:\"hidden\",width:c+\"px\",height:d+\"px\",textAlign:\"left\",lineHeight:\"normal\",zIndex:0,\"-webkit-tap-highlight-color\":\"rgba(0,0,0,0)\"},b.style),this.renderToClone||a);this._cursor=a.style.cursor;this.renderer=b.forExport?new ua(a,c,d,!0):new Ya(a,c,d);da&&this.renderer.create(this,a,c,d)},getMargins:function(){var a=this.spacing,b,c=this.legend,d=this.margin,e=this.options.legend,\n"
"f=n(e.margin,10),g=e.x,h=e.y,i=e.align,j=e.verticalAlign,k=this.titleOffset;this.resetMargins();b=this.axisOffset;if(k&&!t(d[0]))this.plotTop=s(this.plotTop,k+this.options.title.margin+a[0]);if(c.display&&!e.floating)if(i===\"right\"){if(!t(d[1]))this.marginRight=s(this.marginRight,c.legendWidth-g+f+a[1])}else if(i===\"left\"){if(!t(d[3]))this.plotLeft=s(this.plotLeft,c.legendWidth+g+f+a[3])}else if(j===\"top\"){if(!t(d[0]))this.plotTop=s(this.plotTop,c.legendHeight+h+f+a[0])}else if(j===\"bottom\"&&!t(d[2]))this.marginBottom=\n"
"s(this.marginBottom,c.legendHeight-h+f+a[2]);this.extraBottomMargin&&(this.marginBottom+=this.extraBottomMargin);this.extraTopMargin&&(this.plotTop+=this.extraTopMargin);this.hasCartesianSeries&&p(this.axes,function(a){a.getOffset()});t(d[3])||(this.plotLeft+=b[3]);t(d[0])||(this.plotTop+=b[0]);t(d[2])||(this.marginBottom+=b[2]);t(d[1])||(this.marginRight+=b[1]);this.setChartSize()},reflow:function(a){var b=this,c=b.options.chart,d=b.renderTo,e=c.width||jb(d,\"width\"),f=c.height||jb(d,\"height\"),c=\n"
"a?a.target:C,d=function(){if(b.container)b.setSize(e,f,!1),b.hasUserSize=null};if(!b.hasUserSize&&e&&f&&(c===C||c===y)){if(e!==b.containerWidth||f!==b.containerHeight)clearTimeout(b.reflowTimeout),a?b.reflowTimeout=setTimeout(d,100):d();b.containerWidth=e;b.containerHeight=f}},initReflow:function(){var a=this,b=function(b){a.reflow(b)};F(C,\"resize\",b);F(a,\"destroy\",function(){X(C,\"resize\",b)})},setSize:function(a,b,c){var d=this,e,f,g;d.isResizing+=1;g=function(){d&&A(d,\"endResize\",null,function(){d.isResizing-=\n"
"1})};Oa(c,d);d.oldChartHeight=d.chartHeight;d.oldChartWidth=d.chartWidth;if(t(a))d.chartWidth=e=s(0,w(a)),d.hasUserSize=!!e;if(t(b))d.chartHeight=f=s(0,w(b));(oa?kb:D)(d.container,{width:e+\"px\",height:f+\"px\"},oa);d.setChartSize(!0);d.renderer.setSize(e,f,c);d.maxTicks=null;p(d.axes,function(a){a.isDirty=!0;a.setScale()});p(d.series,function(a){a.isDirty=!0});d.isDirtyLegend=!0;d.isDirtyBox=!0;d.getMargins();d.redraw(c);d.oldChartHeight=null;A(d,\"resize\");oa===!1?g():setTimeout(g,oa&&oa.duration||\n"
"500)},setChartSize:function(a){var b=this.inverted,c=this.renderer,d=this.chartWidth,e=this.chartHeight,f=this.options.chart,g=this.spacing,h=this.clipOffset,i,j,k,l;this.plotLeft=i=w(this.plotLeft);this.plotTop=j=w(this.plotTop);this.plotWidth=k=s(0,w(d-i-this.marginRight));this.plotHeight=l=s(0,w(e-j-this.marginBottom));this.plotSizeX=b?l:k;this.plotSizeY=b?k:l;this.plotBorderWidth=f.plotBorderWidth||0;this.spacingBox=c.spacingBox={x:g[3],y:g[0],width:d-g[3]-g[1],height:e-g[0]-g[2]};this.plotBox=\n"
"c.plotBox={x:i,y:j,width:k,height:l};d=2*N(this.plotBorderWidth/2);b=Ha(s(d,h[3])/2);c=Ha(s(d,h[0])/2);this.clipBox={x:b,y:c,width:N(this.plotSizeX-s(d,h[1])/2-b),height:N(this.plotSizeY-s(d,h[2])/2-c)};a||p(this.axes,function(a){a.setAxisSize();a.setAxisTranslation()})},resetMargins:function(){var a=this.spacing,b=this.margin;this.plotTop=n(b[0],a[0]);this.marginRight=n(b[1],a[1]);this.marginBottom=n(b[2],a[2]);this.plotLeft=n(b[3],a[3]);this.axisOffset=[0,0,0,0];this.clipOffset=[0,0,0,0]},drawChartBox:function(){var a=\n"
"this.options.chart,b=this.renderer,c=this.chartWidth,d=this.chartHeight,e=this.chartBackground,f=this.plotBackground,g=this.plotBorder,h=this.plotBGImage,i=a.borderWidth||0,j=a.backgroundColor,k=a.plotBackgroundColor,l=a.plotBackgroundImage,m=a.plotBorderWidth||0,q,o=this.plotLeft,n=this.plotTop,p=this.plotWidth,s=this.plotHeight,r=this.plotBox,t=this.clipRect,w=this.clipBox;q=i+(a.shadow?8:0);if(i||j)if(e)e.animate(e.crisp(null,null,null,c-q,d-q));else{e={fill:j||Q};if(i)e.stroke=a.borderColor,e[\"stroke-width\"]=\n"
,
"i;this.chartBackground=b.rect(q/2,q/2,c-q,d-q,a.borderRadius,i).attr(e).add().shadow(a.shadow)}if(k)f?f.animate(r):this.plotBackground=b.rect(o,n,p,s,0).attr({fill:k}).add().shadow(a.plotShadow);if(l)h?h.animate(r):this.plotBGImage=b.image(l,o,n,p,s).add();t?t.animate({width:w.width,height:w.height}):this.clipRect=b.clipRect(w);if(m)g?g.animate(g.crisp(null,o,n,p,s)):this.plotBorder=b.rect(o,n,p,s,0,-m).attr({stroke:a.plotBorderColor,\"stroke-width\":m,zIndex:1}).add();this.isDirtyBox=!1},propFromSeries:function(){var a=\n"
"this,b=a.options.chart,c,d=a.options.series,e,f;p([\"inverted\",\"angular\",\"polar\"],function(g){c=L[b.type||b.defaultSeriesType];f=a[g]||b[g]||c&&c.prototype[g];for(e=d&&d.length;!f&&e--;)(c=L[d[e].type])&&c.prototype[g]&&(f=!0);a[g]=f})},linkSeries:function(){var a=this,b=a.series;p(b,function(a){a.linkedSeries.length=0});p(b,function(b){var d=b.options.linkedTo;if(fa(d)&&(d=d===\":previous\"?a.series[b.index-1]:a.get(d)))d.linkedSeries.push(b),b.linkedParent=d})},render:function(){var a=this,b=a.axes,\n"
"c=a.renderer,d=a.options,e=d.labels,f=d.credits,g;a.setTitle();a.legend=new zb(a,d.legend);a.getStacks();p(b,function(a){a.setScale()});a.getMargins();a.maxTicks=null;p(b,function(a){a.setTickPositions(!0);a.setMaxTicks()});a.adjustTickAmounts();a.getMargins();a.drawChartBox();a.hasCartesianSeries&&p(b,function(a){a.render()});if(!a.seriesGroup)a.seriesGroup=c.g(\"series-group\").attr({zIndex:3}).add();p(a.series,function(a){a.translate();a.setTooltipPoints();a.render()});e.items&&p(e.items,function(b){var d=\n"
"r(e.style,b.style),f=z(d.left)+a.plotLeft,g=z(d.top)+a.plotTop+12;delete d.left;delete d.top;c.text(b.html,f,g).attr({zIndex:2}).css(d).add()});if(f.enabled&&!a.credits)g=f.href,a.credits=c.text(f.text,0,0).on(\"click\",function(){if(g)location.href=g}).attr({align:f.position.align,zIndex:8}).css(f.style).add().align(f.position);a.hasRendered=!0},destroy:function(){var a=this,b=a.axes,c=a.series,d=a.container,e,f=d&&d.parentNode;A(a,\"destroy\");Ia[a.index]=u;a.renderTo.removeAttribute(\"data-highcharts-chart\");\n"
"X(a);for(e=b.length;e--;)b[e]=b[e].destroy();for(e=c.length;e--;)c[e]=c[e].destroy();p(\"title,subtitle,chartBackground,plotBackground,plotBGImage,plotBorder,seriesGroup,clipRect,credits,pointer,scroller,rangeSelector,legend,resetZoomButton,tooltip,renderer\".split(\",\"),function(b){var c=a[b];c&&c.destroy&&(a[b]=c.destroy())});if(d)d.innerHTML=\"\",X(d),f&&Na(d);for(e in a)delete a[e]},isReadyToRender:function(){var a=this;return!V&&C==C.top&&y.readyState!==\"complete\"||da&&!C.canvg?(da?Mb.push(function(){a.firstRender()},\n"
"a.options.global.canvasToolsURL):y.attachEvent(\"onreadystatechange\",function(){y.detachEvent(\"onreadystatechange\",a.firstRender);y.readyState===\"complete\"&&a.firstRender()}),!1):!0},firstRender:function(){var a=this,b=a.options,c=a.callback;if(a.isReadyToRender())a.getContainer(),A(a,\"init\"),a.resetMargins(),a.setChartSize(),a.propFromSeries(),a.getAxes(),p(b.series||[],function(b){a.initSeries(b)}),a.linkSeries(),A(a,\"beforeRender\"),a.pointer=new $a(a,b),a.render(),a.renderer.draw(),c&&c.apply(a,\n"
"[a]),p(a.callbacks,function(b){b.apply(a,[a])}),a.cloneRenderTo(!0),A(a,\"load\")},splashArray:function(a,b){var c=b[a],c=S(c)?c:[c,c,c,c];return[n(b[a+\"Top\"],c[0]),n(b[a+\"Right\"],c[1]),n(b[a+\"Bottom\"],c[2]),n(b[a+\"Left\"],c[3])]}};fb.prototype.callbacks=[];var xb=Highcharts.CenteredSeriesMixin={getCenter:function(){var a=this.options,b=this.chart,c=2*(a.slicedOffset||0),d,e=b.plotWidth-2*c,f=b.plotHeight-2*c,b=a.center,a=[n(b[0],\"50%\"),n(b[1],\"50%\"),a.size||\"100%\",a.innerSize||0],g=I(e,f),h;return Ra(a,\n"
"function(a,b){h=/%$/.test(a);d=b<2||b===2&&h;return(h?[e,f,g,g][b]*z(a)/100:a)+(d?c:0)})}},Ja=function(){};Ja.prototype={init:function(a,b,c){this.series=a;this.applyOptions(b,c);this.pointAttr={};if(a.options.colorByPoint&&(b=a.options.colors||a.chart.options.colors,this.color=this.color||b[a.colorCounter++],a.colorCounter===b.length))a.colorCounter=0;a.chart.pointCount++;return this},applyOptions:function(a,b){var c=this.series,d=c.pointValKey,a=Ja.prototype.optionsToObject.call(this,a);r(this,\n"
"a);this.options=this.options?r(this.options,a):a;if(d)this.y=this[d];if(this.x===u&&c)this.x=b===u?c.autoIncrement():b;return this},optionsToObject:function(a){var b={},c=this.series,d=c.pointArrayMap||[\"y\"],e=d.length,f=0,g=0;if(typeof a===\"number\"||a===null)b[d[0]]=a;else if(Ka(a)){if(a.length>e){c=typeof a[0];if(c===\"string\")b.name=a[0];else if(c===\"number\")b.x=a[0];f++}for(;g<e;)b[d[g++]]=a[f++]}else if(typeof a===\"object\"){b=a;if(a.dataLabels)c._hasPointLabels=!0;if(a.marker)c._hasPointMarkers=\n"
"!0}return b},destroy:function(){var a=this.series.chart,b=a.hoverPoints,c;a.pointCount--;if(b&&(this.setState(),ha(b,this),!b.length))a.hoverPoints=null;if(this===a.hoverPoint)this.onMouseOut();if(this.graphic||this.dataLabel)X(this),this.destroyElements();this.legendItem&&a.legend.destroyItem(this);for(c in this)this[c]=null},destroyElements:function(){for(var a=\"graphic,dataLabel,dataLabelUpper,group,connector,shadowGroup\".split(\",\"),b,c=6;c--;)b=a[c],this[b]&&(this[b]=this[b].destroy())},getLabelConfig:function(){return{x:this.category,\n"
"y:this.y,key:this.name||this.category,series:this.series,point:this,percentage:this.percentage,total:this.total||this.stackTotal}},select:function(a,b){var c=this,d=c.series,e=d.chart,a=n(a,!c.selected);c.firePointEvent(a?\"select\":\"unselect\",{accumulate:b},function(){c.selected=c.options.selected=a;d.options.data[sa(c,d.data)]=c.options;c.setState(a&&\"select\");b||p(e.getSelectedPoints(),function(a){if(a.selected&&a!==c)a.selected=a.options.selected=!1,d.options.data[sa(a,d.data)]=a.options,a.setState(\"\"),\n"
"a.firePointEvent(\"unselect\")})})},onMouseOver:function(a){var b=this.series,c=b.chart,d=c.tooltip,e=c.hoverPoint;if(e&&e!==this)e.onMouseOut();this.firePointEvent(\"mouseOver\");d&&(!d.shared||b.noSharedTooltip)&&d.refresh(this,a);this.setState(\"hover\");c.hoverPoint=this},onMouseOut:function(){var a=this.series.chart,b=a.hoverPoints;if(!b||sa(this,b)===-1)this.firePointEvent(\"mouseOut\"),this.setState(),a.hoverPoint=null},tooltipFormatter:function(a){var b=this.series,c=b.tooltipOptions,d=n(c.valueDecimals,\n"
"\"\"),e=c.valuePrefix||\"\",f=c.valueSuffix||\"\";p(b.pointArrayMap||[\"y\"],function(b){b=\"{point.\"+b;if(e||f)a=a.replace(b+\"}\",e+b+\"}\"+f);a=a.replace(b+\"}\",b+\":,.\"+d+\"f}\")});return Fa(a,{point:this,series:this.series})},firePointEvent:function(a,b,c){var d=this,e=this.series.options;(e.point.events[a]||d.options&&d.options.events&&d.options.events[a])&&this.importEvents();a===\"click\"&&e.allowPointSelect&&(c=function(a){d.select(null,a.ctrlKey||a.metaKey||a.shiftKey)});A(this,a,b,c)},importEvents:function(){if(!this.hasImportedEvents){var a=\n"
"x(this.series.options.point,this.options).events,b;this.events=a;for(b in a)F(this,b,a[b]);this.hasImportedEvents=!0}},setState:function(a,b){var c=this.plotX,d=this.plotY,e=this.series,f=e.options.states,g=Y[e.type].marker&&e.options.marker,h=g&&!g.enabled,i=g&&g.states[a],j=i&&i.enabled===!1,k=e.stateMarkerGraphic,l=this.marker||{},m=e.chart,q=this.pointAttr,a=a||\"\",b=b&&k;if(!(a===this.state&&!b||this.selected&&a!==\"select\"||f[a]&&f[a].enabled===!1||a&&(j||h&&!i.enabled)||a&&l.states&&l.states[a]&&\n"
"l.states[a].enabled===!1)){if(this.graphic)f=g&&this.graphic.symbolName&&q[a].r,this.graphic.attr(x(q[a],f?{x:c-f,y:d-f,width:2*f,height:2*f}:{}));else{if(a&&i)if(f=i.radius,l=l.symbol||e.symbol,k&&k.currentSymbol!==l&&(k=k.destroy()),k)k[b?\"animate\":\"attr\"]({x:c-f,y:d-f});else e.stateMarkerGraphic=k=m.renderer.symbol(l,c-f,d-f,2*f,2*f).attr(q[a]).add(e.markerGroup),k.currentSymbol=l;if(k)k[a&&m.isInsidePlot(c,d,m.inverted)?\"show\":\"hide\"]()}this.state=a}}};var O=function(){};O.prototype={isCartesian:!0,\n"
"type:\"line\",pointClass:Ja,sorted:!0,requireSorting:!0,pointAttrToOptions:{stroke:\"lineColor\",\"stroke-width\":\"lineWidth\",fill:\"fillColor\",r:\"radius\"},axisTypes:[\"xAxis\",\"yAxis\"],colorCounter:0,parallelArrays:[\"x\",\"y\"],init:function(a,b){var c=this,d,e,f=a.series,g=function(a,b){return n(a.options.index,a._i)-n(b.options.index,b._i)};c.chart=a;c.options=b=c.setOptions(b);c.linkedSeries=[];c.bindAxes();r(c,{name:b.name,state:\"\",pointAttr:{},visible:b.visible!==!1,selected:b.selected===!0});if(da)b.animation=\n"
"!1;e=b.events;for(d in e)F(c,d,e[d]);if(e&&e.click||b.point&&b.point.events&&b.point.events.click||b.allowPointSelect)a.runTrackerClick=!0;c.getColor();c.getSymbol();p(c.parallelArrays,function(a){c[a+\"Data\"]=[]});c.setData(b.data,!1);if(c.isCartesian)a.hasCartesianSeries=!0;f.push(c);c._i=f.length-1;ob(f,g);this.yAxis&&ob(this.yAxis.series,g);p(f,function(a,b){a.index=b;a.name=a.name||\"Series \"+(b+1)})},bindAxes:function(){var a=this,b=a.options,c=a.chart,d;p(a.axisTypes||[],function(e){p(c[e],function(c){d=\n"
"c.options;if(b[e]===d.index||b[e]!==u&&b[e]===d.id||b[e]===u&&d.index===0)c.series.push(a),a[e]=c,c.isDirty=!0});!a[e]&&a.optionalAxis!==e&&ka(18,!0)})},updateParallelArrays:function(a,b){var c=a.series,d=arguments;p(c.parallelArrays,typeof b===\"number\"?function(d){var f=d===\"y\"&&c.toYData?c.toYData(a):a[d];c[d+\"Data\"][b]=f}:function(a){Array.prototype[b].apply(c[a+\"Data\"],Array.prototype.slice.call(d,2))})},autoIncrement:function(){var a=this.options,b=this.xIncrement,b=n(b,a.pointStart,0);this.pointInterval=\n"
"n(this.pointInterval,a.pointInterval,1);this.xIncrement=b+this.pointInterval;return b},getSegments:function(){var a=-1,b=[],c,d=this.points,e=d.length;if(e)if(this.options.connectNulls){for(c=e;c--;)d[c].y===null&&d.splice(c,1);d.length&&(b=[d])}else p(d,function(c,g){c.y===null?(g>a+1&&b.push(d.slice(a+1,g)),a=g):g===e-1&&b.push(d.slice(a+1,g+1))});this.segments=b},setOptions:function(a){var b=this.chart,c=b.options.plotOptions,b=b.userOptions||{},d=b.plotOptions||{},e=c[this.type];this.userOptions=\n"
,
"a;c=x(e,c.series,a);this.tooltipOptions=x(G.tooltip,G.plotOptions[this.type].tooltip,b.tooltip,d.series&&d.series.tooltip,d[this.type]&&d[this.type].tooltip,a.tooltip);e.marker===null&&delete c.marker;return c},getColor:function(){var a=this.options,b=this.userOptions,c=this.chart.options.colors,d=this.chart.counters,e;e=a.color||Y[this.type].color;if(!e&&!a.colorByPoint)t(b._colorIndex)?a=b._colorIndex:(b._colorIndex=d.color,a=d.color++),e=c[a];this.color=e;d.wrapColor(c.length)},getSymbol:function(){var a=\n"
"this.userOptions,b=this.options.marker,c=this.chart,d=c.options.symbols,c=c.counters;this.symbol=b.symbol;if(!this.symbol)t(a._symbolIndex)?a=a._symbolIndex:(a._symbolIndex=c.symbol,a=c.symbol++),this.symbol=d[a];if(/^url/.test(this.symbol))b.radius=0;c.wrapSymbol(d.length)},drawLegendSymbol:R.drawLineMarker,setData:function(a,b){var c=this,d=c.points,e=c.options,f=c.chart,g=null,h=c.xAxis,i=h&&!!h.categories,j;c.xIncrement=null;c.pointRange=i?1:e.pointRange;c.colorCounter=0;var a=a||[],k=a.length;\n"
"j=e.turboThreshold;var l=this.xData,m=this.yData,q=c.pointArrayMap,q=q&&q.length;p(this.parallelArrays,function(a){c[a+\"Data\"].length=0});if(j&&k>j){for(j=0;g===null&&j<k;)g=a[j],j++;if(wa(g)){i=n(e.pointStart,0);e=n(e.pointInterval,1);for(j=0;j<k;j++)l[j]=i,m[j]=a[j],i+=e;c.xIncrement=i}else if(Ka(g))if(q)for(j=0;j<k;j++)e=a[j],l[j]=e[0],m[j]=e.slice(1,q+1);else for(j=0;j<k;j++)e=a[j],l[j]=e[0],m[j]=e[1];else ka(12)}else for(j=0;j<k;j++)if(a[j]!==u&&(e={series:c},c.pointClass.prototype.applyOptions.apply(e,\n"
"[a[j]]),c.updateParallelArrays(e,j),i&&e.name))h.names[e.x]=e.name;fa(m[0])&&ka(14,!0);c.data=[];c.options.data=a;for(j=d&&d.length||0;j--;)d[j]&&d[j].destroy&&d[j].destroy();if(h)h.minRange=h.userMinRange;c.isDirty=c.isDirtyData=f.isDirtyBox=!0;n(b,!0)&&f.redraw(!1)},processData:function(a){var b=this.xData,c=this.yData,d=b.length,e;e=0;var f,g,h=this.xAxis,i=this.options,j=i.cropThreshold,k=this.isCartesian;if(k&&!this.isDirty&&!h.isDirty&&!this.yAxis.isDirty&&!a)return!1;if(k&&this.sorted&&(!j||\n"
"d>j||this.forceCrop))if(a=h.min,h=h.max,b[d-1]<a||b[0]>h)b=[],c=[];else if(b[0]<a||b[d-1]>h)e=this.cropData(this.xData,this.yData,a,h),b=e.xData,c=e.yData,e=e.start,f=!0;for(h=b.length-1;h>=0;h--)d=b[h]-b[h-1],d>0&&(g===u||d<g)?g=d:d<0&&this.requireSorting&&ka(15);this.cropped=f;this.cropStart=e;this.processedXData=b;this.processedYData=c;if(i.pointRange===null)this.pointRange=g||1;this.closestPointRange=g},cropData:function(a,b,c,d){var e=a.length,f=0,g=e,h=n(this.cropShoulder,1),i;for(i=0;i<e;i++)if(a[i]>=\n"
"c){f=s(0,i-h);break}for(;i<e;i++)if(a[i]>d){g=i+h;break}return{xData:a.slice(f,g),yData:b.slice(f,g),start:f,end:g}},generatePoints:function(){var a=this.options.data,b=this.data,c,d=this.processedXData,e=this.processedYData,f=this.pointClass,g=d.length,h=this.cropStart||0,i,j=this.hasGroupedData,k,l=[],m;if(!b&&!j)b=[],b.length=a.length,b=this.data=b;for(m=0;m<g;m++)i=h+m,j?l[m]=(new f).init(this,[d[m]].concat(ja(e[m]))):(b[i]?k=b[i]:a[i]!==u&&(b[i]=k=(new f).init(this,a[i],d[m])),l[m]=k);if(b&&\n"
"(g!==(c=b.length)||j))for(m=0;m<c;m++)if(m===h&&!j&&(m+=g),b[m])b[m].destroyElements(),b[m].plotX=u;this.data=b;this.points=l},setStackedPoints:function(){if(this.options.stacking&&!(this.visible!==!0&&this.chart.options.chart.ignoreHiddenSeries!==!1)){var a=this.processedXData,b=this.processedYData,c=[],d=b.length,e=this.options,f=e.threshold,g=e.stack,e=e.stacking,h=this.stackKey,i=\"-\"+h,j=this.negStacks,k=this.yAxis,l=k.stacks,m=k.oldStacks,q,o,n,p,r;for(n=0;n<d;n++){p=a[n];r=b[n];o=(q=j&&r<f)?\n"
"i:h;l[o]||(l[o]={});if(!l[o][p])m[o]&&m[o][p]?(l[o][p]=m[o][p],l[o][p].total=null):l[o][p]=new Gb(k,k.options.stackLabels,q,p,g,e);o=l[o][p];o.points[this.index]=[o.cum||0];e===\"percent\"?(q=q?h:i,j&&l[q]&&l[q][p]?(q=l[q][p],o.total=q.total=s(q.total,o.total)+M(r)||0):o.total=aa(o.total+(M(r)||0))):o.total=aa(o.total+(r||0));o.cum=(o.cum||0)+(r||0);o.points[this.index].push(o.cum);c[n]=o.cum}if(e===\"percent\")k.usePercentage=!0;this.stackedYData=c;k.oldStacks={}}},setPercentStacks:function(){var a=\n"
"this,b=a.stackKey,c=a.yAxis.stacks;p([b,\"-\"+b],function(b){var d;for(var e=a.xData.length,f,g;e--;)if(f=a.xData[e],d=(g=c[b]&&c[b][f])&&g.points[a.index],f=d)g=g.total?100/g.total:0,f[0]=aa(f[0]*g),f[1]=aa(f[1]*g),a.stackedYData[e]=f[1]})},getExtremes:function(a){var b=this.yAxis,c=this.processedXData,d,e=[],f=0;d=this.xAxis.getExtremes();var g=d.min,h=d.max,i,j,k,l,a=a||this.stackedYData||this.processedYData;d=a.length;for(l=0;l<d;l++)if(j=c[l],k=a[l],i=k!==null&&k!==u&&(!b.isLog||k.length||k>0),\n"
"j=this.getExtremesFromAll||this.cropped||(c[l+1]||j)>=g&&(c[l-1]||j)<=h,i&&j)if(i=k.length)for(;i--;)k[i]!==null&&(e[f++]=k[i]);else e[f++]=k;this.dataMin=n(void 0,La(e));this.dataMax=n(void 0,za(e))},translate:function(){this.processedXData||this.processData();this.generatePoints();for(var a=this.options,b=a.stacking,c=this.xAxis,d=c.categories,e=this.yAxis,f=this.points,g=f.length,h=!!this.modifyValue,i=a.pointPlacement,j=i===\"between\"||wa(i),k=a.threshold,a=0;a<g;a++){var l=f[a],m=l.x,q=l.y,o=\n"
"l.low,p=b&&e.stacks[(this.negStacks&&q<k?\"-\":\"\")+this.stackKey];if(e.isLog&&q<=0)l.y=q=null;l.plotX=c.translate(m,0,0,0,1,i,this.type===\"flags\");if(b&&this.visible&&p&&p[m])p=p[m],q=p.points[this.index],o=q[0],q=q[1],o===0&&(o=n(k,e.min)),e.isLog&&o<=0&&(o=null),l.total=l.stackTotal=p.total,l.percentage=b===\"percent\"&&l.y/p.total*100,l.stackY=q,p.setOffset(this.pointXOffset||0,this.barW||0);l.yBottom=t(o)?e.translate(o,0,1,0,1):null;h&&(q=this.modifyValue(q,l));l.plotY=typeof q===\"number\"&&q!==Infinity?\n"
"e.translate(q,0,1,0,1):u;l.clientX=j?c.translate(m,0,0,0,1):l.plotX;l.negative=l.y<(k||0);l.category=d&&d[l.x]!==u?d[l.x]:l.x}this.getSegments()},setTooltipPoints:function(a){var b=[],c,d,e=this.xAxis,f=e&&e.getExtremes(),g=e?e.tooltipLen||e.len:this.chart.plotSizeX,h,i,j=[];if(this.options.enableMouseTracking!==!1){if(a)this.tooltipPoints=null;p(this.segments||this.points,function(a){b=b.concat(a)});e&&e.reversed&&(b=b.reverse());this.orderTooltipPoints&&this.orderTooltipPoints(b);a=b.length;for(i=\n"
"0;i<a;i++)if(e=b[i],c=e.x,c>=f.min&&c<=f.max){h=b[i+1];c=d===u?0:d+1;for(d=b[i+1]?I(s(0,N((e.clientX+(h?h.wrappedClientX||h.clientX:g))/2)),g):g;c>=0&&c<=d;)j[c++]=e}this.tooltipPoints=j}},tooltipHeaderFormatter:function(a){var b=this.tooltipOptions,c=b.dateTimeLabelFormats,d=b.xDateFormat,e=this.xAxis,f=e&&e.options.type===\"datetime\",b=b.headerFormat,e=e&&e.closestPointRange,g;if(f&&!d){if(e)for(g in E){if(E[g]>=e){d=c[g];break}}else d=c.day;d=d||c.year}f&&d&&wa(a.key)&&(b=b.replace(\"{point.key}\",\n"
"\"{point.key:\"+d+\"}\"));return Fa(b,{point:a,series:this})},onMouseOver:function(){var a=this.chart,b=a.hoverSeries;if(b&&b!==this)b.onMouseOut();this.options.events.mouseOver&&A(this,\"mouseOver\");this.setState(\"hover\");a.hoverSeries=this},onMouseOut:function(){var a=this.options,b=this.chart,c=b.tooltip,d=b.hoverPoint;if(d)d.onMouseOut();this&&a.events.mouseOut&&A(this,\"mouseOut\");c&&!a.stickyTracking&&(!c.shared||this.noSharedTooltip)&&c.hide();this.setState();b.hoverSeries=null},animate:function(a){var b=\n"
"this,c=b.chart,d=c.renderer,e;e=b.options.animation;var f=c.clipBox,g=c.inverted,h;if(e&&!S(e))e=Y[b.type].animation;h=\"_sharedClip\"+e.duration+e.easing;if(a)a=c[h],e=c[h+\"m\"],a||(c[h]=a=d.clipRect(r(f,{width:0})),c[h+\"m\"]=e=d.clipRect(-99,g?-c.plotLeft:-c.plotTop,99,g?c.chartWidth:c.chartHeight)),b.group.clip(a),b.markerGroup.clip(e),b.sharedClipKey=h;else{if(a=c[h])a.animate({width:c.plotSizeX},e),c[h+\"m\"].animate({width:c.plotSizeX+99},e);b.animate=null;b.animationTimeout=setTimeout(function(){b.afterAnimate()},\n"
"e.duration)}},afterAnimate:function(){var a=this.chart,b=this.sharedClipKey,c=this.group;c&&this.options.clip!==!1&&(c.clip(a.clipRect),this.markerGroup.clip());setTimeout(function(){b&&a[b]&&(a[b]=a[b].destroy(),a[b+\"m\"]=a[b+\"m\"].destroy())},100)},drawPoints:function(){var a,b=this.points,c=this.chart,d,e,f,g,h,i,j,k,l=this.options.marker,m,q=this.markerGroup;if(l.enabled||this._hasPointMarkers)for(f=b.length;f--;)if(g=b[f],d=N(g.plotX),e=g.plotY,k=g.graphic,i=g.marker||{},a=l.enabled&&i.enabled===\n"
"u||i.enabled,m=c.isInsidePlot(w(d),e,c.inverted),a&&e!==u&&!isNaN(e)&&g.y!==null)if(a=g.pointAttr[g.selected?\"select\":\"\"],h=a.r,i=n(i.symbol,this.symbol),j=i.indexOf(\"url\")===0,k)k.attr({visibility:m?V?\"inherit\":\"visible\":\"hidden\"}).animate(r({x:d-h,y:e-h},k.symbolName?{width:2*h,height:2*h}:{}));else{if(m&&(h>0||j))g.graphic=c.renderer.symbol(i,d-h,e-h,2*h,2*h).attr(a).add(q)}else if(k)g.graphic=k.destroy()},convertAttribs:function(a,b,c,d){var e=this.pointAttrToOptions,f,g,h={},a=a||{},b=b||{},\n"
"c=c||{},d=d||{};for(f in e)g=e[f],h[f]=n(a[g],b[f],c[f],d[f]);return h},getAttribs:function(){var a=this,b=a.options,c=Y[a.type].marker?b.marker:b,d=c.states,e=d.hover,f,g=a.color,h={stroke:g,fill:g},i=a.points||[],j=[],k,l=a.pointAttrToOptions,m=b.negativeColor,n=c.lineColor,o=c.fillColor,s;b.marker?(e.radius=e.radius||c.radius+2,e.lineWidth=e.lineWidth||c.lineWidth+1):e.color=e.color||ta(e.color||g).brighten(e.brightness).get();j[\"\"]=a.convertAttribs(c,h);p([\"hover\",\"select\"],function(b){j[b]=a.convertAttribs(d[b],\n"
"j[\"\"])});a.pointAttr=j;for(g=i.length;g--;){h=i[g];if((c=h.options&&h.options.marker||h.options)&&c.enabled===!1)c.radius=0;if(h.negative&&m)h.color=h.fillColor=m;k=b.colorByPoint||h.color;if(h.options)for(s in l)t(c[l[s]])&&(k=!0);if(k){c=c||{};k=[];d=c.states||{};f=d.hover=d.hover||{};if(!b.marker)f.color=ta(f.color||h.color).brighten(f.brightness||e.brightness).get();f={color:h.color};if(!o)f.fillColor=h.color;if(!n)f.lineColor=h.color;k[\"\"]=a.convertAttribs(r(f,c),j[\"\"]);k.hover=a.convertAttribs(d.hover,\n"
,
"j.hover,k[\"\"]);k.select=a.convertAttribs(d.select,j.select,k[\"\"])}else k=j;h.pointAttr=k}},destroy:function(){var a=this,b=a.chart,c=/AppleWebKit\\/533/.test(ra),d,e,f=a.data||[],g,h,i;A(a,\"destroy\");X(a);p(a.axisTypes||[],function(b){if(i=a[b])ha(i.series,a),i.isDirty=i.forceRedraw=!0});a.legendItem&&a.chart.legend.destroyItem(a);for(e=f.length;e--;)(g=f[e])&&g.destroy&&g.destroy();a.points=null;clearTimeout(a.animationTimeout);p(\"area,graph,dataLabelsGroup,group,markerGroup,tracker,graphNeg,areaNeg,posClip,negClip\".split(\",\"),\n"
"function(b){a[b]&&(d=c&&b===\"group\"?\"hide\":\"destroy\",a[b][d]())});if(b.hoverSeries===a)b.hoverSeries=null;ha(b.series,a);for(h in a)delete a[h]},getSegmentPath:function(a){var b=this,c=[],d=b.options.step;p(a,function(e,f){var g=e.plotX,h=e.plotY,i;b.getPointSpline?c.push.apply(c,b.getPointSpline(a,e,f)):(c.push(f?\"L\":\"M\"),d&&f&&(i=a[f-1],d===\"right\"?c.push(i.plotX,h):d===\"center\"?c.push((i.plotX+g)/2,i.plotY,(i.plotX+g)/2,h):c.push(g,i.plotY)),c.push(e.plotX,e.plotY))});return c},getGraphPath:function(){var a=\n"
"this,b=[],c,d=[];p(a.segments,function(e){c=a.getSegmentPath(e);e.length>1?b=b.concat(c):d.push(e[0])});a.singlePoints=d;return a.graphPath=b},drawGraph:function(){var a=this,b=this.options,c=[[\"graph\",b.lineColor||this.color]],d=b.lineWidth,e=b.dashStyle,f=b.linecap!==\"square\",g=this.getGraphPath(),h=b.negativeColor;h&&c.push([\"graphNeg\",h]);p(c,function(c,h){var k=c[0],l=a[k];if(l)Za(l),l.animate({d:g});else if(d&&g.length)l={stroke:c[1],\"stroke-width\":d,zIndex:1},e?l.dashstyle=e:f&&(l[\"stroke-linecap\"]=\n"
"l[\"stroke-linejoin\"]=\"round\"),a[k]=a.chart.renderer.path(g).attr(l).add(a.group).shadow(!h&&b.shadow)})},clipNeg:function(){var a=this.options,b=this.chart,c=b.renderer,d=a.negativeColor||a.negativeFillColor,e,f=this.graph,g=this.area,h=this.posClip,i=this.negClip;e=b.chartWidth;var j=b.chartHeight,k=s(e,j),l=this.yAxis;if(d&&(f||g)){d=w(l.toPixels(a.threshold||0,!0));d<0&&(k-=d);a={x:0,y:0,width:k,height:d};k={x:0,y:d,width:k,height:k};if(b.inverted)a.height=k.y=b.plotWidth-d,c.isVML&&(a={x:b.plotWidth-\n"
"d-b.plotLeft,y:0,width:e,height:j},k={x:d+b.plotLeft-e,y:0,width:b.plotLeft+d,height:e});l.reversed?(b=k,e=a):(b=a,e=k);h?(h.animate(b),i.animate(e)):(this.posClip=h=c.clipRect(b),this.negClip=i=c.clipRect(e),f&&this.graphNeg&&(f.clip(h),this.graphNeg.clip(i)),g&&(g.clip(h),this.areaNeg.clip(i)))}},invertGroups:function(){function a(){var a={width:b.yAxis.len,height:b.xAxis.len};p([\"group\",\"markerGroup\"],function(c){b[c]&&b[c].attr(a).invert()})}var b=this,c=b.chart;if(b.xAxis)F(c,\"resize\",a),F(b,\n"
"\"destroy\",function(){X(c,\"resize\",a)}),a(),b.invertGroups=a},plotGroup:function(a,b,c,d,e){var f=this[a],g=!f;g&&(this[a]=f=this.chart.renderer.g(b).attr({visibility:c,zIndex:d||0.1}).add(e));f[g?\"attr\":\"animate\"](this.getPlotBox());return f},getPlotBox:function(){return{translateX:this.xAxis?this.xAxis.left:this.chart.plotLeft,translateY:this.yAxis?this.yAxis.top:this.chart.plotTop,scaleX:1,scaleY:1}},render:function(){var a=this.chart,b,c=this.options,d=c.animation&&!!this.animate&&a.renderer.isSVG,\n"
"e=this.visible?\"visible\":\"hidden\",f=c.zIndex,g=this.hasRendered,h=a.seriesGroup;b=this.plotGroup(\"group\",\"series\",e,f,h);this.markerGroup=this.plotGroup(\"markerGroup\",\"markers\",e,f,h);d&&this.animate(!0);this.getAttribs();b.inverted=this.isCartesian?a.inverted:!1;this.drawGraph&&(this.drawGraph(),this.clipNeg());this.drawDataLabels&&this.drawDataLabels();this.visible&&this.drawPoints();this.options.enableMouseTracking!==!1&&this.drawTracker();a.inverted&&this.invertGroups();c.clip!==!1&&!this.sharedClipKey&&\n"
"!g&&b.clip(a.clipRect);d?this.animate():g||this.afterAnimate();this.isDirty=this.isDirtyData=!1;this.hasRendered=!0},redraw:function(){var a=this.chart,b=this.isDirtyData,c=this.group,d=this.xAxis,e=this.yAxis;c&&(a.inverted&&c.attr({width:a.plotWidth,height:a.plotHeight}),c.animate({translateX:n(d&&d.left,a.plotLeft),translateY:n(e&&e.top,a.plotTop)}));this.translate();this.setTooltipPoints(!0);this.render();b&&A(this,\"updatedData\")},setState:function(a){var b=this.options,c=this.graph,d=this.graphNeg,\n"
"e=b.states,b=b.lineWidth,a=a||\"\";if(this.state!==a)this.state=a,e[a]&&e[a].enabled===!1||(a&&(b=e[a].lineWidth||b+1),c&&!c.dashstyle&&(a={\"stroke-width\":b},c.attr(a),d&&d.attr(a)))},setVisible:function(a,b){var c=this,d=c.chart,e=c.legendItem,f,g=d.options.chart.ignoreHiddenSeries,h=c.visible;f=(c.visible=a=c.userOptions.visible=a===u?!h:a)?\"show\":\"hide\";p([\"group\",\"dataLabelsGroup\",\"markerGroup\",\"tracker\"],function(a){if(c[a])c[a][f]()});if(d.hoverSeries===c)c.onMouseOut();e&&d.legend.colorizeItem(c,\n"
"a);c.isDirty=!0;c.options.stacking&&p(d.series,function(a){if(a.options.stacking&&a.visible)a.isDirty=!0});p(c.linkedSeries,function(b){b.setVisible(a,!1)});if(g)d.isDirtyBox=!0;b!==!1&&d.redraw();A(c,f)},show:function(){this.setVisible(!0)},hide:function(){this.setVisible(!1)},select:function(a){this.selected=a=a===u?!this.selected:a;if(this.checkbox)this.checkbox.checked=a;A(this,a?\"select\":\"unselect\")},drawTracker:J.drawTrackerGraph};r(fb.prototype,{addSeries:function(a,b,c){var d,e=this;a&&(b=\n"
"n(b,!0),A(e,\"addSeries\",{options:a},function(){d=e.initSeries(a);e.isDirtyLegend=!0;e.linkSeries();b&&e.redraw(c)}));return d},addAxis:function(a,b,c,d){var e=b?\"xAxis\":\"yAxis\",f=this.options;new qa(this,x(a,{index:this[e].length,isX:b}));f[e]=ja(f[e]||{});f[e].push(a);n(c,!0)&&this.redraw(d)},showLoading:function(a){var b=this.options,c=this.loadingDiv,d=b.loading;if(!c)this.loadingDiv=c=T(Ga,{className:\"highcharts-loading\"},r(d.style,{zIndex:10,display:Q}),this.container),this.loadingSpan=T(\"span\",\n"
"null,d.labelStyle,c);this.loadingSpan.innerHTML=a||b.lang.loading;if(!this.loadingShown)D(c,{opacity:0,display:\"\",left:this.plotLeft+\"px\",top:this.plotTop+\"px\",width:this.plotWidth+\"px\",height:this.plotHeight+\"px\"}),kb(c,{opacity:d.style.opacity},{duration:d.showDuration||0}),this.loadingShown=!0},hideLoading:function(){var a=this.options,b=this.loadingDiv;b&&kb(b,{opacity:0},{duration:a.loading.hideDuration||100,complete:function(){D(b,{display:Q})}});this.loadingShown=!1}});r(Ja.prototype,{update:function(a,\n"
"b,c){var d=this,e=d.series,f=d.graphic,g,h=e.data,i=e.chart,j=e.options,b=n(b,!0);d.firePointEvent(\"update\",{options:a},function(){d.applyOptions(a);if(S(a)){e.getAttribs();if(f)a&&a.marker&&a.marker.symbol?d.graphic=f.destroy():f.attr(d.pointAttr[d.state||\"\"]);if(a&&a.dataLabels&&d.dataLabel)d.dataLabel=d.dataLabel.destroy()}g=sa(d,h);e.updateParallelArrays(d,g);j.data[g]=d.options;e.isDirty=e.isDirtyData=!0;if(!e.fixedBox&&e.hasCartesianSeries)i.isDirtyBox=!0;j.legendType===\"point\"&&i.legend.destroyItem(d);\n"
"b&&i.redraw(c)})},remove:function(a,b){var c=this,d=c.series,e=d.points,f=d.chart,g,h=d.data;Oa(b,f);a=n(a,!0);c.firePointEvent(\"remove\",null,function(){g=sa(c,h);h.length===e.length&&e.splice(g,1);h.splice(g,1);d.options.data.splice(g,1);d.updateParallelArrays(c,\"splice\",g,1);c.destroy();d.isDirty=!0;d.isDirtyData=!0;a&&f.redraw()})}});r(O.prototype,{addPoint:function(a,b,c,d){var e=this.options,f=this.data,g=this.graph,h=this.area,i=this.chart,j=this.xAxis&&this.xAxis.names,k=g&&g.shift||0,l=e.data,\n"
"m,q=this.xData;Oa(d,i);c&&p([g,h,this.graphNeg,this.areaNeg],function(a){if(a)a.shift=k+1});if(h)h.isArea=!0;b=n(b,!0);d={series:this};this.pointClass.prototype.applyOptions.apply(d,[a]);g=d.x;h=q.length;if(this.requireSorting&&g<q[h-1])for(m=!0;h&&q[h-1]>g;)h--;this.updateParallelArrays(d,\"splice\",h,0,0);this.updateParallelArrays(d,h);if(j)j[g]=d.name;l.splice(h,0,a);m&&(this.data.splice(h,0,null),this.processData());e.legendType===\"point\"&&this.generatePoints();c&&(f[0]&&f[0].remove?f[0].remove(!1):\n"
"(f.shift(),this.updateParallelArrays(d,\"shift\"),l.shift()));this.isDirtyData=this.isDirty=!0;b&&(this.getAttribs(),i.redraw())},remove:function(a,b){var c=this,d=c.chart,a=n(a,!0);if(!c.isRemoving)c.isRemoving=!0,A(c,\"remove\",null,function(){c.destroy();d.isDirtyLegend=d.isDirtyBox=!0;d.linkSeries();a&&d.redraw(b)});c.isRemoving=!1},update:function(a,b){var c=this.chart,d=this.type,e=L[d].prototype,f,a=x(this.userOptions,{animation:!1,index:this.index,pointStart:this.xData[0]},{data:this.options.data},\n"
"a);this.remove(!1);for(f in e)e.hasOwnProperty(f)&&(this[f]=u);r(this,L[a.type||d].prototype);this.init(c,a);n(b,!0)&&c.redraw(!1)}});r(qa.prototype,{update:function(a,b){var c=this.chart,a=c.options[this.coll][this.options.index]=x(this.userOptions,a);this.destroy(!0);this._addedPlotLB=this.userMin=this.userMax=u;this.init(c,r(a,{events:u}));c.isDirtyBox=!0;n(b,!0)&&c.redraw()},remove:function(a){var b=this.chart,c=this.coll;p(this.series,function(a){a.remove(!1)});ha(b.axes,this);ha(b[c],this);\n"
"b.options[c].splice(this.options.index,1);p(b[c],function(a,b){a.options.index=b});this.destroy();b.isDirtyBox=!0;n(a,!0)&&b.redraw()},setTitle:function(a,b){this.update({title:a},b)},setCategories:function(a,b){this.update({categories:a},b)}});var ca=ia(O);L.line=ca;Y.area=x(W,{threshold:0});var Ua=ia(O,{type:\"area\",getSegments:function(){var a=[],b=[],c=[],d=this.xAxis,e=this.yAxis,f=e.stacks[this.stackKey],g={},h,i,j=this.points,k=this.options.connectNulls,l,m,n;if(this.options.stacking&&!this.cropped){for(m=\n"
"0;m<j.length;m++)g[j[m].x]=j[m];for(n in f)f[n].total!==null&&c.push(+n);c.sort(function(a,b){return a-b});p(c,function(a){if(!k||g[a]&&g[a].y!==null)g[a]?b.push(g[a]):(h=d.translate(a),l=f[a].percent?f[a].total?f[a].cum*100/f[a].total:0:f[a].cum,i=e.toPixels(l,!0),b.push({y:null,plotX:h,clientX:h,plotY:i,yBottom:i,onMouseOver:la}))});b.length&&a.push(b)}else O.prototype.getSegments.call(this),a=this.segments;this.segments=a},getSegmentPath:function(a){var b=O.prototype.getSegmentPath.call(this,a),\n"
,
"c=[].concat(b),d,e=this.options;d=b.length;var f=this.yAxis.getThreshold(e.threshold),g;d===3&&c.push(\"L\",b[1],b[2]);if(e.stacking&&!this.closedStacks)for(d=a.length-1;d>=0;d--)g=n(a[d].yBottom,f),d<a.length-1&&e.step&&c.push(a[d+1].plotX,g),c.push(a[d].plotX,g);else this.closeSegment(c,a,f);this.areaPath=this.areaPath.concat(c);return b},closeSegment:function(a,b,c){a.push(\"L\",b[b.length-1].plotX,c,\"L\",b[0].plotX,c)},drawGraph:function(){this.areaPath=[];O.prototype.drawGraph.apply(this);var a=this,\n"
"b=this.areaPath,c=this.options,d=c.negativeColor,e=c.negativeFillColor,f=[[\"area\",this.color,c.fillColor]];(d||e)&&f.push([\"areaNeg\",d,e]);p(f,function(d){var e=d[0],f=a[e];f?f.animate({d:b}):a[e]=a.chart.renderer.path(b).attr({fill:n(d[2],ta(d[1]).setOpacity(n(c.fillOpacity,0.75)).get()),zIndex:0}).add(a.group)})},drawLegendSymbol:R.drawRectangle});L.area=Ua;Y.spline=x(W);ca=ia(O,{type:\"spline\",getPointSpline:function(a,b,c){var d=b.plotX,e=b.plotY,f=a[c-1],g=a[c+1],h,i,j,k;if(f&&g){a=f.plotY;j=\n"
"g.plotX;var g=g.plotY,l;h=(1.5*d+f.plotX)/2.5;i=(1.5*e+a)/2.5;j=(1.5*d+j)/2.5;k=(1.5*e+g)/2.5;l=(k-i)*(j-d)/(j-h)+e-k;i+=l;k+=l;i>a&&i>e?(i=s(a,e),k=2*e-i):i<a&&i<e&&(i=I(a,e),k=2*e-i);k>g&&k>e?(k=s(g,e),i=2*e-k):k<g&&k<e&&(k=I(g,e),i=2*e-k);b.rightContX=j;b.rightContY=k}c?(b=[\"C\",f.rightContX||f.plotX,f.rightContY||f.plotY,h||d,i||e,d,e],f.rightContX=f.rightContY=null):b=[\"M\",d,e];return b}});L.spline=ca;Y.areaspline=x(Y.area);Ua=Ua.prototype;ca=ia(ca,{type:\"areaspline\",closedStacks:!0,getSegmentPath:Ua.getSegmentPath,\n"
"closeSegment:Ua.closeSegment,drawGraph:Ua.drawGraph,drawLegendSymbol:R.drawRectangle});L.areaspline=ca;Y.column=x(W,{borderColor:\"#FFFFFF\",borderWidth:1,borderRadius:0,groupPadding:0.2,marker:null,pointPadding:0.1,minPointLength:0,cropThreshold:50,pointRange:null,states:{hover:{brightness:0.1,shadow:!1},select:{color:\"#C0C0C0\",borderColor:\"#000000\",shadow:!1}},dataLabels:{align:null,verticalAlign:null,y:null},stickyTracking:!1,threshold:0});ca=ia(O,{type:\"column\",pointAttrToOptions:{stroke:\"borderColor\",\n"
"\"stroke-width\":\"borderWidth\",fill:\"color\",r:\"borderRadius\"},cropShoulder:0,trackerGroups:[\"group\",\"dataLabelsGroup\"],negStacks:!0,init:function(){O.prototype.init.apply(this,arguments);var a=this,b=a.chart;b.hasRendered&&p(b.series,function(b){if(b.type===a.type)b.isDirty=!0})},getColumnMetrics:function(){var a=this,b=a.options,c=a.xAxis,d=a.yAxis,e=c.reversed,f,g={},h,i=0;b.grouping===!1?i=1:p(a.chart.series,function(b){var c=b.options,e=b.yAxis;if(b.type===a.type&&b.visible&&d.len===e.len&&d.pos===\n"
"e.pos)c.stacking?(f=b.stackKey,g[f]===u&&(g[f]=i++),h=g[f]):c.grouping!==!1&&(h=i++),b.columnIndex=h});var c=I(M(c.transA)*(c.ordinalSlope||b.pointRange||c.closestPointRange||1),c.len),j=c*b.groupPadding,k=(c-2*j)/i,l=b.pointWidth,b=t(l)?(k-l)/2:k*b.pointPadding,l=n(l,k-2*b);return a.columnMetrics={width:l,offset:b+(j+((e?i-(a.columnIndex||0):a.columnIndex)||0)*k-c/2)*(e?-1:1)}},translate:function(){var a=this.chart,b=this.options,c=b.borderWidth,d=this.yAxis,e=this.translatedThreshold=d.getThreshold(b.threshold),\n"
"f=n(b.minPointLength,5),b=this.getColumnMetrics(),g=b.width,h=this.barW=Ha(s(g,1+2*c)),i=this.pointXOffset=b.offset,j=-(c%2?0.5:0),k=c%2?0.5:1;a.renderer.isVML&&a.inverted&&(k+=1);O.prototype.translate.apply(this);p(this.points,function(a){var b=n(a.yBottom,e),c=I(s(-999-b,a.plotY),d.len+999+b),o=a.plotX+i,p=h,r=I(c,b),t,c=s(c,b)-r;M(c)<f&&f&&(c=f,r=w(M(r-e)>f?b-f:e-(d.translate(a.y,0,1,0,1)<=e?f:0)));a.barX=o;a.pointWidth=g;b=M(o)<0.5;p=w(o+p)+j;o=w(o)+j;p-=o;t=M(r)<0.5;c=w(r+c)+k;r=w(r)+k;c-=r;\n"
"b&&(o+=1,p-=1);t&&(r-=1,c+=1);a.shapeType=\"rect\";a.shapeArgs={x:o,y:r,width:p,height:c}})},getSymbol:la,drawLegendSymbol:R.drawRectangle,drawGraph:la,drawPoints:function(){var a=this,b=this.chart,c=a.options,d=b.renderer,e=b.options.animationLimit||250,f;p(a.points,function(g){var h=g.plotY,i=g.graphic;if(h!==u&&!isNaN(h)&&g.y!==null)f=g.shapeArgs,i?(Za(i),i[b.pointCount<e?\"animate\":\"attr\"](x(f))):g.graphic=d[g.shapeType](f).attr(g.pointAttr[g.selected?\"select\":\"\"]).add(a.group).shadow(c.shadow,null,\n"
"c.stacking&&!c.borderRadius);else if(i)g.graphic=i.destroy()})},drawTracker:J.drawTrackerPoint,animate:function(a){var b=this.yAxis,c=this.options,d=this.chart.inverted,e={};if(V)a?(e.scaleY=0.001,a=I(b.pos+b.len,s(b.pos,b.toPixels(c.threshold))),d?e.translateX=a-b.len:e.translateY=a,this.group.attr(e)):(e.scaleY=1,e[d?\"translateX\":\"translateY\"]=b.pos,this.group.animate(e,this.options.animation),this.animate=null)},remove:function(){var a=this,b=a.chart;b.hasRendered&&p(b.series,function(b){if(b.type===\n"
"a.type)b.isDirty=!0});O.prototype.remove.apply(a,arguments)}});L.column=ca;Y.bar=x(Y.column);ca=ia(ca,{type:\"bar\",inverted:!0});L.bar=ca;Y.scatter=x(W,{lineWidth:0,tooltip:{headerFormat:'<span style=\"font-size: 10px; color:{series.color}\">{series.name}</span><br/>',pointFormat:\"x: <b>{point.x}</b><br/>y: <b>{point.y}</b><br/>\",followPointer:!0},stickyTracking:!1});ca=ia(O,{type:\"scatter\",sorted:!1,requireSorting:!1,noSharedTooltip:!0,trackerGroups:[\"markerGroup\"],takeOrdinalPosition:!1,drawTracker:J.drawTrackerPoint,\n"
"drawGraph:function(){this.options.lineWidth&&O.prototype.drawGraph.call(this)},setTooltipPoints:la});L.scatter=ca;Y.pie=x(W,{borderColor:\"#FFFFFF\",borderWidth:1,center:[null,null],clip:!1,colorByPoint:!0,dataLabels:{distance:30,enabled:!0,formatter:function(){return this.point.name}},ignoreHiddenPoint:!0,legendType:\"point\",marker:null,size:null,showInLegend:!1,slicedOffset:10,states:{hover:{brightness:0.1,shadow:!1}},stickyTracking:!1,tooltip:{followPointer:!0}});W={type:\"pie\",isCartesian:!1,pointClass:ia(Ja,\n"
"{init:function(){Ja.prototype.init.apply(this,arguments);var a=this,b;if(a.y<0)a.y=null;r(a,{visible:a.visible!==!1,name:n(a.name,\"Slice\")});b=function(b){a.slice(b.type===\"select\")};F(a,\"select\",b);F(a,\"unselect\",b);return a},setVisible:function(a){var b=this,c=b.series,d=c.chart,e;b.visible=b.options.visible=a=a===u?!b.visible:a;c.options.data[sa(b,c.data)]=b.options;e=a?\"show\":\"hide\";p([\"graphic\",\"dataLabel\",\"connector\",\"shadowGroup\"],function(a){if(b[a])b[a][e]()});b.legendItem&&d.legend.colorizeItem(b,\n"
"a);if(!c.isDirty&&c.options.ignoreHiddenPoint)c.isDirty=!0,d.redraw()},slice:function(a,b,c){var d=this.series;Oa(c,d.chart);n(b,!0);this.sliced=this.options.sliced=a=t(a)?a:!this.sliced;d.options.data[sa(this,d.data)]=this.options;a=a?this.slicedTranslation:{translateX:0,translateY:0};this.graphic.animate(a);this.shadowGroup&&this.shadowGroup.animate(a)}}),requireSorting:!1,noSharedTooltip:!0,trackerGroups:[\"group\",\"dataLabelsGroup\"],axisTypes:[],pointAttrToOptions:{stroke:\"borderColor\",\"stroke-width\":\"borderWidth\",\n"
"fill:\"color\"},getColor:la,animate:function(a){var b=this,c=b.points,d=b.startAngleRad;if(!a)p(c,function(a){var c=a.graphic,a=a.shapeArgs;c&&(c.attr({r:b.center[3]/2,start:d,end:d}),c.animate({r:a.r,start:a.start,end:a.end},b.options.animation))}),b.animate=null},setData:function(a,b){O.prototype.setData.call(this,a,!1);this.processData();this.generatePoints();n(b,!0)&&this.chart.redraw()},generatePoints:function(){var a,b=0,c,d,e,f=this.options.ignoreHiddenPoint;O.prototype.generatePoints.call(this);\n"
"c=this.points;d=c.length;for(a=0;a<d;a++)e=c[a],b+=f&&!e.visible?0:e.y;this.total=b;for(a=0;a<d;a++)e=c[a],e.percentage=b>0?e.y/b*100:0,e.total=b},translate:function(a){this.generatePoints();var b=0,c=this.options,d=c.slicedOffset,e=d+c.borderWidth,f,g,h,i=c.startAngle||0,j=this.startAngleRad=Aa/180*(i-90),i=(this.endAngleRad=Aa/180*((c.endAngle||i+360)-90))-j,k=this.points,l=c.dataLabels.distance,c=c.ignoreHiddenPoint,m,n=k.length,o;if(!a)this.center=a=this.getCenter();this.getX=function(b,c){h=\n"
"P.asin((b-a[1])/(a[2]/2+l));return a[0]+(c?-1:1)*U(h)*(a[2]/2+l)};for(m=0;m<n;m++){o=k[m];f=j+b*i;if(!c||o.visible)b+=o.percentage/100;g=j+b*i;o.shapeType=\"arc\";o.shapeArgs={x:a[0],y:a[1],r:a[2]/2,innerR:a[3]/2,start:w(f*1E3)/1E3,end:w(g*1E3)/1E3};h=(g+f)/2;h>0.75*i&&(h-=2*Aa);o.slicedTranslation={translateX:w(U(h)*d),translateY:w(ba(h)*d)};f=U(h)*a[2]/2;g=ba(h)*a[2]/2;o.tooltipPos=[a[0]+f*0.7,a[1]+g*0.7];o.half=h<-Aa/2||h>Aa/2?1:0;o.angle=h;e=I(e,l/2);o.labelPos=[a[0]+f+U(h)*l,a[1]+g+ba(h)*l,a[0]+\n"
"f+U(h)*e,a[1]+g+ba(h)*e,a[0]+f,a[1]+g,l<0?\"center\":o.half?\"right\":\"left\",h]}},setTooltipPoints:la,drawGraph:null,drawPoints:function(){var a=this,b=a.chart.renderer,c,d,e=a.options.shadow,f,g;if(e&&!a.shadowGroup)a.shadowGroup=b.g(\"shadow\").add(a.group);p(a.points,function(h){d=h.graphic;g=h.shapeArgs;f=h.shadowGroup;if(e&&!f)f=h.shadowGroup=b.g(\"shadow\").add(a.shadowGroup);c=h.sliced?h.slicedTranslation:{translateX:0,translateY:0};f&&f.attr(c);d?d.animate(r(g,c)):h.graphic=d=b.arc(g).setRadialReference(a.center).attr(h.pointAttr[h.selected?\n"
"\"select\":\"\"]).attr({\"stroke-linejoin\":\"round\"}).attr(c).add(a.group).shadow(e,f);h.visible!==void 0&&h.setVisible(h.visible)})},sortByAngle:function(a,b){a.sort(function(a,d){return a.angle!==void 0&&(d.angle-a.angle)*b})},drawTracker:J.drawTrackerPoint,drawLegendSymbol:R.drawRectangle,getCenter:xb.getCenter,getSymbol:la};W=ia(O,W);L.pie=W;O.prototype.drawDataLabels=function(){var a=this,b=a.options,c=b.cursor,d=b.dataLabels,b=a.points,e,f,g,h;if(d.enabled||a._hasPointLabels)a.dlProcessOptions&&a.dlProcessOptions(d),\n"
"h=a.plotGroup(\"dataLabelsGroup\",\"data-labels\",a.visible?\"visible\":\"hidden\",d.zIndex||6),f=d,p(b,function(b){var j,k=b.dataLabel,l,m,p=b.connector,o=!0;e=b.options&&b.options.dataLabels;j=n(e&&e.enabled,f.enabled);if(k&&!j)b.dataLabel=k.destroy();else if(j){d=x(f,e);j=d.rotation;l=b.getLabelConfig();g=d.format?Fa(d.format,l):d.formatter.call(l,d);d.style.color=n(d.color,d.style.color,a.color,\"black\");if(k)if(t(g))k.attr({text:g}),o=!1;else{if(b.dataLabel=k=k.destroy(),p)b.connector=p.destroy()}else if(t(g)){k=\n"
,
"{fill:d.backgroundColor,stroke:d.borderColor,\"stroke-width\":d.borderWidth,r:d.borderRadius||0,rotation:j,padding:d.padding,zIndex:1};for(m in k)k[m]===u&&delete k[m];k=b.dataLabel=a.chart.renderer[j?\"text\":\"label\"](g,0,-999,null,null,null,d.useHTML).attr(k).css(r(d.style,c&&{cursor:c})).add(h).shadow(d.shadow)}k&&a.alignDataLabel(b,k,d,null,o)}})};O.prototype.alignDataLabel=function(a,b,c,d,e){var f=this.chart,g=f.inverted,h=n(a.plotX,-999),i=n(a.plotY,-999),j=b.getBBox();if(a=this.visible&&(a.series.forceDL||\n"
"f.isInsidePlot(a.plotX,a.plotY,g)))d=r({x:g?f.plotWidth-i:h,y:w(g?f.plotHeight-h:i),width:0,height:0},d),r(c,{width:j.width,height:j.height}),c.rotation?(g={align:c.align,x:d.x+c.x+d.width/2,y:d.y+c.y+d.height/2},b[e?\"attr\":\"animate\"](g)):(b.align(c,null,d),g=b.alignAttr,n(c.overflow,\"justify\")===\"justify\"?this.justifyDataLabel(b,c,g,j,d,e):n(c.crop,!0)&&(a=f.isInsidePlot(g.x,g.y)&&f.isInsidePlot(g.x+j.width,g.y+j.height)));if(!a)b.attr({y:-999}),b.placed=!1};O.prototype.justifyDataLabel=function(a,\n"
"b,c,d,e,f){var g=this.chart,h=b.align,i=b.verticalAlign,j,k;j=c.x;if(j<0)h===\"right\"?b.align=\"left\":b.x=-j,k=!0;j=c.x+d.width;if(j>g.plotWidth)h===\"left\"?b.align=\"right\":b.x=g.plotWidth-j,k=!0;j=c.y;if(j<0)i===\"bottom\"?b.verticalAlign=\"top\":b.y=-j,k=!0;j=c.y+d.height;if(j>g.plotHeight)i===\"top\"?b.verticalAlign=\"bottom\":b.y=g.plotHeight-j,k=!0;if(k)a.placed=!f,a.align(b,null,e)};if(L.pie)L.pie.prototype.drawDataLabels=function(){var a=this,b=a.data,c,d=a.chart,e=a.options.dataLabels,f=n(e.connectorPadding,\n"
"10),g=n(e.connectorWidth,1),h=d.plotWidth,d=d.plotHeight,i,j,k=n(e.softConnector,!0),l=e.distance,m=a.center,q=m[2]/2,o=m[1],r=l>0,t,u,v,x,y=[[],[]],z,A,E,K,B,D=[0,0,0,0],I=function(a,b){return b.y-a.y};if(a.visible&&(e.enabled||a._hasPointLabels)){O.prototype.drawDataLabels.apply(a);p(b,function(a){a.dataLabel&&a.visible&&y[a.half].push(a)});for(K=0;!x&&b[K];)x=b[K]&&b[K].dataLabel&&(b[K].dataLabel.getBBox().height||21),K++;for(K=2;K--;){var b=[],J=[],F=y[K],G=F.length,C;a.sortByAngle(F,K-0.5);if(l>\n"
"0){for(B=o-q-l;B<=o+q+l;B+=x)b.push(B);u=b.length;if(G>u){c=[].concat(F);c.sort(I);for(B=G;B--;)c[B].rank=B;for(B=G;B--;)F[B].rank>=u&&F.splice(B,1);G=F.length}for(B=0;B<G;B++){c=F[B];v=c.labelPos;c=9999;var L,N;for(N=0;N<u;N++)L=M(b[N]-v[1]),L<c&&(c=L,C=N);if(C<B&&b[B]!==null)C=B;else for(u<G-B+C&&b[B]!==null&&(C=u-G+B);b[C]===null;)C++;J.push({i:C,y:b[C]});b[C]=null}J.sort(I)}for(B=0;B<G;B++){c=F[B];v=c.labelPos;t=c.dataLabel;E=c.visible===!1?\"hidden\":\"visible\";c=v[1];if(l>0){if(u=J.pop(),C=u.i,\n"
"A=u.y,c>A&&b[C+1]!==null||c<A&&b[C-1]!==null)A=c}else A=c;z=e.justify?m[0]+(K?-1:1)*(q+l):a.getX(C===0||C===b.length-1?c:A,K);t._attr={visibility:E,align:v[6]};t._pos={x:z+e.x+({left:f,right:-f}[v[6]]||0),y:A+e.y-10};t.connX=z;t.connY=A;if(this.options.size===null)u=t.width,z-u<f?D[3]=s(w(u-z+f),D[3]):z+u>h-f&&(D[1]=s(w(z+u-h+f),D[1])),A-x/2<0?D[0]=s(w(-A+x/2),D[0]):A+x/2>d&&(D[2]=s(w(A+x/2-d),D[2]))}}if(za(D)===0||this.verifyDataLabelOverflow(D))this.placeDataLabels(),r&&g&&p(this.points,function(b){i=\n"
"b.connector;v=b.labelPos;if((t=b.dataLabel)&&t._pos)E=t._attr.visibility,z=t.connX,A=t.connY,j=k?[\"M\",z+(v[6]===\"left\"?5:-5),A,\"C\",z,A,2*v[2]-v[4],2*v[3]-v[5],v[2],v[3],\"L\",v[4],v[5]]:[\"M\",z+(v[6]===\"left\"?5:-5),A,\"L\",v[2],v[3],\"L\",v[4],v[5]],i?(i.animate({d:j}),i.attr(\"visibility\",E)):b.connector=i=a.chart.renderer.path(j).attr({\"stroke-width\":g,stroke:e.connectorColor||b.color||\"#606060\",visibility:E}).add(a.group);else if(i)b.connector=i.destroy()})}},L.pie.prototype.placeDataLabels=function(){p(this.points,\n"
"function(a){var a=a.dataLabel,b;if(a)(b=a._pos)?(a.attr(a._attr),a[a.moved?\"animate\":\"attr\"](b),a.moved=!0):a&&a.attr({y:-999})})},L.pie.prototype.alignDataLabel=la,L.pie.prototype.verifyDataLabelOverflow=function(a){var b=this.center,c=this.options,d=c.center,e=c=c.minSize||80,f;d[0]!==null?e=s(b[2]-s(a[1],a[3]),c):(e=s(b[2]-a[1]-a[3],c),b[0]+=(a[3]-a[1])/2);d[1]!==null?e=s(I(e,b[2]-s(a[0],a[2])),c):(e=s(I(e,b[2]-a[0]-a[2]),c),b[1]+=(a[0]-a[2])/2);e<b[2]?(b[2]=e,this.translate(b),p(this.points,function(a){if(a.dataLabel)a.dataLabel._pos=\n"
"null}),this.drawDataLabels&&this.drawDataLabels()):f=!0;return f};if(L.column)L.column.prototype.alignDataLabel=function(a,b,c,d,e){var f=this.chart,g=f.inverted,h=a.dlBox||a.shapeArgs,i=a.below||a.plotY>n(this.translatedThreshold,f.plotSizeY),j=n(c.inside,!!this.options.stacking);if(h&&(d=x(h),g&&(d={x:f.plotWidth-d.y-d.height,y:f.plotHeight-d.x-d.width,width:d.height,height:d.width}),!j))g?(d.x+=i?0:d.width,d.width=0):(d.y+=i?d.height:0,d.height=0);c.align=n(c.align,!g||j?\"center\":i?\"right\":\"left\");\n"
"c.verticalAlign=n(c.verticalAlign,g||j?\"middle\":i?\"top\":\"bottom\");O.prototype.alignDataLabel.call(this,a,b,c,d,e)};r(Highcharts,{Axis:qa,Chart:fb,Color:ta,Point:Ja,Tick:Qa,Tooltip:tb,Renderer:Ya,Series:O,SVGElement:pa,SVGRenderer:ua,arrayMin:La,arrayMax:za,charts:Ia,dateFormat:ab,format:Fa,pathAnim:vb,getOptions:function(){return G},hasBidiBug:Nb,isTouchDevice:Ib,numberFormat:Da,seriesTypes:L,setOptions:function(a){G=x(!0,G,a);Bb();return G},addEvent:F,removeEvent:X,createElement:T,discardElement:Na,\n"
"css:D,each:p,extend:r,map:Ra,merge:x,pick:n,splat:ja,extendClass:ia,pInt:z,wrap:Va,svg:V,canvas:da,vml:!V&&!da,product:\"Highcharts\",version:\"3.0.9\"})})();\n"
"  </script>\n"
"\n"
"  <script type=\"text/javascript\">\n"
"$(function () {\n"
"        $('#all-together').highcharts({\n"
"            chart: {\n"
"                type: 'scatter',\n"
"                zoomType: 'y'\n"
"            },\n"
"            title: {\n"
"                text: '{$title}'\n"
"            },\n"
"            subtitle: {\n"
"                text: 'generated with <a href=\"http://flamingdangerzone.com/nonius\">nonius</a>'\n"
"            },\n"
"            yAxis: {\n"
"                title: { text: 'Time ({$units})' },\n"
"                startOnTick: false,\n"
"                endOnTick: true,\n"
"                showLastLabel: true,\n"
"            },\n"
"            xAxis: {\n"
"                title: { text: null },\n"
"                startOnTick: true,\n"
"                endOnTick: true,\n"
"                showLastLabel: true,\n"
"                minPadding: 0,\n"
"                maxPadding: 0\n"
"            },\n"
"            legend: {\n"
"                layout: 'vertical',\n"
"                align: 'right',\n"
"                verticalAlign: 'top',\n"
"                x: 0,\n"
"                y: 100,\n"
"                backgroundColor: '#FFFFFF',\n"
"                borderWidth: 1\n"
"            },\n"
"            plotOptions: {\n"
"                scatter: {\n"
"                    marker: {\n"
"                        radius: 5,\n"
"                        states: {\n"
"                            hover: {\n"
"                                enabled: true,\n"
"                                lineColor: 'rgb(100,100,100)'\n"
"                            }\n"
"                        }\n"
"                    },\n"
"                    states: {\n"
"                        hover: {\n"
"                            marker: {\n"
"                                enabled: false\n"
"                            }\n"
"                        }\n"
"                    },\n"
"                    tooltip: {\n"
"                        headerFormat: '<b>{series.name}</b><br>',\n"
"                        pointFormat: 'Run #{point.x}: <b>{point.y} {$units}</b>'\n"
"                    }\n"
"                }\n"
"            },\n"
"            series: [{% for benchmark in benchmarks %}{\n"
"                        name: '{$benchmark.name}',\n"
"                        data: [{% for time in benchmark.times %}[{$time}],{% endfor %}]\n"
"                    },\n"
"                    {% endfor %}\n"
"            ]\n"
"        });\n"
"    });\n"
"  </script>\n"
"\n"
" </head>\n"
"\n"
" <body>\n"
"  <div id=\"all-together\"></div>\n"
" </body>\n"
"</html>\n"
"\n"
            };
            static std::string const the_template = []() -> std::string {
                std::string s;
                for(auto part : template_parts) {
                    s += part;
                }
                return s;
            }();
            return the_template;
        }

        std::string description() override {
            return "outputs an HTML file with a single interactive chart of all benchmarks";
        }

        void do_configure(configuration& cfg) override {
            cfg.no_analysis = true;
            title = cfg.title;
            n_samples = cfg.samples;
            verbose = cfg.verbose;
        }

        void do_warmup_start() override {
            if(verbose) progress_stream() << "warming up\n";
        }
        void do_estimate_clock_resolution_start() override {
            if(verbose) progress_stream() << "estimating clock resolution\n";
        }
        void do_estimate_clock_cost_start() override {
            if(verbose) progress_stream() << "estimating cost of a clock call\n";
        }

        void do_benchmark_start(std::string const& name) override {
            if(verbose) progress_stream() << "\nbenchmarking " << name << "\n";
            current = name;
        }

        void do_measurement_start(execution_plan<fp_seconds> plan) override {
            progress_stream() << std::setprecision(7);
            progress_stream().unsetf(std::ios::floatfield);
            if(verbose) progress_stream() << "collecting " << n_samples << " samples, " << plan.iterations_per_sample << " iterations each, in estimated " << detail::pretty_duration(plan.estimated_duration) << "\n";
        }
        void do_measurement_complete(std::vector<fp_seconds> const& samples) override {
            data[current] = samples;
        }

        void do_benchmark_failure(std::exception_ptr) override {
            error_stream() << current << " failed to run successfully\n";
        }

        void do_suite_complete() override {
            if(verbose) progress_stream() << "\ngenerating HTML report\n";

            auto&& templ = template_string();

            auto magnitude = ideal_magnitude();

            cpptempl::data_map map;
            map["title"] = escape(title);
            map["units"] = detail::units_for_magnitude(magnitude);
            for(auto d : data) {
                cpptempl::data_map item;
                item["name"] = escape(d.first);
                for(auto e : d.second) {
                    item["times"].push_back(truncate(e.count() * magnitude));
                }

                map["benchmarks"].push_back(item);
            }

            cpptempl::parse(report_stream(), templ, map);

            report_stream() << std::flush;

            if(verbose) progress_stream() << "done\n";
        }

        static double truncate(double x) {
            return int(x * 1000.) / 1000.;
        }

        double ideal_magnitude() const {
            std::vector<fp_seconds> mins;
            mins.reserve(data.size());
            for(auto d : data) {
                mins.push_back(*std::min_element(d.second.begin(), d.second.end()));
            }
            auto min = *std::min_element(mins.begin(), mins.end());
            return detail::get_magnitude(min);
        }

        static std::string escape(std::string const& source) {
            static const std::unordered_map<char, std::string> escapes {
                { '\'', "&apos;" },
                { '"',  "&quot;" },
                { '<',  "&lt;"   },
                { '>',  "&gt;"   },
                { '&',  "&amp;"  },
                { '\\',  "\\\\"  },
            };
            return detail::escape(source, escapes);
        }

        int n_samples;
        bool verbose;
        std::string title;
        std::string current;
        std::unordered_map<std::string, std::vector<fp_seconds>> data;
    };

    NONIUS_REPORTER("html", html_reporter);
} // namespace nonius

#endif // NONIUS_HPP

#ifdef NONIUS_RUNNER
// #included from: main.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Executable building kit

#define NONIUS_MAIN_HPP

// #included from: detail/argparse.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// Command-line argument parsing

#define NONIUS_ARGPARSE_HPP

// #included from: detail/mismatch.h++
// Nonius - C++ benchmarking tool
//
// Written in 2014 by Martinho Fernandes <martinho.fernandes@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related
// and neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy of the CC0 Public Domain Dedication along with this software.
// If not, see <http://creativecommons.org/publicdomain/zero/1.0/>

// mismatch algorithm

#define NONIUS_DETAIL_MISMATCH_HPP

#include <utility>

namespace nonius {
    namespace detail {
        template <typename InputIt1, typename InputIt2, typename BinaryPredicate>
        std::pair<InputIt1, InputIt2> mismatch(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, BinaryPredicate p) {
            while(first1 != last1 && first2 != last2 && p(*first1, *first2)) {
                ++first1, ++first2;
            }
            return std::make_pair(first1, first2);
        }
    } // namespace detail
} // namespace nonius

#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <ostream>
#include <iomanip>
#include <tuple>
#include <functional>

namespace nonius {
    namespace detail {
        struct option {
            bool matches_short(std::string const& s) const {
                return s == ("-" + short_form);
            }
            std::tuple<bool, std::string::const_iterator> long_separator(std::string const& s) const {
                auto l = "--" + long_form;
                auto its = detail::mismatch(s.begin(), s.end(), l.begin(), l.end(), [](char a, char b) { return a == b; });
                return std::make_tuple(its.second == l.end(), its.first);
            }
            bool matches_long(std::string const& s) const {
                return std::get<0>(long_separator(s));
            }
            bool matches_long(std::string const& s, std::string& argument) const {
                bool match; std::string::const_iterator it;
                std::tie(match, it) = long_separator(s);
                if(match && it != s.end()) {
                    if(*it == '=') argument.assign(it+1, s.end());
                    else return false;
                }
                return match;
            }

            option(std::string long_form, std::string short_form, std::string description, std::string argument = std::string(), bool multiple = false)
            : long_form(std::move(long_form)), short_form(std::move(short_form)), description(std::move(description)), argument(std::move(argument)), multiple(multiple) {}

            std::string long_form;
            std::string short_form;
            std::string description;
            std::string argument;
            bool multiple;
        };

        using option_set = std::vector<option>;

        struct help_text {
            help_text(std::string name, option_set const& options) : name(std::move(name)), options(options) {}
            std::string name;
            option_set const& options;
        };

        template <typename Iterator, typename Projection>
        int get_max_width(Iterator first, Iterator last, Projection proj) {
            auto it = std::max_element(first, last, [&proj](option const& a, option const& b) { return proj(a) < proj(b); });
            return proj(*it);
        }

        inline std::ostream& operator<<(std::ostream& os, help_text h) {
            os << "Usage: " << h.name << " [OPTIONS]\n\n";

            auto lwidth = 2 + get_max_width(h.options.begin(), h.options.end(), [](option const& o) { return 2 + o.long_form.size() + 1 + o.argument.size(); });
            auto swidth = 2 + get_max_width(h.options.begin(), h.options.end(), [](option const& o) { return 1 + o.short_form.size() + 1 + o.argument.size(); });

            os << std::left;
            for(auto o : h.options) {
                auto l = "--" + o.long_form;
                auto s = "-" + o.short_form;
                if(!o.argument.empty()) {
                    l += "=" + o.argument;
                    s += " " + o.argument;
                }
                os << std::setw(lwidth) << l
                   << std::setw(swidth) << s
                   << o.description
                   << '\n';
            }
            return os;
        }

        using arguments = std::unordered_map<std::string, std::string>;

        struct argument_error {
            virtual ~argument_error() = default;
        };

        template <typename Iterator>
        void parse_short(option const& o, arguments& args, Iterator& first, Iterator last) {
            if(!o.argument.empty()) {
                if(++first != last) {
                    args.emplace(o.long_form, *first);
                } else {
                    throw argument_error();
                }
            } else {
                args.emplace(o.long_form, "");
            }
        }
        inline void parse_long(option const& o, arguments& args, std::string&& arg) {
            args.emplace(o.long_form, std::move(arg));
        }

        template <typename Iterator>
        arguments parse_arguments(option_set const& options, Iterator first, Iterator last) {
            arguments args;
            for(; first != last; ++first) {
                bool parsed = false;
                for(auto&& option : options) {
                    if(option.matches_short(*first)) {
                        parse_short(option, args, first, last);
                        parsed = true;
                        break;
                    } else {
                        std::string arg;
                        if(option.matches_long(*first, arg)) {
                            parse_long(option, args, std::move(arg));
                            parsed = true;
                            break;
                        }
                    }
                }
                if(!parsed) {
                    throw argument_error();
                }
            }
            return args;
        }
    } // namespace detail
} // namespace nonius

#include <vector>
#include <string>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <iomanip>
#include <utility>

namespace nonius {
    namespace detail {
        template <typename T>
        struct parser;
        template <>
        struct parser<int> {
            static int parse(std::string const& s) { return std::stoi(s); }
        };
        template <>
        struct parser<double> {
            static double parse(std::string const& s) { return std::stod(s); }
        };
        template <>
        struct parser<std::string> {
            static std::string parse(std::string const& s) { return s; }
        };
        template <>
        struct parser<bool> {
            static bool parse(std::string const&) { return true; }
        };
        template <typename T, typename Predicate>
        void parse(T& variable, detail::arguments& args, std::string const& option, Predicate&& is_valid) {
            auto it = args.find(option);
            if(it != args.end()) {
                auto value = parser<T>::parse(it->second);
                if(is_valid(value)) {
                    variable = value;
                } else {
                    throw argument_error();
                }
            }
        }
        template <typename T>
        void parse(T& variable, detail::arguments& args, std::string const& option) {
            return parse(variable, args, option, [](T const&) { return true; });
        }

        inline detail::option_set const& command_line_options() {
            static detail::option_set the_options {
                detail::option("help", "h", "show this help message"),
                detail::option("samples", "s", "number of samples to collect (default: 100)", "SAMPLES"),
                detail::option("resamples", "rs", "number of resamples for the bootstrap (default: 100000)", "RESAMPLES"),
                detail::option("confidence-interval", "ci", "confidence interval for the bootstrap (between 0 and 1, default: 0.95)", "INTERVAL"),
                detail::option("output", "o", "output file (default: <stdout>)", "FILE"),
                detail::option("reporter", "r", "reporter to use (default: standard)", "REPORTER"),
                detail::option("title", "t", "set report title", "TITLE"),
                detail::option("no-analysis", "A", "perform only measurements; do not perform any analysis"),
                detail::option("list", "l", "list benchmarks"),
                detail::option("list-reporters", "lr", "list available reporters"),
                detail::option("verbose", "v", "show verbose output (mutually exclusive with -q)"),
                detail::option("summary", "q", "show summary output (mutually exclusive with -v)"),
            };
            return the_options;
        }

        template <typename Iterator>
        configuration parse_args(std::string const& name, Iterator first, Iterator last) {
            try {
                auto args = detail::parse_arguments(command_line_options(), first, last);

                configuration cfg;

                auto is_positive = [](int x) { return x > 0; };
                auto is_ci = [](double x) { return x > 0 && x < 1; };
                auto is_reporter = [](std::string const x) { return global_reporter_registry().count(x) > 0; };

                parse(cfg.help, args, "help");
                parse(cfg.samples, args, "samples", is_positive);
                parse(cfg.resamples, args, "resamples", is_positive);
                parse(cfg.confidence_interval, args, "confidence-interval", is_ci);
                parse(cfg.output_file, args, "output");
                parse(cfg.reporter, args, "reporter", is_reporter);
                parse(cfg.no_analysis, args, "no-analysis");
                parse(cfg.list_benchmarks, args, "list");
                parse(cfg.list_reporters, args, "list-reporters");
                parse(cfg.verbose, args, "verbose");
                parse(cfg.summary, args, "summary");
                parse(cfg.title, args, "title");
                if(cfg.verbose && cfg.summary) throw argument_error();

                return cfg;
            } catch(...) {
                std::cout << help_text(name, command_line_options());
                throw argument_error();
            }
        }
    } // namespace detail

    inline int print_help(std::string const& name) {
        std::cout << detail::help_text(name, detail::command_line_options());
        return 0;
    }
    inline int list_benchmarks() {
        std::cout << "All available benchmarks:\n";
        for(auto&& b : global_benchmark_registry()) {
            std::cout << "  " << b.name << "\n";
        }
        std::cout << global_benchmark_registry().size() << " benchmarks\n\n";
        return 0;
    }
    inline int list_reporters() {
        using reporter_entry_ref = decltype(*global_reporter_registry().begin());
        auto cmp = [](reporter_entry_ref a, reporter_entry_ref b) { return a.first.size() < b.first.size(); };
        auto width = 2 + std::max_element(global_reporter_registry().begin(), global_reporter_registry().end(), cmp)->first.size();

        std::cout << "Available reporters:\n";
        std::cout << std::left;
        for(auto&& r : global_reporter_registry()) {
            if(!r.first.empty()) {
                std::cout << "  " << std::setw(width) << r.first << r.second->description() << "\n";
            }
        }
        std::cout << '\n';
        return 0;
    }
    inline int run_it(configuration cfg) {
        try {
            nonius::go(cfg);
        } catch(...) {
            std::cerr << "PANIC: clock is on fire\n";
            try {
                throw;
            } catch(std::exception const& e) {
                std::cerr << "  " << e.what() << "\n";
            } catch(...) {}
            return 23;
        }
        return 0;
    }

    template <typename Iterator>
    int main(std::string const& name, Iterator first, Iterator last) {
        configuration cfg;

        try {
            cfg = detail::parse_args(name, first, last);
        } catch(detail::argument_error const&) {
            return 17;
        }

        if(cfg.help) return print_help(name);
        else if(cfg.list_benchmarks) return list_benchmarks();
        else if(cfg.list_reporters) return list_reporters();
        else return run_it(cfg);
    }
    inline int main(int argc, char** argv) {
        std::string name(argv[0]);
        std::vector<std::string> args(argv+1, argv+argc);
        return main(name, args.begin(), args.end());
    }
}

int main(int argc, char** argv) {
    return nonius::main(argc, argv);
}

#endif // NONIUS_RUNNER
#endif // NONIUS_SINGLE_INCLUDE_HPP
