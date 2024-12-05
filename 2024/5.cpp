#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <vector>

#define INPUT_SIZE_1 1176
#define INPUT_SIZE_2 200
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

bool valid(map<int, vector<int>>& m, const vector<int>& v) {
    bool valid = true;
    for (int i = 0; i < v.size(); i++) {
        if (m.find(v[i]) == m.end()) continue;
        for (int j = i - 1; j >= 0; j--) {
            if (find(m[v[i]].begin(), m[v[i]].end(), v[j]) != m[v[i]].end()) {
                return false;
            }
        }
    }

    return true;
}

int part1(const vector<pair<int, int>>& r, const vector<vector<int>>& v) {
    map<int, vector<int>> m;
    int ans = 0;

    for (auto p : r) {
        m[p.first].push_back(p.second);
    }

    for (auto cur : v) {
        if (valid(m, cur)) ans += cur[cur.size() / 2];
    }

    return ans;
}

int part2(const vector<pair<int, int>>& r, const vector<vector<int>>& v) {
    map<int, vector<int>> m;
    int ans = 0;

    for (auto p : r) {
        m[p.first].push_back(p.second);
    }

    for (auto cur : v) {
        if (valid(m, cur)) continue;
        sort(cur.begin(), cur.end(), [&](const auto& n1, const auto& n2) {
            if (m.find(n1) != m.end()) {
                return find(m[n1].begin(), m[n1].end(), n2) == m[n1].end();
            }
            else if (m.find(n2) != m.end()) {
                return find(m[n2].begin(), m[n2].end(), n1) != m[n1].end();
            }
            else
                return true;
        });

        ans += cur[cur.size() / 2];
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<pair<int, int>> r, const vector<vector<int>>& v, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(r, v);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(r, v);
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

    vector<pair<int, int>> r;
    r.reserve(INPUT_SIZE_1);

    vector<vector<int>> v;
    v.reserve(INPUT_SIZE_2);
    string line;

    while (getline(input, line)) {
        if (line.empty()) break;
        r.push_back({ stoi(line), stoi(line.substr(3, 2)) });
    }

    while (getline(input, line)) {
        if (line[0] == '\n') break;
        stringstream ss(line);
        vector<int> a;

        string n;
        while (getline(ss, n, ',')) {
            a.push_back(stoi(n));
        }

        v.push_back(a);
    }

    benchmark(part1, r, v, "Part 1");
    benchmark(part2, r, v, "Part 2");

    return 0;
}
