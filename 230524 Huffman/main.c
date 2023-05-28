//
//  main.c
//  230524 Huffman
//  201911695 한설
//  Created by 한설 on 2023/05/24.
//

#include "huffman.h"
#include <time.h>
#include <stdlib.h>


extern void TC1_Aapple(void);
void checktime(void(*func)(void));


int main(int argc, const char * argv[]) {
    checktime(TC1_Aapple);
    return 0;
}

void checktime(void(*func)(void))
{
    clock_t start, finish;
    double duration;
    start = clock();
    (*func)();
    finish = clock();
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
    printf("\n================%lf초입니다.================\n", duration);
}
