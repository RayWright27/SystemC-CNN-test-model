#include <systemc.h>
#include <macro.h>

#define DENSE_KER2 10 //выходная длина вектора
SC_MODULE(dense) {
	sc_in<bool> clk, rst_n;
	sc_in<sc_int<DT_LENGTH>> dense_input[POOL_ED];
	sc_out<sc_int<DT_LENGTH>> dense_output[DENSE_KER2];

	SC_CTOR(dense) {
		SC_METHOD(dense_func);
		sensitive_pos << clk; // позитивный clock
		sensitive_neg << rst_n; // негативный сброс
	}

	void dense_func(void) {
		sc_int<DT_LENGTH> input[POOL_ED];
		for (int i = 0; i < POOL_ED; i++) {
			input[i] = dense_input[i].read();
		}
		for (int i = 0; i < DENSE_KER1; i++) {
			cout << input[i] << " ";
		}
		cout << endl;

		sc_int<DT_LENGTH> kernel[DENSE_KER1][DENSE_KER2];
		for (int i = 0; i < DENSE_KER2; i++) {
			for (int j = 0; j < DENSE_KER1; j++) {
				kernel[i][j] = rand() % 10;
			}
		}
		cout << "[debugging output][dense] kernel size:" << " DENSE_KER1 = " << DENSE_KER1 << " DENSE_KER2 = " << DENSE_KER2 << endl;
		/*cout << "[debugging output][dense] kernel: " << endl;
		for (int i = 0; i < DENSE_KER2; i++) {
			for (int j = 0; j < DENSE_KER1; j++) {
				cout << kernel[i][j] << " ";
			}
			cout << endl;
		}*/
		cout << endl;

		sc_int<DT_LENGTH> biases[BIASES] = { 2 };

		sc_int<DT_LENGTH> result[DENSE_KER2];
		for(int i = 0; i < DENSE_KER1; i++) {
			for (int j = 0; j < DENSE_KER2; j++) {
				result[j] += kernel[i][j] * input[i];
				
			}
			
		}

		for (int i = 0; i < DENSE_KER2; i++) {
			cout << result[i] << " ";
		}
		cout << endl;
	}
};