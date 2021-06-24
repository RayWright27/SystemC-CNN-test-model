#define COLUMNS 5
#include <systemc.h>
#include "mult.cpp"
//топ модуль
SC_MODULE (SYSTEM){
	//объявления модулей внутри
	mult *mult0;

	//объявление сигналов
	//sc_signal<bool> rst_sig;//reset
	//sc_signal<sc_int<16>> matrix1[COLUMNS];//входы пока не используем
	//sc_signal<sc_int<16>> matrix2[COLUMNS];
    //sc_signal<sc_int<16>> result[COLUMNS];
	sc_clock clk_sig;
	
	SC_CTOR (SYSTEM):clk_sig("clk_sig",10,SC_NS) //конструктор мейн модуля
	{
		mult0=new mult("mult0");//выделяем память для инстанса умножителя
		mult0->clk(clk_sig);
		//mult0->rst(rst_sig);
		//mult0->in1(matrix1[COLUMNS]);
		//mult0->in2(matrix2[COLUMNS]);
		mult0->out(result);
	}
	//деструктор топ модуля
	~SYSTEM(){
		delete mult0;
	}
};

//зануляем указатель на топ модуль
SYSTEM *top = NULL;

int sc_main(int argc, char* argv[]){
	top=new SYSTEM("top");//выделяем память для топ модуля
	sc_start();//начинаем симуляцию
	return 0;
}

