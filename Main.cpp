#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

// Перевірка символів з урахуванням режиму регістру
char normalize(char c, bool ignoreCase) {
    if (ignoreCase) {
        return static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return c;
}

// Пошук першого входження
int findFirst(const string& text, const string& pattern, bool ignoreCase = false, long long* comparisons = nullptr, long long* positions = nullptr) {
    size_t n = text.size();
    size_t m = pattern.size();

    if (comparisons) *comparisons = 0;
    if (positions) *positions = 0;

    if (m == 0) return 0;
    if (m > n) return -1;

    for (size_t i = 0; i <= n - m; i++) {
        if (positions) (*positions)++;
        size_t j = 0;
        while (j < m) {
            if (comparisons) (*comparisons)++;
            if (normalize(text[i + j], ignoreCase) != normalize(pattern[j], ignoreCase)) {
                break;
            }
            j++;
        }
        if (j == m) return static_cast<int>(i);
    }
    return -1;
}

// Пошук усіх входжень
vector<int> findAll(const string& text, const string& pattern, bool ignoreCase = false, long long* comparisons = nullptr, long long* positions = nullptr) {
    vector<int> result;
    if (comparisons) *comparisons = 0;
    if (positions) *positions = 0;

    size_t n = text.size();
    size_t m = pattern.size();

    if (m == 0) {
        result.push_back(0);
        return result;
    }
    if (m > n) return result;

    for (size_t i = 0; i <= n - m; i++) {
        if (positions) (*positions)++;
        size_t j = 0;
        while (j < m) {
            if (comparisons) (*comparisons)++;
            if (normalize(text[i + j], ignoreCase) != normalize(pattern[j], ignoreCase)) {
                break;
            }
            j++;
        }
        if (j == m) result.push_back(static_cast<int>(i));
    }
    return result;
}

// Автоматизовані тести
void tests() {
    string texts[] = {
        "algorithm", "algorithm", "algorithm", "algorithm",
        "aaaa", "abababa", "abc", "Data data DATA",
        "one two two", "1, 2, 1, 2, 1", "4, 5, 6", "",
        "banana", "12345123", "mississippi", "121212", ""
    };

    string patterns[] = {
        "algo", "rith", "thm", "sort",
        "aa", "aba", "abcd", "data",
        "two", "1, 2, 1", "5, 7", "abc",
        "ana", "123", "issi", "12", ""
    };

    // Виправлено очікуваний результат для 10-го тесту: {0, 3} замість {0, 2}
    vector<vector<int>> expected = {
        {0}, {4}, {6}, {},
        {0, 1, 2}, {0, 2, 4}, {}, {5},
        {4, 8}, {0, 3}, {}, {},
        {1, 3}, {0, 5}, {1, 4}, {0, 2, 4}, {0}
    };

    int passedFirst = 0;
    int passedAll = 0;

    cout << "\nAUTOMATED TESTS\n";
    for (int i = 0; i < 17; i++) {
        int first = findFirst(texts[i], patterns[i]);
        vector<int> all = findAll(texts[i], patterns[i]);

        int expectedFirst = expected[i].empty() ? -1 : expected[i][0];
        bool firstOK = (first == expectedFirst);
        bool allOK = (all == expected[i]);

        if (firstOK) passedFirst++;
        if (allOK) passedAll++;

        cout << "Test " << i + 1 << ": First = " << (firstOK ? "OK" : "FAIL")
            << ", All = " << (allOK ? "OK" : "FAIL") << endl;
    }

    cout << "\nFirst occurrence: " << passedFirst << "/17" << endl;
    cout << "All occurrences: " << passedAll << "/17" << endl;
}

// Експеримент для текстів різної довжини
void experiment() {
    int sizes[] = { 100, 1000, 10000 };
    cout << "\nEXPERIMENT\n";

    for (int n : sizes) {
        string text(n, 'a');
        string pattern = "aaaaab";
        cout << "\nText length: " << n << endl;

        int starts[] = { 0, n / 2, n - 6, -1 };
        for (int k = 0; k < 4; k++) {
            string current = text;
            if (k < 3) {
                current.replace(starts[k], 6, "aaaaab");
            }
            long long comparisons = 0;
            long long positions = 0;

            int result = findFirst(current, pattern, false, &comparisons, &positions);
            cout << char('A' + k) << ": index = " << result
                << ", comparisons = " << comparisons
                << ", positions = " << positions << endl;
        }
    }
}

int main() {
    string text, pattern;
    cout << "Enter main text: ";
    getline(cin, text);
    cout << "Enter pattern: ";
    getline(cin, pattern);

    long long comparisons = 0;
    long long positions = 0;

    int first = findFirst(text, pattern, false, &comparisons, &positions);
    cout << "\nFirst occurrence: " << first << endl;
    cout << "Comparisons: " << comparisons << endl;
    cout << "Checked positions: " << positions << endl;

    vector<int> all = findAll(text, pattern);
    cout << "All occurrences: ";
    if (all.empty()) {
        cout << "none";
    }
    else {
        for (int index : all) cout << index << " ";
    }
    cout << endl;

    cout << "\nCase-insensitive search: " << findFirst(text, pattern, true) << endl;

    tests();
    experiment();

    return 0;
}
