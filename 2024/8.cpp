#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#define INPUT_SIZE 1000
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

int part1(const vector<string>& v, map<char, vector<pair<int, int>>> m) {
    int ans = 0;
    bool done[v.size()][v[0].size()];
    memset(done, false, sizeof(done));

    for (auto it = m.begin(); it != m.end(); ++it) {
        vector<pair<int, int>> a = it->second;
        for (int i = 0; i < a.size(); i++) {
            for (int j = i + 1; j < a.size(); j++) {
                int dx = a[i].first - a[j].first;
                int dy = a[i].second - a[j].second;

                int x1 = a[i].first + dx, y1 = a[i].second + dy;
                int x2 = a[j].first - dx, y2 = a[j].second - dy;

                if (x1 >= 0 && x1 < v[0].size() && y1 >= 0 && y1 < v.size() && !done[y1][x1]) {
                    done[y1][x1] = true;
                    ans++;
                }

                if (x2 >= 0 && x2 < v[0].size() && y2 >= 0 && y2 < v.size() && !done[y2][x2]) {
                    done[y2][x2] = true;
                    ans++;
                }
            }
        }
    }

    return ans;
}

int part2(const vector<string>& v, map<char, vector<pair<int, int>>> m) {
    int ans = 0;
    bool done[v.size()][v[0].size()];
    memset(done, false, sizeof(done));

    for (auto it = m.begin(); it != m.end(); ++it) {
        vector<pair<int, int>> a = it->second;
        for (int i = 0; i < a.size(); i++) {
            for (int j = i + 1; j < a.size(); j++) {
                int dx = a[i].first - a[j].first;
                int dy = a[i].second - a[j].second;

                int x = a[j].first - dx, y = a[j].second - dy;

                while (x >= 0 && x < v[0].size() && y >= 0 && y < v.size()) {
                    if (!done[y][x]) {
                        done[y][x] = true;
                        ans++;
                    }
                    x -= dx, y -= dy;
                }
                x += dx, y += dy;
                while (x >= 0 && x < v[0].size() && y >= 0 && y < v.size()) {
                    if (!done[y][x]) {
                        done[y][x] = true;
                        ans++;
                    }
                    x += dx, y += dy;
                }
            }
        }
    }
    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<string>& v, map<char, vector<pair<int, int>>> m, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(v, m);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(v, m);
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
    map<char, vector<pair<int, int>>> m;

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '.') continue;
            m[line[i]].push_back({ i, v.size() });
        }
        v.push_back(line);
    }

    benchmark(part1, v, m, "Part 1");
    benchmark(part2, v, m, "Part 2");

    return 0;
}
