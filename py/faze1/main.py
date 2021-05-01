if __name__ != '__main__':
	quit()

from math import *

P_m = 5.5 * 1000 # W
n_m = 1455 / 60  # Hz
n_d = 80   / 60
n_a = 380  / 60

i_12 = n_m/n_a
print(f'i_12 = n_m/n_a = {n_m:.2f}/{n_a:.2f} = {i_12:.2f}')

i_34 = n_a/n_d
print(f'i_34 = n_a/n_d = {n_a:.2f}/{n_d:.2f} = {i_34:.2f}')

z_1 = 17
z_2 = round(z_1 * i_12)

print(f'z_2 = {z_2}')

z_3 = round(z_2 * i_34)

print(f'z_3 = {z_3}')


i_sk = i_12*i_34
print(f'i_sk = i_12*i_34 = {i_12:.2f}/{i_34:.2f} = {i_sk:.2f}')


print()
n_2 = n_m / i_12
print(f'n_2 = n_m / i_12 = {n_m:.2f}/{i_12:.2f} = {n_2:.2f}')


#def avg( array ):
#	return sum(array)/len(array)
#
#A = .5 ** 2
#
#T_k = [
#	33.0,
#	33.8,
#	33.7,
#	33.5,
#	34.5,
#	34.6,
#	33.7,
#	33.9,
#	34.4
#]
#T_l = [
#	30.0,
#	30.7,
#	30.6,
#	30.1,
#	30.9,
#	31.2,
#	29.6,
#	30.1,
#	30.4,
#	31.0
#]
#T_ok = [
#	24.5,
#	24.1,
#	23.7,
#	23.6,
#	23.7,
#	23.8,
#	23.8,
#	23.4,
#	23.4
#]
#
#T_gr = 34.67 # calculated, can't be fucked
#
#T_k  = avg( T_k  )
#T_l  = avg( T_l  )
#T_ok = avg( T_ok )
#
#
#P_e = 45.7
#delta_k  = .009
#delta_l  = .008
#lambda_k = 1.28
#lambda_l = 0.14
#eta_k    = 0.5
#eta_l    = 0.9
#sigma    = 5.67 * 10 ** -8
#
#R_k = delta_k / (lambda_k * A)
#R_l = delta_l / (lambda_l * A)
#
#dQ_kp = (T_gr - T_k)/R_k
#dQ_lp = (T_gr - T_l)/R_l
#
#print(f'dQ_kp = {dQ_kp:.2f}')
#print(f'dQ_lp = {dQ_lp:.2f}')
#
#dQ_ks = eta_k * A * ((T_k + 273.15)**4 - (T_ok + 273.15)**4) * sigma
#dQ_ls = eta_l * A * ((T_l + 273.15)**4 - (T_ok + 273.15)**4) * sigma
#
#print(f'dQ_ks = {dQ_ks:.2f}')
#print(f'dQ_ls = {dQ_ls:.2f}')
#
#dQ_kk = dQ_kp - dQ_ks
#dQ_lk = dQ_lp - dQ_ls
#print(f'dQ_kk = {dQ_kk:.2f}')
#print(f'dQ_lk = {dQ_lk:.2f}')
#