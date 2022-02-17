#include "max_pooling.h"
#include <iomanip>
#include <fstream>
double max_pool::maximum(double a, double b){
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
};

void max_pool::recieve_image(void){
    image_rdy.write(0);
    if( image_recieved == sc_logic(0)){
        for (int i = 0; i < POOL_IN; i++){
            image_rdy.write(1);
            do{
                wait(clk->posedge_event());
            }while (!image_vld.read());
            featuremap[i]=image.read();
            image_rdy.write(0);
        }
        
        for (int k = 0; k < F_M3; k++) {
            for (int i = 0; i < F_M2; i++) {
                for (int j = 0; j < F_M1; j++) {
                    featuremap_in[k][i][j]=featuremap[k * F_M2 * F_M1 + i * F_M1 + j];
                }
            }
        }
        image_recieved = sc_logic(1);
        
        for (int i = 0; i < F_M3; i++) {
            for (int j = 0; j < F_M2; j++) {
                for (int k = 0; k < F_M1; k++) {
                    cout <<std::setprecision(7)<<std::fixed<< featuremap_in[i][j][k] << " ";
                }
                cout << endl;
            }
            cout << "_________" << endl;
        }
        cout << endl;
        /**/
        cout << "[отладочный вывод][max_pooling] размеры кернела:"<< " P1 = " << P1<< " P2 = " << P2<< endl;
        cout << "размеры выходной матрицы: " << endl;
        cout << "POOLOUT1= " << POOLOUT1 << " POOLOUT2= " << POOLOUT2 << " POOLOUT3= " << POOLOUT3<< endl;
        cout << endl;
    }
    else{
        wait(clk->posedge_event());
    }

};

void max_pool::max_pooling(void) {
    for (int k = 0; k < POOLOUT3; k++) {
        for (int i = 0; i < POOLOUT2; i++) {
            for (int j = 0; j < POOLOUT1; j++) {
                result[k][i][j]=0;
            }
        }
    }
    while(true){
        if( image_recieved == sc_logic(1)){
            for (int k = 0; k < POOLOUT3; k++) {
                for (int i = 0; i < POOLOUT2; i++) {//сдвиг кернела в матрице признаков
                    for (int j = 0; j < POOLOUT1; j++) {
                        for (int m = 0; m < P1; m++) {
                            for (int n = 0; n < P2; n++) {
                                value = featuremap_in[k][i * P1 + m][j * P2 + n];
                                result[k][i][j] = maximum(result[k][i][j], value);
                                wait(clk->posedge_event());
                            }       
                        }
                    }
                }
            }
            max_pool_done = sc_logic(0);

            cout << "[отладочный вывод][max_pooling] результат" << endl;
            for (int k = 0; k < POOLOUT3; k++) {
                for (int i = 0; i < POOLOUT2; i++) {
                    for (int j = 0; j < POOLOUT1; j++) {
                        cout <<std::setprecision(3)<<std::fixed<< result[k][i][j]<<" ";
                    }
                    cout << endl;
                }
                cout << "_________" << endl;
            }
            cout << endl;
            for (int k = 0; k < POOLOUT3; k++) {
				for (int i = 0; i < POOLOUT2; i++) {
					for (int j = 0; j < POOLOUT1; j++) {
						max_pooled[k*POOLOUT2*POOLOUT1+i*POOLOUT1+j]=result[k][i][j]; 
					}
				}
			}
        }
        else{
            wait(clk->posedge_event());
        }
    }
};

void max_pool::send_to_dri_tb(void){
    result_vld_tb.write(0);
    while(true){
        if( max_pool_done == sc_logic(1)){
            for(int i = 0; i < POOL_ED; i++){
                result_vld_tb.write(0);
                do{
                    wait(clk->posedge_event());
                }while (!result_rdy_tb.read());
                result_max_pool.write(max_pooled[i]);
                result_vld_tb.write(0);
            }
            result_max_pool.write(0);
            cout<<"@"<<sc_time_stamp()<<" max_pool_2d_1 ["<<this<<"] data transmitted to tb"<<endl;
        }
        else{
            wait(clk->posedge_event());
        }
    }
};

void max_pool::send_to_next(void){
    result_vld_next.write(0);
    while(true){
        if( max_pool_done == sc_logic(1)){
            for(int i = 0; i < POOL_ED; i++){
                result_vld_next.write(0);
                do{
                    wait(clk->posedge_event());
                }while (!result_rdy_next.read());
                result_max_pool.write(max_pooled[i]);
                result_vld_next.write(0);
            }
            result_max_pool.write(0);
            cout<<"@"<<sc_time_stamp()<<" max_pool_2d_1 ["<<this<<"] data transmitted to next layer"<<endl;
        }
        else{
            wait(clk->posedge_event());
        }
    }
};