#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_map>
#include <vector>

#define INPUT_SIZE 8
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

unordered_map<long long, unordered_map<int, long long>> dp;

inline long long solve(long long n, int left) {
    if (left == 0) return 1;
    if (dp.find(n) != dp.end() && dp[n].find(left) != dp[n].end()) return dp[n][left];

    long long ans;
    if (n == 0)
        ans = solve(1, left - 1);
    else {
        string s = to_string(n);
        if (s.size() % 2 == 0)
            ans = solve(stoi(s.substr(0, s.size() / 2)), left - 1) + solve(stoi(s.substr(s.size() / 2)), left - 1);
        else
            ans = solve(n * 2024, left - 1);
    }

    dp[n][left] = ans;
    return ans;
}

int part1(const vector<long long>& v) {
    long long ans = 0;
    for (int i = 0; i < v.size(); i++) {
        ans += solve(v[i], 25);
    }
    return ans;
}

long long part2(const vector<long long>& v) {
    long long ans = 0;
    for (int i = 0; i < v.size(); i++) {
        ans += solve(v[i], 75);
    }
    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<long long>& v, const string& part_name) {
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

    vector<long long> v;

    v.reserve(INPUT_SIZE);
    int n;

    while (input >> n) v.push_back(n);

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
