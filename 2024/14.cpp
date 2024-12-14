#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <regex>
#include <string>
#include <vector>

#define INPUT_SIZE 500
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

struct P {
    int x;
    int y;
};

struct robot {
    P p;
    P v;
};

constexpr int threshold = 300;
constexpr int width = 101;
constexpr int height = 103;

void doIteration(vector<robot>& a, int n) {
    for (int i = 0; i < a.size(); i++) {
        int x = a[i].p.x + n * a[i].v.x, y = a[i].p.y + n * a[i].v.y;

        while (x >= width) x -= width;
        while (y >= height) y -= height;
        while (x < 0) x += width;
        while (y < 0) y += height;

        a[i].p = { x, y };
    }
}

int part1(const vector<robot>& v) {
    vector<robot> a = v;

    doIteration(a, 100);

    int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i].p.x < width / 2) {
            if (a[i].p.y < height / 2)
                q1++;
            else if (a[i].p.y > height / 2)
                q2++;
        }
        else if (a[i].p.x > width / 2) {
            if (a[i].p.y < height / 2)
                q3++;
            else if (a[i].p.y > height / 2)
                q4++;
        }
    }

    return q1 * q2 * q3 * q4;
}

void printField(const vector<robot>& v) {
    vector<vector<bool>> b(height, vector<bool>(width, false));
    for (int i = 0; i < v.size(); i++) b[v[i].p.y][v[i].p.x] = true;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << b[i][j];
        }
        cout << endl;
    }
}

int part2(const vector<robot>& v) {
    vector<robot> a = v;

    int ans = 0;
    while (1) {
        ans++;
        doIteration(a, 1);

        vector<vector<bool>> b(height, vector<bool>(width, false));
        for (int i = 0; i < a.size(); i++) b[a[i].p.y][a[i].p.x] = true;

        vector<vector<bool>> done(height, vector<bool>(width, false));

        for (int i = 0; i < a.size(); i++) {
            if (done[a[i].p.y][a[i].p.x]) continue;
            int curAdj = 0;
            queue<robot> q;
            q.push(a[i]);
            done[a[i].p.y][a[i].p.x] = true;

            while (!q.empty()) {
                auto cur = q.front();
                q.pop();
                curAdj++;
                done[cur.p.y][cur.p.x] = true;

                for (auto p : vector<P> { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, 1 } }) {
                    int x = cur.p.x + p.x, y = cur.p.y + p.y;
                    if (x < 0 || y < 0 || x >= width || y >= height) continue;
                    if (done[y][x]) continue;
                    if (!b[y][x]) continue;

                    q.push({ x, y });
                }
            }
            if (curAdj > threshold) goto end;
        }
    }

end:
    /*printField(a);*/
    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<robot>& v, const string& part_name) {
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

    vector<robot> v;

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        regex r("p=(-?\\d*),(-?\\d*) v=(-?\\d*),(-?\\d*)");
        smatch m;
        regex_search(line, m, r);

        v.push_back(robot {
            .p = P { stoi(m[1]), stoi(m[2]) },
            .v = P { stoi(m[3]), stoi(m[4]) } });
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
