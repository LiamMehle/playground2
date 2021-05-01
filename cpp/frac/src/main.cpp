#include <iostream>
#include "frac.hpp"

int main() {
	Fraction a(2, 1), b(2, 2);
	Fraction c = a + b / 2 + 3;
	a.simplify();
	b.simplify();
	c.simplify();
	std::cout << "\na: " << a.num << '/' << a.den << " = " << a.eval()
	          << "\nb: " << b.num << '/' << b.den << " = " << b.eval()
	          << "\nc: " << c.num << '/' << c.den << " = " << c.eval() << '\n';
}