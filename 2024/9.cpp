#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

#define INPUT_SIZE 20000
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

long long part1(const string& s) {
    long long ans = 0;

    vector<long long> files, free;
    for (int i = 0; i < s.size(); i++) {
        if (i % 2 == 0) {
            files.push_back(s[i] - '0');
        }
        else
            free.push_back(s[i] - '0');
    }

    long long curfile = 0, lastfile = files.size() - 1, curfree = 0;
    long long cur = 0;
    for (int i = 0; i < s.size(); i++) {
        if (curfile == files.size()) break;

        if (i % 2 == 0) {
            while (files[curfile] > 0) {
                ans += cur * curfile;
                cur++;
                files[curfile]--;
            }
            curfile++;
        }
        else {
            while (free[curfree] > 0) {
                while (files[lastfile] == 0 && lastfile >= 0) {
                    lastfile--;
                }
                if (lastfile < 0) break;

                ans += cur * lastfile;
                cur++;
                files[lastfile]--;
                free[curfree]--;
            }
            curfree++;
        }
    }

    return ans;
}

long long part2(const string& s) {
    long long ans = 0;

    // How many - value
    vector<pair<int, int>> v;
    for (int i = 0; i < s.size(); i++) {
        if (i % 2 == 0) {
            v.push_back({ s[i] - '0', i / 2 });
        }
        else {
            v.push_back({ s[i] - '0', 0 });
        }
    }

    for (int i = v.size() - 1; i > 1; i--) {
        if (v[i].second == 0) continue;
        for (int j = 1; j < i; j++) {
            if (v[j].second != 0) continue;

            if (v[j].first >= v[i].first) {
                v[j].first -= v[i].first;
                auto c = v[i];
                v.insert(v.begin() + j, c);
                v[i + 1] = c;
                v[i + 1].second = 0;
                break;
            }
        }
    }

    int cur = 0;
    for (auto p : v) {
        if (p.second == 0) {
            cur += p.first;
            continue;
        }

        while (p.first > 0) {
            ans += p.second * cur;
            cur++;
            p.first--;
        }
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const string& s, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(s);
    }

    long long last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(s);
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

    string s;

    s.reserve(INPUT_SIZE);

    input >> s;

    benchmark(part1, s, "Part 1");
    benchmark(part2, s, "Part 2");

    return 0;
}
