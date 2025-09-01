#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace boost::multiprecision;

cpp_int convertToDecimal(const string &value, int base) {
    cpp_int res = 0;
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else digit = 0;
        res = res * base + digit;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cout << "=== Hashira Placements Assignment Solver ===\n";
    cout << "Reading JSON input...\n";

    // Read JSON input into a string
    string input, line;
    while (getline(cin, line)) input += line;

    int n = 0, k = 0;
    vector<cpp_int> roots;

    // Extract n and k
    {
        size_t pos_n = input.find("\"n\"");
        size_t pos_k = input.find("\"k\"");
        if (pos_n != string::npos) {
            n = stoi(input.substr(input.find(":", pos_n) + 1));
        }
        if (pos_k != string::npos) {
            k = stoi(input.substr(input.find(":", pos_k) + 1));
        }
    }

    cout << "Total roots provided (n): " << n << "\n";
    cout << "Number of roots to use (k): " << k << "\n";
    cout << "Polynomial degree = k - 1 = " << (k - 1) << "\n\n";

    // Extract all roots
    size_t pos = 0;
    int root_count = 0;
    while ((pos = input.find("\"base\"", pos)) != string::npos) {
        size_t colon_b = input.find(":", pos);
        size_t quote_b1 = input.find("\"", colon_b + 1);
        size_t quote_b2 = input.find("\"", quote_b1 + 1);
        string base_str = input.substr(quote_b1 + 1, quote_b2 - quote_b1 - 1);
        int base = stoi(base_str);

        size_t pos_val = input.find("\"value\"", quote_b2);
        size_t colon_v = input.find(":", pos_val);
        size_t quote_v1 = input.find("\"", colon_v + 1);
        size_t quote_v2 = input.find("\"", quote_v1 + 1);
        string val_str = input.substr(quote_v1 + 1, quote_v2 - quote_v1 - 1);

        cpp_int root = convertToDecimal(val_str, base);
        roots.push_back(root);

        root_count++;
        cout << "Root " << root_count << " (base " << base << "): " << val_str 
             << " -> decimal = " << root << "\n";

        pos = quote_v2;
    }

    // Compute constant term using first k roots
    int degree = k - 1;
    cpp_int c = 1;
    for (int i = 0; i < k; i++) c *= roots[i];
    if (degree % 2 == 1) c = -c;

    cout << "\n=== Final Result ===\n";
    cout << "Constant term (c) = " << c << "\n";

    cout << "\n✅ Done. Your output is above.\n";
    cout << "👉 Next steps:\n";
    cout << "1. Save your JSON input as input.json\n";
    cout << "2. Run:   ./solve < input.json > output.txt\n";
    cout << "3. Check output.txt for result\n";
    cout << "4. Push main.cpp, input.json, and output.txt to GitHub\n";

    return 0;
}
