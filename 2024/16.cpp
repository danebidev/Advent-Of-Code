#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <vector>

#define INPUT_SIZE 57
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

#define RIGHT 0
#define UP 1
#define DOWN 2
#define LEFT 3

struct P {
    int x;
    int y;
};

P getNext(P p, int dir) {
    if (dir == RIGHT) return { p.x + 1, p.y };
    if (dir == UP) return { p.x, p.y - 1 };
    if (dir == LEFT) return { p.x - 1, p.y };
    return { p.x, p.y + 1 };
}

int minPath(const vector<string>& v, P p, int dir, vector<vector<int>>& dp) {
    if (v[p.y][p.x] == 'E') return 0;
    if (v[p.y][p.x] == '#') return 1000000000;
    if (dp[p.y][p.x] != -1) return dp[p.y][p.x];

    int cur = minPath(v, getNext(p, dir), dir, dp) + 1;
    dir++;
    if (dir == 4) dir = 0;
    cur = min(cur, minPath(v, getNext(p, dir), dir, dp) + 1001);
    dir--;
    if (dir == -1) dir = 3;
    dir--;
    if (dir == -1) dir = 3;
    cur = min(cur, minPath(v, getNext(p, dir), dir, dp));

    dp[p.y][p.x] = cur;
    return cur;
}

int part1(const vector<string>& v) {
    P s;
    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            if (v[i][j] == 'S') {
                s = { j, i };
                goto main;
            }
        }
    }

main:
    vector<vector<int>> a(v.size(), vector<int>(v[0].size(), -1));
    return minPath(v, s, RIGHT, a);
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

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        v.push_back(line);
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
