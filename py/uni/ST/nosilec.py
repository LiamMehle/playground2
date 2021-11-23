import math

F = 5.5*1000
q = 2000
a = 1.7
b = 2.8
alpha = 30/180*math.pi
print(math.pi)
print(math.sin(30))
print(math.sin(alpha))
Mq = q*(a+b)**2/2
Mf = b*F*.5#math.sin(alpha)
Fq = q*(a+b)
F  = F*.5#math.sin(alpha)

print(f'M  = {(Mq+Mf)/1000:.2f}')
print(f'Az = {(Fq+F)/1000:.2f}')