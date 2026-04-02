/*****************************************
       Butoi Alexandru - Gabriel
         Universitatea de Vest
             din Timisoara
*****************************************/

#include <vector>
#include <chrono>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;
using namespace chrono;

void logResult(const string& type, const string& inputFile, long long duration_ns) {
    cerr << "[" << type << "] " << inputFile << " -> " << duration_ns
         << " ns"
         << "  ~" << fixed << setprecision(9) << (duration_ns / 1000000000.0)
         << " sec\n";
}

template <typename T>
void insertionSortVector(vector<T>& v, int st, int dr) {
    for (int i = st + 1; i <= dr; i++) {
        T key = v[i];
        int j = i - 1;
        while (j >= st && v[j] > key) {
            v[j + 1] = v[j];
            j--;
        }
        v[j + 1] = key;
    }
}

template <typename T>
void insertionSortArray(T a[], int st, int dr) {
    for (int i = st + 1; i <= dr; i++) {
        T key = a[i];
        int j = i - 1;
        while (j >= st && a[j] > key) {
            a[j + 1] = a[j];
            j--;
        }
        a[j + 1] = key;
    }
}

template <typename T>
int medianOfThreeVector(vector<T>& v, int a, int b, int c) {
    if (v[a] > v[b]) swap(v[a], v[b]);
    if (v[a] > v[c]) swap(v[a], v[c]);
    if (v[b] > v[c]) swap(v[b], v[c]);
    return b;
}

template <typename T>
int medianOfThreeArray(T v[], int a, int b, int c) {
    if (v[a] > v[b]) swap(v[a], v[b]);
    if (v[a] > v[c]) swap(v[a], v[c]);
    if (v[b] > v[c]) swap(v[b], v[c]);
    return b;
}

template <typename T>
int partitionVector(vector<T>& v, int st, int dr) {
    int mid = st + (dr - st) / 2;
    int pivIdx = medianOfThreeVector(v, st, mid, dr);
    T pivot = v[pivIdx];
    swap(v[pivIdx], v[dr]);

    int i = st - 1;
    for (int j = st; j < dr; j++) {
        if (v[j] <= pivot) {
            i++;
            swap(v[i], v[j]);
        }
    }

    swap(v[i + 1], v[dr]);
    return i + 1;
}

template <typename T>
int partitionArray(T v[], int st, int dr) {
    int mid = st + (dr - st) / 2;
    int pivIdx = medianOfThreeArray(v, st, mid, dr);
    T pivot = v[pivIdx];
    swap(v[pivIdx], v[dr]);

    int i = st - 1;
    for (int j = st; j < dr; j++) {
        if (v[j] <= pivot) {
            i++;
            swap(v[i], v[j]);
        }
    }

    swap(v[i + 1], v[dr]);
    return i + 1;
}

template <typename T>
void heapifyVectorRange(vector<T>& v, int base, int heapSize, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && v[base + left] > v[base + largest]) largest = left;
        if (right < heapSize && v[base + right] > v[base + largest]) largest = right;

        if (largest == i) break;

        swap(v[base + i], v[base + largest]);
        i = largest;
    }
}

template <typename T>
void heapSortVectorRange(vector<T>& v, int st, int dr) {
    int n = dr - st + 1;

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyVectorRange(v, st, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(v[st], v[st + i]);
        heapifyVectorRange(v, st, i, 0);
    }
}

template <typename T>
void heapifyArrayRange(T v[], int base, int heapSize, int i) {
    while (true) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < heapSize && v[base + left] > v[base + largest]) largest = left;
        if (right < heapSize && v[base + right] > v[base + largest]) largest = right;

        if (largest == i) break;

        swap(v[base + i], v[base + largest]);
        i = largest;
    }
}

template <typename T>
void heapSortArrayRange(T v[], int st, int dr) {
    int n = dr - st + 1;

    for (int i = n / 2 - 1; i >= 0; i--) {
        heapifyArrayRange(v, st, n, i);
    }

    for (int i = n - 1; i > 0; i--) {
        swap(v[st], v[st + i]);
        heapifyArrayRange(v, st, i, 0);
    }
}

template <typename T>
void introSortVectorImpl(vector<T>& v, int st, int dr, int depthLimit) {
    while (dr - st > 16) {
        if (depthLimit == 0) {
            heapSortVectorRange(v, st, dr);
            return;
        }

        depthLimit--;
        int p = partitionVector(v, st, dr);

        if (p - st < dr - p) {
            introSortVectorImpl(v, st, p - 1, depthLimit);
            st = p + 1;
        } else {
            introSortVectorImpl(v, p + 1, dr, depthLimit);
            dr = p - 1;
        }
    }

    if (st < dr) {
        insertionSortVector(v, st, dr);
    }
}

template <typename T>
void introSortVector(vector<T>& v) {
    if (v.empty()) return;
    int depthLimit = 2 * (int)log2(v.size());
    introSortVectorImpl(v, 0, (int)v.size() - 1, depthLimit);
}

template <typename T>
void introSortArrayImpl(T a[], int st, int dr, int depthLimit) {
    while (dr - st > 16) {
        if (depthLimit == 0) {
            heapSortArrayRange(a, st, dr);
            return;
        }

        depthLimit--;
        int p = partitionArray(a, st, dr);

        if (p - st < dr - p) {
            introSortArrayImpl(a, st, p - 1, depthLimit);
            st = p + 1;
        } else {
            introSortArrayImpl(a, p + 1, dr, depthLimit);
            dr = p - 1;
        }
    }

    if (st < dr) {
        insertionSortArray(a, st, dr);
    }
}

template <typename T>
void introSortArray(T a[], int n) {
    if (n <= 0) return;
    int depthLimit = 2 * (int)log2(n);
    introSortArrayImpl(a, 0, n - 1, depthLimit);
}

template <typename T>
bool runTestVector(const string& inputFile, const string& outputFile, long long& duration_ns) {
    ifstream fin(inputFile);
    if (!fin.is_open()) {
        cerr << "Lipseste testul: " << inputFile << "\n";
        return false;
    }

    ofstream fout(outputFile);

    int n;
    fin >> n;

    vector<T> v(n);
    for (int i = 0; i < n; i++)
        fin >> v[i];

    auto start = high_resolution_clock::now();

    introSortVector(v);

    auto stop = high_resolution_clock::now();
    duration_ns = duration_cast<nanoseconds>(stop - start).count();

    for (const T& x : v)
        fout << x << ' ';
    fout << "\n";

    logResult("vector", inputFile, duration_ns);
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

    int n;
    fin >> n;

    T* a = new T[n];
    for (int i = 0; i < n; i++)
        fin >> a[i];

    auto start = high_resolution_clock::now();

    introSortArray(a, n);

    auto stop = high_resolution_clock::now();
    duration_ns = duration_cast<nanoseconds>(stop - start).count();

    for (int i = 0; i < n; i++)
        fout << a[i] << ' ';
    fout << "\n";

    delete[] a;

    logResult("array ", inputFile, duration_ns);
    return true;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    long long totalVector = 0;
    long long totalArray = 0;

    int winsVector = 0;
    int winsArray = 0;

    int tests = 0;

    for (int i = 1;; i++) {
        string inF = "../tests/test" + to_string(i) + ".in";
        long long tVector, tArray;

        bool ok = runTestVector<float>(inF, "../tests/test" + to_string(i) + "_vector.out", tVector);
        if (!ok) break;

        runTestArray<float>(inF, "../tests/test" + to_string(i) + "_array.out", tArray);

        totalVector += tVector;
        totalArray += tArray;
        tests++;

        long long best = min(tVector, tArray);
        if (tVector == best) winsVector++;
        if (tArray == best) winsArray++;
        cerr << "\n";
    }

    if (tests == 0) {
        cerr << "Nu a fost rulat niciun test.\n";
        return 0;
    }

    cerr << "Rezumat final:\n";
    cerr << "vector total: " << totalVector << " ns"
         << "  ~" << fixed << setprecision(9) << (totalVector / 1000000000.0) << " sec\n";
    cerr << "array  total: " << totalArray << " ns"
         << "  ~" << fixed << setprecision(9) << (totalArray / 1000000000.0) << " sec\n";

    cerr << "\nTeste castigate ca timp:\n";
    cerr << "vector: " << winsVector << "\n";
    cerr << "array : " << winsArray << "\n";

    cerr << "\nCel mai rapid per general: ";
    long long bestTotal = min(totalVector, totalArray);
    if (bestTotal == totalVector) cerr << "vector";
    else cerr << "array";
    cerr << "\n";

    cerr << "Cel mai multe teste castigate: ";
    int bestWins = max(winsVector, winsArray);
    bool first = true;
    if (winsVector == bestWins) {
        cerr << "vector";
        first = false;
    }
    if (winsArray == bestWins) {
        if (!first) cerr << ", ";
        cerr << "array";
    }
    cerr << "\n";

    return 0;
}