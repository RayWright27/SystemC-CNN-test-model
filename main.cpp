#include <systemc.h>
#include "tb_driver.h"
#include "conv.h"
#include "max_pooling.h"
#include "dense.h"

int sc_main(int argc, char* argv[]) {
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);//suppress warning due to set_time_resolution
    sc_set_time_resolution(1, SC_FS);
    // сигналы
    sc_signal<bool> clk;//sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst_n;
    sc_signal<double> kernel_sig[KER], image_sig[IMG], biases_sig[BIASES];
    sc_signal<double> convolved_mat_sig[CONV_ED];
    sc_signal<double> pooled_featuremap_sig[POOL_ED];
    sc_signal<double> output_sig[DENSE_KER2];

    // инстанциируем модули и соединяем сигналы

    tb_driver DRI_TB("DRI_TB");
    DRI_TB.clk(clk);
    DRI_TB.rst_n(rst_n);
  
     for (int i = 0; i < KER; ++i) {
        DRI_TB.kernel[i](kernel_sig[i]);   
    }
    for (int k = 0; k < IMG; ++k) {
        DRI_TB.image[k](image_sig[k]);
    }
    for (int i=0;i<BIASES;i++){
        DRI_TB.biases[i](biases_sig[i]);
    }

    conv DUT("DUT");
    DUT.clk(clk);
    DUT.rst_n(rst_n);
    for (int j = 0; j < KER; ++j) {
                DUT.kernel[j](kernel_sig[j]);//сигналы  внутри, снаружи порты
    }
    for (int k = 0; k < IMG; ++k) {
            DUT.image[k](image_sig[k]);
    }
     for (int k = 0; k < BIASES; ++k) {
            DUT.biases[k](biases_sig[k]);
    }
    /*
     for (int j = 0; j <21632; ++j) {
                DUT.convolved_mat[j](convolved_mat_sig[j]);
    } */ 

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

    //начинаем симуляцию
    sc_start(3, SC_NS);
   
    /* rst_n = 0;    // Assert the reset
    cout << "@" << sc_time_stamp() <<" Asserting reset\n" << endl;
    for (int i=0;i<1;i++) {
        clk = 0; 
        sc_start(50, SC_NS);
        clk = 1; 
        sc_start(50, SC_NS);
    }
    rst_n = 1;    // De-assert the reset
    cout << "@" << sc_time_stamp() <<" De-Asserting reset\n" << endl;
    for (int i=0;i<1;i++) {
        clk = 0; 
        sc_start(50, SC_NS);
        clk = 1; 
        sc_start(50, SC_NS);
    } 
      sc_stop();

   /* cout << "результат conv " << endl;
    for (int i = 0; i < CONV_ED; i++) {
        cout << convolved_mat_sig[i] << " ";
    }
    cout << endl;*/
 
    return 0;
}