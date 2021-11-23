balance = 1000
tx_fee  = 10
for t in range(5*60):
	if t == 0:
		balance -= 60
	if t % 10 == 0 and t != 0:
		balance += tx_fee
		print(f't={t:>3} bal={balance}')
