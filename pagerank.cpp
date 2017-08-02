#include "matrix.hpp"
using namespace pagerank;

int main(int argc, char * argv[]){

    float H_arr[] = {0, 0, 0.25, 0, 0,
                     1, 0, 0.25, 0, 0,
                     0, 0, 0, 0.25, 0,
                     0, 0, 0.25, 0, 1,
                     0, 1, 0.25, 0.5, 0};
    // printf("%f\n", H_arr[2]);
    int row1 = 5; int col1 = 5;
   
    /*assert((H_arr = (float**)malloc(sizeof(float*) * row1)) != NULL);
    for(int i = 0; i < row1; ++i){
        assert((H_arr[i] = (float *)malloc(sizeof(float) * col1)) != NULL);
    }
    H_arr[0] = {0, 0, 1/4, 0, 0};
    H_arr[1] = {1, 0, 1/4, 0, 0};
    H_arr[2] = {0, 0, 0, 1/2, 0};
    H_arr[3] = {0, 0, 1/4, 0, 1};
    H_arr[4] = {0, 1, 1/4, 1/2, 0};*/
    
    Matrix<float> H(row1, col1, H_arr);
    H.print();

    float R_arr[] = {0.2,
                     0.2,
                     0.2,
                     0.2,
                     0.2};
    int row2 = 5; int col2 = 1;
    /*assert((R_arr = (float**)malloc(sizeof(float*) * row2)) != NULL);
    for(int i = 0; i < row2; ++i){
        assert((R_arr[i] = (float*)malloc(sizeof(float) * col2)) != NULL);
    }
    R_arr[0] = {1/5};
    R_arr[1] = {1/5};
    R_arr[2] = {1/5};
    R_arr[3] = {1/5};
    R_arr[4] = {1/5};*/
    
    Matrix<float> R(row2, col2, R_arr);
    R.print();

    Matrix<float> a;
    // Matrix<float> res = H * R;


    return 0;
}
