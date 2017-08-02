#include <stdio.h>
struct test{
    int *a;
    int n;
    test(int n){
        a = new int[n];
    }
};
int main(int argc, char *argv[]){
    
    test *b = new test(5);
    printf("int is %zu int* is %zu sizeof b is %zu %zu\n", sizeof(int), sizeof(int *), sizeof(*b), sizeof(test));
    return 0;
}
