#include <utility>
#include <mutex>
#include <thread>
#include <vector>

#include "matrix.hpp"

using namespace pagerank;
// get idx range of a row or a col 
std::pair<int, int> get_range(int sub_id, int partitions, int total_len){
    assert(partitions > 0 && total_len > 0);
    int each_siz = total_len / partitions;
    int start = sub_id * each_siz;
    // > total_len is impossible
    int end = (sub_id + 1) * each_siz;
    // special case is the last partition
    end = (sub_id == partitions - 1)?total_len:end;
    return std::make_pair(start, end);
}
int main(int argc, char * argv[]){

    float H_arr[] = {0, 0, 0.25, 0, 0, 1,
                     1, 0, 0.25, 0, 0, 0.2,
                     0, 0, 0, 0.5, 0, 1,
                     0, 0, 0.25, 0, 1, 0.2,
                     0, 1, 0.25, 0.5, 0, 0.5,
                     1, 0, 1, 0, 1, 0};
    int row1 = 6; int col1 = 6;
   
    Matrix<float> H(row1, col1, H_arr);
    H.print();

    float R_arr[] = {0.2,
                     0.2,
                     0.2,
                     0.2,
                     0.2,
                     0.2};
    int row2 = 6; int col2 = 1;
    
    Matrix<float> R(row2, col2, R_arr);
    R.print();

    // basic way
    /*Matrix<float> res = H * R;
    res.print();*/

    // parallel way
    int partitions = 2;

    Matrix<float> *res_buf;
    assert((res_buf = new Matrix<float>[partitions]) != NULL);
    // different cells having the same rows range store into one matrix
    // which means threads charging the same row range but different col range
    // share one matrix to store result
    // initialize matrix res
    for(int r = 0; r < partitions; ++r){
        std::pair<int, int> row_rg = get_range(r, partitions, H.row_size()); 
        printf("sub %d : range from %d to %d\n", r, row_rg.first, row_rg.second);
        res_buf[r].initialize(row_rg.second - row_rg.first, R.col_size()/*1*/, NULL);
    }
    // used to synchronize threads
    std::mutex mtx;
    // row and col are divided into partitions
    // form a grid, each thread is responsible for one cell of the grid
    // parallelism threads in total
    // int parallelism = partitions * partitions;
    std::vector<std::thread> threads;
    threads.clear();
    for(int i = 0; i < partitions; ++i){
        for(int j = 0; j < partitions; ++j){
            
            // second matrix only has one column, don't need any range 
            threads.emplace_back([&](int th_i, int th_j){
                    
                    std::pair<int, int> fst_rows = get_range(th_i, partitions, H.row_size());
                    std::pair<int, int> fst_cols = get_range(th_j, partitions, H.col_size());
                    std::pair<int, int> snd_rows = get_range(th_j, partitions, R.row_size());
                    // using row range and col range, form two sub-matrix to multiply
                    int rows = fst_rows.second - fst_rows.first;
                    int cols = fst_cols.second - fst_cols.first;
                    printf("rows is %d cols is %d\n", rows, cols);
                    Matrix<float> sub1(rows, cols);
                    // copy part of original matrix H
                    for(int i = fst_rows.first; i < fst_rows.second; ++i){
                        for(int j = fst_cols.first; j < fst_cols.second; ++j){
                           sub1[i - fst_rows.first][j - fst_cols.first] = H[i][j]; 
                        }
                    }
                    rows = snd_rows.second - snd_rows.first;
                    cols = 1;
                    Matrix<float> sub2(rows, cols);
                    // copy part of original matrix R
                    for(int i = snd_rows.first; i < snd_rows.second; ++i){
                        sub2[i - snd_rows.first][0] = R[i][0]; 
                    }
                    // sub1 * sub2, store into shared data structure outside
                    Matrix<float> sub_res = sub1 * sub2;
                    // synchronize
                    std::unique_lock<std::mutex> lck(mtx);
                    // need overload operator += for matrix
                    res_buf[th_i] += sub_res;

            }, i, j);
        }
    }

    for(int t = 0; t < partitions * partitions; ++t){
        threads[t].join();
    }

    // combine sub matrices
    int res_rows = H.row_size();
    int res_cols = R.col_size();
    Matrix<float> res(res_rows, res_cols);
    for(int r = 0; r < partitions; ++r){
        std::pair<int, int> sub_rg = get_range(r, partitions, res_rows);
        Matrix<float> *sub = res_buf + r;
        for(int i = sub_rg.first; i < sub_rg.second; ++i){
            for(int j = 0; j < res_cols; ++j){
                res[i][j] = (*sub)[i - sub_rg.first][j];
            }
        }
    }

    res.print();

    delete[] res_buf;
    
    return 0;
}
