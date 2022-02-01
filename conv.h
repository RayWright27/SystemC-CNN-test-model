#include <macro.h>
#include <systemc.h>

SC_MODULE(conv) {
	//порты
	sc_in<bool> clk, rst_n;
	sc_in<double> biases[BIASES];
	sc_in<double> kernel[KER];
	sc_in<double> image[IMG];
	//sc_out<double> convolved_mat[21632];
	//sc_out<float> convoutput[2000];
	void convolution(void);//делаем kernel таких же размеров как и выходную матрицу
	
	SC_CTOR(conv){
		SC_METHOD(convolution);
		sensitive_pos << clk; // позитивный clock
        sensitive_neg << rst_n; // негативный сброс
	/* 		for (int i = 0; i <2; ++i) {
            for (int k = 0; k < 2; ++k) {
                kernel_padded[i][k]=kernel[i][k];
            }
        }
	for (int i = 0; i < M3; ++i) {
        for (int j = 0; j < N3; ++j) {
            cout << kernel[i][j] << " ";
        }
        cout << endl;
    } */
	
	}
	//наверное так же как и в matrix_mul надо без методов
};
		
		
	