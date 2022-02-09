
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in<bool> clk;
    sc_out<bool> rst;
    
    sc_in<bool> kernel_rdy;
    sc_out<bool> kernel_vld;

    sc_in<bool> kernel2_rdy;
    sc_out<bool> kernel2_vld;

    sc_in<bool> image_rdy;
    sc_out<bool> image_vld;

    sc_in<bool> biases_rdy;
    sc_out<bool> biases_vld;

    sc_in<bool> biases2_rdy;
    sc_out<bool> biases2_vld;

    sc_in<bool> conv_2d_1_result_vld;
    sc_out<bool> conv_2d_1_result_rdy;

    sc_in<double> conv_2d_1_result;
    sc_out<double> kernel, image, biases, kernel2, biases2;
    
    void generate_reset(void);
    void generate_kernel(void);
    void generate_image(void);
    void generate_biases(void);
    void conv_2d_1_sink(void);

    void generate_kernel2(void);
    void generate_biases2(void);
	
	FILE *outfp;

	SC_CTOR(tb_driver) {
        SC_THREAD(generate_reset);
        SC_THREAD(generate_kernel);
        SC_THREAD(generate_image); 
        SC_THREAD(generate_biases);
        SC_THREAD(conv_2d_1_sink);
        SC_THREAD(generate_kernel2);
        SC_THREAD(generate_biases2);
        
    };
};