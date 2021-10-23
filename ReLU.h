#include <systemc.h>
#include "macro.h"
sc_int<DT_LENGTH> ReLU(sc_int<DT_LENGTH> a) {
	if (a <= 0) {
		return 0;
	}
	else {
		return a;
	}
}