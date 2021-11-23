int sqdiff(char a, char b) {
	int const temp = a - b;
	return temp * temp;
}

__kernel void apply_template(__global const char *img,
                             __global const char *template,
                                      const int   width_img,
                                      const int   width_tmp,
                                      const int   height_tmp,
                             __global       float *result) {
	int const idx   = get_global_id(0);
	int const idy   = get_global_id(1);

	float acc = 0;
	int const y = 0;
	for(int x = 0; x < width_tmp; x++) {
		for(int y = 0; y < height_tmp; y++) {
			acc += sqdiff(img[x+idx + (y+idy)*width_img], template[x+y*width_tmp]);
		}
	}
	result[idx + idy*get_global_size(0)] = acc;
}

__kernel void mat_mult(__global float const* const A,
                       __global float const* const B,
					            int   const        aw,
					            int   const        bw,
					   __global float      * const C) {
	int   const x = get_global_id(0);
	int   const y = get_global_id(1);

	float       acc = 0;
	// I don't trust the compiler to optimise this away
	int   const a_off = y*aw;
	for(int n = 0; n < aw; n++)
		acc += A[n+a_off] * B[x+n*bw];
	C[x + y*bw] = acc;
}