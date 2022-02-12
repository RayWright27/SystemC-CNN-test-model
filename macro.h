#define DT_LENGTH 32 
//для conv_2d_1 и conv_2d_2
#define M1 3//число строк КЕРНЕЛА1 
#define N1 3//число столбцов КЕРНЕЛА1
#define L1 32//число кернелов и выходных изображений соответственно
#define KER M1*N1*L1 //длина вектора кернела

#define M2 28//число строк ИЗОБРАЖЕНИЯ
#define N2 28//число столбцов ИЗОБРАЖЕНИЯ
#define C1 1//число изображений 
#define IMG M2*N2 //длина вектора изображения

#define STRIDE 1
#define ZERO_PAD 0
#define M3 ((M2-M1+2*ZERO_PAD)/STRIDE+1)//вычисляем размеры выходной матрицы conv_2d_1
#define N3 ((N2-N1+2*ZERO_PAD)/STRIDE+1)
#define L3 L1 // количество выходных изображений из conv_2d_1 = количество кернелов (после свёртки столько же матриц(изображений), сколько кернелов)
#define CONV_ED M3*N3*L1//длина вектора после свёртки
#define BIASES 32


#define STRIDE2 1
#define M4 3//число строк КЕРНЕЛА2
#define N4 3//число столбцов КЕРНЕЛА2
#define L4 64//число кернелов и выходных изображений соответственно
#define KER2 M4*N4*L4*C2//длина вектора кернела2
#define C2 L3// количество выходных изображений из conv_2d_2 = количеству входных в conv_2d_1
#define M5 ((M3-M4+2*ZERO_PAD)/STRIDE2+1)
#define N5 ((N3-N4+2*ZERO_PAD)/STRIDE2+1)
#define CONV_ED2 M5*N5*C2
#define BIASES2 64

//для max_pool
#define P1 2//число строк КЕРНЕЛА3(POOL) 
#define P2 2//число столбцов КЕРНЕЛА3(POOL)
#define F_M1 M3//строки входной на POOL
#define F_M2 N3//столбцы входной на POOL
#define F_M3 L3//число входных на POOL матриц
#define POOL_STRIDE 2// !! не нужен ??

//метод почти такой же как и для conv
#define POOLOUT1 ((F_M1-P1)/POOL_STRIDE +1)
#define POOLOUT2 ((F_M2-P2)/POOL_STRIDE +1)
#define POOLOUT3 F_M3
#define POOL_ED POOLOUT1*POOLOUT2*POOLOUT3

//dense.h
#define DENSE_KER1 POOL_ED
#define DENSE_KER2 10
