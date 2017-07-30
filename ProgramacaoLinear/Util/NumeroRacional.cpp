/*
 * NumeroRacional.cpp
 *
 *  Created on: 21 de jul de 2017
 *      Author: alexandre
 */

#include "NumeroRacional.h"
#include <string>
#include <stdlib.h>

NumeroRacional::NumeroRacional(int n, int d) {
	if (d < 0) {
		d = -d;
		n = -n;
	}
	int absN = abs(n);
	int g = gcd(absN, d);
	this->numerador = n / g;
	this->denominador = d / g;
}

NumeroRacional::~NumeroRacional() {
	// TODO Auto-generated destructor stub
}

std::string NumeroRacional::toString() {
	char str[30];
	if (denominador != 1)
		sprintf(str, "%+d/%d", this->numerador, this->denominador);
	else
		sprintf(str, "%+d", this->numerador);
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
	return ((double)r.numerador / (double)r.denominador) == ((double)this->numerador / (double)this->denominador);
}

bool NumeroRacional::operator >=(const NumeroRacional& r) {
	return ((double)this->numerador / (double)this->denominador) >= ((double)r.numerador / (double)r.denominador);
}

bool NumeroRacional::operator <=(const NumeroRacional& r) {
	return ((double)this->numerador / (double)this->denominador) <= ((double)r.numerador / (double)r.denominador);
}

bool NumeroRacional::operator >(const NumeroRacional& r) {
	return ((double)this->numerador / (double)this->denominador) > ((double)r.numerador / (double)r.denominador);
}

bool NumeroRacional::operator <(const NumeroRacional& r) {
	return ((double)this->numerador / (double)this->denominador) < ((double)r.numerador / (double)r.denominador);
}

NumeroRacional NumeroRacional::operator +(const int& r) {
	NumeroRacional* nr = new NumeroRacional(r, 1);
	*nr = (*this) + (*nr);
	return *nr;
}

NumeroRacional NumeroRacional::operator -(const int& r) {
	NumeroRacional* nr = new NumeroRacional(r, 1);
	*nr = (*this) - (*nr);
	return *nr;
}

NumeroRacional NumeroRacional::operator *(const int& r) {
	NumeroRacional* nr = new NumeroRacional(r, 1);
	*nr = (*this) * (*nr);
	return *nr;
}

NumeroRacional NumeroRacional::operator /(const int& r) {
	NumeroRacional* nr = new NumeroRacional(r, 1);
	*nr = (*this) / (*nr);
	return *nr;
}

NumeroRacional NumeroRacional::operator =(const int& r) {
	this->numerador = r;
	this->denominador = 1;
	return *this;
}

bool NumeroRacional::operator ==(const double& r) {
	return ((double)this->numerador / (double)this->denominador) == r;
}

bool NumeroRacional::operator >=(const double& r) {
	return ((double)this->numerador / (double)this->denominador) >= r;
}

bool NumeroRacional::operator <=(const double& r) {
	return ((double)this->numerador / (double)this->denominador) <= r;
}

bool NumeroRacional::operator >(const double& r) {
	return ((double)this->numerador / (double)this->denominador) > r;
}

bool NumeroRacional::operator <(const double& r) {
	return ((double)this->numerador / (double)this->denominador) < r;
}

bool NumeroRacional::operator !=(const NumeroRacional& r) {
	return ((double)r.numerador / (double)r.denominador) != ((double)this->numerador / (double)this->denominador);
}

bool NumeroRacional::operator !=(const double& r) {
	return ((double)this->numerador / (double)this->denominador) != r;
}
