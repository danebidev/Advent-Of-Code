#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <vector>

#define INPUT_SIZE 6
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

int part1(const string& s) {
    regex reg("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)");
    sregex_iterator begin(s.begin(), s.end(), reg);
    sregex_iterator end;

    int ans = 0;
    smatch m;

    for (auto it = begin; it != end; ++it) {
        m = *it;

        ans += stoi(m[1]) * stoi(m[2]);
    }

    return ans;
}

int part2(const string& s) {
    regex reg("mul\\(([0-9]{1,3}),([0-9]{1,3})\\)|do\\(\\)|don't\\(\\)");
    sregex_iterator begin(s.begin(), s.end(), reg);
    sregex_iterator end;

    int ans = 0;
    bool active = true;

    smatch m;

    for (auto it = begin; it != end; ++it) {
        m = *it;

        if (m[0] == "do()")
            active = true;
        else if (m[0] == "don't()")
            active = false;
        else if (active)
            ans += stoi(m[1]) * stoi(m[2]);
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const string& v, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(v);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(v);
        const auto end = chrono::high_resolution_clock::now();

        us_times.push_back(chrono::duration_cast<chrono::microseconds>(end - begin).count());
        ns_times.push_back(chrono::duration_cast<chrono::nanoseconds>(end - begin).count());
    }

    long long mean_us_time = accumulate(us_times.begin(), us_times.end(), 0LL) / us_times.size();
    long long mean_ns_time = accumulate(ns_times.begin(), ns_times.end(), 0LL) / ns_times.size();

    cout << part_name << endl;
    cout << "    Last Answer: " << last_ans << endl;
    cout << "    Average Time: " << mean_us_time << " µs | " << mean_ns_time << " ns" << endl;
}

int main() {
    ifstream input("input");

    stringstream v;
    string line;

    while (getline(input, line)) {
        v << line;
    }

    string i = v.str();
    benchmark(part1, i, "Part 1");
    benchmark(part2, i, "Part 2");

    return 0;
}
