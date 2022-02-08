#include "conv.h"
#include "ReLU.h"

void conv::recieve_image(void) {
	//инициализируем хэндшейк
	image_rdy.write(0);
	wait(clk->posedge_event());

	double image_in_flattened[IMG]={};//вектор, принимающий значения из порта

	while(true){
		for (int i=0; i<IMG; i++){
			image_rdy.write(1);
			do{
				wait(clk->posedge_event());
			}while (!image_vld.read());
			image_in_flattened[i]=image.read();
			image_rdy.write(0);
			for (int i=0; i<IMG; i++){
				//cout<<image_in_flattened[i]<<" ";
			}
		}
		for (int i = 0; i < M2; ++i) {
			for (int k = 0; k < N2; ++k) {
				image_in[i][k] = image_in_flattened[i * N2 + k];
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
		
		image_recieved = 1;

		cout<<"IMG REC = "<<image_recieved<<" "<<" K = "<<kernel_recieved<<" B = "<<biases_recieved<<endl;
	}
	
};

void conv::recieve_biases(void) {
	biases_rdy.write(0);
	wait(clk->posedge_event());


	while(true){
		for (int k = 0; k < BIASES; ++k) {
			biases_rdy.write(1);
			do{
				wait(clk->posedge_event());
			}while(!biases_vld.read());
			biases_in[k]=biases.read();
			biases_rdy.write(0);
		}
		

		cout << "[отладочный вывод][convolution] баесы:" << endl;
		for (int k = 0; k < BIASES; ++k) {
				cout<<biases_in[k]<<endl;
		}
		biases_recieved=1;
	}
};

void conv::recieve_kernel(void) {
	//инициализируем хэндшейк
	kernel_rdy.write(0);
	wait(clk->posedge_event());

	double kernel_in_flattened[KER]={};//вектор, принимающий значения из порта
	
	while(true){
		for (int i=0; i<KER; i++){
			kernel_rdy.write(1);
			do{
				wait(clk->posedge_event());
			}while (!kernel_vld.read());
			
				kernel_in_flattened[i]=kernel.read();
			
			kernel_rdy.write(0);
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
		kernel_recieved=1;
	}
};

void conv::convolution(void) {
		/* double kernel_padded[M3][N3];
	//ZERO-PADDING кернела
	//т.к. кернел после паддинга слева-снизу, то строки перебираем сконца (снизу), а столбцы сначала(слева)
	 for (int i = M3-1; i >= (M3-M1); --i) {
		for (int k = 0; k < N3-1; ++k) {
			kernel_padded[i][k]=kernel[i-1][k].read();
		}
	}  */
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
	double convolved_mat[CONV_ED];
	conv_2d_1_result.write(0);
	conv_2d_1_result_vld.write(0);
	while(true){
		if (kernel_recieved == 1 and image_recieved == 1 and biases_recieved == 1 and conv_done==0){
			//свёртка		
			for (int k = 0; k < L1; k++) {//число кернелов и выходных матрицы соотв-но
				for (int i = 0; i < M3; i++) {
					for (int j = 0; j < N3; j++) {
						for (int m = 0; m < M1; m++) {
							for (int n = 0; n < N1; n++) {
								result[k][i][j] += ReLU(image_in[i + m][j + n] * kernel_in[k][m][n]);
							}
						}
						result[k][i][j] += biases_in[k];
						wait(clk->posedge_event());//предположим за 1 такт вычисляется 1 значение выходной карты
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
			conv_done=1;
			cout<<"@" << sc_time_stamp() <<" convolution layer calculated"<<endl;
			cout << "размеры выходной матрицы: " << endl;
			cout << "M3= " << M3 << " N3= " << N3 << " " << endl << endl;
			
			for (int k = 0; k < L3; k++) {
				for (int i = 0; i < M3; i++) {
					for (int j = 0; j < N3; j++) {
						convolved_mat[k*N3*M3+i*N3+j]=result[k][i][j]; 
					}
				}
			}
		}
		else if( conv_done == 1){
			for (int i=0;i<CONV_ED;i++){
				conv_2d_1_result_vld.write(1);
				do{
					wait(clk->posedge_event());
				}while (!conv_2d_1_result_rdy.read());
				conv_2d_1_result.write(convolved_mat[i]);
				conv_2d_1_result_vld.write(0);
			}	
			conv_2d_1_result.write(0);
			cout<<"@" << sc_time_stamp() <<" convolution data transmitted"<<endl;
			
		}
	
		else{
			wait(clk->posedge_event());
		}
		
	}
};

	
/* 
	/* //конечный вывод из функции и модуля conv
	for (int k = 0; k < L3; k++) {
		for (int i = 0; i < M3; i++) {
			for (int j = 0; j < N3; j++) {
				convolved_mat[k*N3*M3+i*N3+j].write(result[k][i][j]); //равно и .write() дают одно и тоже 
				
			}
		}
	} */