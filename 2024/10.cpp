#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <set>
#include <vector>

#define INPUT_SIZE 57
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

int part1(const vector<string>& v) {
    int ans = 0;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            if (v[i][j] != '0') continue;

            set<pair<int, int>> seen;

            queue<pair<int, int>> q;
            q.push({ i, j });

            while (!q.empty()) {
                auto p = q.front();
                q.pop();
                if (v[p.first][p.second] == '9') {
                    seen.insert({ p.first, p.second });
                    continue;
                }

                if (p.first > 0 && v[p.first - 1][p.second] == v[p.first][p.second] + 1) q.push({ p.first - 1, p.second });
                if (p.first < v.size() - 1 && v[p.first + 1][p.second] == v[p.first][p.second] + 1) q.push({ p.first + 1, p.second });
                if (p.second > 0 && v[p.first][p.second - 1] == v[p.first][p.second] + 1) q.push({ p.first, p.second - 1 });
                if (p.second < v[0].size() - 1 && v[p.first][p.second + 1] == v[p.first][p.second] + 1) q.push({ p.first, p.second + 1 });
            }

            ans += seen.size();
        }
    }

    return ans;
}

int part2(const vector<string>& v) {
    int ans = 0;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            if (v[i][j] != '0') continue;
            queue<pair<int, int>> q;
            q.push({ i, j });

            while (!q.empty()) {
                auto p = q.front();
                q.pop();
                if (v[p.first][p.second] == '9') {
                    ans++;
                    continue;
                }

                if (p.first > 0 && v[p.first - 1][p.second] == v[p.first][p.second] + 1) q.push({ p.first - 1, p.second });
                if (p.first < v.size() - 1 && v[p.first + 1][p.second] == v[p.first][p.second] + 1) q.push({ p.first + 1, p.second });
                if (p.second > 0 && v[p.first][p.second - 1] == v[p.first][p.second] + 1) q.push({ p.first, p.second - 1 });
                if (p.second < v[0].size() - 1 && v[p.first][p.second + 1] == v[p.first][p.second] + 1) q.push({ p.first, p.second + 1 });
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
