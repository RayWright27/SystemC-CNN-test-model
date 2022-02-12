#include <systemc.h>
#include "tb_driver.h"
#include "conv.h"
#include "max_pooling.h"
#include "dense.h"

//------------------------------------------------
// Передача параметров слоя в конструктор модуля слоя происходит в том же порядке, в котором эти параметры расположены в файле macro.h
//------------------------------------------------

SC_MODULE(TOP){//топ-модуль нейросетевого ускорителя 
    //объявление модулей
    
    tb_driver *DRI_TB;
    conv *CONV_2D_1;
    conv *CONV_2D_2;
    // сигналы
    sc_clock clk;//("clk", 10, SC_NS);

    sc_signal<bool> rst;
    sc_signal<bool> kernel_rdy_sig;//готовность приёма/передачи данных кернела
    sc_signal<bool> kernel_vld_sig;//=1 когда данные кернела видны для считывания 
    sc_signal<bool> image_rdy_sig;
    sc_signal<bool> image_vld_sig;
    sc_signal<bool> biases_rdy_sig;
    sc_signal<bool> biases_vld_sig;
    sc_signal<bool> conv_2d_1_result_vld_sig;
    sc_signal<bool> conv_2d_1_result_rdy_sig_1;
    sc_signal<bool> conv_2d_1_result_rdy_sig_2;
    sc_signal<double> kernel_sig, image_sig, biases_sig;
    sc_signal<double> conv_2d_1_result_sig;
    sc_signal<bool> kernel2_rdy_sig;//готовность приёма/передачи данных кернела
    sc_signal<bool> kernel2_vld_sig;//=1 когда данные кернела видны для считывания 
    sc_signal<bool> biases2_rdy_sig;
    sc_signal<bool> biases2_vld_sig;
    sc_signal<bool> conv_2d_2_result_vld_sig;
    sc_signal<bool> conv_2d_2_result_rdy_sig;
    sc_signal<double> kernel2_sig, biases2_sig;
    sc_signal<double> conv_2d_2_result_sig;
    sc_signal<bool> dummy;
    
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
        DRI_TB->conv_2d_1_result(conv_2d_1_result_sig);
        DRI_TB->conv_2d_1_result_vld(conv_2d_1_result_vld_sig);
        DRI_TB->conv_2d_1_result_rdy(conv_2d_1_result_rdy_sig_1);
        DRI_TB->kernel2(kernel2_sig);
        DRI_TB->kernel2_rdy(kernel2_rdy_sig);
        DRI_TB->kernel2_vld(kernel2_vld_sig);
        DRI_TB->biases2(biases2_sig);
        DRI_TB->biases2_rdy(biases2_rdy_sig);
        DRI_TB->biases2_vld(biases2_vld_sig);
/*
        CONV_2D_1 = new conv("conv_2d_1", M1, N1, L1, KER, M2, N2, C1, IMG, M3, N3, L3, CONV_ED, BIASES);
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
        CONV_2D_1->conv_2d_result(conv_2d_1_result_sig);
        CONV_2D_1->conv_2d_result_rdy_1(conv_2d_1_result_rdy_sig_1);
        CONV_2D_1->conv_2d_result_rdy_2(conv_2d_1_result_rdy_sig_2);
        CONV_2D_1->conv_2d_result_vld(conv_2d_1_result_vld_sig);
 /**/

        CONV_2D_2 = new conv("conv_2d_2", M4, N4, L4, KER2, M3, N3, L3, CONV_ED, M5, N5, C2, CONV_ED2, BIASES2);
        CONV_2D_2->clk(clk);
        CONV_2D_2->rst(rst);
        CONV_2D_2->kernel(kernel2_sig);
        CONV_2D_2->image(conv_2d_1_result_sig);
        CONV_2D_2->biases(biases2_sig);
        CONV_2D_2->kernel_vld(kernel2_vld_sig);
        CONV_2D_2->kernel_rdy(kernel2_rdy_sig);
        CONV_2D_2->image_vld(conv_2d_1_result_vld_sig);
        CONV_2D_2->image_rdy(conv_2d_1_result_rdy_sig_2);
        CONV_2D_2->biases_vld(biases2_vld_sig);
        CONV_2D_2->biases_rdy(biases2_rdy_sig);
        CONV_2D_2->conv_2d_result(conv_2d_2_result_sig);
        CONV_2D_2->conv_2d_result_rdy_1(conv_2d_2_result_rdy_sig);
        CONV_2D_2->conv_2d_result_vld(conv_2d_2_result_vld_sig); 
        CONV_2D_2->conv_2d_result_rdy_2(dummy);
        /**/ 
    }

    //деструктор
    ~TOP(){
        delete DRI_TB;
        delete CONV_2D_1;
        delete CONV_2D_2;
    }


    // (old) инстанциируем модули и соединяем сигналы
/* 
    /* max_pool DUT2("DUT2");
    DUT2.clk(clk);
    DUT2.rst_n(rst_n);
    for (int k = 0; k < CONV_ED; k++) {
        DUT2.featuremap[k](convolved_mat_sig[k]);
    }
    for (int i = 0; i < POOL_ED; i++) {
        DUT2.pooled_featuremap[i](pooled_featuremap_sig[i]);
    }

    dense DUT3("DUT3");
    DUT3.clk(clk);
    DUT3.rst_n(rst_n);
    for (int k = 0; k < POOL_ED; k++) {
        DUT3.dense_input[k](pooled_featuremap_sig[k]);
    }
    for (int i = 0; i < DENSE_KER2; i++) {
        DUT3.dense_output[i](output_sig[i]);
    } */

};

TOP *top = NULL;//чтобы не указывало на какой-то случайный участок памяти

int sc_main(int argc, char* argv[]) {
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);//suppress warning due to set_time_resolution
    //sc_set_time_resolution(1, SC_NS);
    top = new TOP("top_module");
    //начинаем симуляцию
    
        int sim_step=1;
        sc_start(400000,SC_NS);
        for (int i=0;i<1000000;i++){
            sc_start(sim_step, SC_NS);
            
            /* cout << "clk = "<<top->clk<<"  @ "<<sc_time_stamp()<<endl;
            cout<<" kernel_rdy = "<<top->kernel_rdy_sig<<"| ";
            cout<<" kernel_vld = "<<top->kernel_vld_sig<<"| ";
            cout<<" kernel_sig = "<<top->kernel_sig<<endl;
            cout<<" image_rdy = "<<top->image_rdy_sig<<" | ";
            cout<<" image_vld = "<<top->image_vld_sig<<" | ";
            cout<<" image_sig =  "<<top->image_sig<<endl;
            cout<<" biases_rdy = "<<top->biases_rdy_sig<<"| ";
            cout<<" biases_vld = "<<top->biases_vld_sig<<"| ";
            cout<<" biases_sig = "<<top->biases_sig<<endl;
            cout<<" conv_2d_1_result_rdy = "<<top->conv_2d_1_result_rdy_sig<<"| ";
            cout<<" conv_2d_1_result_vld = "<<top->conv_2d_1_result_vld_sig<<"| ";
            cout<<" conv_2d_1_result_sig = "<<top->conv_2d_1_result_sig<<endl<<endl; */


            
        }
     sc_stop();
 
    return 0;
}