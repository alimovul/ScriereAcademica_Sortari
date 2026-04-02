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
#include <cstdlib>
#include "benchmark_utils.h"

using namespace std;

const int MAX_N = 100000000;

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
Node<T>* getTail(Node<T>* cur) {
    while (cur && cur->next) {
        cur = cur->next;
    }
    return cur;
}

template <typename T>
Node<T>* partitionList(Node<T>* head, Node<T>* end, Node<T>*& newHead, Node<T>*& newEnd) {
    Node<T>* pivot = end;
    Node<T>* prev = nullptr;
    Node<T>* cur = head;
    Node<T>* tail = pivot;

    newHead = nullptr;
    newEnd = pivot;

    while (cur != pivot) {
        if (cur->val < pivot->val) {
            if (!newHead) {
                newHead = cur;
            }
            prev = cur;
            cur = cur->next;
        } else {
            Node<T>* tmp = cur->next;

            if (prev) {
                prev->next = cur->next;
            } else {
                head = cur->next;
            }

            cur->next = nullptr;
            tail->next = cur;
            tail = cur;

            cur = tmp;
        }
    }

    if (!newHead) {
        newHead = pivot;
    }

    newEnd = tail;
    return pivot;
}

template <typename T>
Node<T>* quickSortListRecur(Node<T>* head, Node<T>* end) {
    if (!head || head == end) {
        return head;
    }

    Node<T>* newHead = nullptr;
    Node<T>* newEnd = nullptr;

    Node<T>* pivot = partitionList(head, end, newHead, newEnd);

    if (newHead != pivot) {
        Node<T>* tmp = newHead;
        while (tmp->next != pivot) {
            tmp = tmp->next;
        }

        tmp->next = nullptr;

        newHead = quickSortListRecur(newHead, tmp);

        Node<T>* tail = getTail(newHead);
        tail->next = pivot;
    }

    pivot->next = quickSortListRecur(pivot->next, newEnd);

    return newHead;
}

template <typename T>
void quickSortList(Node<T>*& head) {
    if (!head) return;
    head = quickSortListRecur(head, getTail(head));
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
int partitionVector(vector<T>& v, int st, int dr) {
    T pivot = v[st + rand() % (dr - st + 1)];
    int i = st, j = dr;

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(v[i], v[j]);
            i++;
            j--;
        }
    }

    return i;
}

template <typename T>
void quickSortVector(vector<T>& v, int st, int dr) {
    if (st >= dr) return;

    int index = partitionVector(v, st, dr);

    if (st < index - 1) {
        quickSortVector(v, st, index - 1);
    }
    if (index < dr) {
        quickSortVector(v, index, dr);
    }
}

template <typename T>
int partitionArray(T v[], int st, int dr) {
    T pivot = v[st + rand() % (dr - st + 1)];
    int i = st, j = dr;

    while (i <= j) {
        while (v[i] < pivot) i++;
        while (v[j] > pivot) j--;

        if (i <= j) {
            swap(v[i], v[j]);
            i++;
            j--;
        }
    }

    return i;
}

template <typename T>
void quickSortArray(T v[], int st, int dr) {
    if (st >= dr) return;

    int index = partitionArray(v, st, dr);

    if (st < index - 1) {
        quickSortArray(v, st, index - 1);
    }
    if (index < dr) {
        quickSortArray(v, index, dr);
    }
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
        cerr << "Nu pot crea fisierul: " << outputFile << "\n";
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
    if (n > 0) quickSortVector(v, 0, n - 1);
    duration_ns = timer.stop();

    for (int i = 0; i < n; i++) {
        fout << v[i] << " ";
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
        cerr << "Nu pot crea fisierul: " << outputFile << "\n";
        return false;
    }

    int n;
    fin >> n;

    T* v = new T[n];
    for (int i = 0; i < n; i++) {
        fin >> v[i];
    }

    logRun("array", inputFile, n);

    Timer timer;
    if (n > 0) quickSortArray(v, 0, n - 1);
    duration_ns = timer.stop();

    for (int i = 0; i < n; i++) {
        fout << v[i] << " ";
    }
    fout << "\n";

    delete[] v;

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
        cerr << "Nu pot crea fisierul: " << outputFile << "\n";
        return false;
    }

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
    if (head) quickSortList(head);
    duration_ns = timer.stop();

    Node<T>* p = head;
    while (p) {
        fout << p->val << " ";
        p = p->next;
    }
    fout << "\n";

    freeList(head);

    logDone("list", inputFile, duration_ns);
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);

    srand(1337);

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

        ifstream check(inF);
        if (!check.is_open()) {
            break;
        }
        check.close();

        int n;
        if (!readN(inF, n)) {
            cerr << "[skip   ] " << inF << " | nu pot citi n\n\n";
            testsSkipped++;
            continue;
        }

        if (n > MAX_N) {
            cerr << "[skip   ] " << inF << " | n = " << n << " > " << MAX_N << "\n\n";
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
        bool ok1 = false, ok2 = false, ok3 = false;

        string outVector = "../tests/test" + to_string(i) + "_vector.out";
        string outArray  = "../tests/test" + to_string(i) + "_array.out";
        string outList   = "../tests/test" + to_string(i) + "_list.out";

        if (type == TYPE_INT) {
            cerr << "[type   ] " << inF << " -> int\n";
            ok1 = runTestVector<int>(inF, outVector, tVector);
            ok2 = runTestArray<int>(inF, outArray, tArray);
            ok3 = runTestList<int>(inF, outList, tList);
        } else if (type == TYPE_FLOAT) {
            cerr << "[type   ] " << inF << " -> float\n";
            ok1 = runTestVector<float>(inF, outVector, tVector);
            ok2 = runTestArray<float>(inF, outArray, tArray);
            ok3 = runTestList<float>(inF, outList, tList);
        } else if (type == TYPE_CHAR) {
            cerr << "[type   ] " << inF << " -> char\n";
            ok1 = runTestVector<char>(inF, outVector, tVector);
            ok2 = runTestArray<char>(inF, outArray, tArray);
            ok3 = runTestList<char>(inF, outList, tList);
        }

        if (!ok1 || !ok2 || !ok3) {
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
         << "  ~" << fixed << setprecision(9)
         << (totalVector / 1000000000.0) << " sec\n";

    cerr << "array  total: " << totalArray << " ns"
         << "  ~" << fixed << setprecision(9)
         << (totalArray / 1000000000.0) << " sec\n";

    cerr << "list   total: " << totalList << " ns"
         << "  ~" << fixed << setprecision(9)
         << (totalList / 1000000000.0) << " sec\n";

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