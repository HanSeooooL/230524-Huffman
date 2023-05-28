//
//  test.c
//  230524 Huffman
//
//  Created by 한설 on 2023/05/28.
//

#include "huffman.h"


void TC1_Aapple(void) {
    
    printf("\n===============TC1_Aapple===============\n");
    char *str;
    str = huffmanencode("test.txt");
    filesave(str);
    
    printf("%s", str);
}
