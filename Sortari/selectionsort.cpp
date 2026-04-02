/*****************************************
       Butoi Alexandru - Gabriel
         Universitatea de Vest
             din Timisoara
*****************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include "benchmark_utils.h"

using namespace std;

const int MAX_N = 1000000;

enum DataType {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_UNKNOWN
};

bool isIntegerToken(const string& s) {
    if (s.empty()) return false;

    int start = 0;
    if (s[0] == '-' || s[0] == '+') start = 1;
    if (start == (int)s.size()) return false;

    for (int i = start; i < (int)s.size(); i++) {
        if (!isdigit((unsigned char)s[i])) return false;
    }
    return true;
}

bool isFloatToken(const string& s) {
    if (s.empty()) return false;

    bool hasDigit = false;
    bool hasDot = false;
    bool hasExp = false;

    int start = 0;
    if (s[0] == '-' || s[0] == '+') start = 1;

    for (int i = start; i < (int)s.size(); i++) {
        if (isdigit((unsigned char)s[i])) hasDigit = true;
        else if (s[i] == '.') hasDot = true;
        else if (s[i] == 'e' || s[i] == 'E') hasExp = true;
        else if ((s[i] == '+' || s[i] == '-') &&
                 i > 0 &&
                 (s[i - 1] == 'e' || s[i - 1] == 'E')) {}
        else return false;
    }

    return hasDigit && (hasDot || hasExp);
}

bool isCharToken(const string& s) {
    if (s.size() != 1) return false;

    char c = s[0];
    if (isdigit((unsigned char)c)) return false;
    if (c == '+' || c == '-' || c == '.') return false;

    return true;
}

DataType detectDataType(const string& inputFile) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return TYPE_UNKNOWN;

    int n;
    fin >> n;

    string firstToken;
    if (!(fin >> firstToken)) return TYPE_UNKNOWN;

    if (isCharToken(firstToken)) return TYPE_CHAR;
    if (isFloatToken(firstToken)) return TYPE_FLOAT;
    if (isIntegerToken(firstToken)) return TYPE_INT;

    return TYPE_UNKNOWN;
}

bool readN(const string& inputFile, int& n) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return false;
    fin >> n;
    return !fin.fail();
}

template <typename T>
struct Node {
    T val;
    Node* next;
};

template <typename T>
void append(Node<T>*& head, Node<T>*& tail, const T& x) {
    Node<T>* nou = new Node<T>{x, nullptr};
    if (!head) {
        head = tail = nou;
        return;
    }
    tail->next = nou;
    tail = nou;
}

template <typename T>
void freeList(Node<T>* head) {
    while (head) {
        Node<T>* tmp = head;
        head = head->next;
        delete tmp;
    }
}

template <typename T>
void selectionSortVector(vector<T>& v) {
    int n = v.size();
    for (int i = 0; i < n - 1; i++) {
        int minPos = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minPos]) {
                minPos = j;
            }
        }
        swap(v[i], v[minPos]);
    }
}

template <typename T>
void selectionSortArray(T v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minPos = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[minPos]) {
                minPos = j;
            }
        }
        swap(v[i], v[minPos]);
    }
}

template <typename T>
void selectionSortList(Node<T>* head) {
    for (Node<T>* i = head; i; i = i->next) {
        Node<T>* minNode = i;
        for (Node<T>* j = i->next; j; j = j->next) {
            if (j->val < minNode->val) {
                minNode = j;
            }
        }
        swap(i->val, minNode->val);
    }
}

template <typename T>
bool runTestVector(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return false;

    ofstream fout(outputFile);

    int n;
    fin >> n;

    vector<T> v(n);
    for (int i = 0; i < n; i++) fin >> v[i];

    logRun("vector", inputFile, n);

    Timer timer;
    selectionSortVector(v);
    duration_ns = timer.stop();

    for (auto& x : v) fout << x << " ";
    fout << "\n";

    logDone("vector", inputFile, duration_ns);
    return true;
}

template <typename T>
bool runTestArray(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return false;

    ofstream fout(outputFile);

    int n;
    fin >> n;

    T* v = new T[n];
    for (int i = 0; i < n; i++) fin >> v[i];

    logRun("array", inputFile, n);

    Timer timer;
    selectionSortArray(v, n);
    duration_ns = timer.stop();

    for (int i = 0; i < n; i++) fout << v[i] << " ";
    fout << "\n";

    delete[] v;

    logDone("array", inputFile, duration_ns);
    return true;
}

template <typename T>
bool runTestList(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return false;

    ofstream fout(outputFile);

    int n;
    fin >> n;

    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;

    for (int i = 0; i < n; i++) {
        T x;
        fin >> x;
        append(head, tail, x);
    }

    logRun("list", inputFile, n);

    Timer timer;
    selectionSortList(head);
    duration_ns = timer.stop();

    for (Node<T>* p = head; p; p = p->next) {
        fout << p->val << " ";
    }
    fout << "\n";

    freeList(head);

    logDone("list", inputFile, duration_ns);
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int testsRun = 0;

    for (int i = 1;; i++) {
        string inF = "../tests/test" + to_string(i) + ".in";

        ifstream check(inF);
        if (!check.is_open()) break;

        int n;
        if (!readN(inF, n)) continue;

        if (n > MAX_N) {
            cerr << "[skip] " << inF << " n=" << n << "\n\n";
            continue;
        }

        DataType type = detectDataType(inF);
        if (type == TYPE_UNKNOWN) continue;

        long long t1, t2, t3;

        string base = "../tests/test" + to_string(i);

        if (type == TYPE_INT) {
            runTestVector<int>(inF, base + "_vector.out", t1);
            runTestArray<int>(inF, base + "_array.out", t2);
            runTestList<int>(inF, base + "_list.out", t3);
        } else if (type == TYPE_FLOAT) {
            runTestVector<float>(inF, base + "_vector.out", t1);
            runTestArray<float>(inF, base + "_array.out", t2);
            runTestList<float>(inF, base + "_list.out", t3);
        } else if (type == TYPE_CHAR) {
            runTestVector<char>(inF, base + "_vector.out", t1);
            runTestArray<char>(inF, base + "_array.out", t2);
            runTestList<char>(inF, base + "_list.out", t3);
        }

        testsRun++;
        cerr << "\n";
    }

    cerr << "Teste rulate: " << testsRun << "\n";
    return 0;
}