
#include "radix.h"
#include "suffix.h"

#include <stdlib.h>
#include <string.h>

static int char_at_suffix(Suffix *suf, int d) {
    int len = str_len(suffix_get(suf));
    int idx = suffix_index(suf);
    if (d >= len - idx) return -1; // -1 para fim de string
    return (unsigned char)str_char_at(suffix_get(suf), idx + d);
}

static void exch(Suffix **a, int i, int j) {
    Suffix *tmp = a[i];
    a[i] = a[j];
    a[j] = tmp;
}

static int less_from(Suffix *a, Suffix *b, int d) {
    int lena = str_len(suffix_get(a)) - suffix_index(a);
    int lenb = str_len(suffix_get(b)) - suffix_index(b);
    int min = lena < lenb ? lena : lenb;
    for (int i = d; i < min; i++) {
        char ca = str_char_at(suffix_get(a), suffix_index(a) + i);
        char cb = str_char_at(suffix_get(b), suffix_index(b) + i);
        if (ca < cb) return 1;
        if (ca > cb) return 0;
    }
    return lena < lenb;
}

static void insert_sort_from(Suffix **a, int lo, int hi, int d) {
    for (int i = lo; i <= hi; i++) {
        for (int j = i; j > lo && less_from(a[j], a[j-1], d); j--) {
            exch(a, j, j-1);
        }
    }
}

static int* create_count_array() {
    return (int*)calloc(R+2, sizeof(int));
}

static void clear_count_array(int *count) {
    memset(count, 0, (R+2)*sizeof(int));
}

static void count_sort(Suffix **a, Suffix **aux, int *count, int lo, int hi, int d) {
    clear_count_array(count);
    for (int i = lo; i <= hi; i++) {
        count[char_at_suffix(a[i], d) + 2]++;
    }
    for (int r = 0; r < R+1; r++) {
        count[r+1] += count[r];
    }
    for (int i = lo; i <= hi; i++) {
        int p = count[char_at_suffix(a[i], d) + 1]++;
        aux[p] = a[i];
    }
    for (int i = lo; i <= hi; i++) {
        a[i] = aux[i - lo];
    }
}

static void rec_MSD(Suffix **a, Suffix **aux, int lo, int hi, int d) {
    if (hi <= lo + CUTOFF - 1) {
        insert_sort_from(a, lo, hi, d);
        return;
    }
    int *count = create_count_array(R);
    count_sort(a, aux, count, lo, hi, d);
    for (int r = 0; r < R; r++) {
        rec_MSD(a, aux, lo + count[r], lo + count[r+1] - 1, d+1);
    }
    free(count);
}

void radix_sort_suffixes(Suffix **a, int N) {
    Suffix **aux = (Suffix **)malloc(N * sizeof(Suffix*));
    rec_MSD(a, aux, 0, N-1, 0);
    free(aux);
}
