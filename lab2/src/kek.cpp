#include <bits/stdc++.h>

struct ThreadData {
    std::vector<int>& arr;
    int start;
    int end;
    int thread_id;
    ThreadData(std::vector<int>& a, int s, int e, int id)
        : arr(a), start(s), end(e), thread_id(id) {}
};

std::mt19937 rnd(123);

void unshuffle(std::vector<int>& a, int l, int r) {
    int half = (l + r) / 2;
    std::vector<int> tmp(a.size());

    int cnt = l;
    for (int i = l; i < r; i += 2) {
        tmp[cnt] = a[i];
        cnt++;
    }
    for (int i = l + 1; i < r; i += 2) {
        tmp[cnt] = a[i];
        cnt++;
    }

    for (auto i = 0; i < tmp.size(); i++){
        if (l <= i && i < r) {
            a[i] = tmp[i];
        }
    }
}

void Merge(std::vector<int>& v, int l, int r, int mid) {
    std::vector<int> tmp(v.size());
    
    int i = l, j = mid;
    int cnt = l;
    for (int it = l; it < r; ++it) {
        if (i < mid && (j >= r || v[i] < v[j])) {
            tmp[it] = v[i];
            i++;
        } else {
            tmp[it] = v[j];
            j++;
        }
    }

    for (int i = l; i < r; ++i) {
        v[i] = tmp[i];
    }
}

void OddEvenMergeSort(std::vector<int>& a, int l, int r) {
    if (r == l + 2) {
        if (a[l] > a[r - 1]) std::swap(a[l], a[r - 1]);
        return;
    }
    if (r <= l + 1) {
        return;
    }
    
    unshuffle(a, l, r);
    
    int half = (l + r) / 2;
    OddEvenMergeSort(a, l, half);
    OddEvenMergeSort(a, half, r);

    Merge(a, l, r, half);
}

void* sort_thread(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    OddEvenMergeSort(data->arr, data->start, data->end);
    pthread_exit(nullptr);
}

void parallel_odd_even_sort(std::vector<int>& arr, int num_threads = 1) {
    size_t n = arr.size();
    if (n <= 4 || num_threads == 1) {
        OddEvenMergeSort(arr, 0, n);
        return;
    }
    
    std::vector<pthread_t> threads(num_threads);
    std::vector<ThreadData*> thread_data(num_threads);
    
    int elements_per_thread = n / num_threads;
    int remaining_elements = n % num_threads;
    int current_start = 0;
    
    for (int i = 0; i < num_threads; i++) {
        int extra = (i < remaining_elements) ? 1 : 0;
        int end_pos = current_start + elements_per_thread + extra;
        thread_data[i] = new ThreadData(arr, current_start, end_pos, i);
        current_start = end_pos;
    }
    
    for (int i = 0; i < num_threads; i++) {
        int res = pthread_create(&threads[i], nullptr, sort_thread, thread_data[i]);
        if (res) {
            std::cerr << "Ошибка создания потока " << i << std::endl;
            for (int j = 0; j <= i; j++) { delete thread_data[j]; }
            OddEvenMergeSort(arr, 0, n);
            return;
        }
    }
    
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], nullptr);
    }

    int step = 1;
    while (step < num_threads) {
        for (int i = 0; i < num_threads; i += 2 * step) {
            if (i + step * 2 - 1 < num_threads) {
                int start = thread_data[i]->start;
                int end = thread_data[i + step * 2 - 1]->end;
                int mid = (start + end) / 2;
                Merge(arr, start, end, mid);
            }
        }
        step *= 2;
    }
    
    for (int i = 0; i < num_threads; i++) {
        delete thread_data[i];
    }
}

std::vector<int> makerndV(int n) {
    std::vector<int> v(n);
    for (int i = 0; i < n; ++i) {
        v[i] = rnd() % (int)1e8;
    }
    return v;
}

int main() {
    int n, threads;
    std::cin >> n;
    std::cin >> threads;

    auto v = makerndV(n);
    auto v_ = v;
    sort(v_.begin(), v_.end());

    auto start_parallel = std::chrono::high_resolution_clock::now();
    parallel_odd_even_sort(v, threads);
    auto end_parallel = std::chrono::high_resolution_clock::now();

    auto duration_parallel = std::chrono::duration_cast<std::chrono::microseconds>(end_parallel - start_parallel);


    if (v_ != v) return 1;

    std::cout << duration_parallel.count();
    return 0;
}
