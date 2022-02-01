#include <systemc.h>
#include "macro.h"
double ReLU(double a) {
	if (a <= 0) {
		return 0;
	}
	else {
		return a;
	}
}