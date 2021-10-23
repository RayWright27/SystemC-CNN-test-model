#include <systemc.h>
#include <macro.h>

SC_MODULE(max_pool) {
	sc_in<bool> clk, rst_n;
	sc_in<sc_int<DT_LENGTH>> featuremap[F_M1][F_M2];
	sc_out<sc_int<DT_LENGTH>> pooled_featuremap[POOLOUT1][POOLOUT2];

	SC_CTOR(max_pool) {
		SC_METHOD(max_pooling);
		sensitive_pos << clk; // позитивный clock
		sensitive_neg << rst_n; // негативный сброс
	}

	sc_int<DT_LENGTH> maximum(sc_int<DT_LENGTH> a, sc_int<DT_LENGTH> b){
			if (a > b) {
				return a;
			}
			else if (a < b) {
				return b;
			}
			else if (a = b) {
				return a;
			}
	}

	void max_pooling(void) {

		
		cout << "[отладочный вывод][max_pooling] размеры кернела:"<< " P1 = " << P1<< " P2 = " << P2<< endl;

		cout << "размеры выходной матрицы: " << endl;
		cout << "POOLOUT1= " << POOLOUT2 << " POOLOUT2= " << POOLOUT2 << " " << endl;
		cout << endl;
		// сама операция
		sc_int<DT_LENGTH> result[POOLOUT1][POOLOUT2];
		for (int i = 0; i < POOLOUT1; i++) {
			for (int j = 0; j < POOLOUT2; j++) {
				result[i][j] = 0;
			}
		}

		for (int i = 0; i < POOLOUT1; i++) {//сдвиг кернела в матрице признаков
				for (int j = 0; j < POOLOUT2;j++) {
					for (int m = 0; m < P1;m++) {
						for (int n = 0; n < P2;n++) {
							sc_int<DT_LENGTH> value = featuremap[i * P1 + m][j * P2 + n];
							result[i][j] = maximum(result[i][j],value);
						}
						sc_int<DT_LENGTH> value = 0;
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