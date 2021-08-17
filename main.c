
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Interval Interval;

struct Interval {
    int start;
    int end;
};

Interval *intervals;
int intervalc = 0;

void    load_intervals(char*);
void    print_intervals();
void    merge_intervals();
int     max(int, int);
void    quicksort(int, int);
int     partition(int, int);
void    swap(int, int);
void    *xmalloc(size_t);



int main(int argc, char **argv) {
    char *filename = (argc > 1) ? argv[1] : "input.txt";
    load_intervals(filename);
    printf("loaded %d intervals\n", intervalc);
    print_intervals();
    printf("sorting intervals\n");
    quicksort(0, intervalc - 1);
    print_intervals();
    printf("merging intervals\n");
    merge_intervals();
    print_intervals();
    printf("done\n");
}



void load_intervals(char *filename) {
    FILE *fp = fopen(filename, "r");
    if(!fp) {
        printf("failed to open file %s\n", filename);
        exit(1);
    }
    int n = -1;
    intervalc = 0;
    fscanf(fp, "%d", &n);
    intervals = (Interval*) xmalloc(n * sizeof(Interval));
    while(intervalc < n) {
        Interval in;
        if( fscanf(fp, "%d, %d", &in.start, &in.end) != 2 ) {
            printf("failed to read interval %d\n", ++intervalc);
            fclose(fp);
            exit(1);
        }
        intervals[intervalc] = in;
        intervalc++;
    }
    fclose(fp);
}

void print_intervals() {
    printf("\n");
    for(Interval *ip = intervals; ip < intervals + intervalc; ip++) {
        printf("%d, %d\n", ip->start, ip->end);
    }
    printf("\n");
}

void merge_intervals() {
    quicksort(0, intervalc - 1);
    Interval *current, *next, *write;
    current = write = intervals;
    next = intervals + 1;
    int removed = 0;
    while(next < intervals + intervalc) {
        if(current->end < next->start) {
            *write = *current;
            current = next;
            write++;
            next++;
            continue;
        }
        write->start = current->start;
        write->end = max(current->end, next->end);
        removed++;
        next++;
    }
    intervalc -= removed;
}

int max(int a, int b) {
    if(a >= b) return a;
    return b;
}

void quicksort(int low, int high) {
    if(low < high) {
        int p = partition(low, high);
        quicksort(low, p - 1);
        quicksort(p + 1, high);
    }
}

int partition(int low, int high) {
    int pivot = intervals[high].start;
    register int i = low;
    register int j = low;
    for(; j <= high; j++) {
        int value = intervals[j].start;
        if(value < pivot) {
            swap(i, j);
            i++;
        }
    }
    swap(i, high);
    return i;
}

void swap(int x, int y) {
    if(x == y) return;
    Interval temp = intervals[x];
    intervals[x] = intervals[y];
    intervals[y] = temp;
}

void *xmalloc(size_t size) {
    void *p = malloc(size);
    if(!p) {
        printf("failed to allocate %lu bytes of memory\n", size);
        exit(1);
    }
    return p;
}
