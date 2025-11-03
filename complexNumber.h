#pragma once
#include <iostream>
class ComplexNumber {
public:
	ComplexNumber(float real, float imaginary);
	void out() const; // prints the value
	ComplexNumber operator+(const ComplexNumber& other) const;
	ComplexNumber operator*(const ComplexNumber& other) const;
	ComplexNumber operator-(const ComplexNumber& other) const;
	ComplexNumber operator/(const ComplexNumber& other) const;
	ComplexNumber operator+(float f) const;
	ComplexNumber operator-(float f) const;
	ComplexNumber operator*(float f) const;
	ComplexNumber operator/(float f) const;
	friend ComplexNumber operator+(float f, const ComplexNumber& c);
	friend ComplexNumber operator-(float f, const ComplexNumber& c);
	friend ComplexNumber operator*(float f, const ComplexNumber& c);
	friend ComplexNumber operator/(float f, const ComplexNumber& c);
	ComplexNumber Power(int p);
	ComplexNumber(const ComplexNumber& other);
	void Set(float real, float imaginary);
	void Set(const ComplexNumber& other);
	float abs() const;
private:
	float real;
	float imaginary;

	mutable float cachedAbs;
    mutable bool isDirty;
};