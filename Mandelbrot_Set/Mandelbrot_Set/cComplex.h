#pragma once

class Complex {
public:
	Complex(float, float);
	Complex();
	~Complex();
public:
	Complex operator*(const Complex) const;
	Complex operator+(const Complex) const;
	float mod2();
public:
	float real;
	float imag;
};