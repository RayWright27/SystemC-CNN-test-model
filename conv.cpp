#include "conv.h"
#include "ReLU.h"
void conv::convolution(void) {
	sc_int<DT_LENGTH> biases[BIASES] = { 1 };
	/* sc_int<DT_LENGTH> kernel_padded[M3][N3];
	//ZERO-PADDING кернела
	//т.к. кернел после паддинга слева-снизу, то строки перебираем сконца (снизу), а столбцы сначала(слева)
	 for (int i = M3-1; i >= (M3-M1); --i) {
		for (int k = 0; k < N3-1; ++k) {
			kernel_padded[i][k]=kernel[i-1][k].read();
		}
	}  */
	sc_int<DT_LENGTH> kernel_in[L1][M1][N1];
	for (int j = 0; j < L1; ++j) {
		for (int i = 0; i < M1; ++i) {
			for (int k = 0; k < N1; ++k) {
				kernel_in[j][i][k] = kernel[j * N1 * M1 + i * M1 + k].read();
			}
		}
	}

	cout << "[отладочный вывод][convolution] кернел:" << endl;
	for (int j = 0; j < L1; ++j) {
		for (int i = 0; i < M1; ++i) {
			for (int k = 0; k < N1; ++k) {
				cout << kernel_in[j][i][k] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
	//нормализация кернела
	/* sc_int<16> k_sum;
	for (int i = 0; i < M1; ++i) {
		for (int k = 0; k < N1; ++k) {
			k_sum+=kernel[i][k].read();
		}
	} */
	/* for (int i = 0; i < M1; ++i) {
		for (int k = 0; k < N1; ++k) {
			kernel[i][k].write(kernel[i][k].read/k_sum);
		}
	} */
	/* cout<<"k_sum= "<<k_sum<<endl;
	cout<<endl; */
	cout << "размеры выходной матрицы: " << endl;
	cout << "M3= " << M3 << " N3= " << N3 << " " << endl;
	cout << endl;
	/* cout<<"[отладочный вывод][convolution] кернел нормализованный:"<<endl;
	for (int i = 0; i < M1; ++i) {
		for (int k = 0; k < N1; ++k) {
			cout << kernel[i][k] << " ";
		}
		cout << endl;
	} */


	sc_int<DT_LENGTH> image_in[M2][N2];
	for (int i = 0; i < M2; ++i) {
		for (int k = 0; k < N2; ++k) {
			image_in[i][k] = image[i * N2 + k].read();
		}
	}
	cout << "[отладочный вывод][convolution] изображение:" << endl;
	for (int i = 0; i < M2; ++i) {
		for (int k = 0; k < N2; ++k) {
			cout << image_in[i][k] << " ";
		}
		cout << endl;
	}
	cout << endl;
	//свёртка
	sc_int<DT_LENGTH> result[L3][M3][N3];
	

	for (int k = 0; k < L1; k++) {//число кернелов и выходных матрицы соотв-но
		for (int i = 0; i < M3; i++) {
			for (int j = 0; j < N3; j++) {
				for (int m = 0; m < M1; m++) {
					for (int n = 0; n < N1; n++) {
						result[k][i][j] += ReLU(image_in[i + m][j + n] * kernel_in[k][m][n]);
					}
				}
				result[k][i][j] += biases[k];
			}
		}
	}
    
	cout<<"[отладочный вывод][convolution] результат:"<<endl;
	for (int k = 0; k < L3; ++k) {
		for (int i = 0; i < M3; ++i) {
			for (int j = 0; j < N3; ++j) {
				cout << result[k][i][j] << " ";
			}
			cout << endl;
		}
		cout << "_________________" << endl;
		cout << endl << endl;
	}
	cout << endl;

	
	//конечный вывод из функции и модуля conv
	for (int k = 0; k < L3; k++) {
		for (int i = 0; i < M3; i++) {
			for (int j = 0; j < N3; j++) {
				convolved_mat[k*N3*M3+i*N3+j].write(result[k][i][j]); //равно и .write() дают одно и тоже 
			}
		}
	}
	
};