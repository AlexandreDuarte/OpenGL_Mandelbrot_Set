#include "cComplex.h"

Complex::Complex(float x, float y) {
	real = x;
	imag = y;
}
Complex::~Complex() {
}
Complex::Complex() {
	real = 0.0f;
	imag = 0.0f;
}

Complex Complex::operator*(const Complex c) const {
	Complex result;
	result.real = real * c.real - imag * c.imag;
	result.imag = real * c.imag + imag * c.real;
	return result;
}
Complex Complex::operator+(const Complex c) const {
	Complex result;
	result.real = real + c.real;
	result.imag = imag + c.imag;
	return result;
}

float Complex::mod2() {
	return real * real + imag * imag;
}