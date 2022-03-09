#include <macro.h>
#include <systemc.h>
	SC_MODULE(conv) {
		SC_HAS_PROCESS(conv);
		int 			M1_param;
		int 			L1_param;
		int 			N1_param;
		int 			KER_param;
		int				M2_param;
		int 			N2_param;
		int 			C1_param;
		int 			IMG_param;
		int 			M3_param;
		int 			N3_param;
		int 			L3_param;
		int 			CONV_ED_param;
		int 			BIASES_param;
		int 			verbose;
		char* 			module_name;
		
		//порты
		sc_in<bool> 	clk, rst;

		sc_in<bool> 	kernel_vld;
		sc_out<bool> 	kernel_rdy;

		sc_in<bool> 	image_vld;
		sc_out<bool> 	image_rdy;

		sc_in<bool> 	biases_vld;
		sc_out<bool> 	biases_rdy;

		sc_in<bool> 	conv_2d_result_rdy_tb;
		sc_in<bool> 	conv_2d_result_rdy_next;
		sc_out<bool> 	conv_2d_result_vld_tb;
		sc_out<bool> 	conv_2d_result_vld_next;

		sc_in<double> 	biases;
		sc_in<double> 	kernel;
		sc_in<double> 	image;
		sc_out<double> 	conv_2d_result_tb;
		sc_out<double> 	conv_2d_result_next;

		sc_logic 		kernel_recieved = sc_logic(0);
		sc_logic 		image_recieved = sc_logic(0);
		sc_logic 		biases_recieved = sc_logic(0);
		sc_logic 		conv_done = sc_logic(0);

		
		double**** 		kernel_in;// = new double***[L1_param];//указатель на динамический массив т.к. в С++ недоступна инициализация массива переменной длинны
		double*** 		image_in;// = new double**[M2_param];
		double* 		biases_in;// = new double[BIASES_param];
		double*** 		result;// = new double**[L3_param];
		double* 		convolved_mat;// = new double[CONV_ED_param];
		double*			image_in_flattened;
		
		void recieve_image(void);
		void recieve_biases(void);
		void recieve_kernel(void);
		void convolution(void);
		void send_to_dri_tb(void);
		void send_to_next_layer(void);

		conv(sc_module_name module_name, int param1, int param2, int param3, int param4,// кастомный конструктор с параметрами для SystemC модуля 
		int param5, int param6, int param7,int param8, int param9, int param10,
		int param11, int param12, int param13, int param14):sc_module(module_name),
		M1_param(param1),N1_param(param2),L1_param(param3), 
		KER_param(param4),M2_param(param5),N2_param(param6),
		C1_param(param7),IMG_param(param8),M3_param(param9),N3_param(param10),
		L3_param(param11),CONV_ED_param(param12),BIASES_param(param13), verbose(param14)
		{
			cout<<"------------------------------"<< module_name <<"["<<this<<"]"<< " MODULE PARAMETERS-------------------------------"<<endl;
			cout<<M1_param<<" "<<L1_param<<" "<<N1_param<<" "<< endl;
			
			//объявление динамического kernel_in
			kernel_in = new double***[M1_param];//выходные
			for (int k = 0; k < M1_param; k++){
				kernel_in[k] = new double**[N1_param];//входные
				for (int i=0; i < N1_param; i++){
					kernel_in[k][i] = new double*[C1_param];
					for (int j = 0; j < C1_param; j++){
						kernel_in[k][i][j] = new double[L1_param];
					}
				}
			}
			//объявление динамического image_in
			image_in = new double**[N2_param];
			for (int j = 0; j < N2_param; j++){
				image_in[j] = new double*[M2_param];
				for (int i = 0; i < M2_param; i++){
					image_in[j][i] = new double[C1_param];		
				}
			}
			//объявление динамического result
			result = new double**[N3_param];
			for (int i=0; i<N3_param;i++){
				result[i] = new double*[M3_param];
				for (int j=0;j<M3_param;j++){
					result[i][j] = new double[L3_param];
				}
			}

			biases_in = new double[BIASES_param];
			convolved_mat = new double[CONV_ED_param];

			image_in_flattened = new double[IMG_param];//вектор, принимающий значения из порта
			
			SC_THREAD(recieve_kernel);
			
			SC_THREAD(recieve_image);
			
			SC_THREAD(recieve_biases);
//			sensitive<<clk.pos();
			SC_METHOD(convolution);
			sensitive<<clk.pos();
			SC_THREAD(send_to_dri_tb); 
			SC_THREAD(send_to_next_layer);/**/
//			reset_signal_is(rst, true);
		}

		~conv(){
			for (int k=0; k<M1_param;k++){
				for (int i=0; i<N1_param;i++){
					for (int j=0;j<C1_param;j++){
						delete[] kernel_in[k][i][j];
					}
					delete[] kernel_in[k][i];
				}
				delete[] kernel_in[k];
			}
			delete[] kernel_in;

			for (int j = 0; j < N2_param; j++){
				for (int i = 0; i < M2_param; i++){
					delete[] image_in[j][i];
				}
				delete[] image_in[j];
			}
			delete[] image_in;

			delete[] biases_in;

			for (int i=0; i<N3_param;i++){
				for (int j=0;j<M3_param;j++){
					delete[] result[i][j];
				}
				delete[] result[i];
			}
			delete[] result;

			delete[] convolved_mat;
			delete[] image_in_flattened;
		} 
	};


		
	