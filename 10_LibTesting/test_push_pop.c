#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int main(void) {
    int pass = 0;
    int fail = 0;
    
    /* buf_push(), [] operator */
    long *ai = 0;
    for (int i = 0; i < 10000; i++)
        buf_push(ai, i);
    
    if (buf_size(ai) == 10000) { 
        printf("PASS size 10000\n"); pass++; 
    } else { 
        printf("FAIL size 10000\n"); fail++; 
    }
    
    int match = 0;
    for (int i = 0; i < (int)(buf_size(ai)); i++)
        match += ai[i] == i;
    
    if (match == 10000) { 
        printf("PASS match 10000\n"); pass++; 
    } else { 
        printf("FAIL match 10000\n"); fail++; 
    }
    buf_free(ai);
    
    /* buf_pop() */
    float *a = 0;
    buf_push(a, 1.1);
    buf_push(a, 1.2);
    buf_push(a, 1.3);
    buf_push(a, 1.4);
    
    if (buf_size(a) == 4) { printf("PASS size 4\n"); pass++; } 
    else { printf("FAIL size 4\n"); fail++; }
    
    if (buf_pop(a) == (float)1.4f) { printf("PASS pop 3\n"); pass++; } 
    else { printf("FAIL pop 3\n"); fail++; }
    
    buf_trunc(a, 3);
    if (buf_size(a) == 3) { printf("PASS size 3\n"); pass++; } 
    else { printf("FAIL size 3\n"); fail++; }
    
    if (buf_pop(a) == (float)1.3f) { printf("PASS pop 2\n"); pass++; } 
    else { printf("FAIL pop 2\n"); fail++; }
    
    if (buf_pop(a) == (float)1.2f) { printf("PASS pop 1\n"); pass++; } 
    else { printf("FAIL pop 1\n"); fail++; }
    
    if (buf_pop(a) == (float)1.1f) { printf("PASS pop 0\n"); pass++; } 
    else { printf("FAIL pop 0\n"); fail++; }
    
    if (buf_size(a) == 0) { printf("PASS size 0 (pop)\n"); pass++; } 
    else { printf("FAIL size 0 (pop)\n"); fail++; }
    
    buf_free(a);
    
    printf("\n%d fail, %d pass\n", fail, pass);
    return fail != 0;
}