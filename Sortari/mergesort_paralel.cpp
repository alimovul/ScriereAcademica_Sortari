/*****************************************
       Butoi Alexandru - Gabriel
         Universitatea de Vest
             din Timisoara
*****************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <string>
#include <cctype>
#include "benchmark_utils.h"

using namespace std;

const int MAX_N = 500000000;
const int MAX_DEPTH = 4; // maxim 16 thread-uri

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
void splitList(Node<T>* source, Node<T>*& front, Node<T>*& back) {
    if (!source || !source->next) {
        front = source;
        back = nullptr;
        return;
    }

    Node<T>* slow = source;
    Node<T>* fast = source->next;

    while (fast) {
        fast = fast->next;
        if (fast) {
            slow = slow->next;
            fast = fast->next;
        }
    }

    front = source;
    back = slow->next;
    slow->next = nullptr;
}

template <typename T>
Node<T>* mergeLists(Node<T>* a, Node<T>* b) {
    if (!a) return b;
    if (!b) return a;

    Node<T> dummy{T(), nullptr};
    Node<T>* tail = &dummy;

    while (a && b) {
        if (a->val <= b->val) {
            tail->next = a;
            a = a->next;
        } else {
            tail->next = b;
            b = b->next;
        }
        tail = tail->next;
    }

    if (a) tail->next = a;
    else tail->next = b;

    return dummy.next;
}

template <typename T>
void mergeSortList(Node<T>*& head) {
    if (!head || !head->next) return;

    Node<T>* a = nullptr;
    Node<T>* b = nullptr;

    splitList(head, a, b);

    mergeSortList(a);
    mergeSortList(b);

    head = mergeLists(a, b);
}

template <typename T>
void freeList(Node<T>* head) {
    while (head) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

template <typename T>
void mergeVector(vector<T>& v, int st, int mid, int dr) {
    vector<T> temp;
    temp.reserve(dr - st + 1);

    int i = st, j = mid + 1;

    while (i <= mid && j <= dr) {
        if (v[i] <= v[j]) temp.push_back(v[i++]);
        else temp.push_back(v[j++]);
    }

    while (i <= mid) temp.push_back(v[i++]);
    while (j <= dr) temp.push_back(v[j++]);

    for (int k = 0; k < (int)temp.size(); k++) {
        v[st + k] = temp[k];
    }
}

template <typename T>
void mergeSortParallelVector(vector<T>& v, int st, int dr, int depth = 0) {
    if (st >= dr) return;

    int mid = (st + dr) / 2;

    if (depth < MAX_DEPTH) {
        thread t1(mergeSortParallelVector<T>, ref(v), st, mid, depth + 1);
        thread t2(mergeSortParallelVector<T>, ref(v), mid + 1, dr, depth + 1);

        t1.join();
        t2.join();
    } else {
        mergeSortParallelVector<T>(v, st, mid, depth + 1);
        mergeSortParallelVector<T>(v, mid + 1, dr, depth + 1);
    }

    mergeVector(v, st, mid, dr);
}

template <typename T>
void mergeArray(T v[], int st, int mid, int dr) {
    int n1 = mid - st + 1;
    int n2 = dr - mid;

    T* L = new T[n1];
    T* R = new T[n2];

    for (int i = 0; i < n1; i++)
        L[i] = v[st + i];
    for (int i = 0; i < n2; i++)
        R[i] = v[mid + 1 + i];

    int i = 0, j = 0, k = st;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) v[k++] = L[i++];
        else v[k++] = R[j++];
    }

    while (i < n1) v[k++] = L[i++];
    while (j < n2) v[k++] = R[j++];

    delete[] L;
    delete[] R;
}

template <typename T>
void mergeSortParallelArray(T v[], int st, int dr, int depth = 0) {
    if (st >= dr) return;

    int mid = (st + dr) / 2;

    if (depth < MAX_DEPTH) {
        thread t1(mergeSortParallelArray<T>, v, st, mid, depth + 1);
        thread t2(mergeSortParallelArray<T>, v, mid + 1, dr, depth + 1);

        t1.join();
        t2.join();
    } else {
        mergeSortParallelArray<T>(v, st, mid, depth + 1);
        mergeSortParallelArray<T>(v, mid + 1, dr, depth + 1);
    }

    mergeArray(v, st, mid, dr);
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
    if (n > 0) mergeSortParallelVector<T>(v, 0, n - 1);
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

    T* v = new T[n];
    for (int i = 0; i < n; i++) {
        fin >> v[i];
    }

    logRun("array", inputFile, n);

    Timer timer;
    if (n > 0) mergeSortParallelArray<T>(v, 0, n - 1);
    duration_ns = timer.stop();

    for (int i = 0; i < n; i++) {
        fout << v[i] << ' ';
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
    mergeSortList<T>(head);
    duration_ns = timer.stop();

    Node<T>* p = head;
    while (p) {
        fout << p->val << ' ';
        p = p->next;
    }
    fout << "\n";

    freeList<T>(head);

    logDone("list", inputFile, duration_ns);
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