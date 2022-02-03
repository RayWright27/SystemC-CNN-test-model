#include <macro.h>
#include <systemc.h>

SC_MODULE(conv) {
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

	double kernel_in[L1][M1][N1];
	double image_in[M2][N2];
	double biases_in[BIASES];
	double result[L3][M3][N3];

	void recieve_kernel(void);
	void recieve_image(void);
	void recieve_biases(void);
	void convolution(void);

	SC_CTOR(conv){
		SC_THREAD(recieve_kernel);
		SC_THREAD(recieve_image);
		SC_THREAD(recieve_biases);
		SC_THREAD(convolution);
		reset_signal_is(rst, true);
	}
};
		
		
	