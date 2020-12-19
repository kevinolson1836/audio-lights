# Beat tracking example
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
import serial


song = "stacy's_mom"

# 2. Load the audio as a waveform `y`
#    Store the sampling rate as `sr`
y, sr = librosa.load(f"{song}.mp3", sr=None, duration=None)

# 3. Run the default beat tracker
tempo, beat_frames = librosa.beat.beat_track(y=y, sr=sr)

# 4. Convert the frame indices of beat events into timestamps
beat_times = librosa.frames_to_time(beat_frames, sr=sr)

maxValue = 256
p=pyaudio.PyAudio()
stream=p.open(format=pyaudio.paInt16,channels=2,rate=44100,
              input=True, frames_per_buffer=1024)


# convert mp3 to wav
sound = AudioSegment.from_mp3(f"{song}.mp3")
sound.export(f"{song}.wav", format="wav")

beat_times = beat_times.tolist()
arduino=serial.Serial('/dev/ttyACM0', 9600)
time.sleep(2)

mp3 = mutagen.mp3.MP3(f"{song}.mp3")
pygame.mixer.init(frequency=mp3.info.sample_rate)
pygame.mixer.music.load(f"{song}.mp3")
pygame.mixer.music.play()



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

prev_time =0
count =0
prev_write = 0
write_avg = 0
for i in beat_times:
    if count != 0:
        prev_time = beat_times[count-1] - 0.05
    data = np.fromstring(stream.read(1024),dtype=np.int16)
    dataL = data[0::2]
    # dataR = data[1::2]
    peakL = np.abs(np.max(dataL)-np.min(dataL))/maxValue
    write = math.ceil((peakL*100)%255)
    write_avg = (write + prev_write)/2
    print("wrote: ",write_avg, "\nsleeping for: ", abs(prev_time - beat_times[count]))
    print("write size: ", len(str(write_avg).encode('utf-8')))
    arduino.write(bytes(str(write_avg), 'utf-8'))
    arduino.write(bytes('\0', 'utf-8'))
    time.sleep(abs(beat_times[count]- prev_time))
    count = count +1
    prev_write = write_avg