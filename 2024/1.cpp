#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#define INPUT_SIZE 1000

using namespace std;

int part1(const vector<int>& a, const vector<int>& b) {
    vector<int> sorted_a = a;
    vector<int> sorted_b = b;

    sort(sorted_a.begin(), sorted_a.end());
    sort(sorted_b.begin(), sorted_b.end());

    int ans = 0;
    const size_t size = sorted_a.size();
    for (size_t i = 0; i < size; ++i) {
        ans += std::abs(sorted_a[i] - sorted_b[i]);
    }

    return ans;
}

int part2(const vector<int>& a, const vector<int>& b) {
    unordered_map<int, int> freq;

    for (int n : b) {
        ++freq[n];
    }

    int ans = 0;
    for (int n : a) {
        ans += n * freq[n];
    }

    return ans;
}

int main() {
    ifstream input("input");
    if (!input) {
        cerr << "Error opening input file" << endl;
        return 1;
    }

    vector<int> a, b;
    int n1, n2;

    a.reserve(INPUT_SIZE);
    b.reserve(INPUT_SIZE);

    while (input >> n1 >> n2) {
        a.emplace_back(n1);
        b.emplace_back(n2);
    }

    auto benchmark = [&](auto func, const string& part_name) {
        const auto begin = chrono::high_resolution_clock::now();
        int ans = func(a, b);
        const auto end = chrono::high_resolution_clock::now();

        const auto microsecs = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        const auto nanosecs = chrono::duration_cast<chrono::nanoseconds>(end - begin).count();

        cout << part_name << endl;
        cout << "    Answer: " << ans << endl;
        cout << "    Time: " << microsecs << " µs | " << nanosecs << " ns" << endl;
    };

    benchmark(part1, "Part 1");
    benchmark(part2, "Part 2");
}
