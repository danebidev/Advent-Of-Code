#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#define INPUT_SIZE 500
#define WARMUP_RUNS 10
#define BENCHMARK_RUNS 100

using namespace std;

#define EMPTY 0
#define ROBOT 1
#define BOX 2
#define EDGE 3
#define BOXL 4
#define BOXR 5

struct P {
    int x;
    int y;
};

P getNext(char dir, P p) {
    if (dir == '^')
        p.y--;
    else if (dir == '>')
        p.x++;
    else if (dir == '<')
        p.x--;
    else if (dir == 'v')
        p.y++;

    return p;
}

int part1(const vector<vector<int>>& v, string s) {
    vector<vector<int>> a = v;

    P cur;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            if (a[i][j] == ROBOT) {
                cur = { j, i };
                goto main;
            }
        }
    }

main:
    for (auto c : s) {
        P next = getNext(c, cur);

        if (a[next.y][next.x] == EMPTY) {
            a[cur.y][cur.x] = EMPTY;
            a[next.y][next.x] = ROBOT;
            cur = next;
            continue;
        }

        if (a[next.y][next.x] != BOX) continue;

        P n = next;
        while (1) {
            n = getNext(c, n);
            if (a[n.y][n.x] == EDGE)
                break;
            else if (a[n.y][n.x] == EMPTY) {
                a[cur.y][cur.x] = EMPTY;
                a[next.y][next.x] = ROBOT;
                a[n.y][n.x] = BOX;
                cur = next;
                break;
            }
        }
    }

    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            if (a[i][j] == BOX) {
                ans += 100 * i + j;
            }
        }
    }

    return ans;
}

bool check(vector<vector<int>>& a, char& dir, P p) {
    if (a[p.y][p.x] == EMPTY) return true;
    if (a[p.y][p.x] == EDGE) return false;

    P n;
    if (a[p.y][p.x] == BOXL)
        n = { p.x + 1, p.y };
    else if (a[p.y][p.x] == BOXR)
        n = { p.x - 1, p.y };

    if (check(a, dir, getNext(dir, p)) && check(a, dir, getNext(dir, n))) return true;
    return false;
}

void mov(vector<vector<int>>& a, char& dir, P p) {
    if (a[p.y][p.x] == EMPTY) return;
    if (a[p.y][p.x] != BOXL && a[p.y][p.x] != BOXR) return;

    P n;
    if (a[p.y][p.x] == BOXL)
        n = { p.x + 1, p.y };
    else if (a[p.y][p.x] == BOXR)
        n = { p.x - 1, p.y };

    P next1 = getNext(dir, p);
    P next2 = getNext(dir, n);
    mov(a, dir, next1);
    mov(a, dir, next2);

    a[next1.y][next1.x] = a[p.y][p.x];
    a[next2.y][next2.x] = a[n.y][n.x];
    a[p.y][p.x] = EMPTY;
    a[n.y][n.x] = EMPTY;
}

int part2(const vector<vector<int>>& v, string s) {
    vector<vector<int>> a(v.size());
    P cur;

    for (int i = 0; i < v.size(); i++) {
        for (int j = 0; j < v[i].size(); j++) {
            if (v[i][j] == EDGE) {
                a[i].push_back(EDGE);
                a[i].push_back(EDGE);
            }
            if (v[i][j] == BOX) {
                a[i].push_back(BOXL);
                a[i].push_back(BOXR);
            }
            if (v[i][j] == EMPTY) {
                a[i].push_back(EMPTY);
                a[i].push_back(EMPTY);
            }
            if (v[i][j] == ROBOT) {
                cur = { 2 * j, i };
                a[i].push_back(ROBOT);
                a[i].push_back(EMPTY);
            }
        }
    }

    for (auto c : s) {
        P next = getNext(c, cur);

        if (a[next.y][next.x] == EMPTY) {
            a[cur.y][cur.x] = EMPTY;
            a[next.y][next.x] = ROBOT;
            cur = next;
            continue;
        }

        if (a[next.y][next.x] == EDGE) continue;

        if (c == '>' || c == '<') {
            P n = next;
            bool f = false;
            while (1) {
                n = getNext(c, n);
                if (a[n.y][n.x] == EDGE)
                    break;
                else if (a[n.y][n.x] == EMPTY) {
                    f = true;
                    break;
                }
            }

            if (f) {
                a[cur.y][cur.x] = EMPTY;
                a[next.y][next.x] = ROBOT;
                n = getNext(c, next);
                f = false;
                while (a[n.y][n.x] != EMPTY && a[n.y][n.x] != EDGE) {
                    if (!f)
                        a[n.y][n.x] = c == '>' ? BOXL : BOXR;
                    else
                        a[n.y][n.x] = c == '>' ? BOXR : BOXL;
                    n = getNext(c, n);
                    f = !f;
                }
                a[n.y][n.x] = c == '<' ? BOXL : BOXR;
                cur = next;
            }
        }
        else if (check(a, c, next)) {
            mov(a, c, next);
            a[cur.y][cur.x] = EMPTY;
            a[next.y][next.x] = ROBOT;
            cur = next;
        }
    }

    int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < a[i].size(); j++) {
            if (a[i][j] == BOXL) {
                ans += 100 * i + j;
            }
        }
    }

    return ans;
}

template <typename Func>
void benchmark(Func func, const vector<vector<int>>& v, string s, const string& part_name) {
    vector<long long> us_times;
    vector<long long> ns_times;
    us_times.reserve(BENCHMARK_RUNS);
    ns_times.reserve(BENCHMARK_RUNS);

    for (int i = 0; i < WARMUP_RUNS; ++i) {
        func(v, s);
    }

    int last_ans = 0;
    for (int i = 0; i < BENCHMARK_RUNS; ++i) {
        const auto begin = chrono::high_resolution_clock::now();
        last_ans = func(v, s);
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

    vector<vector<int>> v;

    v.reserve(INPUT_SIZE);
    string line;

    while (getline(input, line)) {
        if (line.empty()) break;
        vector<int> a(line.size());
        for (int i = 0; i < line.size(); i++) {
            if (line[i] == '.')
                a[i] = EMPTY;
            else if (line[i] == '@')
                a[i] = ROBOT;
            else if (line[i] == 'O')
                a[i] = BOX;
            else if (line[i] == '#')
                a[i] = EDGE;
        }
        v.push_back(a);
    }

    stringstream s;
    while (getline(input, line)) {
        s << line;
    }

    benchmark(part1, v, s.str(), "Part 1");
    benchmark(part2, v, s.str(), "Part 2");

    return 0;
}
