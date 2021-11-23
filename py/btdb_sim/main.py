import multiprocessing as mp
from threading import Thread
from matplotlib import pyplot as plt
import printer
from   printer import *
from   game    import *

# init window in main process
perf_log('launching off-screen window creation thread')
window_thread = Thread(target=plt.show())
window_thread.start()
##########################################################################
# configure sim
perf_log('generating action sets')
sim_time = 1000 # seconds

actions_wait        = [Wait()] * sim_time
actions_invest      = ([Send_eco()] * 5) + ([Wait()] * (sim_time-5))
actions_full_invest = [Send_eco()] * sim_time

action_sets = [actions_wait, actions_invest, actions_full_invest]
##########################################################################

# work to be performed by workers, each in its own process
def work(id, action_set, sim_time, return_q):

	replay = simulate(action_set, sim_time)
	x = []
	y = []
	with open(f'{str(i)}.csv', 'w+') as file:
		for frame in replay:
			file.write(f'{frame.time},{frame.money}\n')
			x.append(frame.time)
			y.append(frame.money)

	return_q.put([id, x, y])

# spawn workers
perf_log('spawning workers')
replay_queue = mp.Queue()
workers = []
for i,action_set in enumerate(action_sets):
	workers.append(mp.Process(target=work, args=[i, action_set, sim_time, replay_queue]))
	workers[-1].start()

perf_log('joining off-screen window creation thread')
window_thread.join()

perf_log('plotting data form workers')
for _ in range(len(action_sets)):
	replay = replay_queue.get()
	x = replay[1]
	y = replay[2]
	plt.plot(x, y)
	# id is neglected here

perf_log('drawing plots')
plt.pause(0.001)

perf_log('joining workers to save resources')
for worker in workers:
	worker.join()

perf_log('done')
perf_log(None)
plt.show()

stop_printer()