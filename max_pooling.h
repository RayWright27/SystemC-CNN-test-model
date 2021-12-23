#include <systemc.h>
#include <macro.h>

SC_MODULE(max_pool) {
	sc_in<bool> clk, rst_n;
	sc_in<sc_int<DT_LENGTH>> featuremap[CONV_ED];
	sc_out<sc_int<DT_LENGTH>> pooled_featuremap[POOL_ED];

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
			return 0;
	}

	void max_pooling(void) {
		sc_int<DT_LENGTH> featuremap_in[L3][M3][N3];
		for (int k = 0; k < L3; k++) {
			for (int i = 0; i < M3; i++) {
				for (int j = 0; j < N3; j++) {
					featuremap_in[k][i][j]=featuremap[k * N3 * M3 + i * N3 + j].read();//применеяем ReLU
				}
			}
		}
		for (int i = 0; i < L3; i++) {
			for (int j = 0; j < M3; j++) {
				for (int k = 0; k < N3; k++) {
					cout << featuremap_in[i][j][k] << " ";
				}
				cout << endl;
			}
			cout << "_________" << endl;
		}
		cout << endl;
		
		cout << "[отладочный вывод][max_pooling] размеры кернела:"<< " P1 = " << P1<< " P2 = " << P2<< endl;
		cout << "размеры выходной матрицы: " << endl;
		cout << "POOLOUT1= " << POOLOUT1 << " POOLOUT2= " << POOLOUT2 << " POOLOUT3= " << POOLOUT3<< endl;
		cout << endl;
		// сама операция

		sc_int<DT_LENGTH> result[POOLOUT3][POOLOUT1][POOLOUT2];

		for (int k = 0; k < POOLOUT3; k++) {
			for (int i = 0; i < POOLOUT1; i++) {//сдвиг кернела в матрице признаков
				for (int j = 0; j < POOLOUT2; j++) {
					for (int m = 0; m < P1; m++) {
						for (int n = 0; n < P2; n++) {
							sc_int<DT_LENGTH> value = featuremap_in[k][i * P1 + m][j * P2 + n];
							result[k][i][j] = maximum(result[k][i][j], value);
						}
						sc_int<DT_LENGTH> value = 0;
						
					}
				}
			}
		}

		cout << "[отладочный вывод][max_pooling] результат" << endl;
		for (int k = 0; k < POOLOUT3; k++) {
			for (int i = 0; i < POOLOUT1; i++) {
				for (int j = 0; j < POOLOUT2; j++) {
					cout << result[k][i][j]<<" ";
				}
				cout << endl;
			}
			cout << "_________" << endl;
		}
		cout << endl;

		sc_int<DT_LENGTH> TEST[POOL_ED];

		for (int k = 0; k < POOLOUT3; k++) {
			for (int i = 0; i < POOLOUT1; i++) {
				for (int j = 0; j < POOLOUT2; j++) {
					pooled_featuremap[k * POOLOUT1 * POOLOUT2 + i * POOLOUT1 + j]=result[k][i][j];
				}
			}
		}
		cout<<"@" << sc_time_stamp() <<" pooling layer value calculated"<<endl;
		/*for (int j = 0; j < POOL_ED; j++) {
			cout << pooled_featuremap[j] << " ";
		}
		cout << endl;*/
	}
};