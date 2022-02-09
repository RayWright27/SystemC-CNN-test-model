#include <macro.h>
#include <systemc.h>

		SC_MODULE(conv) {
			SC_HAS_PROCESS(conv);

			int M1_param;
			int L1_param;
			int N1_param;
			int KER_param;
			int M2_param;
			int N2_param;
			int IMG_param;
			int M3_param;
			int N3_param;
			int L3_param;
			int CONV_ED_param;
			int BIASES_param;

			//порты
			sc_in<bool> clk, rst;

			sc_in<bool> kernel_vld;
			sc_out<bool> kernel_rdy;

			sc_in<bool> image_vld;
			sc_out<bool> image_rdy;

			sc_in<bool> biases_vld;
			sc_out<bool> biases_rdy;

			sc_in<bool> conv_2d_1_result_rdy;
			sc_out<bool> conv_2d_1_result_vld;

			sc_in<double> biases;
			sc_in<double> kernel;
			sc_in<double> image;
			sc_out<double> conv_2d_1_result;

			sc_logic kernel_recieved;
			sc_logic image_recieved;
			sc_logic biases_recieved;
			sc_logic conv_done = sc_logic(0);

			double*** kernel_in = new double**[L1_param];//указатель на динамический массив т.к. в С++ недоступна инициализация массива переменной длинны
			//double image_in[M2][N2];
			double** image_in = new double*[M2_param];
			//double biases_in[BIASES];
			double* biases_in = new double[BIASES_param];
			double*** result = new double**[L3_param];
			//double result[L3][M3][N3];

			void recieve_kernel(void);
			void recieve_image(void);
			void recieve_biases(void);
			void convolution(void);

			conv(sc_module_name nm, int param1, int param2, int param3, int param4,// кастомный конструктор с параметрами для SystemC модуля 
			int param5, int param6, int param7,int param8, int param9, int param10,
			int param11, int param12):sc_module(nm),
			M1_param(param1),N1_param(param2),L1_param(param3),
			KER_param(param4),M2_param(param5),N2_param(param6),
			IMG_param(param7),M3_param(param8),N3_param(param9),
			L3_param(param10),CONV_ED_param(param11),BIASES_param(param12){
				cout<<"------------------------------CONV_2D_1 MODULE PARAMETERS----------------"<<endl;
				cout<<M1_param<<" "<<L1_param<<" "<<N1_param<<" "<< endl;
				//объявление динамического kernel_in
				for (int i=0; i<L1_param;i++){
					kernel_in[i] = new double*[M1_param];
					for (int j=0;j<M1_param;j++){
						kernel_in[i][j] = new double[N1_param];
					}
				}
				//объявление динамического image_in
				for (int i=0; i<M2_param;i++){
					image_in[i] = new double[N2_param];
				}
				//объявление динамического result
				for (int i=0; i<L3_param;i++){
					result[i] = new double*[M3_param];
					for (int j=0;j<M3_param;j++){
						result[i][j] = new double[N3_param];
					}
				}
				
				SC_THREAD(recieve_kernel);
				SC_THREAD(recieve_image);
				SC_THREAD(recieve_biases);
				SC_THREAD(convolution);
				reset_signal_is(rst, true);
			}

			
		};


		
	