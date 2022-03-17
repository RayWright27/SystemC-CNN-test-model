#include "dense.h"
#include <math.h>
#include <assert.h>
void softmax(double* input, size_t size) {

//	assert(0 <= size <= sizeof(input) / sizeof(double));

	int i;
	double m, sum, constant;

	m = -INFINITY;
	for (i = 0; i < size; ++i) {
		if (m < input[i]) {
			m = input[i];
		}
	}

	sum = 0.0;
	for (i = 0; i < size; ++i) {
		sum += exp(input[i] - m);
	}

	constant = m + log(sum);
	for (i = 0; i < size; ++i) {
		input[i] = exp(input[i] - constant);
	}

}

void dense::recieve_input(void){
    input_rdy.write(0);
    while(true){
        if(input_recieved == sc_logic(0)){
            while(true){
                for(int i = 0; i < IN_param; i++){ 
                    input_rdy.write(1);
                    do{
                        wait(clk->posedge_event());
                    }while(!input_vld.read());
                    dense_input[i]=input.read();
                    input_rdy.write(0);
                }
                input_recieved = sc_logic(1);
/*               for(int i = 0; i < IN_param; i++){
                    cout<<dense_input[i]<<endl;
                }
                cout<<endl;*/
                cout<<"@"<<sc_time_stamp()<<" dense input recieved ["<<this<<"]"<<endl;
            }
                
        }
        else if(input_recieved == sc_logic(1)){
            input_rdy.write(0);
            wait(clk->posedge_event());
            
        }
    }
};
 
void dense::recieve_coeff(void){
    coeff_rdy.write(0);
    while(true){
        if(coeff_recieved == sc_logic(0)){
            for(int i = 0; i < DENSE_COEFF_param; i++){
                coeff_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while(!coeff_vld.read());
                coeff_flattened[i] = coeff.read();
                coeff_rdy.write(0);
            }
            for(int j = 0; j < DENSE_COEFF1_param; j++){
                for(int i = 0; i < DENSE_COEFF2_param; i++){
                    coeff_arr[j][i] = coeff_flattened[DENSE_COEFF2_param * j + i];
                }
            }
            coeff_recieved = sc_logic(1);
            cout<<"@"<<sc_time_stamp()<<" dense coeff recieved ["<<this<<"]"<<endl;
/*           for(int j = 0; j < DENSE_COEFF2; j++){
                for(int i = 0; i < DENSE_COEFF1; i++){
                    cout<<coeff_arr[j][i]<<" ";
                }
                cout<<endl;
            }
            cout<<endl<<endl<<endl<<endl;
            */
        }
        else{
            coeff_rdy.write(0);
            wait(clk->posedge_event());
        }   
    }
};

void dense::recieve_biases(void){
    biases_rdy.write(0);
    while(true){
        if(biases_recieved == sc_logic(0)){
            for(int i = 0; i < BIASES_param; i++){
                biases_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while(!biases_vld.read());
                biases_arr[i]=biases.read();
                biases_rdy.write(0);
            }
            biases_recieved = sc_logic(1);
            cout<<"@"<<sc_time_stamp()<<" dense biases recieved ["
            <<this<<"]"<<endl;

        }
        else{
            biases_rdy.write(0);
            wait(clk->posedge_event());
        }
    }
};

void dense::dense_func(void) {
    while(true){
        if(biases_recieved == sc_logic(1) and coeff_recieved == sc_logic(1) and input_recieved == sc_logic(1) and dense_done == sc_logic(0)){
            for(int i = 0; i < DENSE_COEFF1_param; i++) {
                for (int j = 0; j < DENSE_COEFF2_param; j++) {
                    dense_result_arr[j] += coeff_arr[i][j] * dense_input[i];
                    wait(clk->posedge_event());    
                }			
            }
             for (int i = 0; i < DENSE_COEFF2_param; i++){
                dense_result_arr[i] += biases_arr[i];
                if (func==1){
                    if (dense_result_arr[i] <= 0) {
                        dense_result_arr[i]=0;
                        //wait(clk->posedge_event());
                        //next_trigger();
                    }
                }
                wait(clk->posedge_event());
            }
            if (func==2){
                softmax(dense_result_arr, DENSE_COEFF2_param);
            }
            cout<<"@" << sc_time_stamp() <<" dense layer calculated ["
            <<this<<"]"<<endl;
            dense_done = sc_logic(1);
            /*
             cout<<"-------------------------------------------------------------"<<endl;
            for (int i = 0; i < DENSE_OUT1; i++) {
                cout << dense_result_arr[i] <<endl;
            }
            cout<<"-------------------------------------------------------------"<<endl;
            cout <<endl<< endl;*/ 
            /**/
        }
        else{
            wait(clk->posedge_event());
        }
    }
    /**/
};

void dense::send_to_dri_tb(void){
    dense_result_tb.write(0);
    while(true){
        if(dense_done == sc_logic(1) and dense_result_sent_tb == sc_logic(0)){
            for(int i = 0; i < OUT_param; i++){
                dense_result_vld_tb.write(1);
                do{
                    wait(clk->posedge_event());
                }while(!dense_result_rdy_tb.read());
                dense_result_tb.write(dense_result_arr[i]);
                dense_result_vld_tb.write(0);
            }
            dense_result_tb.write(0);
            cout<<"@" << sc_time_stamp() 
            <<" dense layer output sent to dri_tb ["
            <<this<<"]"<<endl;
            dense_result_sent_tb == sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};

void dense::send_to_next(void){
    dense_result_next.write(0);
    while(true){
        if(dense_done == sc_logic(1) and dense_result_sent_next == sc_logic(0)){
            for(int i = 0; i < OUT_param; i++){
                dense_result_vld_next.write(1);
                do{
                    wait(clk->posedge_event());
                }while(!dense_result_rdy_next.read());
                dense_result_next.write(dense_result_arr[i]);
                dense_result_vld_next.write(0);
            }
            dense_result_next.write(0);
            cout<<"@" << sc_time_stamp() <<" dense layer output sent to next layer ["
            <<this<<"]"<<endl;
            dense_result_sent_next = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};