#define DT_LENGTH 32//длина переменных 
#define KK 2//длина вектора

#include <systemc.h>

SC_MODULE(vector_mul) {
    // обозначение портов
    sc_in<bool> clk, rst_n;
    sc_in<sc_int<DT_LENGTH>> vec1[KK], vec2[KK];
    sc_out<sc_int<DT_LENGTH * 2>> vec_o;//
    
    void compute_vector_mul(void);
    
    SC_CTOR(vector_mul) {
        SC_METHOD(compute_vector_mul); // регистрация метода compute_vector_mul как SC_METHOD запускает его только при срабатывании sensitivity list'ов
        sensitive_pos << clk; // позитивный clock
        sensitive_neg << rst_n; // негативный сброс
    };
};