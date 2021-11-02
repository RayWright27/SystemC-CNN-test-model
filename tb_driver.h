/* #define DT_LENGTH 32//длина переменных 
#define M1 2//число строк КЕРНЕЛА 
#define N1 2//число столбцов КЕРНЕЛА 
#define M2 5//число строк ИЗОБРАЖЕНИЯ
#define N2 5//число столбцов ИЗОБРАЖЕНИЯ 

#define STRIDE 1
#define ZERO_PAD 0
#define M3 (M2-M1+2*ZERO_PAD)/STRIDE+1//вычисляем размеры выходной матрицы
#define N3 (N2-N1+2*ZERO_PAD)/STRIDE+1 */
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in <bool> clk;
    sc_out<bool> rst_n;
    sc_out<sc_int<DT_LENGTH> > kernel[KER], image[IMG];

    void generate_input(void);
	void generate_reset(void);
	
	SC_CTOR(tb_driver) {
        SC_THREAD(generate_input); //создает THREAD, происходит срабатывание путём изменения входного сигнала, приостановка происходит при wait() пока сигнал из sensetive листа не переключится снова 
        sensitive_neg << clk;
        SC_THREAD(generate_reset);
    };
};