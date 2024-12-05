#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define INPUT_SIZE 140
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

string diag1(const vector<string>& v, int y, int x, int l) {
    string s(l, '.');
    for (int k = 0; k < l; k++) s[k] = v[y + k][x + k];
    return s;
}

string diag2(const vector<string>& v, int y, int x, int l) {
    string s(l, '.');
    for (int k = 0; k < l; k++) s[k] = v[y + k][x - k];
    return s;
}

int part1(const vector<string>& v) {
    int ans = 0;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            if (v[i][j] != 'X' && v[i][j] != 'S') continue;
            if (j < v[i].size() - 3 && i < v.size() - 3) {
                string s = diag1(v, i, j, 4);
                if (s == "XMAS") ans++;
                if (s == "SAMX") ans++;
            }
            if (j >= 3 && i < v.size() - 3) {
                string s = diag2(v, i, j, 4);
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
    int ans = 0;
    for (int i = 1; i < v.size() - 1; i++) {
        for (int j = 1; j < v[i].size() - 1; j++) {
            if (v[i][j] == 'A') {
                string s1 = diag1(v, i - 1, j - 1, 3);
                if (s1 != "MAS" && s1 != "SAM") continue;
                string s2 = diag2(v, i - 1, j + 1, 3);
                if (s2 == "MAS" || s2 == "SAM") ans++;
            }
        }
    }

    return ans;
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
    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        v.push_back(line);
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
