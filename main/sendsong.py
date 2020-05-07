import numpy as np
import serial
import time

waitTime = 0.1

# generate the waveform table
signalLength = 42

signalTable = [
  261, 261, 392, 392, 440, 440, 392,
  349, 349, 330, 330, 294, 294, 261,
  392, 392, 349, 349, 330, 330, 294,
  392, 392, 349, 349, 330, 330, 294,
  261, 261, 392, 392, 440, 440, 392,
  349, 349, 330, 330, 294, 294, 261]

noteLength = [
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2 ]


# output formatter
formatter = lambda x: "%.3d" % x

# send the waveform table to K66F
serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)
print("Sending song ...")
print("It may take about %d seconds ..." % (int(signalLength * waitTime)))
for data in signalTable:
  s.write(bytes(formatter(data), 'UTF-8'))
  time.sleep(waitTime)

print("Sending Length ...")
print("It may take about %d seconds ..." % (int(signalLength * waitTime)))

for data in noteLength:
  s.write(bytes(formatter(data), 'UTF-8'))
  time.sleep(waitTime)


s.close()
print("Signal sended")