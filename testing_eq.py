import pyaudio
import wave
from array import array
from struct import pack
import numpy as np
import time 
import chunk
from pathlib import Path

song = "break_my_stride.wav"
path = Path(song)

# x = chunk.Chunk(open(song))

# print(x)
CHUNK = 32993180 #measured in bytes
maxValue = 2**16
bars = 35

wf = wave.open(song, 'rb')

p = pyaudio.PyAudio()

stream = p.open(format=p.get_format_from_width(wf.getsampwidth()),
                channels=wf.getnchannels(),
                rate=wf.getframerate(),
                output=True)

data = wf.readframes(CHUNK)
# print(data)
print("here")
while len(data) > 0:
        stream.write(data)
        # print(stream.read(1024))
        # data = wf.readframes(CHUNK)
        data = np.fromstring(wf.readframes(CHUNK),dtype=np.int16)
        # print(data)
        dataL = data[0::2]
        dataR = data[1::2]
        peakL = np.abs(np.max(dataL)-np.min(dataL))/maxValue
        peakR = np.abs(np.max(dataR)-np.min(dataR))/maxValue
        lString = "#"*int(peakL*bars)+"-"*int(bars-peakL*bars)
        rString = "#"*int(peakR*bars)+"-"*int(bars-peakR*bars)
        print("L=[%s]\tR=[%s]"%(lString, rString))
        time.sleep(1)