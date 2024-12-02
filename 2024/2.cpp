#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <sstream>
#include <vector>

#define INPUT_SIZE 1000
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

bool is_safe(const list<int>& v) {
    auto end = v.end();
    auto prev = v.begin();

    bool incr = *next(prev) > *prev;
    bool valid = true;

    for (auto it = next(prev); it != end; it++) {
        int p = *prev;
        int diff = abs(*it - p);
        if (incr != (*it > p) || diff < 1 || diff > 3) {
            valid = false;
            break;
        }
        prev = it;
    }

    return valid;
}

int part1(const vector<list<int>>& v) {
    int ans = 0;

    for (const auto& cur : v) {
        if (is_safe(cur)) ans++;
    }

    return ans;
}

int part2(const vector<list<int>>& v) {
    int ans = 0;

    for (list<int> cur : v) {
        for (auto it = cur.begin(); it != cur.end(); ++it) {
            int rem = *it;
            cur.erase(it++);
            if (is_safe(cur)) {
                ans++;
                break;
            }
            cur.insert(it, rem);
            it--;
        }
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<list<int>>& v, const string& part_name) {
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

    vector<list<int>> v;

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        istringstream lineStream(line);
        list<int> a;

        int n;
        while (lineStream >> n) a.push_back(n);

        v.push_back(a);
    }

    // Run benchmarks
    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
