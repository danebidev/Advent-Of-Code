#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <vector>

#define INPUT_SIZE 140
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

struct P {
    int x;
    int y;
};

int hasBorder(const vector<string>& v, P p) {
    char cur = v[p.y][p.x];
    int n = 0;

    if (p.x == 0 || v[p.y][p.x - 1] != cur) n |= 1;
    if (p.x == v[0].size() - 1 || v[p.y][p.x + 1] != cur) n |= 2;

    if (p.y == 0 || v[p.y - 1][p.x] != cur) n |= 4;
    if (p.y == v.size() - 1 || v[p.y + 1][p.x] != cur) n |= 8;

    return n;
}

int part1(const vector<string>& v) {
    int ans = 0;

    vector<vector<bool>> visited(v.size(), vector<bool>(v[0].size(), false));
    vector<pair<int, int>> dis { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            if (visited[i][j]) continue;
            visited[i][j] = true;
            queue<P> q;
            q.push(P { j, i });
            int p = 0, a = 0;

            while (!q.empty()) {
                P cur = q.front();
                q.pop();

                a++;
                char curC = v[cur.y][cur.x];

                int n = hasBorder(v, cur);
                if (n & 1) p++;
                if (n & 2) p++;
                if (n & 4) p++;
                if (n & 8) p++;

                for (auto t : dis) {
                    P c = P { cur.x + t.first, cur.y + t.second };
                    if (c.x < 0 || c.x >= v[0].size() || c.y < 0 || c.y >= v.size()) continue;
                    if (visited[c.y][c.x]) continue;
                    if (v[c.y][c.x] != curC) continue;

                    q.push(c);
                    visited[c.y][c.x] = true;
                }
            }
            ans += p * a;
        }
    }

    return ans;
}

int part2(const vector<string>& v) {
    int ans = 0;

    vector<vector<bool>> visited(v.size(), vector<bool>(v[0].size(), false));
    vector<pair<int, int>> dis { { 0, -1 }, { 0, 1 }, { -1, 0 }, { 1, 0 } };

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[0].size(); j++) {
            if (visited[i][j]) continue;
            visited[i][j] = true;
            queue<P> q;
            q.push(P { j, i });
            int p = 0, a = 0;

            while (!q.empty()) {
                P cur = q.front();
                q.pop();

                a++;
                char curC = v[cur.y][cur.x];

                int n = hasBorder(v, cur);
                if ((n & 5) == 5) p++;
                if ((n & 9) == 9) p++;
                if ((n & 6) == 6) p++;
                if ((n & 10) == 10) p++;

                if (!(n & 1) && !(n & 4)) {
                    int nt = hasBorder(v, { cur.x, cur.y - 1 });
                    int nl = hasBorder(v, { cur.x - 1, cur.y });
                    if ((nl & 4) && (nt & 1)) p++;
                }
                if (!(n & 2) && !(n & 4)) {
                    int nt = hasBorder(v, { cur.x, cur.y - 1 });
                    int nr = hasBorder(v, { cur.x + 1, cur.y });
                    if ((nr & 4) && (nt & 2)) p++;
                }
                if (!(n & 1) && !(n & 8)) {
                    int nt = hasBorder(v, { cur.x, cur.y + 1 });
                    int nl = hasBorder(v, { cur.x - 1, cur.y });
                    if ((nl & 8) && (nt & 1)) p++;
                }
                if (!(n & 2) && !(n & 8)) {
                    int nt = hasBorder(v, { cur.x, cur.y + 1 });
                    int nl = hasBorder(v, { cur.x + 1, cur.y });
                    if ((nl & 8) && (nt & 2)) p++;
                }

                for (auto t : dis) {
                    P c = P { cur.x + t.first, cur.y + t.second };
                    if (c.x < 0 || c.x >= v[0].size() || c.y < 0 || c.y >= v.size()) continue;
                    if (visited[c.y][c.x]) continue;
                    if (v[c.y][c.x] != curC) continue;

                    q.push(c);
                    visited[c.y][c.x] = true;
                }
            }
            ans += p * a;
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
