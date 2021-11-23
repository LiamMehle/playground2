import requests as req

from threading import Thread
import requests as req

import multiprocessing as mp
import time
import re

def make_thread(f, *args):
	t = Thread(target=f, args=list(args))
	t.start()
	return t

stealthy = False

url = 'https://visfs.uni-lj.si/'

s = req.Session()
s.get(url)
if stealthy:
	t = make_thread(s.get, url) # dummy request
	time.sleep(2)
	t.join()

login_req = {
	'up_ime':    'lm5027@student.uni-lj.si',
	'geslo':     'M0jegesl0',
	'potrditev': 'Vstop'
}


r = s.post(url+'prijava_kon.asp', data=login_req)
#if r.status_code != 302:
#	print('failed to login')
#	quit(1)
if stealthy:
	response_main = s.post(url+'main.asp')
	time.sleep(3)








def make_subject_req(index, token, klic):
	data = {	
		'predmet_opravljen': '0',
		'predmet':           f'{index}',
		'klic':              klic,
		'token':             token
	}
	return data

r = s.get(url+'port_pred_pregled.asp')
codes = re.findall('(?<=<option value=")[0-9]*', r.text)
names = re.findall('.*(?=$\n\r</option>)', r.text)

token = re.findall('(?<=<input type="hidden" name="token" value=")[0-9A-Z]+-[0-9A-Z]+-[0-9A-Z]+-[0-9A-Z]', r.text)[0]
klic  = re.findall('(?<=document.obrazec.klic.value=\').*(?=\')', r.text)[0]

def scrape_subject(url, data, name):
	def download_file(url):
		local_filename = url.split('/')[-1]
		# NOTE the stream=True parameter below
		with requests.get(url, stream=True) as r:
			r.raise_for_status()
			with open(local_filename, 'wb') as f:
				for chunk in r.iter_content(chunk_size=8192): 
					# If you have chunk encoded response uncomment if
					# and set chunk_size parameter to None.
					#if chunk: 
					f.write(chunk)
		return local_filename
	

	r = s.post(url, data)
	print(f'{i:4} = {r}')
	urls = re.findall('(?<=href=").*(?=".{1,2}<img src="download.png">)', r.text)
	for url in urls:
		print(url)
		download_file(url)


for i, name in zip(codes, names):
	i = int(i)
	scrape_subject(url+'port_pred_pregled_1.asp', make_subject_req(i, token, klic), name)

del make_subject_req
del codes
del token
del klic