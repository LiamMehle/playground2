class Fraction {
private:
	constexpr
	long long gcd( long long a, long long b );

public:
	long long num;
	long long den;

	Fraction();
	Fraction( const long long _num, const long long _den );
	Fraction( const long long _num );
	
	Fraction fraction_from_float( const double frac );
	Fraction fraction_with_precision( const double frac, const long long precision );

	double eval();
	void   simplify();
	Fraction operator*( const Fraction& rhs );
	Fraction operator/( const Fraction& rhs );
	Fraction operator+( const Fraction& rhs );
	Fraction operator-( Fraction rhs );
};
