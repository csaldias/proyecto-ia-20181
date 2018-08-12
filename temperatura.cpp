#include "temperatura.h"

float Temperatura::getTemp() {
 	float curr_temp = this->temp;
 	return curr_temp;
 }

 void Temperatura::coolDown() {
	this->temp *= decay_factor;
 }

 void Temperatura::setTemp(float newTemp) {
	 this->temp = newTemp;
 }