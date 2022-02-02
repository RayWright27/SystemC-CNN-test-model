#include "conv.h"
#include "ReLU.h"
void conv::recieve_kernel(void) {
	/* double kernel_padded[M3][N3];
	//ZERO-PADDING кернела
	//т.к. кернел после паддинга слева-снизу, то строки перебираем сконца (снизу), а столбцы сначала(слева)
	 for (int i = M3-1; i >= (M3-M1); --i) {
		for (int k = 0; k < N3-1; ++k) {
			kernel_padded[i][k]=kernel[i-1][k].read();
		}
	}  */
	
	//инициализируем хэндшейк
	kernel_rdy.write(0);
	wait();

	double kernel_in_flattened[KER]={};//вектор, принимающий значения из порта
	double kernel_in[L1][M1][N1];
	while(true){
	for (int i=0; i<KER; i++){
		
		kernel_rdy.write(1);
		do{
			wait();
		}while (!kernel_vld.read());
		
			kernel_in_flattened[i]=kernel.read();
		
		kernel_rdy.write(0);
		/* for (int i=0; i<KER; i++){
			cout<<kernel_in_flattened[i]<<" ";
		} */
	}
		//разворачивание вектора в 3D массив (сделать в отдельный метод)
		
		for (int j = 0; j < L1; ++j) {
			for (int i = 0; i < M1; ++i) {
				for (int k = 0; k < N1; ++k) {
					kernel_in[j][i][k] = kernel_in_flattened[j * N1 * M1 + i * M1 + k];
				}
			}
		}

		cout<<endl<< "[отладочный вывод][convolution] кернел:" << endl;
		for (int j = 0; j < L1; ++j) {
			cout<< "кернел "<<j<<":"<<endl;
			for (int i = 0; i < M1; ++i) {
				for (int k = 0; k < N1; ++k) {
					cout << kernel_in[j][i][k] << " ";
				}
				cout << endl;
			}
			cout << endl;
		}

	}
	
/* 
	double biases_in[BIASES];
	 for (int k = 0; k < BIASES; ++k) {
            biases_in[k]=biases[k].read();
    }
	cout << "[отладочный вывод][convolution] баесы:" << endl;
	 for (int k = 0; k < BIASES; ++k) {
            cout<<biases_in[k]<<endl;
    }
	//нормализация кернела
	/* double k_sum;
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
	/* cout << "размеры выходной матрицы: " << endl;
	cout << "M3= " << M3 << " N3= " << N3 << " " << endl;
	cout << endl; */
	/* cout<<"[отладочный вывод][convolution] кернел нормализованный:"<<endl;
	for (int i = 0; i < M1; ++i) {
		for (int k = 0; k < N1; ++k) {
			cout << kernel[i][k] << " ";
		}
		cout << endl;
	} */

	/*
	double image_in[M2][N2];
	for (int i = 0; i < M2; ++i) {
		for (int k = 0; k < N2; ++k) {
			image_in[i][k] = image[i * N2 + k].read();
		}
	}
	


	//свёртка
	double result[L3][M3][N3];
	
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
	cout << endl; */

	/* //конечный вывод из функции и модуля conv
	for (int k = 0; k < L3; k++) {
		for (int i = 0; i < M3; i++) {
			for (int j = 0; j < N3; j++) {
				convolved_mat[k*N3*M3+i*N3+j].write(result[k][i][j]); //равно и .write() дают одно и тоже 
				
			}
		}
	} */
	//cout<<"@" << sc_time_stamp() <<" convolution layer calculated"<<endl;
};

void conv::recieve_image(void) {
	//инициализируем хэндшейк
	image_rdy.write(0);
	wait();

	double image_in_flattened[IMG]={};//вектор, принимающий значения из порта
	double image_in[M2][N2];
	while(true){
		for (int i=0; i<IMG; i++){
			image_rdy.write(1);
			do{
				wait();
			}while (!image_vld.read());
			image_in_flattened[i]=image.read();
			image_rdy.write(0);
			for (int i=0; i<IMG; i++){
				cout<<image_in_flattened[i]<<" ";
			}
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
};