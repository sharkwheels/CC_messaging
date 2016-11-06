#################################
# Twitter Plays With My Cat
# v1.0
# python3 <--- remember that for print statements!
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
	#sys.exit() 														## exit the program if there is an error


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
	""" Twitter Streamer Class """

	## can be accessed by all methods in the class
	numberOfReplies = 0												# number of replies
	windDownCount = 10												# 10 seconds of wind down count
	topLimit = 5													# setting this to 20 but could be like 100 or 1000 or whatever

	def on_success(self, data):

		if 'text' in data:
			#print(data['text'].encode('utf-8')) # why its like this in the documentation I will never know. 

			### get your data as a str
			body = data['text']
			user = data['user']['screen_name']

			## set up some specific things to track
			acct = "@smolcatbug"
	
			if body.startswith(acct):								# if it is an @reply not a mention 	
				bodyStrip = re.sub(r'[^\w\s]','',body.lower())		# strip everything but words
				
				self.numberOfReplies+=1								# update the counter by 1
				ser.write(struct.pack('>B',5))						# command to increase the positioning 	
				
				## this will only find the first incident of each word. whatever comes first. ok with that. 

				if "faster" in bodyStrip:
					print("faster")
					ser.write(struct.pack('>B', 4))					# increase servo speed
				elif "slower" in bodyStrip:
					print("slower")
					ser.write(struct.pack('>B', 3))					# decrease servo speed
				elif "stop" in bodyStrip and user == "smolcatbug":
					print("stop")
					self.wind_it_down()								# only a particular user account can stop it
				elif "go" in bodyStrip and user == "smolcatbug":
					print("go")
					ser.write(struct.pack('>B', 2))					# command a particular user account can start it
				else:
					print("no commands, do nothing")

		## This is to basically manipulate people to keep interacting with it.
		## it also puts in some natural high and low movements into the toy

		if self.numberOfReplies >= self.topLimit:
			self.numberOfReplies = self.topLimit	## keep it high.
			self.wind_it_down()						## start winding it down ?

		print("!on_success: ", self.numberOfReplies)

	def wind_it_down(self):
		"""Wind it down to zero over 10 seconds """
		for x in range(self.windDownCount):
			self.windDownCount -= 1							# decrease the wind down count
			ser.write(struct.pack('>B', 6)) 				# send a command do decrease positioning 		
			print("!wind_it_down",self.windDownCount)		# track what's going on in this function
			sleep(1)										# sleep one second
		
		if self.windDownCount <= 0:
			self.hard_reset()

	def hard_reset(self):
		""" reset everything """
		self.numberOfReplies = 0;			# reset the reply number
		ser.write(struct.pack('>B', 7))		# send a reset command to the arduino
		self.windDownCount = 10				# reset the wind down count 

	

	def on_error(self, status_code, data):
		print(status_code)


cStream = catStream(APP_KEY, APP_SECRET, OAUTH_TOKEN, OAUTH_TOKEN_SECRET)
cStream.statuses.filter(track="@smolcatbug") #only works if you are public


