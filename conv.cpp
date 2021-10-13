#include "conv.h"

void conv::convolution(void){
	/* sc_int<DT_LENGTH> kernel_padded[M3][N3];
    //ZERO-PADDING кернела
	//т.к. кернел после паддинга слева-снизу, то строки перебираем сконца (снизу), а столбцы сначала(слева)
 	 for (int i = M3-1; i >= (M3-M1); --i) {
        for (int k = 0; k < N3-1; ++k) {
			kernel_padded[i][k]=kernel[i-1][k].read();
        }
    }  */
	cout<<"[отладочный вывод][convolution] кернел:"<<endl;
	for (int i = 0; i < M1; ++i) {
        for (int k = 0; k < N1; ++k) {
            cout << kernel[i][k] << " ";
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
	cout<<"размеры выходной матрицы: "<<endl;
	cout<<"M3= "<<M3<<" N3= "<<N3<<" "<<endl;
	cout<<endl;
	/* cout<<"[отладочный вывод][convolution] кернел нормализованный:"<<endl;
	for (int i = 0; i < M1; ++i) {
        for (int k = 0; k < N1; ++k) {
            cout << kernel[i][k] << " ";
        }
        cout << endl;
    } */
	
	cout<<"[отладочный вывод][convolution] изображение:"<<endl;
	for (int i = 0; i < M2; ++i) {
        for (int k = 0; k < N2; ++k) {
            cout << image[i][k] << " ";
        }
        cout << endl;
    }
	
	//свёртка
	sc_int<DT_LENGTH> result[M3][N3];
	
	for (int i=0; i<M3; /*i++*/){
		for (int j=0; j<N3; /*j++*/){//STRIDE
			for (int m=0;m<M1;m++){
				for (int n=0;n<N1;n++){
					result[i][j]+=image[i+m][j+n].read()*kernel[m][n].read();
				}
			}
			j = j + STRIDE;
		}
		i = i + STRIDE;
	}
	/* cout<<"[отладочный вывод][convolution] результат:"<<endl;
	for (int i = 0; i < M3; ++i) {
        for (int k = 0; k < N3; ++k) {
            cout << result[i][k] << " ";
        }
        cout << endl;
    } */
	cout<<endl;

	//конечный вывод из функции и модуля conv
	for (int i=0;i<M3;i++){
		for (int k=0;k<N3;k++){
			convolved_mat[i][k].write(result[i][k]);
		}
	}
			
};