//для параметризации надо будет сделать вместо фиксированных коэффициентов переменные
#define DT_LENGTH 32//длина переменных 
#define M1 3//число строк КЕРНЕЛА1 
#define N1 3//число столбцов КЕРНЕЛА1
#define L1 1//число кернелов 
#define KER M1*N1*L1 //длина вектора кернела

#define M2 28//число строк ИЗОБРАЖЕНИЯ
#define N2 28//число столбцов ИЗОБРАЖЕНИЯ
#define IMG 784 //длина вектора изображения

#define STRIDE 1
#define ZERO_PAD 0
#define M3 ((M2-M1+2*ZERO_PAD)/STRIDE+1)//вычисляем размеры выходной матрицы
#define N3 ((N2-N1+2*ZERO_PAD)/STRIDE+1)
#define L3 L1 //после свёртки столько же матриц, сколько кернелов
#define CONV_ED M3*N3*L3//длина вектора после свёртки
#define BIASES 1

//для matrix multiplication.h:
#define MM 2//число строк первой матрицы
#define NN 2//число столбцов второй матрицы 
#define KK 2//длина вектора (число столбцов первой матрицы и число строк второй)

//для max_pool
#define P1 2//число строк КЕРНЕЛА2(POOL) 
#define P2 2//число столбцов КЕРНЕЛА2(POOL)
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
