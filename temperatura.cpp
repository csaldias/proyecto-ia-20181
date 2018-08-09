#include "temperatura.h"

float Temperatura::get_temp() {
 	float curr_temp = this->temp;
 	temp *= decay_factor;
 	return curr_temp;
 }