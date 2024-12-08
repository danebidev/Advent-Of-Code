#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#define INPUT_SIZE 850
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

inline bool check1(const pair<long long, vector<int>>& p, long long cur, int index) {
    if (cur > p.first) return false;
    if (index >= p.second.size()) return cur == p.first;

    cur += p.second[index];
    if (check1(p, cur, index + 1)) return true;
    cur -= p.second[index];

    cur *= p.second[index];
    return check1(p, cur, index + 1);
}

long long part1(const vector<pair<long long, vector<int>>>& v) {
    long long ans = 0;
    for (auto p : v) {
        if (check1(p, 0, 0)) ans += p.first;
    }

    return ans;
}

inline bool check2(const pair<long long, vector<int>>& p, long long cur, int index) {
    if (cur > p.first) return false;
    if (index >= p.second.size()) return cur == p.first;

    cur += p.second[index];
    if (check2(p, cur, index + 1)) return true;
    cur -= p.second[index];

    cur *= p.second[index];
    if (check2(p, cur, index + 1)) return true;
    cur /= p.second[index];

    cur *= pow(10, to_string(p.second[index]).length());
    cur += p.second[index];
    return check2(p, cur, index + 1);
}

long long part2(const vector<pair<long long, vector<int>>>& v) {
    long long ans = 0;
    for (auto p : v) {
        if (check2(p, 0, 0)) ans += p.first;
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<pair<long long, vector<int>>>& v, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(v);
    }

    long long last_ans = 0;
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

    vector<pair<long long, vector<int>>> v;
    v.reserve(INPUT_SIZE);

    string line;
    char c;

    while (getline(input, line)) {
        istringstream ss(line);
        pair<long long, vector<int>> a;
        ss >> a.first;
        ss >> c;
        int n;
        while (ss >> n) a.second.push_back(n);

        v.push_back(a);
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
