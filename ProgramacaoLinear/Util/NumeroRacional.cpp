/*
 * NumeroRacional.cpp
 *
 *  Created on: 21 de jul de 2017
 *      Author: alexandre
 */

#include "NumeroRacional.h"
#include <string>

NumeroRacional::NumeroRacional(int n, int d) {
	if (d < 0) {
		d = -d;
		n = -n;
	}
	int g = gcd(n, d);
	this->numerador = n / g;
	this->denominador = d / g;
}

NumeroRacional::~NumeroRacional() {
	// TODO Auto-generated destructor stub
}

std::string NumeroRacional::toString() {
	char str[30];
	if (denominador != 1)
		sprintf(str, "%d/%d", this->numerador, this->denominador);
	else
		sprintf(str, "%d", this->numerador);
	std::string s = str;
	return s;
}

std::string NumeroRacional::toString(int totalCaracteres) {
	char str[totalCaracteres];
	if (denominador != 1)
		sprintf(str, "%d/%d", this->numerador, this->denominador);
	else
		sprintf(str, "%d", this->numerador);
	std::string s = str;
	return s;
}

NumeroRacional NumeroRacional::operator +(const NumeroRacional& r) {
	int g = mmc(this->denominador, r.denominador);
	int n = (g / r.denominador) * r.numerador + (g / this->denominador) * this->numerador;
	NumeroRacional* nr = new NumeroRacional(n, g);
	return *nr;
}

NumeroRacional NumeroRacional::operator -(const NumeroRacional& r) {
	int g = mmc(this->denominador, r.denominador);
	int n = (g / this->denominador) * this->numerador - (g / r.denominador) * r.numerador;
	NumeroRacional* nr = new NumeroRacional(n, g);
	return *nr;
}

NumeroRacional NumeroRacional::operator *(const NumeroRacional& r) {
	NumeroRacional* nr = new NumeroRacional(this->numerador * r.numerador,
			this->denominador * r.denominador);
	return *nr;
}

NumeroRacional NumeroRacional::operator /(const NumeroRacional& r) {
	NumeroRacional* nr = new NumeroRacional(this->numerador * r.denominador,
			this->denominador * r.numerador);
	return *nr;
}

NumeroRacional NumeroRacional::operator =(const NumeroRacional& r) {
	int g = gcd(r.numerador, r.denominador);
	this->numerador = r.numerador / g;
	this->denominador = r.denominador / g;
	return *this;
}

bool NumeroRacional::operator ==(const NumeroRacional& r) {
	return (r.numerador / r.numerador) == (this->numerador / this->numerador);
}

bool NumeroRacional::operator >=(const NumeroRacional& r) {
	return (r.numerador / r.numerador) >= (this->numerador / this->numerador);
}

bool NumeroRacional::operator <=(const NumeroRacional& r) {
	return (r.numerador / r.numerador) <= (this->numerador / this->numerador);
}

bool NumeroRacional::operator >(const NumeroRacional& r) {
	return (r.numerador / r.numerador) > (this->numerador / this->numerador);
}

bool NumeroRacional::operator <(const NumeroRacional& r) {
	return (r.numerador / r.numerador) < (this->numerador / this->numerador);
}
