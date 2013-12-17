/* Include
************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <memory.h>
#include <math.h>


/* Typedefs
***************/
typedef int bool;


/* Macros
************/
#define swap(a , b) { \
                        uint32_t *t = a; \
                        a = b; \
                        b = t; \
                    }

/* Constants
***************/
#define ARRAY_SIZE 1000000
#define true (bool)1 
#define false (bool)0


void print_array(uint32_t a[], uint32_t size, char *msg) {
    int i=0;
    if(msg) {
        printf("%s\n", msg);
    }
    for(i=0; i<size; i++) {
        printf("%u\n", a[i]);
    }
}

bool is_pow2(int n) {
    return (n & (n-1)) ? false : true;
}

//Size of the subkey (in bits): Call it sosk
//Range of Values of key = 2 ^ (sosk)
//Keymask Initial = 2 ^(sosk) - 1 
//Keymask with each iteration = keymask << sosk
//Number of iterations to perform = size_of_the_entire_key / sok

void radix_sort(uint32_t a[], uint32_t size_array, uint32_t size_subkey) {
    int range_value = (int)pow(2, size_subkey); //Range of values in key-unit
    uint32_t keymask = range_value-1;
    int nr_bits = 32;
    uint32_t *count = calloc( range_value+1 , sizeof(uint32_t));
    uint32_t *interim = calloc(size_array, sizeof(uint32_t));
    uint32_t * p_inter_source = NULL;
    uint32_t* p_inter_dest = NULL;
    int i = 0;
    int key = 0;
    int fullkey = 0;
    int bit = 0;


    //Validate that size_subkey is a power of 2
    if( ! is_pow2(size_subkey)) {
        printf("Size of subkey (%d) is not a power of 2. Exiting.\n", size_subkey);
        return;
    }

    printf("initial keymask = %04X\n",keymask);
    p_inter_source=&a[0];
    p_inter_dest=interim;
    for(bit=0; bit < nr_bits; bit+=size_subkey, keymask <<= size_subkey) {

        // Re/Initialize the frequency/counts
        memset(count, 0, (range_value+1) * sizeof(uint32_t));


        //Count the frequencies
        for(i=0; i<size_array; i++) {
            fullkey = (p_inter_source[i] & keymask);
            key = fullkey >> bit;
            count[ key +1 ]++;
        }

        //Perform cumulative count
        for(i=1; i < (range_value+1); i++) {
            count[i] += count[i-1];
        }
    
        //Sort based on the current key
        for(i=0; i<size_array; i++) {
            fullkey = (p_inter_source[i] & keymask);
            key = fullkey >> bit;
            //printf("Sorting index: %d keymask = %d fullkey = %d  key = %d elem = %d\n",
            //                       i, keymask, fullkey, key, p_inter_source[i]);
            p_inter_dest[ count[key]++ ] = p_inter_source[i];
        }
        swap(p_inter_dest, p_inter_source);
        printf("keymask = %08X\n",keymask);
    }


    if( nr_bits % 2) {
        for(i=0; i < size_array; i++) {
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
    return true;
}

int main() {

    int i = 0;
    time_t start_time = 0;
    uint32_t a[ARRAY_SIZE] = {0};
    

    srand(time(NULL));

    prepare_array_to_sort(a, sizeof(a)/sizeof(a[0]), true);
    printf("before radix_sort()\n");
    //print_array(a, sizeof(a)/sizeof(a[0]), "Array (before sort):");
    printf("is sorted : %s\n", is_sorted(a , sizeof(a)/sizeof(a[0])) ? "True" : " False");
    
    start_time = time(NULL);
    radix_sort(a, sizeof(a)/sizeof(a[0]), 4);
    printf("Time taken: %ld\n", time(NULL) - start_time);

    printf("after radix_sort()\n");
    //print_array(a, sizeof(a)/sizeof(a[0]), "Array (after sort):");
    
    printf("is sorted : %s\n", is_sorted(a , sizeof(a)/sizeof(a[0])) ? "True" : " False");
       
    return 0;
}
