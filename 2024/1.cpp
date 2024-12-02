#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#define INPUT_SIZE 1000
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

int part1(const vector<int>& a, const vector<int>& b) {
    vector<int> sorted_a = a;
    vector<int> sorted_b = b;

    sort(sorted_a.begin(), sorted_a.end());
    sort(sorted_b.begin(), sorted_b.end());

    int ans = 0;
    const size_t size = sorted_a.size();
    for (size_t i = 0; i < size; ++i) {
        ans += std::abs(sorted_a[i] - sorted_b[i]);
    }

    return ans;
}

int part2(const vector<int>& a, const vector<int>& b) {
    unordered_map<int, int> freq;

    for (int n : b) {
        ++freq[n];
    }

    int ans = 0;
    for (int n : a) {
        ans += n * freq[n];
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<int>& a, const vector<int>& b, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(a, b);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(a, b);
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

    vector<int> a, b;
    int n1, n2;

    a.reserve(INPUT_SIZE);
    b.reserve(INPUT_SIZE);

    while (input >> n1 >> n2) {
        a.emplace_back(n1);
        b.emplace_back(n2);
    }

    // Run benchmarks
    benchmark(part1, a, b, "Part 1");
    benchmark(part2, a, b, "Part 2");

    return 0;
}
