
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in <bool> clk;
    sc_out<bool> rst_n;
    sc_out<double> kernel[KER], image[IMG], biases[BIASES];

    void generate_input(void);
	void generate_reset(void);
	
	SC_CTOR(tb_driver) {
        SC_THREAD(generate_input); //создает THREAD, происходит срабатывание путём изменения входного сигнала, приостановка происходит при wait() пока сигнал из sensetive листа не переключится снова 
        sensitive_neg << clk;
        SC_THREAD(generate_reset);
    };
};