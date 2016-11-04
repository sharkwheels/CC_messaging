#################################
# CatBot Toy 
# v1.0
# python3 <--- remember that!
#################################

### IMPORTS  #########################################################################

import random, sys, json, datetime, re, os, unicodedata, logging, time, struct, requests
import serial
from random import choice
from time import sleep

### twython ###
from twython import Twython, TwythonError
from twython import TwythonStreamer

### LOGGING ##########################################

logging.basicConfig(filename='twitterWarn.log',level=logging.INFO)

### SERIAL ############################################

try:
	ser = serial.Serial('/dev/cu.usbmodem1421', 9600, timeout=0)	## open port
	time.sleep(2)													## sleep two seconds
	print("Connection to arduino established succesfully!\n")	## signal success
	
except Exception as e:
	print(e)
	sys.exit() 														## exit the program if there is an error


### GET KEYS  #########################################################################

### local ###
### try and find a way to do os.environ things locally
keys = []

with open('keys.txt','r') as my_file:
	keys = my_file.read().splitlines()

APP_KEY = keys[0]
APP_SECRET = keys[1]
OAUTH_TOKEN = keys[2]
OAUTH_TOKEN_SECRET = keys[3]

twitter = Twython(APP_KEY, APP_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
twitter.verify_credentials()


### CAT STREAMER TRACKER ###############################################################

class catStream(TwythonStreamer):
	def on_success(self, data):
		if 'text' in data:
			print(data['text'].encode('utf-8'))
			toSend = 2
			ser.write(struct.pack('>B', toSend))	# read more: https://docs.python.org/3/library/struct.html
			## track some particular text
			## send a command
				## flip a timer for 30 seconds
				## send an off command. 

	def on_error(self, status_code, data):
		print(status_code)


cStream = catStream(APP_KEY, APP_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
cStream.statuses.filter(track="@smolcatbug") #only works if you are public