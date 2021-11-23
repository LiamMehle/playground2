import numpy as np
import pyopencl as cl
import time

class Profiler:
	name = None
	start = None
	factor = None
	def __init__(self, name, factor=1):
		self.name = name
		self.factor = factor
		self.start = float(time.time())

	def __enter__(self):
		return self

	def __del__(self):
		time_elapsed = float(time.time()) - self.start
		print(f'[{self.name}]: {time_elapsed*1000/self.factor:.2f} ms')
	
	def __exit__(self, *other):
		pass

def example() -> None:
	# -------------------------------------------------------------------
	#ctx = cl.create_some_context()
	ctx = cl.Context(dev_type=cl.device_type.GPU)
	queue = cl.CommandQueue(ctx)

	kb = int(1000*8/32)
	Mb = 1000*kb
	a_np = np.random.rand(1000*Mb).astype(np.float32)
	b_np = np.random.rand(1000*Mb).astype(np.float32)
	res_np = np.empty_like(a_np)


	mf = cl.mem_flags
	#additional_flags = mf.READ_ONLY | mf.HOST_NO_ACCESS | 
	a_g = cl.Buffer(ctx, mf.COPY_HOST_PTR, hostbuf=a_np)
	b_g = cl.Buffer(ctx, mf.COPY_HOST_PTR, hostbuf=b_np)
	res_g = cl.Buffer(ctx, mf.WRITE_ONLY | mf.HOST_READ_ONLY, res_np.nbytes)

	prg_vec = cl.Program(ctx, """
	__kernel void sum(__global const float *a_g,
	                  __global const float *b_g,
	                  __global       float *res_g) {
	  int gid = get_global_id(0)*8;
	  __global float8* const a = (__global float8*) &a_g[gid];
	  __global float8* const b = (__global float8*) &b_g[gid];
	  __global float8*       c = (__global float8*) &res_g[gid];
	  *c = 1/((*a)*(*a) + (*b)*(*b));
	}
	""").build()
	
	prg = cl.Program(ctx, """
	__kernel void sum(__global const float *a_g,
	                  __global const float *b_g,
	                  __global       float *res_g) {
	  int gid = get_global_id(0);
	  __global float* const a = (__global float*) &a_g[gid];
	  __global float* const b = (__global float*) &b_g[gid];
	  __global float*       c = (__global float*) &res_g[gid];
	  *c = 1/((*a)*(*a) + (*b)*(*b));
	  *c = 1;
	}
	""").build()

	knl = prg.sum  # Use this Kernel object for repeated calls
	#rint('enqueueing')
	def gpu_bench(x) -> None:
		#global a_np
		#global b_np
		#global res_np
		#global a_g
		#global b_g
		#global res_g

		with Profiler(f'GPU offload x{x}', x):
			size = a_np.shape[0]//1
			print(a_np.shape)
			for _ in range(x):
				knl(queue, tuple([size, 1]) , None, a_g, b_g, res_g)
			cl.enqueue_copy(queue, res_np, res_g)

	for x in range(1):
		gpu_bench(10**x)


	# Check on CPU with Numpy:
	with Profiler('CPU - numpy'):
		_ = 1/(a_np*a_np + b_np*b_np)
	with Profiler('error'):
		print(np.linalg.norm(res_np - 1))#1/(a_np*a_np + b_np*b_np)))
	with Profiler('assert'):
		assert np.allclose(res_np, 1)#/(a_np*a_np + b_np*b_np))



	# -------------------------------------------------------------------

class OCL:
	ctx = cl.Context(dev_type=cl.device_type.GPU)
	queue = cl.CommandQueue(ctx)
	program = cl.Program(ctx, open('template.cl', 'r').read()).build()
	'''
	image and template must be 1d or 2d, uint8
	'''
	def template_sqdiff(self, image, template) -> np.Array:
		ctx   = self.ctx
		queue = self.queue
		program = self.program

		dim   = len(image.shape)
		args  = [image.shape[d]-template.shape[d]+1 for d in range(dim)]
		result = np.zeros(args, dtype=np.float32)
		print(result.shape)

		# create buffer objects (no allocations)
		mf = cl.mem_flags
		image_buf    = cl.Buffer(ctx, mf.READ_ONLY  |  mf.HOST_NO_ACCESS | mf.USE_HOST_PTR, hostbuf=image)
		template_buf = cl.Buffer(ctx, mf.READ_ONLY  |  mf.HOST_NO_ACCESS | mf.USE_HOST_PTR, hostbuf=template)
		result_buf   = cl.Buffer(ctx, mf.WRITE_ONLY |  mf.HOST_READ_ONLY , result.nbytes)

		if len(result.shape) == 2:
			w     = result.shape[1]
			w_img = image.shape[1]
			h     = result.shape[0]
		elif len(result.shape) == 1:
			w     = result.shape[0]
			w_img = image.shape[1]
			h     = 1
		knl = program.apply_template
		kernel_ev = knl(queue, result.shape, None, image_buf, template_buf, np.int32(w_img), np.int32(w), np.int32(h), result_buf)
		cl.enqueue_copy(queue, result, result_buf, wait_for=[kernel_ev])
		#result = result / result.max()
		return result

	def mat_mult(self, A, B) -> np.Array:
		ctx = self.ctx
		queue = self.queue
		program = self.program
		C = np.zeros((A.shape[0], B.shape[1]), np.float32)

		# create buffer objects (no allocations)
		mf = cl.mem_flags
		A_buf = cl.Buffer(ctx, mf.READ_ONLY  |  mf.HOST_NO_ACCESS | mf.USE_HOST_PTR, hostbuf=A)
		B_buf = cl.Buffer(ctx, mf.READ_ONLY  |  mf.HOST_NO_ACCESS | mf.USE_HOST_PTR, hostbuf=B)
		C_buf = cl.Buffer(ctx, mf.WRITE_ONLY |  mf.HOST_READ_ONLY , C.nbytes)
		knl = program.mat_mult
		kernel_ev = knl(queue, C.shape, None, A_buf, B_buf, np.int32(A.shape[1]), np.int32(B.shape[1]), C_buf)
		copy_ev   = cl.enqueue_copy(queue, C, C_buf, wait_for=[kernel_ev], is_blocking=False)

		assert A.shape[1] == B.shape[0]
		assert A.dtype == np.float32
		assert B.dtype == np.float32
		assert C.dtype == np.float32

		copy_ev.wait()
		return C

def mat_mult(A, B) -> np.Array:
	assert A.shape[1] == B.shape[0]
	C = np.zeros((A.shape[0], B.shape[1]), np.float32)
	n = A.shape[1]
	for y in range(C.shape[0]):
		for x in range(C.shape[1]):
			acc = 0
			for z in range(n):
				acc += A[y,z] * B[z,x]
			C[y,x] = acc
	return C			


A = np.array([[2,3,4],[1,0,0]], np.float32)
B = np.array([[0,1000],[1,100],[0,10]], np.float32)

print(A)
print(B)
C = OCL().mat_mult(A, B)
print(C)