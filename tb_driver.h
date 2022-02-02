
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in<bool> clk;
    sc_in<bool> kernel_rdy;
    sc_out<bool> kernel_vld;
    sc_in<bool> image_rdy;
    sc_out<bool> image_vld;
    sc_out<bool> rst;
    sc_out<double> kernel, image, biases;
    
    void generate_reset(void);
    void generate_kernel(void);
    void generate_image(void);
    void generate_biases(void);
	
	
	SC_CTOR(tb_driver) {
        SC_THREAD(generate_reset);
        SC_CTHREAD(generate_kernel, clk.pos());
        SC_CTHREAD(generate_image, clk.pos()); 
        
        
    };
};