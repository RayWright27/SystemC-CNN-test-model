#include <systemc.h>
#include <macro.h>

#define DENSE_KER2 10 //выходная длина вектора
SC_MODULE(dense) {
	sc_in<bool> clk, rst_n;
	sc_in<double> dense_input[POOL_ED];
	sc_out<double> dense_output[DENSE_KER2];

	SC_CTOR(dense) {
		SC_METHOD(dense_func);
		sensitive_pos << clk; // позитивный clock
		sensitive_neg << rst_n; // негативный сброс
	}

	void dense_func(void) {
		double input[POOL_ED];
		for (int i = 0; i < POOL_ED; i++) {
			input[i] = dense_input[i].read();
		}
		for (int i = 0; i < DENSE_KER1; i++) {
			cout << input[i] << " ";
		}
		cout << endl;

		double kernel[DENSE_KER1][DENSE_KER2];
		for (int i = 0; i < DENSE_KER2; i++) {
			for (int j = 0; j < DENSE_KER1; j++) {
				kernel[i][j] = 1;//rand() % 2;/// ИЗ-ЗА ЭТОГО ЗНАЧЕНИЯ В ПОСЛЕДНЕМ СЛОЕ ВСЕГДА РАЗНЫЕ!
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

		double biases[BIASES] = { 2 };

		double result[DENSE_KER2];
		for(int i = 0; i < DENSE_KER1; i++) {
			for (int j = 0; j < DENSE_KER2; j++) {
				result[j] += kernel[i][j] * input[i];
				
			}			
		}



		for (int i = 0; i < DENSE_KER2; i++) {
			cout << result[i] << " ";
		}
		cout << endl;
		//cout<<"@" << sc_time_stamp() <<" dense layer calculated"<<endl;
	}
};