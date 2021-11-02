#include <systemc.h>

#include "tb_driver.h"
#include "conv.h"
#include "max_pooling.h"
#include "dense.h"

int sc_main(int argc, char* argv[]) {
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);//выяснить

    // сигналы
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst_n;
    sc_signal<sc_int<DT_LENGTH>> kernel_sig[KER], image_sig[IMG];
    sc_signal<sc_int<DT_LENGTH>> convolved_mat_sig[CONV_ED];
    sc_signal<sc_int<DT_LENGTH>> pooled_featuremap_sig[POOL_ED];
    sc_signal<sc_int<DT_LENGTH>> output_sig[DENSE_KER2];

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

    conv DUT("DUT");
    DUT.clk(clk);
    DUT.rst_n(rst_n);
    for (int j = 0; j < KER; ++j) {
                DUT.kernel[j](kernel_sig[j]);//сигналы же внутри, снаружи порты
    }
    for (int k = 0; k < IMG; ++k) {
            DUT.image[k](image_sig[k]);
    }
    for (int j = 0; j < CONV_ED; ++j) {
                DUT.convolved_mat[j](convolved_mat_sig[j]);
    }

    max_pool DUT2("DUT2");
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
    }

    //начинаем симуляцию
    sc_start(15, SC_NS);
   /* cout << "результат conv " << endl;
    for (int i = 0; i < CONV_ED; i++) {
        cout << convolved_mat_sig[i] << " ";
    }
    cout << endl;*/

    return 0;
}