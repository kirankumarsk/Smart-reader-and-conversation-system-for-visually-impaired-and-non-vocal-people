import RPi.GPIO as GPIO
import os, sys
import logging
import subprocess
import threading
import time
USER VARIABLES
DEBUG = 0 Debug 0/1 off/on (writes to debug.log)
SPEED = 1.0 Speech speed, 0.5 - 2.0
VOLUME = 90 Audio volume
OTHER SETTINGS
SOUNDS = "/home/pi/PiTextReader/sounds/" Directory for sound effect(s)
CAMERA = "raspistill -cfx 128:128 --awb auto -rot 180 -t 500 -o /tmp/image.jpg"
GPIO BUTTONS
BTN1 = 24 The button!
LED = 18 The button's LED!
FUNCTIONS
Thread controls for background processing
class RaspberryThread(threading.Thread):
def __init__(self, function):
self.running = False
self.function = function
super(RaspberryThread, self).__init__()
def start(self):
self.running = True
super(RaspberryThread, self).start()
def run(self):
while self.running:
self.function()
def stop(self):
self.running = False
LED ON/OFF
def led(val):
logger.info('led('+str(val)+')')
if val:
GPIO.output(LED,GPIO.HIGH)
else:
GPIO.output(LED,GPIO.LOW)
PLAY SOUND
def sound(val): Play a sound
logger.info('sound()')
time.sleep(0.2)
cmd = "/usr/bin/aplay -q "+str(val)
logger.info(cmd)
os.system(cmd)
return
SPEAK STATUS
def speak(val): TTS Speak
logger.info('speak()')
cmd = "/usr/bin/flite -voice awb --setf duration_stretch="+str(SPEED)+" -t \""+str(val)+"\""
logger.info(cmd)
os.system(cmd)
return
SET VOLUME
def volume(val): Set Volume for Launch
logger.info('volume('+str(val)+')')
vol = int(val)
cmd = "sudo amixer -q sset PCM,0 "+str(vol)+"%"
logger.info(cmd)
os.system(cmd)
return
TEXT CLEANUP
def cleanText():
logger.info('cleanText()')
cmd = "sed -e 's/\([0-9]\)/& /g' -e 's/[[:punct:]]/ /g' -e 'G' -i /tmp/text.txt"
logger.info(cmd)
os.system(cmd)
return
Play TTS (Allow Interrupt)
def playTTS():
logger.info('playTTS()')
global current_tts
current_tts=subprocess.Popen(['/usr/bin/flite','-voice','awb','-f', '/tmp/text.txt'],
stdin=subprocess.PIPE,stdout=subprocess.PIPE,
stderr=subprocess.PIPE,close_fds=True)
Kick off stop audio thread
rt.start()
Wait until finished speaking (unless interrupted)
current_tts.communicate()
return
Stop TTS (with Interrupt)
def stopTTS():
global current_tts
If button pressed, then stop audio
if GPIO.input(BTN1) == GPIO.LOW:
logger.info('stopTTS()')
current_tts.terminate()
current_tts.kill()
time.sleep(0.5)
return
GRAB IMAGE AND CONVERT
def getData():
logger.info('getData()')
led(0) Turn off Button LED
Take photo
sound(SOUNDS+"camera-shutter.wav")
cmd = CAMERA
logger.info(cmd)
os.system(cmd)
OCR to text
speak("now working. please wait.")
cmd = "/usr/bin/tesseract /tmp/image.jpg /tmp/text"
logger.info(cmd)
os.system(cmd)
Cleanup text
cleanText()
Start reading text
playTTS()
return
MAIN
try:
global rt
Setup Logging
logger = logging.getLogger()
handler = logging.FileHandler('debug.log')
if DEBUG:
logger.setLevel(logging.INFO)
handler.setLevel(logging.INFO)
else:
logger.setLevel(logging.ERROR)
handler.setLevel(logging.ERROR)
log_format = '%(asctime)-6s: %(name)s - %(levelname)s - %(message)s'
handler.setFormatter(logging.Formatter(log_format))
logger.addHandler(handler)
logger.info('Starting')
Setup GPIO buttons
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings (False)
GPIO.setup(BTN1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(LED, GPIO.OUT)
Threaded audio player
rt = RaspberryThread( function = repeatTTS ) Repeat Speak text
rt = RaspberryThread( function = stopTTS ) Stop Speaking text
volume(VOLUME)
speak("OK, ready")
led(1)
while True:
if GPIO.input(BTN1) == GPIO.LOW:
Btn 1
getData()
rt.stop()
rt = RaspberryThread( function = stopTTS ) Stop Speaking text
led(1)
time.sleep(0.5)
speak("OK, ready")
time.sleep(0.2)
except KeyboardInterrupt:
logger.info("exiting")
GPIO.cleanup() Reset GPIOs
sys.exit(0)
Programs for the conversation system:
from bluetooth import *
import binascii
from num2words import num2words
from subprocess import call
cmd_beg= 'espeak '
cmd_end= ' 2>/dev/null'
server_sock=BluetoothSocket( RFCOMM )
bd_addr = "00:18:E4:40:00:06"
port = 1
server_sock.connect((bd_addr,port))
data = ""
while True:
data = server_sock.recv(1024)
if len(data) != 0 :
Dept. of ECE, AIT, Chikmagalur Page 53
_str = str(data)
_str = _str.replace("b","")
_str = _str.replace("'","")
print (_str)
data = ""
if(_str == '1'):
text = "i need water"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '2'):
text = "i need food"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '3'):
text = "i need medicine"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '4'):
text = "Good morning"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '5'):
text = "Good afternoon"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '6'):
text = "Good night"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '7'):
text = "Ok"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '8'):
text = "Correct"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
if(_str == '9'):
text = "Wrong"
text = text.replace(' ', '_')
call([cmd_beg+text+cmd_end], shell=True)
server_sock.close()
print ("all done")