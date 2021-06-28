#include "vector_mul.h"

void vector_mul::compute_vector_mul(void){
	int temp = 0;
    if (rst_n.read() == false) {
		vec_o.write(0);
        return;
	}
	for (int k = 0; k < KK; ++k) {
        temp = temp + vec1[k].read() * vec2[k].read();
    }
    vec_o.write(temp);
    };