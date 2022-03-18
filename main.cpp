#include <systemc.h>
#include "tb_driver.h"
#include "conv.h"
#include "max_pooling.h"
#include "dense.h"

//------------------------------------------------
// Передача параметров слоя в конструктор модуля слоя происходит в том же порядке, 
//в котором эти параметры расположены в файле macro.h
//------------------------------------------------

//------------------------------------------------
//ИСПРАВИТЬ ПЕРЕДАЧУ ДАННЫХ ИЗ MAX_POOLING!
//


SC_MODULE(TOP){//топ-модуль нейросетевого ускорителя 
    //объявление модулей
    
    tb_driver   *DRI_TB;
    conv        *CONV_2D_1;
    conv        *CONV_2D_2;
    max_pool    *MAX_POOL_2D_1;
    dense       *DENSE1;
    dense       *DENSE2;
    /**/
    // сигналы
    sc_clock clk;//("clk", 10, SC_NS);
    
    sc_signal<bool>   rst;
    sc_signal<bool>   kernel_rdy_sig;//готовность приёма/передачи данных кернела
    sc_signal<bool>   kernel_vld_sig;//=1 когда данные кернела видны для считывания 
    sc_signal<bool>   image_rdy_sig;
    sc_signal<bool>   image_vld_sig;
    sc_signal<bool>   biases_rdy_sig;
    sc_signal<bool>   biases_vld_sig;
    sc_signal<bool>   conv_2d_1_result_vld_sig_1;
    sc_signal<bool>   conv_2d_1_result_vld_sig_2;
    sc_signal<bool>   conv_2d_1_result_rdy_sig_1;
    sc_signal<bool>   conv_2d_1_result_rdy_sig_2;
    sc_signal<double> kernel_sig, image_sig, biases_sig;
    sc_signal<double> conv_2d_1_result_sig_1;
    sc_signal<double> conv_2d_1_result_sig_2;

    sc_signal<bool>   kernel2_rdy_sig;//готовность приёма/передачи данных кернела
    sc_signal<bool>   kernel2_vld_sig;//=1 когда данные кернела видны для считывания 
    sc_signal<bool>   biases2_rdy_sig;
    sc_signal<bool>   biases2_vld_sig;
    sc_signal<bool>   conv_2d_2_result_vld_sig_1;
    sc_signal<bool>   conv_2d_2_result_rdy_sig_1;
    sc_signal<double> kernel2_sig, biases2_sig;
    sc_signal<double> conv_2d_2_result_sig_1;
    sc_signal<bool>   conv_2d_2_result_vld_sig_2;
    sc_signal<bool>   conv_2d_2_result_rdy_sig_2;
    sc_signal<double> conv_2d_2_result_sig_2;

    sc_signal<double> result_max_pool_sig_1;
    sc_signal<double> result_max_pool_sig_2;
    sc_signal<bool>   max_pool_result_rdy_sig_1;
    sc_signal<bool>   max_pool_result_vld_sig_1;
    sc_signal<bool>   max_pool_result_rdy_sig_2;
    sc_signal<bool>   max_pool_result_vld_sig_2;

    sc_signal<double> coeff_sig;
    sc_signal<bool>   coeff_rdy_sig;
    sc_signal<bool>   coeff_vld_sig;
    sc_signal<double> biases3_sig;
    sc_signal<bool>   biases3_rdy_sig;
    sc_signal<bool>   biases3_vld_sig;
    sc_signal<double> dense1_result_tb_sig;
    sc_signal<double> dense1_result_next_sig;
    sc_signal<bool>   dense1_result_rdy_tb_sig;
    sc_signal<bool>   dense1_result_vld_tb_sig;
    sc_signal<bool>   dense1_result_rdy_next_sig;
    sc_signal<bool>   dense1_result_vld_next_sig;

    sc_signal<double> coeff2_sig;
    sc_signal<bool>   coeff2_rdy_sig;
    sc_signal<bool>   coeff2_vld_sig;
    sc_signal<double> biases4_sig;
    sc_signal<bool>   biases4_rdy_sig;
    sc_signal<bool>   biases4_vld_sig;
    sc_signal<double> dense2_result_tb_sig;
    sc_signal<double> dense2_result_next_sig;
    sc_signal<bool>   dense2_result_rdy_tb_sig;
    sc_signal<bool>   dense2_result_vld_tb_sig;
    sc_signal<bool>   dense2_result_rdy_next_sig;
    sc_signal<bool>   dense2_result_vld_next_sig;
    
    SC_CTOR(TOP):clk("clk",sc_time(2,SC_NS)){//конструктор копирования clk_sig
        //инстанциируем модули и соединения сигналами
        DRI_TB = new tb_driver("tb");
        DRI_TB->clk(clk);
        DRI_TB->rst(rst);
        DRI_TB->kernel(kernel_sig);
        DRI_TB->image(image_sig);
        DRI_TB->biases(biases_sig);
        DRI_TB->kernel_rdy(kernel_rdy_sig);
        DRI_TB->kernel_vld(kernel_vld_sig);
        DRI_TB->image_rdy(image_rdy_sig);
        DRI_TB->image_vld(image_vld_sig);
        DRI_TB->biases_rdy(biases_rdy_sig);
        DRI_TB->biases_vld(biases_vld_sig);
        DRI_TB->conv_2d_1_result(conv_2d_1_result_sig_1);
        DRI_TB->conv_2d_1_result_vld(conv_2d_1_result_vld_sig_1);
        DRI_TB->conv_2d_1_result_rdy(conv_2d_1_result_rdy_sig_1);

        DRI_TB->kernel2(kernel2_sig);
        DRI_TB->kernel2_rdy(kernel2_rdy_sig);
        DRI_TB->kernel2_vld(kernel2_vld_sig);
        DRI_TB->biases2(biases2_sig);
        DRI_TB->biases2_rdy(biases2_rdy_sig);
        DRI_TB->biases2_vld(biases2_vld_sig);
        DRI_TB->conv_2d_2_result(conv_2d_2_result_sig_1);
        DRI_TB->conv_2d_2_result_vld(conv_2d_2_result_vld_sig_1);
        DRI_TB->conv_2d_2_result_rdy(conv_2d_2_result_rdy_sig_1);

        DRI_TB->max_pool_2d_1_result(result_max_pool_sig_1);
        DRI_TB->max_pool_2d_1_result_vld(max_pool_result_vld_sig_1);
        DRI_TB->max_pool_2d_1_result_rdy(max_pool_result_rdy_sig_1);

        DRI_TB->coeff(coeff_sig);
        DRI_TB->coeff_rdy(coeff_rdy_sig);
        DRI_TB->coeff_vld(coeff_vld_sig);
        DRI_TB->biases3(biases3_sig);
        DRI_TB->biases3_rdy(biases3_rdy_sig);
        DRI_TB->biases3_vld(biases3_vld_sig);
        DRI_TB->dense1_result(dense1_result_tb_sig);
        DRI_TB->dense1_result_vld(dense1_result_vld_tb_sig);
        DRI_TB->dense1_result_rdy(dense1_result_rdy_tb_sig);

        DRI_TB->coeff2(coeff2_sig);
        DRI_TB->coeff2_rdy(coeff2_rdy_sig);
        DRI_TB->coeff2_vld(coeff2_vld_sig);
        DRI_TB->biases4(biases4_sig);
        DRI_TB->biases4_rdy(biases4_rdy_sig);
        DRI_TB->biases4_vld(biases4_vld_sig);
        DRI_TB->dense2_result(dense2_result_tb_sig);
        DRI_TB->dense2_result_vld(dense2_result_vld_tb_sig);
        DRI_TB->dense2_result_rdy(dense2_result_rdy_tb_sig);


        CONV_2D_1 = new conv("conv_2d_1", M1, N1, L1, KER, M2, N2, C1, IMG, M3, N3, L3, CONV_ED, BIASES, 0);
        //std::shared_ptr<conv> CONV_2D_1(new conv("conv_2d_1", M1, N1, L1, KER, M2, N2, C1, IMG, M3, N3, L3, CONV_ED, BIASES)); 
        CONV_2D_1->clk(clk);
        CONV_2D_1->rst(rst);
        CONV_2D_1->kernel(kernel_sig);
        CONV_2D_1->image(image_sig);
        CONV_2D_1->biases(biases_sig); 
        CONV_2D_1->kernel_vld(kernel_vld_sig);
        CONV_2D_1->kernel_rdy(kernel_rdy_sig);
        CONV_2D_1->image_vld(image_vld_sig);
        CONV_2D_1->image_rdy(image_rdy_sig);
        CONV_2D_1->biases_vld(biases_vld_sig);
        CONV_2D_1->biases_rdy(biases_rdy_sig);
        CONV_2D_1->conv_2d_result_tb(conv_2d_1_result_sig_1);
        CONV_2D_1->conv_2d_result_next(conv_2d_1_result_sig_2);
        CONV_2D_1->conv_2d_result_rdy_tb(conv_2d_1_result_rdy_sig_1);
        CONV_2D_1->conv_2d_result_rdy_next(conv_2d_1_result_rdy_sig_2);
        CONV_2D_1->conv_2d_result_vld_tb(conv_2d_1_result_vld_sig_1);
        CONV_2D_1->conv_2d_result_vld_next(conv_2d_1_result_vld_sig_2);
        /**/

        CONV_2D_2 = new conv("conv_2d_2", M4, N4, L4, KER2, M3, N3, L3, CONV_ED, M5, N5, L4, CONV_ED2, BIASES2, 0);
        CONV_2D_2->clk(clk);
        CONV_2D_2->rst(rst); 
        CONV_2D_2->kernel(kernel2_sig);
        CONV_2D_2->image(conv_2d_1_result_sig_2);
        CONV_2D_2->biases(biases2_sig);
        CONV_2D_2->kernel_vld(kernel2_vld_sig);
        CONV_2D_2->kernel_rdy(kernel2_rdy_sig);
        CONV_2D_2->image_vld(conv_2d_1_result_vld_sig_2);
        CONV_2D_2->image_rdy(conv_2d_1_result_rdy_sig_2);
        CONV_2D_2->biases_vld(biases2_vld_sig);
        CONV_2D_2->biases_rdy(biases2_rdy_sig);
        CONV_2D_2->conv_2d_result_tb(conv_2d_2_result_sig_1);
        CONV_2D_2->conv_2d_result_next(conv_2d_2_result_sig_2);
        CONV_2D_2->conv_2d_result_rdy_tb(conv_2d_2_result_rdy_sig_1);
        CONV_2D_2->conv_2d_result_rdy_next(conv_2d_2_result_rdy_sig_2);
        CONV_2D_2->conv_2d_result_vld_tb(conv_2d_2_result_vld_sig_1); 
        CONV_2D_2->conv_2d_result_vld_next(conv_2d_2_result_vld_sig_2); 
        /**/ 

        MAX_POOL_2D_1 = new max_pool("max_pool_2d_1");
        MAX_POOL_2D_1->clk(clk);
        MAX_POOL_2D_1->rst(rst);
        MAX_POOL_2D_1->image(conv_2d_2_result_sig_2);
        MAX_POOL_2D_1->image_vld(conv_2d_2_result_vld_sig_2);
        MAX_POOL_2D_1->image_rdy(conv_2d_2_result_rdy_sig_2);
        MAX_POOL_2D_1->max_pool_result_tb(result_max_pool_sig_1);
        MAX_POOL_2D_1->max_pool_result_next(result_max_pool_sig_2);
        MAX_POOL_2D_1->max_pool_result_rdy_tb(max_pool_result_rdy_sig_1);
        MAX_POOL_2D_1->max_pool_result_rdy_next(max_pool_result_rdy_sig_2);
        MAX_POOL_2D_1->max_pool_result_vld_tb(max_pool_result_vld_sig_1);
        MAX_POOL_2D_1->max_pool_result_vld_next(max_pool_result_vld_sig_2);
        
        DENSE1 = new dense("dense1", POOL_ED, DENSE1_COEFF1, 
        DENSE1_COEFF2, DENSE1_COEFF, BIASES3, DENSE1_OUT, 1);
        DENSE1->clk(clk);
        DENSE1->rst(rst);
        DENSE1->input(result_max_pool_sig_2);
        DENSE1->input_vld(max_pool_result_vld_sig_2);
        DENSE1->input_rdy(max_pool_result_rdy_sig_2);
        DENSE1->biases(biases3_sig);
        DENSE1->biases_vld(biases3_vld_sig);
        DENSE1->biases_rdy(biases3_rdy_sig);
        DENSE1->coeff(coeff_sig);
        DENSE1->coeff_vld(coeff_vld_sig);
        DENSE1->coeff_rdy(coeff_rdy_sig);
        DENSE1->dense_result_tb(dense1_result_tb_sig);
        DENSE1->dense_result_next(dense1_result_next_sig);
        DENSE1->dense_result_rdy_tb(dense1_result_rdy_tb_sig);
        DENSE1->dense_result_rdy_next(dense1_result_rdy_next_sig);
        DENSE1->dense_result_vld_tb(dense1_result_vld_tb_sig);
        DENSE1->dense_result_vld_next(dense1_result_vld_next_sig);

        DENSE2 = new dense("dense2", DENSE1_OUT, DENSE2_COEFF1, 
        DENSE2_COEFF2, DENSE2_COEFF, BIASES4, DENSE2_OUT, 2);
        DENSE2->clk(clk);
        DENSE2->rst(rst);
        DENSE2->input(dense1_result_next_sig);
        DENSE2->input_vld(dense1_result_vld_next_sig);
        DENSE2->input_rdy(dense1_result_rdy_next_sig);
        DENSE2->biases(biases4_sig);
        DENSE2->biases_vld(biases4_vld_sig);
        DENSE2->biases_rdy(biases4_rdy_sig);
        DENSE2->coeff(coeff2_sig);
        DENSE2->coeff_vld(coeff2_vld_sig);
        DENSE2->coeff_rdy(coeff2_rdy_sig);
        DENSE2->dense_result_tb(dense2_result_tb_sig);
        DENSE2->dense_result_next(dense2_result_next_sig);
        DENSE2->dense_result_rdy_tb(dense2_result_rdy_tb_sig);
        DENSE2->dense_result_rdy_next(dense2_result_rdy_next_sig);
        DENSE2->dense_result_vld_tb(dense2_result_vld_tb_sig);
        DENSE2->dense_result_vld_next(dense2_result_vld_next_sig);
/**/
    } 
    //деструктор
    ~TOP(){
        delete DRI_TB;
        delete CONV_2D_1;
        delete CONV_2D_2;
       delete MAX_POOL_2D_1;
       delete DENSE1;
        delete DENSE2;
        /**/
    }
}; 

TOP *top = NULL;//чтобы не указывало на какой-то случайный участок памяти

int sc_main(int argc, char* argv[]) {
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);//suppress warning due to set_time_resolution
    //sc_set_time_resolution(1, SC_NS);
    top = new TOP("top_module");
    //начинаем симуляцию 
    
        int sim_step=1;
        sc_start(10000000,SC_NS);

        for (int i = 0; i <1; i++){
            
            sc_start(sim_step, SC_NS);
            
//            cout << "clk = "<<top->clk<<"  @ "<<sc_time_stamp()<<endl;
/**//*
            cout<<" kernel_rdy = "<<top->kernel_rdy_sig<<"| ";
            cout<<" kernel_vld = "<<top->kernel_vld_sig<<"| ";
            cout<<" kernel_sig = "<<top->kernel_sig<<endl;
            cout<<" image_rdy = "<<top->image_rdy_sig<<" | "; 
            cout<<" image_vld = "<<top->image_vld_sig<<" | ";
            cout<<" image_sig =  "<<top->image_sig<<endl;
            cout<<" biases_rdy = "<<top->biases_rdy_sig<<"| ";
            cout<<" biases_vld = "<<top->biases_vld_sig<<"| ";
            cout<<" biases_sig = "<<top->biases_sig<<endl;/**/
 /*               cout<<sc_time_stamp()<<" ";
                cout<<" biases_recieved = "<<top->DENSE1->biases_recieved;
                cout<<" coeff_recieved = "<<top->DENSE1->coeff_recieved;
                cout<<" input_recieved = "<<top->DENSE1->input_recieved<<endl<<endl; /**/
        } 
    return 0;
}