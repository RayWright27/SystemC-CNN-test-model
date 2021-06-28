#include "tb_driver.h"

void tb_driver::generate_input(void){
	//это входные матрицы изображения и кернела
	//нормализация кернела- деление каждого элемента кернела на сумму его элементов
		const sc_int<16> A[M1][N1]={1,0,0,-1};
		const sc_int<16> B[M2][N2]={1,8,7,6,5,4,3,2,1,0,1,8,7,6,5,4,3,2,1,0,1,2,3,4,5};
         for (int i = 0; i < M1; ++i) {
            for (int k = 0; k < N1; ++k) {
                kernel[i][k].write(A[i][k]);
            }
        }
        for (int k = 0; k < M2; ++k) {
            for (int j = 0; j < N2; ++j) {
                image[k][j].write(B[k][j]);
            }
        }
};

void tb_driver::generate_reset(void) {
        rst_n.write(1);
        wait(1, SC_NS);
        rst_n.write(0);
        wait(1, SC_NS);
        rst_n.write(1);
    };