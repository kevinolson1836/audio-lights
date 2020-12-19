# Beat tracking example
import RPi.GPIO as GPIO
import librosa
from datetime import datetime
from os import path
from pydub import AudioSegment
import time
import pygame
import mutagen.mp3
import pyaudio
import numpy as np
import math

song = "break_my_stride"

# 2. Load the audio as a waveform `y`
#    Store the sampling rate as `sr`
y, sr = librosa.load(f"{song}.mp3", sr=None, duration=None)

# 3. Run the default beat tracker
tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)

# 4. Convert the frame indices of beat events into timestamps
beat_times = librosa.frames_to_time(beat_frames, sr=sr)

maxValue = 2**1
p=pyaudio.PyAudio()
stream=p.open(format=pyaudio.paInt16,channels=2,rate=44100,
              input=True, frames_per_buffer=1024)


# convert mp3 to wav
sound = AudioSegment.from_mp3(f"{song}.mp3")
sound.export(f"{song}.wav", format="wav")

beat_times = beat_times.tolist()

mp3 = mutagen.mp3.MP3(f"{song}.mp3")
pygame.mixer.init(frequency=mp3.info.sample_rate)
pygame.mixer.music.load(f"{song}.mp3")
pygame.mixer.music.play()


ledpin1 = 12				# PWM pin connected to LED
ledpin2 = 13				# PWM pin connected to LED
ledpin3 = 18				# PWM pin connected to LED
GPIO.setwarnings(False)			#disable warnings
GPIO.setmode(GPIO.BOARD)		#set pin numbering system
GPIO.setup(ledpin1,GPIO.OUT)
GPIO.setup(ledpin2,GPIO.OUT)
GPIO.setup(ledpin3,GPIO.OUT)
pi_pwm1 = GPIO.PWM(ledpin1,1000)		#create PWM instance with frequency
pi_pwm2 = GPIO.PWM(ledpin2,1000)		#create PWM instance with frequency
pi_pwm3 = GPIO.PWM(ledpin3,1000)		#create PWM instance with frequency
pi_pwm1.start(0)				#start PWM of required Duty Cycle 
pi_pwm2.start(0)				#start PWM of required Duty Cycle 
pi_pwm3.start(0)				#start PWM of required Duty Cycle 
# for i in beat_times:
#     data = random.randint(0,5)
#     while prev_data == data:
#         data = random.randint(0,5)
#     print(data)
#     time.sleep(beat_times[int(count)] - prev_time)
#     arduino.write(bytes(str(data), 'utf-8'))   
#     print("pulse a color, delay is: ", beat_times[int(count)] - prev_time)
#     prev_data = data  
#     prev_time = beat_times[int(count)]   
#     count += 1


for i in beat_times:
    data = np.fromstring(stream.read(1024),dtype=np.int16)
    dataL = data[0::2]
    # dataR = data[1::2]
    peakL = np.abs(np.max(dataL)-np.min(dataL))/maxValue
    print(peakL)
    write = math.ceil((peakL*100)%255)
    print(write)

    pi_pwm1.ChangeDutyCycle(write) #provide duty cycle in the range 0-100
    pi_pwm2.ChangeDutyCycle(write) #provide duty cycle in the range 0-100
    pi_pwm3.ChangeDutyCycle(write) #provide duty cycle in the range 0-100
    sleep(0.01)
    # time.sleep(.1)

