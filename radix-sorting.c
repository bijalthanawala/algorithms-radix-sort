#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>

#define swap(a , b) { \
                        uint32_t *t = a; \
                        a = b; \
                        b = t; \
                    }

#define ARRAY_SIZE 100000000

typedef int bool;
#define true (int)1 
#define false (int)0


void print_array(uint32_t a[], uint32_t size, char *msg) {
    int i=0;
    if(msg) {
        printf("%s\n", msg);
    }
    for(i=0; i<size; i++) {
        printf("%u\n", a[i]);
    }
}

void radix_sort(uint32_t a[], uint32_t size) {
    int radix = 2; //Range of values in key-unit (in this case key-unit = a bit. So values are 2 (0 and 1)
    uint32_t keymask = 1;
    int nr_bits = 32;
    uint32_t *count = calloc( radix+1 , sizeof(uint32_t));
    uint32_t *interim = calloc(size, sizeof(uint32_t));
    uint32_t * p_inter_source = NULL;
    uint32_t* p_inter_dest = NULL;
    int i = 0;
    int key = 0;
    int fullkey = 0;
    int bit = 0;

    p_inter_source=&a[0];
    p_inter_dest=interim;
    for(bit=0; bit < nr_bits; bit++, keymask <<= 1) {

        // Re/Initialize the frequency/counts
        memset(count, 0, (radix+1) * sizeof(uint32_t));


        //Count the frequencies
        for(i=0; i<size; i++) {
            fullkey = (p_inter_source[i] & keymask);
            key = fullkey >> bit;
            count[ key +1 ]++;
        }

        //Perform cumulative count
        for(i=1; i < (radix+1); i++) {
            count[i] += count[i-1];
        }
    
        //Sort based on the current key
        for(i=0; i<size; i++) {
            fullkey = (p_inter_source[i] & keymask);
            key = fullkey >> bit;
            //printf("Sorting index: %d keymask = %d fullkey = %d  key = %d elem = %d\n",
            //                       i, keymask, fullkey, key, p_inter_source[i]);
            p_inter_dest[ count[key]++ ] = p_inter_source[i];
        }
        swap(p_inter_dest, p_inter_source);
    }

    if( nr_bits % 2) {
        for(i=0; i < size; i++) {
            a[i] = interim[i];
        }
    }

    printf("radix_sort: exit\n");
}


void prepare_array_to_sort(uint32_t a[], uint32_t nr_elements, bool brandom) {

    int i;
    for(i=0; i < nr_elements; i++) {
        if(brandom) 
            a[i] = random();
        else
            a[i] = i;
    }

}

bool is_sorted(uint32_t* a, uint32_t size) {
    int i;
    printf("is_sorted: entry (size=%d)\n", size);
    for(i=1; i<size; i++) {
        if(a[i] < a[i-1]) { 
            //printf("Not sorted: a[%d] = %d, a[%d]=%d\n", i, a[i], i-1, a[i-1]);
            return false;
        }
    }
    printf("is_sorted: true\n");
    return true;
}

uint32_t a[ARRAY_SIZE] = {0};
int main() {

    int i = 0;
    time_t start_time = 0;
    

    srand(time(NULL));

    prepare_array_to_sort(a, sizeof(a)/sizeof(a[0]), true);
    printf("before radix_sort()\n");
    //print_array(a, sizeof(a)/sizeof(a[0]), "Array (before sort):");
    printf("is sorted : %s\n", is_sorted(a , sizeof(a)/sizeof(a[0])) ? "True" : " False");
    
    start_time = time(NULL);
    radix_sort(a, sizeof(a)/sizeof(a[0]));
    printf("Time taken: %ld\n", time(NULL) - start_time);

    printf("after radix_sort()\n");
    //print_array(a, sizeof(a)/sizeof(a[0]), "Array (after sort):");
    
    printf("is sorted : %s\n", is_sorted(a , sizeof(a)/sizeof(a[0])) ? "True" : " False");
       
    return 0;
}
