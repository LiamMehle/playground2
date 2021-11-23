import copy
'''
farm
1200 initial cost
20/banana
0/0 - 4
1/0 - 6
2/0 - 13
3/0 - 25
4/0 - 10x$200

0/2 - 1.5x multiplier
0/3 - $450/round
0/4 - $1000/round
'''
class Farm:
	eco                  = None
	upgrades             = [0,0]
	bananas_per_round    = 4
	banana_value         = 20
	bananas_per_round_ug = [[None,6,13,25,10],[None]*5]
	banana_value_ug  = [[None,None,None,None,lambda x: 200],[lambda banana_value: banana_value*1.5,None,None,None,None]]

	def upgrade(this, side):
		this.upgrades[side] += 1
		level = this.upgrades[side]
		if type(this.bananas_per_round_ug[side][level]) != type(None):
			this.bananas_per_round = this.bananas_per_round_ug[side][level]
		if type(this.banana_value_ug[side][level]) != type(None):
			this.banana_value = this.banana_value_ug[side][level](this.banana_value)


	def update(this):
		this.eco = this.banana_per_round * this.eco_per_banana

	def __init__(this):
		this.update()

class Game:
	eco = 200.0
	money = 500.0
	cost_per_eco = 25

	farm_cost = 1200
	farm_upgrade_costs = [] # todo: implement
	farms = []

	queue = []

	eco_costs = [25, 42, 60, 75, 90]
	eco_cds   = [.8, .6, .5,.52, .2]

	time  = 0
	level = 0 # aka round, unimplemented
	last_eco_time = 0

	def spend(this, cost):
		if cost > this.money:
			raise Exception('Not enough money')
		this.money -= cost

	def buy_farm(this): # return id (index)
		raise NotImplementedError()
	
	def upgrade_farm(this, id, side, times=1):
		raise NotImplementedError()

	def send_eco(this, cost, time):
		if len(this.queue) > 4:
			raise Exception('Queue is full')
		this.spend(cost) # throws if request is invalid
		# request is valid
		bonus_eco = cost/this.cost_per_eco
		# apply changes
		this.eco      += bonus_eco
		if len(this.queue) == 0:
			this.queue.append(time+this.time)
		else:
			this.queue.append(this.queue[-1]+time)


	def send_max_eco_impl(this):
		max_cost = 0
		time     = 0
		for cost, t in zip(this.eco_costs, this.eco_cds):
			if cost > max_cost and cost < this.money:
				max_cost = cost
				time = t
		this.send_eco(max_cost, time)
		return cost
	
	def send_max_eco(this):
		while len(this.queue) < 5:
			money_spent = this.send_max_eco_impl()
			if not money_spent: # will raise exception when full
				break                        # if nothing was sent, no more eco can be sent

	def handle_eco(this):
		while this.time >= this.last_eco_time + 6:
			this.money += this.eco
			this.last_eco_time += 6
	
	def handle_queue(this):
		for time in this.queue:
			if time < this.time:
				this.queue.remove(time)

	def update(this):
		this.handle_eco()
		this.handle_queue()

	def tick(this, delta_t):
		this.time += delta_t
		this.update()
	
	def skip_to(this, t):
		if t < this.time:
			raise Exception('Going back in time is not supported.')
		if t == this.time:
			return
		this.time = t
		this.update()


class Action:
	desc = None
	def do_action(self, game):
		raise NotImplementedError()

class Send_eco(Action):
	desc = 'Send eco'
	def do_action(self, game):
		game.send_max_eco()

class Wait(Action):
	desc = 'nothing'
	def do_action(self, game):
		pass

class Buy(Action):
	desc = 'buying defence'
	cost = 0
	def __init__(self, c):
		self.cost = c

	def do_action(self, game):
		game.spend(self.cost)

# simulation
def simulate(action_list, sim_time):
	g = Game()
	game_states = []
	for t in range(sim_time):
		g.skip_to(t)                # advance game state
		try:
			action_list[t].do_action(g) # do action
		except Exception as e:
			pass
		game_states.append(copy.deepcopy(g))
	return game_states