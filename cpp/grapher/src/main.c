#include <stdio.h>

#include <sys/ioctl.h> //ioctl() and TIOCGWINSZ
#include <unistd.h>    // for STDOUT_FILENO

const/*expr*/ char empty = ' ', full = '#',
                   diff = empty ^ full;

const/*expr*/ float x_lim[] = {-5, 15},
                    y_lim[] = {-5, 5};

const/*expr*/ int A = 6;

inline float w_avg( float a, float b, float weight_a ) {
	return a*weight_a + b*(1-weight_a);
}

inline float f(float x){
	const unsigned iterations = 1000000;
	const float    dampening  = 1.0f/iterations;

	float c0 = x,
	      c1 = 0,
	      c2 = 0,
	      c3 = 0,
	      c4 = 0;

	for(unsigned i = 0; i < iterations; i++) {
		c1 = w_avg(c0 - c2, c1, dampening);
		c2 = w_avg(c1 * A/x, c2, dampening);
		c3 = c2 * 1/x;
		c4 = c3 + c3;
	}
	//printf("%5f %5f %5f %5f %5f\n", c0, c1, c2, c3, c4);
	return c4;
	//return x;
}
inline float g(float x) {
	return 2*A/(x*x-A*x);
}

inline char render_pixel(float x, float y, float delta) {
	y = (f(x) - y) < 0 ? -(f(x) - y) : (f(x) - y);
	int is_equal = y < delta;
	return empty ^ ((diff)*is_equal);
}

/*constexpr float cs_map(const float x0, const float x1,
                       const float y0, const float y1,
                       const float x ) {
	constexpr float k = (y1 - y0)/(x1 - x0);
	constexpr float m = y0 - x0;
	return x*k + m;
}*/

float map(float x0, float x1,
                    float y0, float y1,
                    float x ) {
	const float k = (y1 - y0)/(x1 - x0);
	const float m = y0 - x0;
	return x*k + m;
}

int main() {


	struct winsize size;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	const unsigned width = size.ws_col, height = size.ws_row;

	
	__asm volatile("#LLVM-MCA-BEGIN calc");
	for(int y = height-1; y >= 0; y--) {
		for(unsigned x = 0; x < width; x++) {

			putchar(render_pixel(map(0       , width,
			                         x_lim[0], x_lim[1], x),
			                     map(0       , height,
			                         y_lim[0], y_lim[1], y),
			                     (float)((y_lim[1])-y_lim[0])/height/2));
		}
		putchar('\n');
	}
	__asm volatile("#LLVM-MCA-END");

	return 0;
}