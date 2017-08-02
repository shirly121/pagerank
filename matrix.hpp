#ifndef PAGERANK_INCLUDE_MATRIX_H
#define PAGERANK_INCLUDE_MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

namespace pagerank{
// Row represent a line in a matrix
template <typename T>
struct Row{
    int n;
    T *rdata;
    Row(){
        n = 0;
        rdata = NULL;
    }
    void set_row(int n, T *rdata){
        assert(n > 0);
        this->n = n;
        this->rdata = rdata;
    }
    T& operator[](int j){
        assert(j >= 0 && j < n);
        return rdata[j];
    }
    ~Row(){
        rdata = NULL;
    }
};
template <typename T>
class Matrix{
protected:
    // m rows and n cols
    int m;
    int n;
    Row<T> *data;
public:
    Matrix(){
        m = n = 0;
        data = NULL;
    }
    void initialize(int m, int n, T *_data){
        // ensure parameter _m and _n are all valid
        assert(m > 0 && n > 0);
        this->m = m;
        this->n = n;
        T *buf;
        assert((buf = (T*)calloc(m * n, sizeof(T))) != NULL);
        if(_data){
            memcpy(buf, _data, sizeof(T) * m*n);
        }
        // allocate memory for each row in a matrix
        assert((data = new Row<T>[m]) != NULL);
        for(int i = 0; i < m; ++i){
            data[i].set_row(n, buf+i*n);
        }
    }
    Matrix(int m, int n, T *_data = NULL){
        initialize(m, n, _data);
    }
    void clear(){
        if(data != NULL && data[0].rdata != NULL){
            free(data[0].rdata);
        }
        if(data){
            delete[] data;
        }
        data = NULL;
    }
    ~Matrix(){
        clear();
    }
    Row<T>& operator[](int i) const{
        assert(i >= 0 && i < m);
        return data[i];
    }
    int row_size() const{
        return m;
    }
    int col_size() const{
        return n;
    }
    Matrix operator*(const Matrix<T> &other){
        // precondtion
        assert(n == other.row_size());
        // res is result to be returned
        int res_col = other.col_size();
        // create res to store result
        Matrix<T> res(m, res_col);
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < res_col; ++j){
                res[i][j] = 0;
                // matrix A, B: sum of aik * bkj, k from 1 to n
                for(int k = 0; k < n; ++k){
                    res[i][j] += data[i][k] * other[k][j];
                }
            }
        }
        // overload operator= 
        return res;
    }
    // avoid shadow copy
    void operator=(const Matrix<T> &other){
        if(&other == this)return;
        // this is not empty
        if(this->m != 0){
            clear();
        }
        this->m = other.row_size();
        this->n = other.col_size();
        // empty, don't need to copy data
        if(m <= 0 || n <= 0)return;

        initialize(m, n, NULL);
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                data[i][j] = other[i][j];
            }
        }
    }
    void operator+=(const Matrix<T> &other){
        // allow add itself
        if(m != other.row_size() || n != other.col_size())return;
        assert(m > 0 && n > 0 && data != NULL);
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                data[i][j] += other[i][j];
            }
        }
    }
    void print(){
        for(int i = 0; i < m; ++i){
            for(int j = 0; j < n; ++j){
                printf("%f ", data[i][j]);
            }
            printf("\n");
        }
    }
};
};
#endif
