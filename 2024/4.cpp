#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define INPUT_SIZE 6
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

int part1(const vector<string>& v) {
    int ans = 0;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            if (j < v[i].size() - 3 && i < v.size() - 3) {
                string s(4, '.');
                for (int k = 0; k < 4; k++) s[k] = v[i + k][j + k];
                if (s == "XMAS") ans++;
                if (s == "SAMX") ans++;
            }
            if (j >= 3 && i < v.size() - 3) {
                string s(4, '.');
                for (int k = 0; k < 4; k++) s[k] = v[i + k][j - k];
                if (s == "XMAS") ans++;
                if (s == "SAMX") ans++;
            }
            if (j < v[i].size() - 3) {
                string s = v[i].substr(j, 4);
                if (s == "XMAS") ans++;
                if (s == "SAMX") ans++;
            }
            if (i < v.size() - 3) {
                string s(4, '.');
                for (int k = 0; k < 4; k++) s[k] = v[i + k][j];
                if (s == "XMAS") ans++;
                if (s == "SAMX") ans++;
            }
        }
    }

    return ans;
}

int part2(const vector<string>& v) {
    return 0;
}

template <typename Func>
void benchmark(Func func, const vector<string>& v, const string& part_name) {
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

    vector<string> v;
    string line;

    while (getline(input, line)) {
        v.push_back(line);
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
