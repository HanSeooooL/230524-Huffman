//
//  main.c
//  230524 Huffman
//
//  Created by 한설 on 2023/05/24.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_CHAR_PER_LINE 100
#define MAX_HEAP_NODE 1000

typedef struct _element {
    char sp;    //글자
    int w;      //가중치
} Element;

typedef struct _node {
    char sp;    //글자
    int w;      //가중치
    struct _node *left;
    struct _node *right;
} Node;

typedef struct _table {
    char sp;
    char num[100];
} Table;

typedef Node HNode;
#define Key(n)  (n.w)
#define Parent(i) (heap[i/2])
#define Left(i) (heap[i * 2])
#define Right(i) (heap[i * 2 + 1])

HNode heap[MAX_HEAP_NODE];
int heap_size;

void init_heap(void) {heap_size = 0;}
int is_empty_heap(void) {return heap_size == 0;}
int is_full_heap(void) {return (heap_size == MAX_HEAP_NODE - 1);}
HNode find_heap(void) {return heap[1];}
void insert_heap(HNode n);
HNode delete_heap(void);
void print_heap(void);
void Npreorder(HNode *n);
void error(char *msg);

Node *fileleadtext(char *name, int *n);  //파일 읽기
Node *fileleadstats(int *n);  //h
void filesave(char *str);
void makegraph(char *name, Node *arr, int n);  //글자와 가중치 파일에 기록
Node* makehuff(int *n);
Table *maketable(HNode *n, int h);
void makecode(char *str);
void maketable1(HNode *n, char *parentstr, Table *tarr, int *a);
int compare(const void *a, const void *b);


Node *createNode(char spell, int weight, Node *l, Node *r)
{
    Node *new;
    new = (Node*)malloc(sizeof(Node));
    new -> sp = spell;
    new -> w = weight;
    new -> left = l;
    new -> right = r;
    
    return new;
}

int main(int argc, const char * argv[]) {
    Node *arr;
    int n = 0;
    Table *tarr;
    arr = fileleadtext("test.txt", &n);
    qsort(arr, n, sizeof(Node), compare);
    makegraph("stats.txt", arr, n);
    free(arr);
    arr = makehuff(&n);
    //Npreorder(arr);
    tarr = maketable(arr, n);
    return 0;
}

void Npreorder(HNode *n)
{
    if(n != NULL) {
        printf("%c [%d] ", n -> sp, n -> w);
        Npreorder(n -> left);
        Npreorder(n -> right);
    }
}

void makecode()
{
}


Table *maketable(HNode *n, int h)
{
    Table *tarr;
    char left[1000], right[1000];
    strcpy(left, "0");
    strcpy(right, "1");
    int a;
    tarr = (Table*)malloc(sizeof(tarr) * h);
    a = 0;
    if(n != NULL) {
        //printf("%c [%d] ", n -> sp, n -> w);
        maketable1(n -> left, left, tarr, &a);
        maketable1(n -> right, right, tarr, &a);
    }
    
    return tarr;
}

void maketable1(HNode *n, char *parentstr, Table *tarr, int *a)
{
    if(n != NULL) {
        if(n -> sp != NULL)                 //자식노드가 없는 경우
        {
            //배열에 스펠링과 그에 대응하는 이진수 문자열 기입
            (tarr + *a) -> sp = n -> sp;
            strcpy((tarr + *a) -> num, parentstr);
            printf("%c [%s] \n", tarr[*a].sp, tarr[*a].num);
            *a += 1;
        }
        else                                //자식노드가 있는 경우
        {
            maketable1(n -> left, strncat(parentstr, "0", 1), tarr, a);
            maketable1(n -> right, strncat(parentstr, "1", 1), tarr, a);
        }
    }
}
                
Node* makehuff(int *n)
{
    Node *arr;
    HNode *e1, *e2, *parent;
    arr = fileleadstats(n);
    init_heap();
    for(int i = 0; i < *n; i++)
    {
        insert_heap(arr[i]);
    }
    print_heap();
    while(heap_size >= 2) {
        e1 = (Node*)malloc(sizeof(Node));
        e2 = (Node*)malloc(sizeof(Node));
        *e1 = delete_heap();
        *e2 = delete_heap();
        parent = createNode(NULL, (e1 -> w) + (e2 -> w), e1, e2);
        insert_heap(*parent);
        print_heap();
    }
    printf("\n");
    return &heap[1];
}

void insert_heap(HNode n)
{
    int i;
    if(is_full_heap()) return;
    i = ++(heap_size);  //heap 사이즈 1개 늘리고
    while(i != 1 && Key(n) < Key(Parent(i))) {
        heap[i] = Parent(i);
        i /= 2;
    }
    heap[i] = n;
}

HNode delete_heap(void)
{
    HNode hroot, last;
    int parent = 1, child = 2;
    
    if(is_empty_heap())
        error("힙 트리 공백 에러");
    
    hroot = heap[1];
    last = heap[heap_size--];
    while(child <= heap_size) {
        if(child < heap_size && Key(Left(parent)) > Key(Right(parent)))
            child++;
        if(Key(last) <= Key(heap[child]))
            break;
        heap[parent] = heap[child];
        parent = child;
        child *= 2;
    }
    heap[parent] = last;
    return hroot;
}

void print_heap(void)
{
    int i, level;
    for(i = 1, level = 1; i <= heap_size; i++)
    {
        if(i == level) {
            printf("\n");
            level *= 2;
        }
        if(heap[i].sp == NULL)
        {
            printf("%c %c %2d", (heap[i].left) -> sp, (heap[i].right) -> sp, heap[i].w);
        }
        else
            printf("%c %2d ", heap[i].sp, Key(heap[i]));
    }
    printf("\n-----------");
}

void filesave(char *str)
{
    FILE *fp = fopen("output.huf", "w");    //파일 쓰기 모드로 open
    if (fp != NULL) {
        fprintf(fp, "%s", str);   //파일에 입력
    }
    fclose(fp); //파일 닫기
}


Node *fileleadtext(char *name, int *n)
{
    int count = 1, can = 0, i;
    char str;
    Node *arr;
    FILE *fp;                                   //파일 포인터
    fp = fopen(name, "r");                          //파일 읽기
    arr = (Node*)malloc(sizeof(Node) * 3);        //3칸 메모리 할당
    if (fp == NULL)
        printf("파일이 없습니다.\n");
    else if(fp != NULL) {
        str = fgetc(fp);
        while(str != EOF) {
            can = 0;
            for(i = 0; i < *n; i++) //배열에 같은 글자가 이미 저장되어있는지 확인
            {
                if(arr[i].sp == str) //저장되어 있는 경우
                {
                    arr[i].w += 1;  //가중치만 1 늘리고
                    can = 1;
                    break;          //중단
                }
            }
            if (can == 0)           //저장되어 있지 않은 경우
            {
                if ((*n % 3) == 0 && !(*n == 0))  //배열에 할당된 메모리가 부족해진 경우
                {
                    count += 1;
                    arr = realloc(arr, sizeof(Node) * (count * 3));
                }
                *(arr + *n) = *createNode(str, 1, NULL, NULL);
                *n = *n + 1;
            }
            putchar(str);
            str = fgetc(fp);
        }
    }
    fclose(fp);
    return arr;
}

Node *fileleadstats(int *n)
{
    int count = 1;
    char str[10], *weight, *sp;
    Node *arr;                              //point구조체 선언
    FILE *fp;                                   //파일 포인터
    fp = fopen("stats.txt", "r");                          //파일 읽기
    arr = (Node*)malloc(sizeof(Node) * 3);        //3칸 메모리 할당
    *n = 0;
    if (fp == NULL)
        printf("파일이 없습니다.\n");
    else if(fp != NULL) {
        while(fgets(str, MAX_CHAR_PER_LINE, fp))    //한줄씩 읽어들이고
            {
                if ((*n % 3) == 0 && !(*n == 0)) {      //n을 3으로 나눴을 때 나머지가 없고, (3의 배수) n이 0이 아닐 때
                    count += 1; //count를 1 늘리고
                    arr = realloc(arr, sizeof(Element) * (count * 3));    //초기값이 3칸이므로 조건을 처음 만족했을 때 6칸 다음은 9칸. 3의 배수로 칸을 늘려간다.
                }
                sp = strtok(str, "\t");                   //공백기준 토큰화
                weight = strtok(NULL, "\t");                  //공백기준 토큰화
                *(arr + *n) = *createNode(*sp, atoi(weight), NULL, NULL);                    //arr배열의 n번째 칸에 point 대입
                *n = *n + 1;                            //반복할때마다 +1 (점의 개수 파악)
            }
    }
    fclose(fp);
    return arr;
}

void makegraph(char *name, Node *arr, int n)    //name: 파일 이름, arr: 글자와 가중치 저장된 배열, n: 저장된 글자 수
{
    FILE *fp;
    fp = fopen(name, "w");
    if (fp != NULL) {
        for(int i = 0; i < n; i++)
        {
            fprintf(fp, "%c\t", arr[i].sp); //글자 저장하고 \t(필드 구분자)
            fprintf(fp, "%d\n", arr[i].w);  //가중치 저장하고 \n(레코드 구분자)
        }
    }
    fclose(fp);
}

int compare(const void *a, const void *b)
{
    Element num1 = *(Element *)a;
    Element num2 = *(Element *)b;
    
    if (num1.w < num2.w)
        return -1;
    
    if (num1.w > num2.w)
        return 1;
    
    return 0;
}

void error(char *msg)
{
    printf("\n%s\n", msg);
    exit(1);
}
