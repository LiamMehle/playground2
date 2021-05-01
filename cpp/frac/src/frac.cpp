#include "frac.hpp"

constexpr
long long Fraction::gcd( long long a, long long b ) {
	if( a == 1 || b == 1 )
		return 1;
	if( a == 0 )
		return b;
	if( b == 0 )
		return a;
	while(true) {
		if( a > b )
			a %= b;
		else if( a < b )
			b %= a;
		
		if( a == 1 || b == 1 )
			return 1;
		if( a == 0 )
			return b;
		if( b == 0 )
			return a;
		if( a == b )
			return a;
	}
}

Fraction Fraction::fraction_with_precision( const double frac, const long long precision ) {
	Fraction ret;
	ret.num = static_cast<long long>(frac * static_cast<double>(precision));
	ret.den = precision;
	ret.simplify();
	return ret;
}

Fraction::Fraction() : num(0), den(0) {}

Fraction::Fraction( const long long _num, const long long _den ) {
	num = _num;
	den = _den;
}

Fraction::Fraction( const long long _num ) {
	num = _num;
	den = 1;
}

Fraction Fraction::fraction_from_float( const double frac ) {
	return fraction_with_precision( frac, 1000000000 );
}

double Fraction::eval() {
	return static_cast<double>(num)/static_cast<double>(den);
}

Fraction Fraction::operator*( const Fraction& rhs ) {
	Fraction ret;
	ret.num = num * rhs.num;
	ret.den = den * rhs.den;
	return ret;
}

Fraction Fraction::operator/( const Fraction& rhs ) {
	Fraction ret;
	ret.num = num * rhs.den;
	ret.den = den * rhs.num;
	return ret;
}

Fraction Fraction::operator+( const Fraction& rhs ) {
	Fraction ret;
	if( den == rhs.den ) {
		ret.den = den;
		ret.num = num + rhs.num;
		return ret;
	}
	ret.num = num*rhs.den + rhs.num*den;
	ret.den = den*rhs.den;
	return ret;
}

Fraction Fraction::operator-( Fraction rhs ) {
	rhs.num = -rhs.num;
	return *this + rhs;
}

void Fraction::simplify() {
	auto _gcd = gcd( num, den );
	num /= _gcd;
	den /= _gcd;
}