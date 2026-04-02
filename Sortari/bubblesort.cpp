/*****************************************
       Butoi Alexandru - Gabriel
         Universitatea de Vest
             din Timisoara
*****************************************/

/// Bubble Sort benchmark:
/// - vector
/// - array
/// - lista simplu inlantuita
///
/// Detecteaza automat tipul:
/// - int
/// - float
/// - char
///
/// Daca un test are mai mult de 20000 elemente, trece peste.

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include "benchmark_utils.h"

using namespace std;

const int MAX_BUBBLE_N = 100000;

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
        if (isdigit((unsigned char)s[i])) {
            hasDigit = true;
        } else if (s[i] == '.') {
            hasDot = true;
        } else if (s[i] == 'e' || s[i] == 'E') {
            hasExp = true;
        } else if ((s[i] == '+' || s[i] == '-') &&
                   i > 0 &&
                   (s[i - 1] == 'e' || s[i - 1] == 'E')) {
            // permis in exponent
        } else {
            return false;
        }
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

bool getTestSize(const string& inputFile, int& n) {
    ifstream fin(inputFile);
    if (!fin.is_open()) return false;
    return static_cast<bool>(fin >> n);
}

template <typename T>
struct Node {
    T val;
    Node* next;
};

template <typename T>
void pushBack(Node<T>*& head, Node<T>*& tail, const T& x) {
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
void bubbleSortVector(vector<T>& v) {
    int n = (int)v.size();
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (v[j] > v[j + 1]) {
                swap(v[j], v[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template <typename T>
void bubbleSortArray(T a[], int n) {
    bool swapped;

    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (a[j] > a[j + 1]) {
                swap(a[j], a[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

template <typename T>
void bubbleSortList(Node<T>* head) {
    if (!head) return;

    bool swapped;
    Node<T>* last = nullptr;

    do {
        swapped = false;
        Node<T>* p = head;

        while (p->next != last) {
            if (p->val > p->next->val) {
                swap(p->val, p->next->val);
                swapped = true;
            }
            p = p->next;
        }

        last = p;
    } while (swapped);
}
template <typename T>
bool runTestVector(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Lipseste testul: " << inputFile << "\n";
        return false;
    }

    ofstream fout(outputFile);
    if (!fout.is_open()) {
        cerr << "Nu pot crea output-ul: " << outputFile << "\n";
        return false;
    }

    int n;
    fin >> n;

    vector<T> v(n);
    for (int i = 0; i < n; i++) {
        fin >> v[i];
    }

    logRun("vector", inputFile, n);

    Timer timer;
    bubbleSortVector(v);
    duration_ns = timer.stop();

    for (const T& x : v) {
        fout << x << ' ';
    }
    fout << "\n";

    logDone("vector", inputFile, duration_ns);
    return true;
}

template <typename T>
bool runTestArray(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Lipseste testul: " << inputFile << "\n";
        return false;
    }

    ofstream fout(outputFile);
    if (!fout.is_open()) {
        cerr << "Nu pot crea output-ul: " << outputFile << "\n";
        return false;
    }

    int n;
    fin >> n;

    T* a = new T[n];
    for (int i = 0; i < n; i++) {
        fin >> a[i];
    }

    logRun("array", inputFile, n);

    Timer timer;
    bubbleSortArray(a, n);
    duration_ns = timer.stop();

    for (int i = 0; i < n; i++) {
        fout << a[i] << ' ';
    }
    fout << "\n";

    delete[] a;

    logDone("array", inputFile, duration_ns);
    return true;
}

template <typename T>
bool runTestList(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Lipseste testul: " << inputFile << "\n";
        return false;
    }

    ofstream fout(outputFile);
    if (!fout.is_open()) {
        cerr << "Nu pot crea output-ul: " << outputFile << "\n";
        return false;
    }

    int n;
    fin >> n;

    Node<T>* head = nullptr;
    Node<T>* tail = nullptr;

    for (int i = 0; i < n; i++) {
        T x;
        fin >> x;
        pushBack(head, tail, x);
    }

    logRun("list", inputFile, n);

    Timer timer;
    bubbleSortList(head);
    duration_ns = timer.stop();

    Node<T>* p = head;
    while (p) {
        fout << p->val << ' ';
        p = p->next;
    }
    fout << "\n";

    freeList(head);

    logDone("list", inputFile, duration_ns);
    return true;
}
template <typename T>
bool runAllForType(const string& inputFile,
                   const string& outVector,
                   const string& outArray,
                   const string& outList,
                   long long& tVector,
                   long long& tArray,
                   long long& tList) {
    bool ok = runTestVector<T>(inputFile, outVector, tVector);
    if (!ok) return false;

    ok = runTestArray<T>(inputFile, outArray, tArray);
    if (!ok) return false;

    ok = runTestList<T>(inputFile, outList, tList);
    if (!ok) return false;

    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long totalVector = 0;
    long long totalArray = 0;
    long long totalList = 0;

    int winsVector = 0;
    int winsArray = 0;
    int winsList = 0;

    int testsRun = 0;
    int testsSkipped = 0;

    for (int i = 1;; i++) {
        string inF = "../tests/test" + to_string(i) + ".in";

        ifstream testFile(inF);
        if (!testFile.is_open()) {
            break; // primul test inexistent => stop
        }
        testFile.close();

        int n;
        if (!getTestSize(inF, n)) {
            cerr << "[skip   ] " << inF << " | nu pot citi n\n\n";
            testsSkipped++;
            continue;
        }

        if (n > MAX_BUBBLE_N) {
            cerr << "[skip   ] " << inF << " | n = " << n
                 << " > " << MAX_BUBBLE_N
                 << " (prea mare pentru Bubble Sort)\n\n";
            testsSkipped++;
            continue;
        }

        DataType type = detectDataType(inF);
        if (type == TYPE_UNKNOWN) {
            cerr << "[skip   ] " << inF << " | tip necunoscut\n\n";
            testsSkipped++;
            continue;
        }

        long long tVector = 0, tArray = 0, tList = 0;

        string outVector = "../tests/test" + to_string(i) + "_vector.out";
        string outArray  = "../tests/test" + to_string(i) + "_array.out";
        string outList   = "../tests/test" + to_string(i) + "_list.out";

        bool ok = false;

        if (type == TYPE_INT) {
            cerr << "[type   ] " << inF << " -> int\n";
            ok = runAllForType<int>(inF, outVector, outArray, outList, tVector, tArray, tList);
        } else if (type == TYPE_FLOAT) {
            cerr << "[type   ] " << inF << " -> float\n";
            ok = runAllForType<float>(inF, outVector, outArray, outList, tVector, tArray, tList);
        } else if (type == TYPE_CHAR) {
            cerr << "[type   ] " << inF << " -> char\n";
            ok = runAllForType<char>(inF, outVector, outArray, outList, tVector, tArray, tList);
        }

        if (!ok) {
            cerr << "[skip   ] " << inF << " | eroare la rulare\n\n";
            testsSkipped++;
            continue;
        }

        totalVector += tVector;
        totalArray += tArray;
        totalList += tList;
        testsRun++;

        long long best = min(tVector, min(tArray, tList));
        if (tVector == best) winsVector++;
        if (tArray == best) winsArray++;
        if (tList == best) winsList++;

        cerr << "\n";
    }

    if (testsRun == 0) {
        cerr << "Nu a fost rulat niciun test.\n";
        return 0;
    }

    cerr << "Rezumat final:\n";
    cerr << "Teste rulate : " << testsRun << "\n";
    cerr << "Teste sarite : " << testsSkipped << "\n\n";

    cerr << "vector total: " << totalVector << " ns"
         << "  ~" << fixed << setprecision(9) << (totalVector / 1000000000.0) << " sec\n";
    cerr << "array  total: " << totalArray << " ns"
         << "  ~" << fixed << setprecision(9) << (totalArray / 1000000000.0) << " sec\n";
    cerr << "list   total: " << totalList << " ns"
         << "  ~" << fixed << setprecision(9) << (totalList / 1000000000.0) << " sec\n";

    cerr << "\nTeste castigate ca timp:\n";
    cerr << "vector: " << winsVector << "\n";
    cerr << "array : " << winsArray << "\n";
    cerr << "list  : " << winsList << "\n";

    cerr << "\nCel mai rapid per general: ";
    long long bestTotal = min(totalVector, min(totalArray, totalList));
    if (bestTotal == totalVector) cerr << "vector";
    else if (bestTotal == totalArray) cerr << "array";
    else cerr << "list";
    cerr << "\n";

    cerr << "Cel mai multe teste castigate: ";
    int bestWins = max(winsVector, max(winsArray, winsList));
    bool first = true;

    if (winsVector == bestWins) {
        cerr << "vector";
        first = false;
    }
    if (winsArray == bestWins) {
        if (!first) cerr << ", ";
        cerr << "array";
        first = false;
    }
    if (winsList == bestWins) {
        if (!first) cerr << ", ";
        cerr << "list";
    }
    cerr << "\n";

    return 0;
}