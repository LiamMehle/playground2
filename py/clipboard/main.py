from pynput.keyboard import Key, Controller
import keyboard
import pyperclip
from time import sleep
kb = Controller()


def write():
	delay = .05
	text = ''
	with open('clipboard.txt') as f:
		for line in f.readlines():
			#line = line.strip('\t').rstrip('\n')
			if line == '\n':
				#pyperclip.copy(text)
				#pyperclip.paste()
				kb.type(text)
				kb.press(Key.enter)
				sleep(delay)
				kb.release(Key.enter)
				sleep(delay)
				text = ''
			else:
				text = f'{text}{line}'
			

keyboard.add_hotkey('ctrl+alt+v', lambda: write())
keyboard.wait('ctrl+alt+q')
