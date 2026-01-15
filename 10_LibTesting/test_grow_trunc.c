#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int main(void) {
    int pass = 0;
    int fail = 0;
    
    /* buf_grow(), buf_trunc() */
    long *ai = 0;
    buf_grow(ai, 1000);
    
    if (buf_capacity(ai) == 1000) { 
        printf("PASS grow 1000\n"); pass++; 
    } else { 
        printf("FAIL grow 1000\n"); fail++; 
    }
    
    if (buf_size(ai) == 0) { 
        printf("PASS size 0 (grow)\n"); pass++; 
    } else { 
        printf("FAIL size 0 (grow)\n"); fail++; 
    }
    
    buf_trunc(ai, 100);
    if (buf_capacity(ai) == 100) { 
        printf("PASS trunc 100\n"); pass++; 
    } else { 
        printf("FAIL trunc 100\n"); fail++; 
    }
    
    buf_free(ai);
    
    printf("\n%d fail, %d pass\n", fail, pass);
    return fail != 0;
}