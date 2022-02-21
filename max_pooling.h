#include <systemc.h>
#include <macro.h>

SC_MODULE(max_pool) {
	
	//порты
	sc_in<bool> 		clk, rst;

	sc_in<bool> 		image_vld;
	sc_out<bool> 		image_rdy;

	sc_in<bool> 		max_pool_result_rdy_tb;
	sc_out<bool> 		max_pool_result_vld_tb;
	sc_in<bool> 		max_pool_result_rdy_next;
	sc_out<bool> 		max_pool_result_vld_next;

	sc_in<double> 		image;
	sc_out<double> 		max_pool_result_tb;
	sc_out<double> 		max_pool_result_next;
	

	sc_logic 			image_recieved = sc_logic(0);
	sc_logic 			max_pool_done = sc_logic(0);
	sc_logic			max_pool_result_sent_tb = sc_logic(0);
	sc_logic			max_pool_result_sent_next = sc_logic(0);

	double 				value = 0;
	double* 			featuremap;
	double*** 			featuremap_in;
	double*** 			result;
	double* 			max_pooled;


	double maximum (double a, double b);
	void recieve_image(void);
	void max_pooling(void);
	void send_to_dri_tb(void);
	void send_to_next(void);

	SC_CTOR(max_pool) {
		cout<<"------------------------------ MAX_POOL_2D_1 "<<"["<<this<<"]"<< "-------------------------------"<<endl;
		featuremap = new double[POOL_IN];

		featuremap_in = new double**[F_M3];
		for (int j = 0; j < F_M3; j++){
			featuremap_in[j] = new double*[F_M2];
			for (int i = 0; i < F_M2; i++){
				featuremap_in[j][i] = new double[F_M1];
			}
		}
		result = new double**[POOLOUT3];
		for (int j = 0; j < POOLOUT3; j++){
			result[j] = new double*[POOLOUT2];
			for (int i = 0; i < POOLOUT2; i++){
				result[j][i] = new double[POOLOUT1];
			}
		}
		max_pooled = new double[POOL_ED];
		SC_THREAD(recieve_image);
		SC_THREAD(max_pooling);
		SC_THREAD(send_to_dri_tb);
		SC_THREAD(send_to_next);
		reset_signal_is(rst, true);
	}

	~max_pool(){
	delete[] featuremap;

	for (int j = 0; j < F_M3; j++){
		for (int i = 0; i < F_M2; i++){
			delete[] featuremap_in[j][i];
		}
		delete[] featuremap_in[j];
	}
	delete[] featuremap_in;

	for (int j = 0; j < POOLOUT3; j++){
		for (int i = 0; i < POOLOUT2; i++){
			delete[] result[j][i];
		}
		delete[] result[j];
	}
	delete[] result;

	delete[] max_pooled;
}
};

