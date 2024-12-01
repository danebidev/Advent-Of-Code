#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

long long part1(vector<int> a, vector<int> b) {
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    long long int ans = 0;
    for (int i = 0; i < a.size(); i++) {
        ans += abs(a[i] - b[i]);
    }

    return ans;
}

long long part2(vector<int> a, vector<int> b) {
    map<int, int> m;
    for (int n : b) {
        m[n]++;
    }

    long long int ans = 0;
    for (int n : a) {
        ans += n * m[n];
    }

    return ans;
}

int main() {
    ifstream input("input");
    string line;
    chrono::steady_clock::time_point begin, end;
    long long ans;

    vector<int> a, b;
    int n1, n2;

    /*while (cin >> n1 >> n2) {*/
    while (input >> n1 >> n2) {
        a.push_back(n1);
        b.push_back(n2);
    }

    begin = chrono::steady_clock::now();
    ans = part1(a, b);
    end = chrono::steady_clock::now();
    printf("Part 1\n");
    printf("    Answer: %lld\n", ans);
    printf("    Time: %ld µs | %ld ns\n", chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());

    begin = chrono::steady_clock::now();
    ans = part2(a, b);
    end = chrono::steady_clock::now();
    printf("\nPart 2\n");
    printf("    Answer: %lld\n", ans);
    printf("    Time: %ld µs | %ld ns\n", chrono::duration_cast<std::chrono::microseconds>(end - begin).count(), std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count());
}
