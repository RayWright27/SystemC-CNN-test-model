
#include <macro.h>
#include <systemc.h>
SC_MODULE(tb_driver){
	sc_in<bool>     clk;
    sc_out<bool>    rst;
    
    sc_in<bool>     kernel_rdy;
    sc_out<bool>    kernel_vld;

    sc_in<bool>     kernel2_rdy;
    sc_out<bool>    kernel2_vld;

    sc_in<bool>     image_rdy;
    sc_out<bool>    image_vld;

    sc_in<bool>     biases_rdy;
    sc_out<bool>    biases_vld;

    sc_in<bool>     biases2_rdy;
    sc_out<bool>    biases2_vld;

    sc_in<bool>     conv_2d_1_result_vld;
    sc_out<bool>    conv_2d_1_result_rdy;

    sc_in<double>   conv_2d_1_result;
    sc_out<double>  kernel, image, biases, kernel2, biases2;

    sc_in<double>   conv_2d_2_result;
    sc_in<bool>     conv_2d_2_result_vld;
    sc_out<bool>    conv_2d_2_result_rdy;
    
    sc_in<double>   max_pool_2d_1_result;
    sc_in<bool>     max_pool_2d_1_result_vld;
    sc_out<bool>    max_pool_2d_1_result_rdy;

    sc_out<double>  coeff;
    sc_in<bool>     coeff_rdy;
    sc_out<bool>    coeff_vld;
    sc_out<double>  biases3;
    sc_in<bool>     biases3_rdy;
    sc_out<bool>    biases3_vld;
    sc_in<double>   dense1_result;
    sc_in<bool>     dense1_result_vld;
    sc_out<bool>    dense1_result_rdy;

    sc_out<double>  coeff2;
    sc_in<bool>     coeff2_rdy;
    sc_out<bool>    coeff2_vld;
    sc_out<double>  biases4;
    sc_in<bool>     biases4_rdy;
    sc_out<bool>    biases4_vld;
    sc_in<double>   dense2_result;
    sc_in<bool>     dense2_result_vld;
    sc_out<bool>    dense2_result_rdy;

    double*         kernel2_flattened;
    double***       conv_2d_1_result_arr;
    double***       conv_2d_2_result_arr;
    double*         conv_2d_2_result_flattened;
    double***       max_pool_2d_1_result_arr;
    double*         max_pool_2d_1_result_flattened;
    double*         coeff_flattened;
    double          biases3_arr[BIASES3];
    double*         dense1_result_arr;
    double*         coeff2_flattened;
    double          biases4_arr[BIASES4];
    double*         dense2_result_arr;
    

    const char*     kernel2file = "conv_2d_2_kernels.txt"; // название файла для кернелов conv2d_2
    const char*     coefffile = "dense_1_coeff.txt";
    const char*     biasesfile = "dense_1_biases.txt";
    const char*     coefffile2 = "dense_2_coeff.txt";
    const char*     biasesfile2 = "dense_2_biases.txt";

//generated переменные не задействованы
    sc_logic        kernels_generated1 = sc_logic(0);
    sc_logic        biases_generated1 = sc_logic(0);
    sc_logic        image_generated = sc_logic(0);

    sc_logic        conv_2d_1_fetched = sc_logic(0);

    sc_logic        kernels_generated2 = sc_logic(0);
    sc_logic        biases_generated2 = sc_logic(0);

    sc_logic        conv_2d_2_fetched = sc_logic(0);

    sc_logic        max_pool_2d_1_fetched = sc_logic(0);

    sc_logic        dense1_coeff_generated = sc_logic(0);
    sc_logic        dense1_biases_generated = sc_logic(0);
    sc_logic        dense1_fetched = sc_logic(0);

    sc_logic        dense2_coeff_generated = sc_logic(0);
    sc_logic        dense2_biases_generated = sc_logic(0);
    sc_logic        dense2_fetched = sc_logic(0);

    void generate_reset(void);
    void generate_kernel(void);
    void generate_image(void);
    void generate_biases(void);
    void conv_2d_1_sink(void);

    void generate_kernel2(void);
    void generate_biases2(void);
    void conv_2d_2_sink(void);

    void max_pool_2d_1_sink(void);

    void generate_coeff(void);
    void generate_biases3(void);
    void dense1_sink(void);

    void generate_coeff2(void);
    void generate_biases4(void);
    void dense2_sink(void);

    double C[BIASES]={0.0691566914, 0.0087906541, 0.0805735663, 0.0110252723, 0.0001045055, 0.0136789260, 0.0097796526, -0.0016237695, 0.0948802680, 0.0211041402, -0.0034900929, 0.0307639018, 0.0448412858, -0.0014694935, 
    0.0388998576, 0.0056192460, 0.0029507081, -0.0039654267, 0.0023400388, 0.0503108688, -0.0020105073, -0.0073869051, 0.0047671571, 0.0638136268, 0.0234106053, 0.0156689044, -0.0022752020, 0.0103466120, 0.0103880670, 
    0.0530131310, -0.0050606704, 0.0142416079};
	double biases_flattened[BIASES];
    double biases_tmp;   

	SC_CTOR(tb_driver) {
        kernel2_flattened = new double[KER2];
        conv_2d_2_result_flattened = new double[CONV_ED2];
        max_pool_2d_1_result_flattened = new double[POOL_ED];
        coeff_flattened = new double[DENSE1_COEFF];
        dense1_result_arr = new double[DENSE1_OUT];
        coeff2_flattened = new double[DENSE2_COEFF];
        dense2_result_arr = new double[DENSE2_OUT];

    /* Поскольку в SystemC есть ограничение по выделяемой памти создаём динамические 
    массивы*/
        conv_2d_1_result_arr = new double**[N3];
        for (int i=0; i<N3;i++){
            conv_2d_1_result_arr[i] = new double*[M3];
            for (int j=0;j<M3;j++){
                conv_2d_1_result_arr[i][j] = new double[L3];
            }
        }

        conv_2d_2_result_arr = new double**[N5];
        for (int i=0; i<N5;i++){
            conv_2d_2_result_arr[i] = new double*[M5];
            for (int j=0;j<M5;j++){
                conv_2d_2_result_arr[i][j] = new double[L4];
            }
        }

        max_pool_2d_1_result_arr = new double**[POOLOUT1];
        for (int i=0; i<POOLOUT1;i++){
            max_pool_2d_1_result_arr[i] = new double*[POOLOUT2];
            for (int j=0;j<POOLOUT2;j++){
                max_pool_2d_1_result_arr[i][j] = new double[POOLOUT3];
            }
        }

        SC_THREAD(generate_reset);
//        sensitive<<clk.pos();
        SC_THREAD(generate_kernel);
        SC_THREAD(generate_image); 
        SC_THREAD(generate_biases);
//        sensitive<<clk.pos();
        SC_THREAD(conv_2d_1_sink);

        SC_THREAD(generate_kernel2);
        SC_THREAD(generate_biases2);
        SC_THREAD(conv_2d_2_sink);

        SC_THREAD(max_pool_2d_1_sink);

        SC_THREAD(generate_coeff);
        SC_THREAD(generate_biases3);
        SC_THREAD(dense1_sink);
        
        SC_THREAD(generate_coeff2);
        SC_THREAD(generate_biases4);
        SC_THREAD(dense2_sink);/**/
    };

    ~tb_driver(){
        delete[] kernel2_flattened;
        delete[] conv_2d_2_result_flattened;  
        delete[] max_pool_2d_1_result_flattened; 
        delete[] coeff_flattened; 
        delete[] dense1_result_arr;
        delete   kernel2file;
        delete   coefffile;

        for (int i = 0; i < N3; i++) {
            for (int j = 0; j < M3; j++) {               
                delete[] conv_2d_1_result_arr[i][j];
            }
            delete[] conv_2d_1_result_arr[i];
		}
        delete[] conv_2d_1_result_arr; 

        for (int i = 0; i < N5; i++) {
            for (int j = 0; j < M5; j++) {               
                delete[] conv_2d_2_result_arr[i][j];
            }
            delete[] conv_2d_2_result_arr[i];
		}
        delete[] conv_2d_2_result_arr;

        for (int i = 0; i < POOLOUT1; i++) {
            for (int j = 0; j < POOLOUT2; j++) {               
                delete[] max_pool_2d_1_result_arr[i][j];
            }
            delete[] max_pool_2d_1_result_arr[i];
		}
        delete[] max_pool_2d_1_result_arr;  
    }
};