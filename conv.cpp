#include "conv.h"
#include "ReLU.h"
void conv::convolution(void) {
	/* sc_int<DT_LENGTH> kernel_padded[M3][N3];
	//ZERO-PADDING кернела
	//т.к. кернел после паддинга слева-снизу, то строки перебираем сконца (снизу), а столбцы сначала(слева)
	 for (int i = M3-1; i >= (M3-M1); --i) {
		for (int k = 0; k < N3-1; ++k) {
			kernel_padded[i][k]=kernel[i-1][k].read();
		}
	}  */
	cout << "[отладочный вывод][convolution] кернел:" << endl;
	for (int k = 0; k < N1; k++) {
		for (int i = k*M1; i < (k+1)*M1; ++i) {
			cout << kernel[i] << " ";
		}
		cout << endl;
	}

	cout << endl;
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

	cout << "[отладочный вывод][convolution] изображение:" << endl;
	for (int h = 0; h < M2; h++) {
		for (int g = h*N2; g < (h+1)*N2; ++g) {
			cout << image[g] << " ";
		}
		cout << endl;
	}
	//--------------------------------------------------------------------
	//2D свёртка как 1D свёртка:
	//zero_pad to equal dimensions->reshape+truncate->1D Conv->reshape ?????
	//--------------------------------------------------------------------
	//zero-padding векторов
	//КЕРНЕЛ
	//создаём вектор для padded kernel
	sc_int<DT_LENGTH> kernel_padded[784];
	for (int p = 0; p < 784; p++) {
		kernel_padded[p] = 0;
	}

	sc_int<16> chunk_cnt = 0;
	sc_int<16> chunk_max = M1;//число строк кернела
	sc_int<16> addition = 0;  
	for (int i = 0; i < 784;) {
		for (int k = addition; k < N1+addition; k++) {//(колво столбцов кернела+аддишн)
			kernel_padded[i] = kernel[k];// КУДА ДЕВАЕТСЯ kernel[1]??????
			i++;
		}
		i=i+ (M2-M1);//
		chunk_cnt++;
		addition = N1* chunk_cnt;//число столбоцов кернела (сколько нужно элементов между паддингами)
		if (chunk_cnt == chunk_max) {
			break;//i = 784;
		}
	}
	cout << "[отладочный вывод][convolution] Kernel_padded:" << endl;
	//for(int i=0;i<676;i++){ 
	for (int k = 0; k < M2; k++) {
		for (int i = k * N2; i < (k + 1) * N2; ++i) {
			cout << kernel_padded[i] << " ";
		}
		cout << endl;
	}
	//truncation
	const int last_elem = (M1+(M2-M1))*N1- (M2 - M1);// последний элемент до которого нужно truncate

	sc_int<DT_LENGTH> kernel_padded_truncated[last_elem] = { 0 };
	for (int i = 0; i < last_elem; i++) {
		kernel_padded_truncated[i] = kernel_padded[i];
	}
	cout << "[отладочный вывод][convolution] Kernel_padded_truncated:" << endl;
	for (int i : kernel_padded_truncated) //альтернативный вывод
		cout << i << " ";
	cout << endl;

	//свёртка
	sc_int<DT_LENGTH> result[CONV_ED];

	for (int i = 0; i < CONV_ED; i++) {
		for (int m = 0; m < last_elem; m++) {
			result[i] += image[i + m].read() * kernel_padded_truncated[m];
		}
		result[i] = ReLU(result[i]);
	}

	/*cout<<"[отладочный вывод][convolution] результат:"<<endl;
	for (int k = 0; k < M3; k++) {
        for (int i = k*N3; i < (k+1)*N3; i++) {
            cout << result[i] << " ";
        }
        cout << endl;
    } 
	cout<<endl;*/

	//конечный вывод из функции и модуля conv
	for (int k = 0; k < CONV_ED; k++) {
		convolved_mat[k].write(result[k]);//применеяем ReLU
	}


};