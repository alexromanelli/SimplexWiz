/*
 * NumeroRacional.h
 *
 *  Created on: 21 de jul de 2017
 *      Author: alexandre
 */

#ifndef UTIL_NUMERORACIONAL_H_
#define UTIL_NUMERORACIONAL_H_

#include <math.h>
#include <string>

class NumeroRacional {
private:
	int numerador;
	int denominador;

	int gcd(int a, int b) { return b == 0 ? abs(a) : gcd(abs(b), abs(a) % abs(b)); }

	int mmc(int num1, int num2) {
	    int resto, a, b;
	    a = num1;
	    b = num2;
	    do {
	        resto = a % b;
	        a = b;
	        b = resto;
	    } while (resto != 0);
	    return (num1 * num2) / a;
	}

public:
	NumeroRacional(int n, int d = 1);
	virtual ~NumeroRacional();

	std::string toString();
	std::string toString(int totalCaracteres);

	NumeroRacional operator+(const NumeroRacional& r);
	NumeroRacional operator-(const NumeroRacional& r);
	NumeroRacional operator*(const NumeroRacional& r);
	NumeroRacional operator/(const NumeroRacional& r);
	NumeroRacional operator=(const NumeroRacional& r);
	bool operator==(const NumeroRacional& r);
	bool operator>=(const NumeroRacional& r);
	bool operator<=(const NumeroRacional& r);
	bool operator>(const NumeroRacional& r);
	bool operator<(const NumeroRacional& r);
};

#endif /* UTIL_NUMERORACIONAL_H_ */
