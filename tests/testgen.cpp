/*****************************************
       Butoi Alexandru - Gabriel
         Universitatea de Vest
             din Timisoara
*****************************************/ 

#include <iostream>
#include <fstream>
#include <random>
#include <filesystem>
#include <vector>
#include <iomanip>
using namespace std;

mt19937 rng(1337);

bool exists(const string& filename) {
    return filesystem::exists(filename);
}

int randInt(int l, int r) {
    return uniform_int_distribution<int>(l, r)(rng);
}

double randDouble(double l, double r) {
    return uniform_real_distribution<double>(l, r)(rng);
}

char randChar(char l, char r) {
    return (char)randInt((int)l, (int)r);
}

bool shouldSkip(const string& file, bool overwrite) {
    return (!overwrite && exists(file));
}

void genRandom(string file, int n, int l, int r, bool overwrite = true) { 
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";
    for (int i = 0; i < n; i++) {
        fout << randInt(l, r) << " ";
    }
}

void genSorted(string file, int n, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";
    for (int i = 1; i <= n; i++) {
        fout << i << " ";
    }
}

void genReverse(string file, int n, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";
    for (int i = n; i >= 1; i--) {
        fout << i << " ";
    }
}

void genEqual(string file, int n, int val, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";
    for (int i = 0; i < n; i++) {
        fout << val << " ";
    }
}

void genAlmostSorted(string file, int n, int swaps, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    vector<int> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = i + 1;
    }

    for (int i = 0; i < swaps && n > 1; i++) {
        int a = randInt(0, n - 1);
        int b = randInt(0, n - 1);
        swap(v[a], v[b]);
    }

    for (int i = 0; i < n; i++) {
        fout << v[i] << " ";
    }
}

void genRandomFloat(string file, int n, double l, double r, int precision = 3, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << fixed << setprecision(precision);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << randDouble(l, r) << " ";
    }
}

void genEqualFloat(string file, int n, double val, int precision = 3, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << fixed << setprecision(precision);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << val << " ";
    }
}

void genAlmostSortedFloat(string file, int n, int swaps, int precision = 3, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << fixed << setprecision(precision);
    fout << n << "\n";

    vector<double> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = i + 1 + randDouble(0.0, 0.999);
    }

    for (int i = 0; i < swaps && n > 1; i++) {
        int a = randInt(0, n - 1);
        int b = randInt(0, n - 1);
        swap(v[a], v[b]);
    }

    for (int i = 0; i < n; i++) {
        fout << v[i] << " ";
    }
}

void genRandomChar(string file, int n, char l = 'a', char r = 'z', bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << randChar(l, r) << " ";
    }
}

void genEqualChar(string file, int n, char val, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << val << " ";
    }
}

void genSortedChar(string file, int n, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << (char)('a' + (i % 26)) << " ";
    }
}

void genReverseChar(string file, int n, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    for (int i = 0; i < n; i++) {
        fout << (char)('z' - (i % 26)) << " ";
    }
}

void genAlmostSortedChar(string file, int n, int swaps, bool overwrite = true) {
    if (shouldSkip(file, overwrite)) return;

    ofstream fout(file);
    fout << n << "\n";

    vector<char> v(n);
    for (int i = 0; i < n; i++) {
        v[i] = (char)('a' + (i % 26));
    }

    for (int i = 0; i < swaps && n > 1; i++) {
        int a = randInt(0, n - 1);
        int b = randInt(0, n - 1);
        swap(v[a], v[b]);
    }

    for (int i = 0; i < n; i++) {
        fout << v[i] << " ";
    }
}

int main() {
    bool o = false; // overwrite

    // int

    genRandom("test1.in", 10, -100, 100);
    genSorted("test2.in", 10);

    genRandom("test3.in", 1000, -1000, 1000);
    genReverse("test4.in", 1000);

    genRandom("test5.in", 5000, -10000, 10000);
    genEqual("test6.in", 5000, 7);

    genRandom("test7.in", 10000, -100000, 100000);

    genReverse("test8.in", 50000);
    genRandom("test9.in", 100000, -1000000, 1000000);

    genRandom("test10.in", 200000, -1000000, 1000000);
    genReverse("test11.in", 500000);
    genRandom("test12.in", 1000000, -1000000, 1000000);

    genRandom("test13.in", 10000000, -1000000, 1000000, o);
    genRandom("test14.in", 100000000, -1000000, 1000000, o);
    genRandom("test15.in", 500000000, -1000000, 1000000, o);

    genRandom("test16.in", 1, -10, 10);
    genEqual("test17.in", 50, 0);
    genSorted("test18.in", 100);
    genReverse("test19.in", 100);

    genRandom("test20.in", 2000, -5000, 5000);
    genSorted("test21.in", 2000);
    genReverse("test22.in", 2000);

    genRandom("test23.in", 7500, -20000, 20000);
    genEqual("test24.in", 7500, -3);

    genRandom("test25.in", 20000, -100000, 100000);
    genSorted("test26.in", 20000);
    genReverse("test27.in", 20000);

    genRandom("test28.in", 50000, -100000, 100000);
    genEqual("test29.in", 50000, 42);

    genRandom("test30.in", 100000, -1, 1);
    genRandom("test31.in", 100000, 0, 1);
    genRandom("test32.in", 100000, -100, 100);

    genRandom("test33.in", 200000, -1000000, 1000000);
    genReverse("test34.in", 200000);

    genRandom("test35.in", 500000, -1000000, 1000000);
    genSorted("test36.in", 500000);

    genRandom("test37.in", 1000000, -1000000, 1000000);

    genAlmostSorted("test38.in", 10000, 10);
    genAlmostSorted("test39.in", 10000, 100);
    genAlmostSorted("test40.in", 10000, 1000);

    genAlmostSorted("test41.in", 100000, 100);
    genAlmostSorted("test42.in", 100000, 5000);

    genAlmostSorted("test43.in", 500000, 1000);

    //float

    genRandomFloat("test44.in", 1000, -100.0, 100.0, 3, o);
    genRandomFloat("test45.in", 5000, -1000.0, 1000.0, 4, o);
    genEqualFloat("test46.in", 3000, 3.1415, 4, o);
    genAlmostSortedFloat("test47.in", 10000, 100, 3, o);
    genRandomFloat("test48.in", 50000, -100000.0, 100000.0, 2, o);

    //char

    genRandomChar("test49.in", 100, 'a', 'z', o);
    genEqualChar("test50.in", 200, 'k', o);
    genSortedChar("test51.in", 300, o);
    genReverseChar("test52.in", 300, o);
    genAlmostSortedChar("test53.in", 1000, 50, o);
    genRandomChar("test54.in", 5000, 'A', 'Z', o);

    return 0;
}