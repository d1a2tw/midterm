import numpy as np
import serial
import time

waitTime = 0.3

information=1
# generate the waveform table



# output formatter
formatter = lambda x: "%.3d" % x


# send the waveform table to K66F

s = serial.Serial('/dev/ttyACM0',115200)

while(1):
  if(information==1):
    print("available song list:\r\n")
    print("song1\r\n")
    print("song2\r\n")
    print("song3\r\n")
    print("wait for mbed calling to send song....")
    information=0

  


  line=s.readline()
  index=int(line)

  if(index==1):
    information==1
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

    print("Sending song1 ...")
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

    #s.close()
    print("Signal sended")



  if(index==2):
    information==1
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

    print("Sending song2 ...")
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

    #s.close()
    print("Signal sended")

  if(index==3):
    information==1
    songname= 3
    signalLength = 42

    signalTable = [
    659, 622, 659, 622, 659, 494, 554,
    523, 440, 0  , 262, 330, 440, 494,
    0  , 330, 415, 494, 523, 0, 659, 
    622, 659, 622, 659, 494, 554,523, 
    440, 0  , 262, 330, 440, 494, 0 ,
    330, 415, 494, 523,   0 ,  0 ,0]

    noteLength = [
    1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 1, 1, 2, 1,
    1, 1, 1, 2, 1, 1, 1,
    1, 1, 1, 1, 1, 2, 1,
    1, 1, 2, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1 ]

    print("Sending song 3...")
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

    #s.close()
    print("Signal sended")
