#include <systemc.h>
#include <macro.h>

SC_MODULE(max_pool) {
	sc_in<bool> clk, rst_n;
	sc_in<sc_int<DT_LENGTH>> pool_kernel[P1][P2], featuremap[F_M1][F_M2];
	sc_out<sc_int<DT_LENGTH>> pooled_featuremap[POOLOUT1][POOLOUT2];




	SC_CTOR(max_pool) {
		SC_METHOD(max_pooling);
		sensitive_pos << clk; // позитивный clock
		sensitive_neg << rst_n; // негативный сброс
	}

	sc_int<DT_LENGTH> maximum(sc_int<DT_LENGTH> a, sc_int<DT_LENGTH> b) {
		if (a > b) {
			return a;
		}
		if (a < b) {
			return b;
		}

	}
	void max_pooling(void) {

		
		cout << "[отладочный вывод][max_pooling] кернел:" << endl;
		for (int i = 0; i < P1; ++i) {
		for (int k = 0; k < P2; ++k) {
			cout << pool_kernel[i][k] << " ";
		}
		cout << endl;
		}

		cout << "размеры выходной матрицы: " << endl;
		cout << "POOLOUT1= " << POOLOUT2 << " POOLOUT2= " << POOLOUT2 << " " << endl;
		cout << endl;
		// сама операция
		sc_int<DT_LENGTH> result[POOLOUT1][POOLOUT2];

		for (int i = 0; i < POOLOUT1; i+P1) {//сдвиг кернела в матрице признаков
			for (int j = 0; j < POOLOUT2; j+P2) {
				for (int m = 0; m < i+P1; m++) {//сравнение значений внутри квадрата, на котором стоит кернел
					for (int n = 0; n < j+P2; n++) {
						result[i][j] = 1;
					}
				}
			}
		}

		for (int i = 0; i < POOLOUT1; i++) {
			for (int k = 0; k < POOLOUT2; k++) {
				pooled_featuremap[i][k].write(result[i][k]);
			}
		}
	}
};
