#include "conv.h"
#include "ReLU.h"
#include <iomanip>
void conv::recieve_image(void) {
	//инициализируем хэндшейк
	image_rdy.write(0);
	wait(clk->posedge_event());
	double image_in_flattened[IMG_param];//вектор, принимающий значения из порта
	while(true){
		if(image_recieved == sc_logic(1)){
			while(true){
				image_rdy.write(0);
				wait(clk->posedge_event());
			}
		}
		else{
			for (int i=0; i<IMG_param; i++){
				image_rdy.write(1);
				do{
					wait(clk->posedge_event());
				}while (!image_vld.read());
				image_in_flattened[i]=image.read();
				image_rdy.write(0);			
			}
			for (int k = 0; k < C1_param; ++k) {
				for (int j = 0; j < M2_param; j++){
					for (int i = 0; i < N2_param; ++i) {
					
						image_in[k][j][i] = image_in_flattened[k * M2_param * N2_param + j *N2_param + i]; //можно цикл по C1 сделать в конце
					}
				}
			}
		/*	
			cout << "[отладочный вывод]["<< this <<"] изображение:" << endl;
			cout <<"размеры: "<< M2_param <<" "<< N2_param<<" " << C1_param<<endl;
			for (int k = 0; k < C1_param; ++k) {
				for (int j = 0; j < M2_param; j++){
					for (int i = 0; i < N2_param; ++i) {
					
						cout <<std::setprecision(3)<< image_in[k][j][i] << " ";
					}
					cout << endl;
				}
				cout << endl;
				cout << endl;
			}
			*/
			image_recieved = sc_logic(1);
			cout<<"IMG RECIEVED = "<<image_recieved<<" "<<" KER RECIEVED = "<<kernel_recieved<<" BIASES RECIEVED = "<<biases_recieved<<endl;
		}
	}	
};/**/

void conv::recieve_biases(void) {
	biases_rdy.write(0);
	wait(clk->posedge_event());
	while(true){
		if(biases_recieved == sc_logic(1)){
			while(true){
				biases_rdy.write(0);
				wait(clk->posedge_event());
			}
		}
		for (int k = 0; k < BIASES_param; ++k) {
			biases_rdy.write(1);
			do{
				wait(clk->posedge_event());
			}while(!biases_vld.read());
			biases_in[k]=biases.read();
			biases_rdy.write(0);
		}
		
/*
		cout << "[отладочный вывод]["<<this <<"] баесы:" << endl;
		for (int k = 0; k < BIASES_param; ++k) {
				cout<<biases_in[k]<<endl;
		}*/
		biases_recieved=sc_logic(1);
	}
};
/**/ 
void conv::recieve_kernel(void) {
	//инициализируем хэндшейк
	kernel_rdy.write(0);
	wait(clk->posedge_event());
	double kernel_in_flattened[KER_param]={};//вектор, принимающий значения из порта
	while(true){
		if(kernel_recieved == sc_logic(1)){
			while(true){
				kernel_rdy.write(0);
				wait(clk->posedge_event());
			}
		}
		for (int i = 0; i < KER_param; i++){
			kernel_rdy.write(1);
			do{
				wait(clk->posedge_event());
			}while (!kernel_vld.read());
			kernel_in_flattened[i] = kernel.read();
			kernel_rdy.write(0);
		}
		//разворачивание вектора в 3D массив (сделать в отдельный метод) (зачем?)
		for (int c = 0; c < C1_param; c++){
			for (int j = 0; j < L1_param; ++j) {
				for (int i = 0; i < M1_param; ++i) {
					for (int k = 0; k < N1_param; ++k) {
						kernel_in[c][j][i][k] = kernel_in_flattened[c * L1_param * M1_param * N1_param + j * M1_param * N1_param + i * N1_param + k];
					}
				}
			}
		}
/*
		cout<<endl<< "[отладочный вывод]["<< this <<"] кернел:" << endl;
		for (int j = 0; j < L1_param; ++j) {
			cout<< "кернел "<<j<<":"<<endl;
			for (int i = 0; i < M1_param; ++i) {
				cout << "разделение из-за С1"<< endl; 
				for (int c = 0; c < C1_param; c++){	
					for (int k = 0; k < N1_param; ++k) {
						cout <<std::setprecision(10)<< kernel_in[j][i][c][k] << " ";
					}
					cout << endl;
				}
				
			}
			cout << endl;
		}*/
		kernel_recieved=sc_logic(1);
		
	}
};

void conv::convolution(void) {
	while(true){
		if(conv_done == sc_logic(1)){
			while(true){
				wait(clk->posedge_event());
			}
		}
		else if (kernel_recieved == sc_logic(1) and image_recieved == sc_logic(1) and biases_recieved == sc_logic(1) and conv_done == sc_logic(0)){
			//свёртка		
			for (int c = 0; c < C1_param; c++){//количество входных изображений
				for (int k = 0; k <L1_param; k++) {//число кернелов и выходных матрицы соотв-но
					for (int i = 0; i < M3_param; i++) {//размерность (высота/кол-во строк) выходного изображения
							for (int j = 0; j < N3_param; j++) {//размерность (ширина/кол-во столбцов) выходного изображения
								for (int m = 0; m < M1_param; m++) {// размерность (высота/кол-во строк) кернела
									for (int n = 0; n < N1_param; n++) {//размерность (ширина/кол-во столбцов) кернела
										result[k][i][j] += image_in[c][i + m][j + n]
										* kernel_in[c][k][m][n];
										
									}		
								}
							
							result[k][i][j] += biases_in[k]; // частично отличаются значения, неверно извлёк кернелы?
							//cout<<"result[k][i][j] = "<<result[k][i][j] << endl;
							if (result[k][i][j] <= 0) {
								result[k][i][j]=0;
								wait(clk->posedge_event());
							//	cout<<"ReLU(result[k][i][j]) = "<<result[k][i][j] << endl;
							}
							else {
								wait(clk->posedge_event());
							//	cout<<"ReLU(result[k][i][j]) = "<<result[k][i][j] << endl;
							}
							//cout << std::setprecision(1) << result[k][i][j] << " ";
							//предположим за 1 такт вычисляется 1 значение выходной карты (для conv_2d_1)
						}
						//cout << endl;
					
					}
					//cout << "_________________" << endl;
					//cout << endl << endl;
				}
			}
			
		
		//	cout<<"[отладочный вывод]["/*<< module_name*/ <<"] результат:"<<endl;
		/*	for (int k = 0; k < L3_param; ++k) {
				for (int i = 0; i < M3_param; ++i) {
					for (int j = 0; j < N3_param; ++j) {
						cout <<std::setprecision(7) << result[k][i][j] << " ";
					}
					cout << endl;
				}
				cout << "_________________" << endl;
				cout << endl << endl;
			}
			cout << endl;   
         */   
			conv_done = sc_logic(1);
			cout << "@" << sc_time_stamp() <<" "<<this<<" layer calculated"<<endl;
			cout << "размеры выходной матрицы: " << endl;
			cout << "M3_param= " << M3_param << " N3_param= " << N3_param << " " << endl << endl;
			

			for (int k = 0; k < L3_param; k++) {
				for (int i = 0; i < M3_param; i++) {
					for (int j = 0; j < N3_param; j++) {
						convolved_mat[k*N3_param*M3_param+i*N3_param+j]=result[k][i][j]; 
					}
				}
			}
		}
		else{
			wait(clk->posedge_event());
		}

	}

};/**/

void conv::send_to_dri_tb(void){
	//conv_2d_result_tb.write(0);
	conv_2d_result_vld_tb.write(0);
	while(true){
		if ( conv_done == sc_logic(1) ){
			for (int i=0;i<CONV_ED_param;i++){
					conv_2d_result_vld_tb.write(1);
					do{
						wait(clk->posedge_event());
					}while (!conv_2d_result_rdy_tb.read());
					conv_2d_result_tb.write(convolved_mat[i]);
					conv_2d_result_vld_tb.write(0);
				}	
				conv_2d_result_tb.write(0);
				cout<<"@" << sc_time_stamp() <<" "<<this<<" data transmitted"<<endl;
		}
		else{
			wait(clk->posedge_event());
		}
	
	}
};/**/

void conv::send_to_next_layer(void){
	//conv_2d_result_next.write(0);
	conv_2d_result_vld_next.write(0);
	while(true){
		if ( conv_done == sc_logic(1)){
			for (int i=0;i<CONV_ED_param;i++){
					conv_2d_result_vld_next.write(1);
					do{
						wait(clk->posedge_event());
					}while (!conv_2d_result_rdy_next.read());
					conv_2d_result_next.write(convolved_mat[i]);
					conv_2d_result_vld_next.write(0);
				}	
				conv_2d_result_next.write(0);
				cout<<"@" << sc_time_stamp() <<" "<< this << " data transmitted"<<endl;
		}
		else{
			wait(clk->posedge_event());
		}
	
	}
};