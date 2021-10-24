/* #define DT_LENGTH 32//длина переменных 
#define MM 2//число строк первой матрицы
#define NN 2//число столбцов второй матрицы 
#define KK 2//длина вектора (число столбцов первой матрицы и число строк второй) */
#include <macro.h>
#include <systemc.h>
#include <sstream>

#include <vector_mul.h>

SC_MODULE(matrix_mul) {
    sc_in<bool> clk, rst_n;
    sc_in<sc_int<DT_LENGTH> > matA[MM][KK], matB[KK][NN];
    sc_out<sc_int<DT_LENGTH * 2> > matC[MM][NN];

    vector_mul *mul[MM][NN];

    SC_CTOR(matrix_mul) {
        std::ostringstream mul_name;
        for (int i = 0; i < MM; ++i) {
            for (int j = 0; j < NN; ++j) {
                mul_name << "pe_" << i << "_" << j;
                mul[i][j] = new vector_mul(mul_name.str().c_str());
                mul[i][j]->clk(clk);
                mul[i][j]->rst_n(rst_n);
                for (int k = 0; k < KK; ++k) {
                    mul[i][j]->vec1[k](matA[i][k]);
                    mul[i][j]->vec2[k](matB[k][j]);
                }
                mul[i][j]->vec_o(matC[i][j]);
                mul_name.str("");
            }
        }
    };
};