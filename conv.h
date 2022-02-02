#include <macro.h>
#include <systemc.h>

SC_MODULE(conv) {
	//порты
	sc_in<bool> clk, rst;
	sc_in<bool> kernel_vld;
	sc_out<bool> kernel_rdy;
	sc_in<bool> image_vld;
	sc_out<bool> image_rdy;
	sc_in<double> biases;
	sc_in<double> kernel;
	sc_in<double> image;

	void recieve_kernel(void);
	void recieve_image(void);

	SC_CTOR(conv){
		SC_CTHREAD(recieve_kernel, clk.pos());
		SC_CTHREAD(recieve_image, clk.pos());
		reset_signal_is(rst, true);
	}
};
		
		
	