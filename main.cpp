#include <systemc.h>
#include "tb_driver.h"
#include "conv.h"
#include "max_pooling.h"

int sc_main(int argc, char* argv[]) {
    sc_core::sc_report_handler::set_actions("/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING);//выяснить

    // сигналы
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst_n;
    sc_signal<sc_int<DT_LENGTH>> kernel_sig[KER];
    sc_signal<sc_int<DT_LENGTH>> image_sig[IMG];
    //sc_vector<sc_signal<sc_int<DT_LENGTH>>> kernel_sig_v{ "kernel_sig_v", KER };
   // sc_vector<sc_signal<sc_int<DT_LENGTH>>> image_sig_v{ "image_sig_v", IMG };
    sc_signal<sc_int<DT_LENGTH>> convolved_mat_sig[CONV_ED];
    sc_signal<sc_int<DT_LENGTH>> pooled_featuremap_sig[POOL_ED];

    // инстанциируем модули и соединяем сигналы

    tb_driver DRI_TB("DRI_TB");
    DRI_TB.clk(clk);
    DRI_TB.rst_n(rst_n);
    for (int j = 0; j < KER; ++j) {
            DRI_TB.kernel[j](kernel_sig[j]);
    }
 //   DRI_TB.kernelv(kernel_sig_v);

    for (int k = 0; k < IMG; ++k) {
            DRI_TB.image[k](image_sig[k]);
    }
   // DRI_TB.imagev(image_sig_v);

    conv DUT("DUT");
    DUT.clk(clk);
    DUT.rst_n(rst_n);
    for (int j = 0; j < KER; ++j) {
        DUT.kernel[j](kernel_sig[j]);//сигналы же внутри, снаружи порты
    }
      
    for (int k = 0; k <IMG; ++k) {
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

 
    //начинаем симуляцию
    sc_start(5, SC_NS);
    
    //вывод результата в консоль
	cout<<"результат свёртки: "<<endl;
    for (int i = 0; i < M3; i++) {
        for (int j = i*M3; j < (i+1)*M3; ++j) {
            cout << convolved_mat_sig[j] << " ";
        }
        cout << endl;
    }
    cout << endl;

    cout << "результат max_pool: " << endl;
    for (int i = 0; i < POOLOUT1; ++i) {
        for (int j = i*POOLOUT2; j < (i+1)*POOLOUT2; ++j) {
            cout << pooled_featuremap_sig[j] << " ";
        }
        cout << endl;
    }

    return 0;
}