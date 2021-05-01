from pynput.keyboard import Key, Controller
import keyboard
from time import sleep
kb = Controller()
delay = .007
def write():
	with open('clipboard.txt') as f:
		for line in f.readlines():
			line = line.strip('\t').rstrip('\n')
			kb.type(line)
			sleep(delay)
			kb.press(Key.enter)
			sleep(delay)
			kb.release(Key.enter)
			sleep(delay)

keyboard.add_hotkey('ctrl+alt+v', lambda: write())
keyboard.wait('ctrl+alt+q')
