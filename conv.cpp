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
				wait(clk->posedge_event());
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
			for (int k = 0; k < N2_param; ++k) {
				for (int j = 0; j < M2_param; j++){
					for (int i = 0; i < C1_param; ++i) {
						image_in[k][j][i] = 
						image_in_flattened[k * M2_param * C1_param + j *C1_param + i]; //можно цикл по C1 сделать в конце
					}
				}
			}
			cout << "[отладочный вывод]["<< this <<"] изображение:" << endl;
			cout <<"размеры: "<< M2_param <<" "<< N2_param<<" " << C1_param<<endl;
			for (int k = 0; k < N2_param; ++k) {
				for (int j = 0; j < M2_param; j++){
					for (int i = 0; i < C1_param; ++i) {
					
						cout <<std::fixed <<std::setprecision(35)<< image_in[k][j][i] << "\n ";
					}
					//cout << endl;
				}
				//cout << endl;
			}
			cout << endl;
			/**/
			image_recieved = sc_logic(1);/*
			cout<<"conv_2d ["<<this<<"]: IMG RECIEVED = "
			<<image_recieved<<" "<<" KER RECIEVED = "<<kernel_recieved
			<<" BIASES RECIEVED = "<<biases_recieved<<endl;/**/
				
			}	
		}
	
};/**/

void conv::recieve_biases(void) {
	biases_rdy.write(0);
	wait(clk->posedge_event());
//	next_trigger();
	while(true){
		if(biases_recieved == sc_logic(1)){
			while(true){
				biases_rdy.write(0);
				wait(clk->posedge_event());
//				next_trigger();
			}
		}
		for (int k = 0; k < BIASES_param; ++k) {
			biases_rdy.write(1);
			do{
				wait(clk->posedge_event());
//				next_trigger();
			}while(!biases_vld.read());
			biases_in[k]=biases.read();
			biases_rdy.write(0);
		}
/*
		cout << "[отладочный вывод]["<<this <<"] баесы:" << endl;
		for (int k = 0; k < BIASES_param; ++k) {
				cout<<biases_in[k]<<endl;
		}/**/
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
		//разворачивание вектора в 3D массив (сделать в отдельный метод) 
		for (int i = 0; i < M1_param; ++i) {
			for (int k = 0; k < N1_param; ++k) {
				for (int j = 0; j < C1_param; ++j) {
					for (int c = 0; c < L1_param; c++){
						kernel_in[i][k][j][c] = kernel_in_flattened[i * N1_param * C1_param * L1_param + 
						k * C1_param * L1_param + j * L1_param + c];
					}
				}
			}
		}/*
		cout<<endl<< "[отладочный вывод]["<< this <<"] кернел:" << endl; 
		for (int i = 0; i < M1_param; i++){	
			for (int k = 0; k < N1_param; ++k) {
				for (int c = 0; c < C1_param; ++c) {
					cout << "разделение из-за С1"<< endl;
					for (int j = 0; j < L1_param; ++j) {
						cout <<std::setprecision(10)<< kernel_in[i][k][c][j] << " ";
					}
					cout << endl;
				}
				
			}
			cout << endl;
		}/**/
		kernel_recieved=sc_logic(1);
		
	}
};

void conv::convolution(void) {
	//while(true){
		if(conv_done == sc_logic(1)){
			//while(true){
				//wait(clk->posedge_event());
				next_trigger();
			//}
		}
		else if (kernel_recieved == sc_logic(1) and image_recieved == sc_logic(1) and biases_recieved == sc_logic(1) and conv_done == sc_logic(0)){
		if (verbose==1){	
			cout<<this<<" N1_param="<<N1_param<<" M1_param="<<M1_param<<" N2_param="<<N2_param<<" M2_param="<<M2_param
			<<" C1_param="<<C1_param<<" N3_param="<<N3_param<<" M3_param="<<M3_param<<" L1_param="<<L1_param<<endl
			<<" IMG_param="<<IMG_param<<" BIASES_param="<<BIASES_param<<" KER_param="<<KER_param<<endl;	
		}
		//свёртка		
		for (int k = 0; k <L1_param; k++) {//число кернелов и выходных матрицы соотв-но
			for (int i = 0; i < M3_param; i++) {//(высота/кол-во строк) выходного изображения
					for (int j = 0; j < N3_param; j++) {//(ширина/кол-во столбцов) выходного изображения
						for (int c = 0; c < C1_param; c++){//количество входных изображений
							for (int m = 0; m < M1_param; m++) {//(высота/кол-во строк) кернела
								for (int n = 0; n < N1_param; n++) {//(ширина/кол-во столбцов) кернела
									result[i][j][k] += image_in[i + m][j + n][c]
									* kernel_in[m][n][c][k];
									if(verbose==1){
									cout<<this<<" "<<n<<" "<<m<<" "<<c<<" "<<j<<" "<<i<<" "<<k<<" "<<endl;
									}
								}		
							}
					    }
						result[i][j][k] += biases_in[k]; // частично отличаются значения, неверно извлёк кернелы?
						if (result[i][j][k] <= 0) {
							result[i][j][k]=0;
							//wait(clk->posedge_event());
							next_trigger();
						}
						else {
							//wait(clk->posedge_event());
							next_trigger();
						}				
				}
				//cout << "_________________" << endl;
				//cout << endl << endl;
			}
		}/**/
			
		/*
			cout<<"[отладочный вывод]["<< this <<"] результат:"<<endl;
			for (int k = 0; k < N3_param; ++k) {
				for (int i = 0; i < M3_param; ++i) {
					for (int j = 0; j < L3_param; ++j) {
						cout <<std::setprecision(25) << result[k][i][j] << " ";
					}
					cout << endl;
				}
				cout << "_________________" << endl;
				cout << endl << endl;
			}
			cout << endl;   
         /**/   
			conv_done = sc_logic(1);
			cout << "@" << sc_time_stamp() <<" "<<this<<" layer calculated"<<endl;
			/*
			cout << "размеры выходной матрицы: " << endl;
			cout << "M3_param= " << M3_param << " N3_param= " << N3_param << " " << endl << endl;
			/**/
		
			for (int i = 0; i < M3_param; i++) {
					for (int j = 0; j < N3_param; j++) {
						for (int k = 0; k < L3_param; k++) {
						convolved_mat[i*N3_param*L3_param+j*L3_param+k]=result[i][j][k];
						
					}
				}
			}/**/
			cout<<this<<" CONV results\n";
			for (int i = 0; i < CONV_ED_param; i++){
				cout<<"["<<i<<"] = "<<convolved_mat[i]<<endl;
			}/**/
		}
		else{
			//wait(clk->posedge_event());
			next_trigger();
		}

	//}

};/**/

void conv::send_to_dri_tb(void){
	//conv_2d_result_tb.write(0);
	conv_2d_result_vld_tb.write(0);
	while(true){
		if ( conv_done == sc_logic(1) and conv_result_sent_tb == sc_logic(0)){
			for (int i = 0; i < CONV_ED_param; i++){
					conv_2d_result_vld_tb.write(1);
					conv_2d_result_tb.write(convolved_mat[i]);
					do{
						wait(clk->posedge_event());
					}while (!conv_2d_result_rdy_tb.read());
					
					conv_2d_result_vld_tb.write(0);
				}	
				conv_2d_result_tb.write(0);
				cout<<"@" << sc_time_stamp() <<" "<<this<<" data transmitted"<<endl;
				conv_result_sent_tb = sc_logic(1);
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
		if (conv_done == sc_logic(1) and conv_result_sent_next == sc_logic(0)){
			for (int i=0;i<CONV_ED_param;i++){
					conv_2d_result_vld_next.write(1);
					conv_2d_result_next.write(convolved_mat[i]);
					
					do{
						wait(clk->posedge_event());
					}while (!conv_2d_result_rdy_next.read());
					conv_2d_result_vld_next.write(0);
					
				}	
			conv_2d_result_next.write(0);
			cout<<"@" << sc_time_stamp() <<" "<< this << " data transmitted"<<endl;
			conv_result_sent_next = sc_logic(1);
		}
		else{
			wait(clk->posedge_event());
		}
	
	}
};/**/