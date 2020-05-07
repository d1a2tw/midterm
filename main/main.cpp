#include "mbed.h"
#include <cmath>
#include "DA7212.h"

DA7212 audio;


#define bufferLength (32)

#define songLength (42)



InterruptIn button(SW2);

InterruptIn keyboard0(SW3);

DigitalOut green_led(LED2);


int16_t waveform[kAudioTxBufferSize];
Serial pc(USBTX, USBRX);
EventQueue queue(32 * EVENTS_EVENT_SIZE);
Thread t;

int song[42] ;
int noteLength[42];

int play=0;
int idC = 0;


//int16_t signal[signalLength];



char serialInBuffer[bufferLength];

int serialCount = 0;

/*int song[42] = {
  261, 261, 392, 392, 440, 440, 392,
  349, 349, 330, 330, 294, 294, 261,
  392, 392, 349, 349, 330, 330, 294,
  392, 392, 349, 349, 330, 330, 294,
  261, 261, 392, 392, 440, 440, 392,
  349, 349, 330, 330, 294, 294, 261};

int noteLength[42] = {
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2,
  1, 1, 1, 1, 1, 1, 2};*/

void loadSong(void)

{

  green_led = 0;

  int i=0 ;

  serialCount = 0;

  audio.spk.pause();

  while( i < songLength)

  {
    

    if(pc.readable())

    {

      serialInBuffer[serialCount] = pc.getc();

      serialCount++;

      if(serialCount == 3)

      {

        serialInBuffer[serialCount] = '\0';

        song[i] = (int) atoi(serialInBuffer);
        
        serialCount = 0;

        i++;

      }

    }

  }
  i=0;
  while( i < songLength)

  {
  
    
    if(pc.readable())

    {

      serialInBuffer[serialCount] = pc.getc();

      serialCount++;

      if(serialCount == 3)

      {

        serialInBuffer[serialCount] = '\0';

        noteLength[i] = (int) atoi(serialInBuffer);
        
        serialCount = 0;

        i++;

      }

    }

  }

  




 green_led = 1;
play=1;
}

void loadSongHandler(void) {

queue.call(loadSong);

}




void playNote(int freq)

{

  for (int i = 0; i < kAudioTxBufferSize; i++)

  {

    waveform[i] = (int16_t) (sin((double)i * 2. * M_PI/(double) (kAudioSampleFrequency / freq)) * ((1<<16) - 1));

  }

  // the loop below will play the note for the duration of 1s

  for(int j = 0; j < kAudioSampleFrequency / kAudioTxBufferSize; ++j)

  {

    audio.spk.play(waveform, kAudioTxBufferSize);

  }

}

void ini_playNote(void) {  
    

    /*for(int i=0;i<42;i++)
    { 
      printf("song=%d\r\n",song[i]);
      printf("length=%d\r\n",noteLength[i]);

    }*/

  int song1[]= {

  261, 261, 392, 392, 440, 440, 392,

  349, 349, 330, 330, 294, 294, 261,

  392, 392, 349, 349, 330, 330, 294,

  392, 392, 349, 349, 330, 330, 294,

  261, 261, 392, 392, 440, 440, 392,

  349, 349, 330, 330, 294, 294, 261};


  int noteLength1[] = {

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2};



    for(int i = 0; i < 42; i++)
    {
      printf("kick my ass\r\n");
      printf("song=%d\r\n",song1[i]);
      printf("length=%d\r\n",noteLength1[i]);
      int length = noteLength1[i];
      
      while(length--)
      {
        // the loop below will play the note for the duration of 1s
        for(int j = 0; j < kAudioSampleFrequency / kAudioTxBufferSize; ++j)
        {
          queue.call(printf, "song=%d \r\n",song1[i]);
          queue.call(playNote, song1[i]); 
          
        }
        if(length < 1) wait(1.0);
      }
    }

    
  }











/*int song1[]= {

  261, 261, 392, 392, 440, 440, 392,

  349, 349, 330, 330, 294, 294, 261,

  392, 392, 349, 349, 330, 330, 294,

  392, 392, 349, 349, 330, 330, 294,

  261, 261, 392, 392, 440, 440, 392,

  349, 349, 330, 330, 294, 294, 261};


int noteLength1[] = {

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2,

  1, 1, 1, 1, 1, 1, 2};*/






int main(void)
{
  green_led = 1;
  
  
  t.start(callback(&queue, &EventQueue::dispatch_forever));


  button.rise(queue.event(loadSongHandler));

  while(1)
  {
      if(play==1)
      {
        for(int i=0;i<42;i++)
        {
          printf("song %d\r\n",song[i] );
          printf("length %d\r\n",noteLength[i] );

        }
        
        for(int i = 0; i < 42; i++)
        {

          int length = noteLength[i];

          while(length--)

          { 
      
            queue.call(playNote, song[i]);  

            if(length <= 1) wait(1.0);

          }

        }
       
      }
  
  play=0;
  }


}