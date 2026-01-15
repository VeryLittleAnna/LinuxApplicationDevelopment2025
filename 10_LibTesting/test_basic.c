#include <stdio.h>
#include <stdlib.h>
#include "lib.h"

int main(void) {
    int pass = 0;
    int fail = 0;
    
    float *a = 0;
    if (buf_capacity(a) == 0) {
        printf("PASS capacity init\n");
        pass++;
    } else {
        printf("FAIL capacity init\n");
        fail++;
    }
    
    if (buf_size(a) == 0) {
        printf("PASS size init\n");
        pass++;
    } else {
        printf("FAIL size init\n");
        fail++;
    }
    
    buf_push(a, 1.3f);
    if (buf_size(a) == 1) {
        printf("PASS size 1\n");
        pass++;
    } else {
        printf("FAIL size 1\n");
        fail++;
    }
    
    if (a[0] == (float)1.3f) {
        printf("PASS value\n");
        pass++;
    } else {
        printf("FAIL value\n");
        fail++;
    }
    
    buf_clear(a);
    if (buf_size(a) == 0) {
        printf("PASS clear\n");
        pass++;
    } else {
        printf("FAIL clear\n");
        fail++;
    }
    
    if (a != 0) {
        printf("PASS clear not-free\n");
        pass++;
    } else {
        printf("FAIL clear not-free\n");
        fail++;
    }
    
    buf_free(a);
    if (a == 0) {
        printf("PASS free\n");
        pass++;
    } else {
        printf("FAIL free\n");
        fail++;
    }
    
    /* Clearing an NULL pointer is a no-op */
    buf_clear(a);
    if (buf_size(a) == 0) {
        printf("PASS clear empty\n");
        pass++;
    } else {
        printf("FAIL clear empty\n");
        fail++;
    }
    
    if (a == 0) {
        printf("PASS clear no-op\n");
        pass++;
    } else {
        printf("FAIL clear no-op\n");
        fail++;
    }
    
    printf("\n%d fail, %d pass\n", fail, pass);
    return fail != 0;
}