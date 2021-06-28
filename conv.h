#define DT_LENGTH 32//длина переменных 
#define M1 2//число строк КЕРНЕЛА 
#define N1 2//число столбцов КЕРНЕЛА 
#define M2 5//число строк ИЗОБРАЖЕНИЯ
#define N2 5//число столбцов ИЗОБРАЖЕНИЯ
#define STRIDE 1
#define ZERO_PAD 0
#define M3 (M2-M1+2*ZERO_PAD)/STRIDE+1//вычисляем размеры выходной матрицы
#define N3 (N2-N1+2*ZERO_PAD)/STRIDE+1

#include <systemc.h>

#include "matrix_mul.h"

SC_MODULE(conv){
	//порты
	sc_in<bool> clk, rst_n;
	sc_in<sc_int<DT_LENGTH>> kernel[M1][N1], image[M2][N2];
	sc_out<sc_int<DT_LENGTH*2>> convolved_mat[M3][N3];
	
	//matrix_mul *max_mul[MM][NN];
	
	void convolution(void);//делаем kernel таких же размеров как и выходную матрицу
	
	SC_CTOR(conv){
		SC_METHOD(convolution);
		sensitive_pos << clk; // позитивный clock
        sensitive_neg << rst_n; // негативный сброс
	/* 		for (int i = 0; i <2; ++i) {
            for (int k = 0; k < 2; ++k) {
                kernel_padded[i][k]=kernel[i][k];
            }
        }
	for (int i = 0; i < M3; ++i) {
        for (int j = 0; j < N3; ++j) {
            cout << kernel[i][j] << " ";
        }
        cout << endl;
    } */
	
	}
	//наверное так же как и в matrix_mul надо без методов
};
		
		
	