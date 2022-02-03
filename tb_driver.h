
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in<bool> clk;
    sc_out<bool> rst;
    
    sc_in<bool> kernel_rdy;
    sc_out<bool> kernel_vld;

    sc_in<bool> image_rdy;
    sc_out<bool> image_vld;

    sc_in<bool> biases_rdy;
    sc_out<bool> biases_vld;

    sc_in<bool> conv_2d_1_result_vld;
    sc_out<bool> conv_2d_1_result_rdy;

    sc_in<double> conv_2d_1_result;
    sc_out<double> kernel, image, biases;
    
    void generate_reset(void);
    void generate_kernel(void);
    void generate_image(void);
    void generate_biases(void);
    void conv_2d_1_sink(void);
	
	FILE *conv_2d_1_file;
    
	SC_CTOR(tb_driver) {
        SC_THREAD(generate_reset);
        SC_THREAD(generate_kernel);
        SC_THREAD(generate_image); 
        SC_THREAD(generate_biases);
        SC_THREAD(conv_2d_1_sink);
        
    };
};