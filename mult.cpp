#include <systemc.h>
//почему рантайм ошибка если все порты заассайнены??
SC_MODULE(mult){
	sc_in<bool> clk;
	//sc_in<bool> rst;
	//sc_in<sc_int<16>> in1;
	//sc_in<sc_int<16>> in2;
	sc_out<sc_int<16>> out;
	//тестовые матрицы
    sc_uint<8> matrix1[5]={1, 2, 3, 4, 5};
    sc_uint<8> matrix2[5]={6, 7, 8, 9, 10};

	void mult_main(void){
	
	sc_int<16> out_val;
	
	//reset part
	out.write(0);
	wait();
	
	while(true){
		for (int i=0; i<5; i++){
			out_val+=matrix1[i]*matrix2[i];
		}
		out.write(out_val);
	}
};//тут все вычисления
	
	SC_CTOR(mult){
		SC_CTHREAD(mult_main,clk.pos());
		//reset_signal_is(rst,true);//positive reset (пока без ресета)
	}
};

/*void mult::mult_main(void){
	
	sc_int<16> out_val;
	
	//reset part
	out.write(0);
	wait();
	
	while(true){
		for (int i=0; i<5; i++){
			out_val+=matrix1[i]*matrix2[i];
		}
		out.write(out_val);
	}
}*/
		
		
	
	