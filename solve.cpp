#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

// --- Big Integer (string-based) ---
struct BigInt {
    string val; // stored as normal decimal string

    BigInt(long long x = 0) {
        val = to_string(x);
    }

    BigInt(const string &s) {
        int i = 0;
        while (i < (int)s.size() - 1 && s[i] == '0') i++; // remove leading zeros
        val = s.substr(i);
    }

    static BigInt multiply(const BigInt &a, const BigInt &b) {
        string A = a.val, B = b.val;
        int n = A.size(), m = B.size();
        vector<int> res(n + m, 0);
        for (int i = n - 1; i >= 0; i--) {
            for (int j = m - 1; j >= 0; j--) {
                int mul = (A[i] - '0') * (B[j] - '0');
                int sum = res[i + j + 1] + mul;
                res[i + j + 1] = sum % 10;
                res[i + j] += sum / 10;
            }
        }
        string s;
        for (int num : res) {
            if (!(s.empty() && num == 0)) s.push_back('0' + num);
        }
        return BigInt(s.empty() ? "0" : s);
    }

    BigInt operator*(const BigInt &other) const {
        return multiply(*this, other);
    }

    BigInt operator-() const {
        if (val == "0") return *this;
        return BigInt("-" + val);
    }

    friend ostream& operator<<(ostream &os, const BigInt &b) {
        os << b.val;
        return os;
    }
};

// --- Convert from base to decimal BigInt ---
BigInt convertToDecimal(const string &value, int base) {
    BigInt res(0);
    BigInt B(base);
    for (char c : value) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = tolower(c) - 'a' + 10;
        else digit = 0;

        // res = res * base + digit
        res = BigInt::multiply(res, B);
        res = BigInt::multiply(res, BigInt(1)); // copy
        if (digit > 0) {
            res = BigInt::multiply(res, BigInt(1)); // safe copy
            string s = res.val;
            int carry = digit, i = s.size() - 1;
            while (i >= 0 && carry > 0) {
                int sum = (s[i] - '0') + carry;
                s[i] = '0' + (sum % 10);
                carry = sum / 10;
                i--;
            }
            if (carry) s = to_string(carry) + s;
            res = BigInt(s);
        }
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
    vector<BigInt> roots;

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

        BigInt root = convertToDecimal(val_str, base);
        roots.push_back(root);

        root_count++;
        cout << "Root " << root_count << " (base " << base << "): " << val_str 
             << " -> decimal = " << root << "\n";

        pos = quote_v2;
    }

    // Compute constant term using first k roots
    int degree = k - 1;
    BigInt c(1);
    for (int i = 0; i < k; i++) c = c * roots[i];
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
