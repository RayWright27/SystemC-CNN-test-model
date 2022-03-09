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
        
        for (int k = 0; k < F_M2; k++) {
            for (int i = 0; i < F_M1; i++) {
                for (int j = 0; j < F_M3; j++) {
                    featuremap_in[k][i][j]=featuremap[k * F_M1 * F_M3 + i * F_M3 + j];
                }
            }
        }
        image_recieved = sc_logic(1);
        /*
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
    for (int k = 0; k < POOLOUT2; k++) {
        for (int i = 0; i < POOLOUT1; i++) {
            for (int j = 0; j < POOLOUT3; j++) {
                result[k][i][j]=0;
            }
        }
    }
    while(true){
        if( image_recieved == sc_logic(1)){
            for (int k = 0; k < POOLOUT2; k++) {
                for (int i = 0; i < POOLOUT1; i++) {//сдвиг кернела в матрице признаков
                    for (int j = 0; j < P2; j++) {
                        for (int m = 0; m < P1; m++) {
                            for (int n = 0; n < POOLOUT3; n++) {
                                value = featuremap_in[k * P2 + j][i * P1 + m][n];
                                result[k][i][n] = maximum(result[k][i][n], value);
                                wait(clk->posedge_event());
                            }       
                        }
                    }
                }
            }
            max_pool_done = sc_logic(1);
/*
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
            /**/
            for (int k = 0; k < POOLOUT2; k++) {
				for (int i = 0; i < POOLOUT1; i++) {
					for (int j = 0; j < POOLOUT3; j++) {
						max_pooled[k*POOLOUT1*POOLOUT3+i*POOLOUT3+j]=result[k][i][j]; 
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
    max_pool_result_vld_tb.write(0);
    while(true){
        if( max_pool_done == sc_logic(1) and max_pool_result_sent_tb == sc_logic(0)){
            for(int i = 0; i < POOL_ED; i++){
                max_pool_result_vld_tb.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!max_pool_result_rdy_tb.read());
                max_pool_result_tb.write(max_pooled[i]);
                max_pool_result_vld_tb.write(0);
            }
            max_pool_result_tb.write(0);
            cout<<"@"<<sc_time_stamp()<<" max_pool_2d_1 ["<<this<<"] data transmitted to tb"<<endl;
            max_pool_result_sent_tb = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};

void max_pool::send_to_next(void){
    max_pool_result_vld_next.write(0);
    while(true){
        if( max_pool_done == sc_logic(1) and max_pool_result_sent_next == sc_logic(0)){
            for(int i = 0; i < POOL_ED; i++){
                max_pool_result_vld_next.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!max_pool_result_rdy_next.read());
                max_pool_result_next.write(max_pooled[i]);
                max_pool_result_vld_next.write(0);
            }
            max_pool_result_next.write(0);
            cout<<"@"<<sc_time_stamp()<<" max_pool_2d_1 ["<<this<<"] data transmitted to next layer"<<endl;
            max_pool_result_sent_next = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};