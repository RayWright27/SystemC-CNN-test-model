#include "tb_driver.h"
#include <iomanip>
#include <fstream>
void tb_driver::generate_reset(void){
     //генерация ресета
    rst.write(1);
    wait(clk->posedge_event());//next_trigger();
    rst.write(0);
    wait(clk->posedge_event());//next_trigger();
};

void tb_driver::generate_biases(void) {
    if(biases_generated1 == sc_logic(0)){ 
        std::fstream file(  biases_conv_1_file, std::fstream::in);
        if (!file){
            cout<<"Файл " <<  biases_conv_1_file << " не найден\n";
        }
        for (int i = 0; i < KER; i++){
            file >> biases_flattened[i];
            
        }
        file.close();
        biases.write(0);
        biases_vld.write(0);
        for (int i=0;i<BIASES;i++){
            biases_vld.write(1);
            biases_tmp=biases_flattened[i];
            biases.write(biases_tmp);
            do{
                wait(clk->posedge_event());
//                next_trigger();
            }while(!biases_rdy.read());
            biases_vld.write(0);
        }
        biases.write(0);
        biases_generated1 = sc_logic(1);
    }
    else{
        wait(clk->posedge_event());
//        next_trigger();
    }
};

void tb_driver::generate_kernel(void) {    
    if(kernels_generated1 == sc_logic(0)){
        std::fstream file( kernelfile, std::fstream::in);
        if (!file){
            cout<<"Файл " << kernelfile << " не найден\n";
        }
        for (int i = 0; i < KER; i++){
            file >> kernel_flattened[i];
            
        }
        cout<<"kernel_flattened\n";
        for (int i = 0; i < KER; i++){
            cout << kernel_flattened[i] << ' ';
            
        } /**/ 
        cout<<'\n';
        file.close(); 
    
        kernel.write(0);
        kernel_vld.write(0);

        //поэлементная передача данных на порты
        double kernel_tmp;
        for (int i=0; i<KER; i++){
            kernel_vld.write(1);
            kernel_tmp=kernel_flattened[i];
            kernel.write(kernel_tmp);
            do{
                wait(clk->posedge_event());
        }while(!kernel_rdy.read());
        kernel_vld.write(0);
        }
        kernel.write(0);
        kernels_generated1 = sc_logic(1);
    }
    else{
        wait(clk->posedge_event());
    }
};


void tb_driver::generate_image(void){
    if(image_generated == sc_logic(0)){
        std::fstream file( imagefile, std::fstream::in);
        if (!file){
            cout<<"Файл " << imagefile << " не найден\n";
        }
        for (int i = 0; i < IMG; i++){
            file >> image_flattened[i];
            
        }
        file.close();
        
            cout<<"------------image_flattened-----------"<<endl;
        for (int i=0;i<IMG;i++){
            cout<<std::fixed <<std::setprecision(35)<< image_flattened[i]<< " \n";
        } 
        /* */
        //поэлементная передача данных на порты
        image.write(0);
        image_vld.write(0);
            double image_tmp;
            for (int i=0; i<IMG; i++){
                image_vld.write(1);
                image_tmp=image_flattened[i];
                image.write(image_tmp);
                do{
                    wait(clk->posedge_event());
            }while(!image_rdy.read());
            image_vld.write(0);
            }
        image.write(0);
        image_generated = sc_logic(1);
    }
    else{
        wait(clk->posedge_event());
    }
};

void tb_driver::conv_2d_1_sink(void){
    //инициализируем хэндшейк
    conv_2d_1_result_rdy.write(0);
    wait(clk->posedge_event());

    double conv_2d_1_result_flattened[CONV_ED];
    //double conv_2d_1_result_arr[L3][M3][N3];
    while(true){
        if (conv_2d_1_fetched == sc_logic(0)){
            for(int i=0;i<CONV_ED;i++){
                conv_2d_1_result_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!conv_2d_1_result_vld.read());
                conv_2d_1_result_flattened[i]=conv_2d_1_result.read();
                conv_2d_1_result_rdy.write(0);
                //fprintf(outfp, "%f\n", conv_2d_1_result.read());
            }
            cout<<"@" << sc_time_stamp() <<" convolution data recieved from CONV_2D_1"<<endl;
            /* 
            for (int i=0;i<CONV_ED;i++){
                cout<<conv_2d_1_result_flattened[i]<<" ";
            } */
            
            for (int i = 0; i < N3; i++) {
                    for (int j = 0; j < M3; j++) {
                        for (int k = 0; k < L3; k++) {
                            conv_2d_1_result_arr[i][j][k] = 
                             conv_2d_1_result_flattened[i*L3*M3+j*L3+k]; 
                        }
                    }
            } 
            #ifdef TB_OUTPUT    
                cout<<"[отладочный вывод][DRI_TB] результат CONV_2D_1: "<<endl;
                for (int i = 0; i < N3; i++) {
                    for (int j = 0; j < M3; j++) {
                        for (int k = 0; k < L3; k++) {
                            cout<<std::fixed <<std::setprecision(35)<< conv_2d_1_result_arr[i][j][k] << "\n ";
                        }
                    }
                } 
                cout << endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl<< endl << endl;
            #endif

            conv_2d_1_fetched = sc_logic(1);
/**/    }
        else{
            wait(clk->posedge_event());
        }
        
        //sc_stop();
    }
};

void tb_driver::generate_kernel2(void){
    if(kernels_generated2 == sc_logic(0)){
    //передача кернелов для conv_2d_2 из файла в динамический массив kernel_flattened2
        std::fstream file( kernel2file, std::fstream::in);
        if (!file){
            cout<<"Файл " << kernel2file << " не найден\n";
        }
        for (int i = 0; i < KER2; i++){
            file >> kernel2_flattened[i];
            
        }
        cout<<"kernel2_flattened\n";
        for (int i = 0; i < KER2; i++){
            cout << kernel2_flattened[i] << ' ';
            
        } /**/
        file.close(); 

        kernel2.write(0);
        kernel2_vld.write(0);

        //поэлементная передача данных на порты
        double kernel2_tmp;
        for (int i = 0; i < KER2; i++){
            kernel2_vld.write(1);
            kernel2_tmp=kernel2_flattened[i];
            kernel2.write(kernel2_tmp);
            do{
                wait(clk->posedge_event());
            }while(!kernel2_rdy.read());
            kernel2_vld.write(0);
        }
        kernel2.write(0);
        kernels_generated2 = sc_logic(1);
    }
    else{
        wait(clk->posedge_event());
    }
};

void tb_driver::generate_biases2(void){
    std::fstream file(  biases_conv_2_file, std::fstream::in);
    if (!file){
        cout<<"Файл " <<  biases_conv_2_file << " не найден\n";
    }
    for (int i = 0; i < KER; i++){
        file >> biases_flattened[i];
        
    }
    file.close();
    biases2.write(0);
    biases2_vld.write(0);
    
    double biases2_tmp;
    for (int i = 0 ;i < BIASES2;i++){
        biases2_vld.write(1);
        biases2_tmp=biases2_flattened[i];
        biases2.write(biases2_tmp);
        do{
            wait(clk->posedge_event());
        }while(!biases2_rdy.read());
        biases2_vld.write(0);
    }
    biases2.write(0);
};

void tb_driver::conv_2d_2_sink(void){
    //инициализируем хэндшейк
    conv_2d_2_result_rdy.write(0);
    //wait(clk->posedge_event());
    while(true){
        if (conv_2d_2_fetched == sc_logic(0)){
            for(int i=0;i<CONV_ED2;i++){
                conv_2d_2_result_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!conv_2d_2_result_vld.read());
                conv_2d_2_result_flattened[i]=conv_2d_2_result.read();
                conv_2d_2_result_rdy.write(0);
                //fprintf(outfp, "%f\n", conv_2d_1_result.read());
            }
            cout<<"@" << sc_time_stamp() <<" convolution data from CONV_2D_2 recieved"<<endl;
            #ifdef TB_OUTPUT 
            for(int i=0;i<CONV_ED2+1;i++){
               cout<<std::setprecision(35)<<
               std::fixed<<conv_2d_2_result_flattened[i]<<endl;
            }/*
            for (int i = 0; i < N5; i++) {
                    for (int j = 0; j < M5; j++) {
                        for (int k = 0; k < L4; k++) {
                            conv_2d_2_result_arr[i][j][k]=conv_2d_2_result_flattened[i*M5*L4+j*L4+k+1]; 
                        }
                    }
            } 
           
                cout<<"[отладочный вывод][DRI_TB] результат CONV_2D_2:"<<endl;
                
                for (int i = 0; i < N5; i++) {
                    for (int j = 0; j < M5; j++) {
                            for (int k = 0; k < L4; k++) {
                            cout <<std::setprecision(35)<<std::fixed<<conv_2d_2_result_arr[i][j][k] << "\n ";
                        }
                       
                    } 
                }/**/
                cout << "_________________[CONV_2D_2]" << endl;
            #endif 
            conv_2d_2_fetched = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};/**/

void tb_driver::max_pool_2d_1_sink(void){
    max_pool_2d_1_result_rdy.write(0);
    while(true){
        if (max_pool_2d_1_fetched == sc_logic(0)){
            for(int i = 0; i < POOL_ED; i++){
                max_pool_2d_1_result_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!max_pool_2d_1_result_vld.read());
                max_pool_2d_1_result_flattened[i]=max_pool_2d_1_result.read();
            }
            cout<<"@" << sc_time_stamp() <<" data from MAX_POOL_2D_1 recieved"<<endl;
            for (int i = 0; i < POOLOUT1; i++) {
                    for (int j = 0; j < POOLOUT2; j++) {
                        for (int k = 0; k < POOLOUT3; k++) {
                            max_pool_2d_1_result_arr[i][j][k] =
                             max_pool_2d_1_result_flattened[i*POOLOUT2*POOLOUT3+j*POOLOUT3+k+1]; 
                        }
                    }
            } 
            #ifdef TB_OUTPUT 
                cout<<"[отладочный вывод][DRI_TB] результат MAX_POOL_2D_1:"<<endl;
                for (int i = 0; i < POOLOUT1; i++) {
                    for (int j = 0; j < POOLOUT2; j++) {
                        for (int k = 0; k < POOLOUT3; k++) {
                            cout <<std::setprecision(35)<<std::fixed<< 
                            max_pool_2d_1_result_arr[i][j][k] << "\n ";
                        }
                    }
                }
                 cout << "_________________[MAX_POOL_2D_1]" << endl;
            #endif 
            max_pool_2d_1_fetched == sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
    
};/**/

void tb_driver::generate_coeff(void){
    std::fstream file( coefffile, std::fstream::in);
    if(!file){
        cout<<"Файл "<<coefffile<<" не найден\n";
    }
    for (int i = 0; i < DENSE1_COEFF; i++){
        file >> coeff_flattened[i];
    }
    /*
    cout<<"------------------------------------------------------------------"<<endl;
    for (int i = 0; i < DENSE_COEFF; i++){
        cout<<std::setprecision(10)<<std::fixed
        <<coeff_flattened[i]<<endl;
    }
    cout<<"------------------------------------------------------------------"<<endl;
    /**/
    
    file.close();
    double coeff_tmp;
    coeff.write(0);
    coeff_vld.write(0);
    for (int i = 0; i < DENSE1_COEFF; i++){
        coeff_vld.write(1);
        coeff_tmp=coeff_flattened[i];
        coeff.write(coeff_tmp);
        do{
            wait(clk->posedge_event());
        }while(!coeff_rdy.read());
        coeff_vld.write(0);
    }
    coeff.write(0);
    
};/**/

void tb_driver::generate_biases3(void){
    std::fstream file( biasesfile, std::fstream::in);
    if(!file){
        cout<<"Файл "<<biasesfile<<" не найден\n";
    }
    for (int i = 0; i < BIASES3; i++){
        file >> biases3_arr[i];
    }
    file.close();

    biases3.write(0);
    biases3_vld.write(0);
    for (int i = 0; i < BIASES3; i++){
        biases3_vld.write(1);
        biases3.write(biases3_arr[i]);
        do{
            wait(clk->posedge_event());
        }while(!biases3_rdy.read());
        biases3_vld.write(0);
    }
};

void tb_driver::dense1_sink(void){
    dense1_result_rdy.write(0);
    while(true){
        if(dense1_fetched == sc_logic(0)){
            for(int i = 0; i < DENSE1_OUT; i++){
                dense1_result_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!dense1_result_vld.read());
                dense1_result_arr[i]=dense1_result.read();
                dense1_result_rdy.write(0);
            }
            cout<<"@" << sc_time_stamp() 
            <<" data from DENSE1 recieved"<<endl;
            #ifdef TB_OUTPUT
            cout<<"____________________________________"<<endl; 
            for(int i = 0; i < DENSE1_OUT; i++){
                cout<<std::setprecision(35)<<std::fixed
                <<dense1_result_arr[i]<<endl;
            }
            cout<<"____________________________________[DENSE1]"<<endl;
            #endif 
            dense1_fetched = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};/**/

void tb_driver::generate_coeff2(void){
    std::fstream file2( coefffile2, std::fstream::in);
    if(!file2){
        cout<<"Файл "<<coefffile2<<" не найден\n";
    }
    for (int i = 0; i < DENSE2_COEFF; i++){
        file2 >> coeff2_flattened[i];
    }
    /*
    cout<<"------------------------------------------------------------------"<<endl;
    for (int i = 0; i < DENSE2_COEFF; i++){
        cout<<std::setprecision(10)<<std::fixed
        <<coeff2_flattened[i]<<endl;
    }
    cout<<"------------------------------------------------------------------"<<endl;
    */
   
    file2.close();
    double coeff2_tmp;
    coeff2.write(0);
    coeff2_vld.write(0);
    for (int i = 0; i < DENSE2_COEFF; i++){
        coeff2_vld.write(1);
        coeff2_tmp=coeff2_flattened[i];
        coeff2.write(coeff2_tmp);
        do{
            wait(clk->posedge_event());
        }while(!coeff2_rdy.read());
        coeff2_vld.write(0);
    }
    coeff2.write(0);
    
};

void tb_driver::generate_biases4(void){
    std::fstream file( biasesfile2, std::fstream::in);
    if(!file){
        cout<<"Файл "<<biasesfile2<<" не найден\n";
    }
    for (int i = 0; i < BIASES4; i++){
        file >> biases4_arr[i];
    }
    file.close();

    biases4.write(0);
    biases4_vld.write(0);
    for (int i = 0; i < BIASES4; i++){
        biases4_vld.write(1);
        biases4.write(biases4_arr[i]);
        do{
            wait(clk->posedge_event());
        }while(!biases4_rdy.read());
        biases4_vld.write(0);
    }
};

void tb_driver::dense2_sink(void){
    dense2_result_rdy.write(0);
    while(true){
        if(dense2_fetched == sc_logic(0)){
            for(int i = 0; i < DENSE2_OUT; i++){
                dense2_result_rdy.write(1);
                do{
                    wait(clk->posedge_event());
                }while (!dense2_result_vld.read());
                dense2_result_arr[i]=dense2_result.read();
                dense2_result_rdy.write(0);
            }
            cout<<"@" << sc_time_stamp() 
            <<" data from DENSE2 recieved"<<endl;
            #ifdef TB_OUTPUT
            cout<<"____________________________________"<<endl; 
            for(int i = 0; i < DENSE2_OUT; i++){
                cout<<std::setprecision(64)<<std::fixed
                <<dense2_result_arr[i]<<endl;
            }
            cout<<"____________________________________[DENSE2]"<<endl;
            #endif 
            dense2_fetched = sc_logic(1);
        }
        else{
            wait(clk->posedge_event());
        }
    }
};/**/

