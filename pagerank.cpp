#include "matrix.hpp"
using namespace pagerank;

int main(int argc, char * argv[]){

    float H_arr[] = {0, 0, 0.25, 0, 0,
                     1, 0, 0.25, 0, 0,
                     0, 0, 0, 0.5, 0,
                     0, 0, 0.25, 0, 1,
                     0, 1, 0.25, 0.5, 0};
    int row1 = 5; int col1 = 5;
   
    Matrix<float> H(row1, col1, H_arr);
    H.print();

    float R_arr[] = {0.2,
                     0.2,
                     0.2,
                     0.2,
                     0.2};
    int row2 = 5; int col2 = 1;
    
    Matrix<float> R(row2, col2, R_arr);
    R.print();

    Matrix<float> res = H * R;
    res.print();

    return 0;
}
