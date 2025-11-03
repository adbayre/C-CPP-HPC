#include "complexNumber.h"

ComplexNumber::ComplexNumber(float x, float y) : real(x), imaginary(y), isDirty(true) { }

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const {
	return ComplexNumber(real + other.real, imaginary + other.imaginary);
}

ComplexNumber ComplexNumber::operator*(const ComplexNumber& other) const {
	return ComplexNumber(real * other.real - imaginary * other.imaginary, real * other.imaginary + imaginary * other.real);
}

ComplexNumber ComplexNumber::operator-(const ComplexNumber& other) const {
	return ComplexNumber(real - other.real, imaginary - other.imaginary);
}

ComplexNumber ComplexNumber::operator/(const ComplexNumber& other) const {
	return ComplexNumber((real * other.real + imaginary * other.imaginary) / (other.real * other.real + other.imaginary * other.imaginary), (imaginary * other.real - real * other.imaginary) / (other.real * other.real + other.imaginary * other.imaginary));
}

ComplexNumber ComplexNumber::operator+(float f) const {
        return ComplexNumber(real + f, imaginary);
}
ComplexNumber ComplexNumber::operator-(float f) const {
	return ComplexNumber(real - f, imaginary);
}
ComplexNumber ComplexNumber::operator*(float f) const {
	return ComplexNumber(real * f, imaginary * f);
}
ComplexNumber ComplexNumber::operator/(float f) const {
	return ComplexNumber(real / f, imaginary / f);
}

ComplexNumber operator+(float f, const ComplexNumber& c) {
	return ComplexNumber(f + c.real, c.imaginary);
}
ComplexNumber operator-(float f, const ComplexNumber& c) {
	return ComplexNumber(f - c.real, -c.imaginary);
}
ComplexNumber operator*(float f, const ComplexNumber& c) {
	return ComplexNumber(f * c.real, f * c.imaginary);
}
ComplexNumber operator/(float f, const ComplexNumber& c) {
	float denom = c.real * c.real + c.imaginary * c.imaginary;
	return ComplexNumber(f * c.real / denom, -f * c.imaginary / denom);
}
void ComplexNumber:: Set(float x, float y) {
	real = x;
	imaginary = y;
	isDirty = true;
}

void ComplexNumber:: Set(const ComplexNumber& other) {
	real = other.real;
	imaginary = other.imaginary;
	isDirty = true;

}

ComplexNumber ComplexNumber:: Power(int n) {
	ComplexNumber res = *this;
	if (n == 0)
		return ComplexNumber(1, 0);
	if (n < 0)
		return ComplexNumber(1, 0) / this -> Power(-n); 
	res.Set(1, 0);
	ComplexNumber temp(*this);
	while (n > 0) {
		if (n % 2 == 1)
			res = res * temp;
		temp = temp * temp;
		n /= 2;
	}
	return res;
}

ComplexNumber:: ComplexNumber(const ComplexNumber& other) : real(other.real), imaginary(other.imaginary) { }

float ComplexNumber:: abs() const {
	if (isDirty) {
		cachedAbs = std::sqrt( real * real + imaginary * imaginary );
		isDirty = false;
	}
	return cachedAbs;
}

void ComplexNumber::out() const {
	std::cout << real;
	if (imaginary >= 0)
		std::cout << " + " << imaginary;
	else
		std::cout << " - " << -imaginary;
	std::cout << "i" << std::endl;
}