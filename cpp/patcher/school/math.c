#include <stdio.h>

#define DX ((float)0.1)

float integrate(float f(float), float a, float b) {
	float sum = 0;
	for(float i = a; i < b; i += DX)
		sum += f(i);
	return sum/(float)DX;
}

float diff(float f(float), float x) {
	const float dx = DX;
	const float dy = f(x+dx) - f(x);
	return (float)dy/dx;
}

float f(float x) {
	if(x >= 0 && x <= 3)
		return x/3;
	if(x >= 3 && x <= 6)
		return -x/3+2;
	return 0;
}

int main_math() {
	for(float x = (float)0.1; x < 6; x += (float)0.1)
		printf("x = %.2f,\tf(x) = %.2f,\tdf(x)/dx = %.2f,\tint(f(x),0,y) = %.2f\n",
			    x,         f(x),         diff(f,x),          integrate(f, 0, x));
	return 0;
}
