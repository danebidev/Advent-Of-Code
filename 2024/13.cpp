#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define INPUT_SIZE 1280
#define WARMUP_RUNS 0
#define BENCHMARK_RUNS 1

using namespace std;

struct P {
    long long x;
    long long y;
};

int part1(const vector<vector<P>>& v) {
    int ans = 0;

    for (auto cur : v) {
        P a = cur[0];
        P b = cur[1];
        P p = cur[2];

        int B = a.y * p.x - a.x * p.y;
        int n1 = b.x * a.y - b.y * a.x;
        if (B % n1 != 0) continue;
        B /= n1;

        int A = p.x - B * b.x;
        if (A % a.x) continue;
        A /= a.x;

        ans += 3 * A + B;
    }

    return ans;
}

long long part2(const vector<vector<P>>& v) {
    long long ans = 0;

    for (auto cur : v) {
        P a = cur[0];
        P b = cur[1];
        P p = cur[2];
        p.x += 10000000000000;
        p.y += 10000000000000;

        long long B = a.y * p.x - a.x * p.y;
        long long n1 = b.x * a.y - b.y * a.x;
        if (B % n1 != 0) continue;
        B /= n1;

        long long A = p.x - B * b.x;
        if (A % a.x) continue;
        A /= a.x;

        ans += 3 * A + B;
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<vector<P>>& v, const string& part_name) {
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

P getInput(string s) {
    s = s.substr(s.find("X"));
    int pos = s.find_first_of("0123456789");
    int x = stoi(s.substr(pos));

    s = s.substr(s.find("Y"));
    pos = s.find_first_of("0123456789");
    int y = stoi(s.substr(pos));

    return { x, y };
}

int main() {
    ifstream input("input");

    vector<vector<P>> v;

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        vector<P> p;

        p.push_back(getInput(line));
        getline(input, line);
        p.push_back(getInput(line));
        getline(input, line);
        p.push_back(getInput(line));
        getline(input, line);

        v.push_back(p);
    }

    benchmark(part1, v, "Part 1");
    benchmark(part2, v, "Part 2");

    return 0;
}
