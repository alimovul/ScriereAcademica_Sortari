#ifndef BENCHMARK_UTILS_H
#define BENCHMARK_UTILS_H

#include <iostream>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace chrono;

inline void logRun(const string& type, const string& file, int n) {
    cerr << "[" << setw(7) << left << type << "] "
         << file << " | n = " << n << "\n";
}

inline void logDone(const string& type, const string& file, long long duration_ns) {
    cerr << "[" << setw(7) << left << type << "] "
         << file << " -> "
         << duration_ns << " ns"
         << "  ~" << fixed << setprecision(9)
         << (duration_ns / 1000000000.0)
         << " sec\n";
}

struct Timer {
    high_resolution_clock::time_point start;

    Timer() : start(high_resolution_clock::now()) {}

    long long stop() const {
        auto end = high_resolution_clock::now();
        return duration_cast<nanoseconds>(end - start).count();
    }
};

#endif