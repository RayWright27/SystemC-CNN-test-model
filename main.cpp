#define DT_LENGTH 32//длина переменных 
#define M1 2//число строк КЕРНЕЛА 
#define N1 2//число столбцов КЕРНЕЛА 
#define M2 5//число строк ИЗОБРАЖЕНИЯ
#define N2 5//число столбцов ИЗОБРАЖЕНИЯ
#define STRIDE 1
#define ZERO_PAD 0
#define M3 (M2-M1+2*ZERO_PAD)/STRIDE+1//вычисляем размеры выходной матрицы
#define N3 (N2-N1+2*ZERO_PAD)/STRIDE+1

#include <systemc.h>

#include "tb_driver.h"
#include "conv.h"

int sc_main(int argc,char* argv[]) {
    sc_core::sc_report_handler::set_actions( "/IEEE_Std_1666/deprecated", sc_core::SC_DO_NOTHING );//выяснить
    
    // сигналы
    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> rst_n;
    sc_signal<sc_int<DT_LENGTH> > kernel_sig[M1][N1], image_sig[M2][N2];
    sc_signal<sc_int<DT_LENGTH * 2> > convolved_mat_sig[M3][N3];
    
    // инстанциируем модулм и соединяем сигналы
	
   tb_driver DRI_TB("DRI_TB");
    DRI_TB.clk(clk);
    DRI_TB.rst_n(rst_n);
    for (int i = 0; i < M1; ++i) {
        for (int k = 0; k < N1; ++k) {
            DRI_TB.kernel[i][k](kernel_sig[i][k]);
        }
    }
    for (int k = 0; k < M2; ++k) {
        for (int j = 0; j < N2; ++j) {
            DRI_TB.image[k][j](image_sig[k][j]);
        }
    }
    
    conv DUT("DUT");
    DUT.clk(clk);
    DUT.rst_n(rst_n);
    for (int i = 0; i < M1; ++i) {
        for (int k = 0; k < N1; ++k) {
            DUT.kernel[i][k](kernel_sig[i][k]);//сигналы же внутри, снаружи порты
        }
    }
    for (int k = 0; k < M2; ++k) {
        for (int j = 0; j < N2; ++j) {
            DUT.image[k][j](image_sig[k][j]);
        }
    }
    for (int i = 0; i < M3; ++i) {
        for (int j = 0; j < N3; ++j) {
            DUT.convolved_mat[i][j](convolved_mat_sig[i][j]);
        }
    }

 
    //начинаем симуляцию
    sc_start(1, SC_NS);
    
    //вывод результата в консоль
	cout<<"результат свёртки: "<<endl;
    for (int i = 0; i < M3; ++i) {
        for (int j = 0; j < N3; ++j) {
            cout << convolved_mat_sig[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}