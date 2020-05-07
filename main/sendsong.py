import numpy as np
import serial
import time

waitTime = 0.1

# generate the waveform table



# output formatter
formatter = lambda x: "%.3d" % x


# send the waveform table to K66F
serdev = '/dev/ttyACM0'
s = serial.Serial(serdev)
while(1):

  line=s.readline()
  index=int(line)

  if(index==1):

    songname= 1
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

    print("Sending song index ...")
    s.write(bytes(formatter(songname), 'UTF-8'))
    time.sleep(waitTime)

    s.close()
    print("Signal sended")



  if(index==2):

    songname= 2
    signalLength = 42

    signalTable = [
    392, 330, 330, 349, 294, 294, 262,
    294, 330, 349, 392, 392, 392, 392,
    330, 330, 349, 294, 294, 262, 330,
    392, 392, 262, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0]

    noteLength = [
    1, 1, 2, 1, 1, 2, 1,
    1, 1, 1, 1, 1, 2, 1,
    1, 1, 1, 1, 2, 1, 1,
    1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1 ]

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

    print("Sending song index ...")
    s.write(bytes(formatter(songname), 'UTF-8'))
    time.sleep(waitTime)

    s.close()
    print("Signal sended")