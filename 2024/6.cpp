#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define INPUT_SIZE 130
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

inline pair<int, int> nextPos(const vector<vector<bool>>& v, const pair<int, int>& p, int& dir) {
    constexpr pair<int, int> directions[] = { { 0, -1 }, { 1, 0 }, { 0, 1 }, { -1, 0 } };
    int nextX = p.first + directions[dir].first, nextY = p.second + directions[dir].second;
    if (nextX < 0 || nextX >= v[0].size() || nextY < 0 || nextY >= v.size()) return { -1, -1 };

    while (v[nextY][nextX]) {
        dir = (dir + 1) % 4;
        nextX = p.first + directions[dir].first, nextY = p.second + directions[dir].second;
        if (nextX < 0 || nextX >= v[0].size() || nextY < 0 || nextY >= v.size()) return { -1, -1 };
    }

    return { nextX, nextY };
}

int part1(const vector<vector<bool>>& v, const pair<int, int>& p) {
    int x = p.first, y = p.second;
    int dir = 0;
    vector<vector<bool>> vis(v.size(), vector<bool>(v[0].size(), false));
    vis[y][x] = true;
    int ans = 1;

    while (1) {
        auto next = nextPos(v, { x, y }, dir);
        if (next.first == -1) break;
        if (!vis[next.second][next.first]) {
            ans++;
            vis[next.second][next.first] = true;
        }
        x = next.first;
        y = next.second;
    }

    return ans;
}

inline bool check(const vector<vector<bool>>& v, const pair<int, int>& p) {
    constexpr int masks[] = { 0b0001, 0b0010, 0b0100, 0b1000 };
    int dir = 0;
    int x = p.first, y = p.second;
    vector<vector<int>> vis(v.size(), vector<int>(v[0].size(), 0));

    while (1) {
        auto next = nextPos(v, { x, y }, dir);
        if (next.first == -1) return false;
        if (vis[next.second][next.first] & masks[dir]) return true;

        vis[next.second][next.first] |= masks[dir];

        x = next.first;
        y = next.second;
    }

    return true;
}

int part2(const vector<vector<bool>>& v, const pair<int, int>& p) {
    int ans = 0;
    int x = p.first, y = p.second;
    int dir = 0;
    vector<vector<bool>> a = v;
    vector<vector<bool>> vis(v.size(), vector<bool>(v[0].size(), false));

    while (1) {
        auto block = nextPos(v, { x, y }, dir);
        if (block.first == -1) break;

        a[block.second][block.first] = true;
        if (check(a, p) && !vis[block.second][block.first]) {
            ans++;
            vis[block.second][block.first] = true;
        }
        a[block.second][block.first] = false;

        x = block.first;
        y = block.second;
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<vector<bool>>& v, const pair<int, int>& p, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(v, p);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(v, p);
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

    vector<vector<bool>> v;
    v.reserve(INPUT_SIZE);
    string line;
    pair<int, int> p;

    while (getline(input, line)) {
        vector<bool> a(line.size(), false);
        for (int i = 0; i < line.size(); i++) {
            if (line[i] != '^')
                a[i] = line[i] == '#';
            else
                p = { i, v.size() };
        }
        v.push_back(a);
    }

    benchmark(part1, v, p, "Part 1");
    benchmark(part2, v, p, "Part 2");

    return 0;
}
