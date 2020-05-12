#include "mbed.h"
#include "uLCD_4DGL.h"
#include "DA7212/DA7212.h"

#include "accelerometer_handler.h"
#include "config.h"
#include "magic_wand_model_data.h"

#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/version.h"

DA7212 audio;



#define bufferLength (32)

#define songLength (42)


Serial pc(USBTX, USBRX);
uLCD_4DGL uLCD(D1, D0, D2);
InterruptIn button(SW2);

InterruptIn button3(SW3);

DigitalOut green_led(LED2);



int16_t waveform[kAudioTxBufferSize];



int song[42] ;
int noteLength[42];
int songname = 1;
int play=0;
int idC = 0;
int playindex = 0;





constexpr int kTensorArenaSize = 60 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
bool should_clear_buffer = false;
bool got_data = false;
int gesture_index;
static tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter *error_reporter;
const tflite::Model *model;
static tflite::MicroOpResolver<6> micro_op_resolver;
tflite::MicroInterpreter *interpreter;
TfLiteTensor *model_input;
int input_length;





Thread t; 
Thread t_gesture;
Thread t1; 

EventQueue queue(32 * EVENTS_EVENT_SIZE);
EventQueue queue1(32 * EVENTS_EVENT_SIZE);

Timer buttontimer;

void loadSong(void)

{
  char serialInBuffer[bufferLength];

  int serialCount = 0;

  green_led = 0;

  int i=0 ;

  
  uLCD.cls();
  uLCD.printf("loading song....\n\r");
 

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
        
        //uLCD.printf("i=%d,song=%d\n\r",i,song[i]);
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
        //uLCD.printf("i=%d,length=%d\n\r",i,noteLength[i]);
        serialCount = 0;

        i++;

      }

    }

  }

  i=0;
  while( i < 1)

  {
  
    
    if(pc.readable())

    {

      serialInBuffer[serialCount] = pc.getc();

      serialCount++;

      if(serialCount == 3)

      {

        serialInBuffer[serialCount] = '\0';

        songname = (int) atoi(serialInBuffer);
        
        //uLCD.printf("songname%d\n\r",songname);
        
        serialCount = 0;

        i++;

      }

    }

  }

  


uLCD.printf("load song%d success\n\r",songname);
wait(0.5);
green_led = 1;
play=1;   

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




// Return the result of the last prediction

int PredictGesture(float* output) {
  // How many times the most recent gesture has been matched in a row
  static int continuous_count = 0;
  // The result of the last prediction
  static int last_predict = -1;

  // Find whichever output has a probability > 0.8 (they sum to 1)
  int this_predict = -1;
  for (int i = 0; i < label_num; i++) {
    if (output[i] > 0.8) this_predict = i;
  }

  // No gesture was detected above the threshold
  if (this_predict == -1) {
    continuous_count = 0;
    last_predict = label_num;
    return label_num;
  }

  if (last_predict == this_predict) {
    continuous_count += 1;
  } else {
    continuous_count = 0;
  }
  last_predict = this_predict;

  // If we haven't yet had enough consecutive matches for this gesture,
  // report a negative result
  if (continuous_count < config.consecutiveInferenceThresholds[this_predict]) {
    return label_num;
  }
  // Otherwise, we've seen a positive result, so clear all our variables
  // and report it
  continuous_count = 0;
  last_predict = -1;

  return this_predict;
}


void gestureThread() {
	while(true) {
		got_data = ReadAccelerometer(error_reporter, model_input->data.f, input_length, should_clear_buffer);

		if (!got_data)
		{
			should_clear_buffer = false;
			continue;
		}

		TfLiteStatus invoke_status = interpreter->Invoke();
		if (invoke_status != kTfLiteOk)
		{
			error_reporter->Report("Invoke failed on index: %d\n", begin_index);
			continue;
		}

		gesture_index = PredictGesture(interpreter->output(0)->data.f);

		should_clear_buffer = gesture_index < label_num;


	}
}


void cake()
{
 // audio.spk.pause();
  uLCD.printf("mode selection (forward)or(backward)\r\n");
  int confirm=0;
  
  int ex_gest=-1;  // keep the last gesture in var   
  while (confirm==0) {
   //while (true) { 
     
    // Attempt to read new data from the accelerometer
    got_data = ReadAccelerometer(error_reporter, model_input->data.f,
                                 input_length, should_clear_buffer);

    // If there was no new data,
    // don't try to clear the buffer again and wait until next time
    if (!got_data) {
      should_clear_buffer = false;
      continue;
    }

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
     // error_reporter->Report("Invoke failed on index: %d\n", begin_index);
      continue;
    }

    // Analyze the results to obtain a prediction
    gesture_index = PredictGesture(interpreter->output(0)->data.f);

    // Clear the buffer next time we read data
    should_clear_buffer = gesture_index < label_num;

    // Produce an output
    //if (gesture_index < label_num) {
    //  error_reporter->Report(config.output_message[gesture_index]);
    //
    //}
    if(gesture_index==0){         //*******mode0
      uLCD.cls();
      uLCD.printf("\r\nforward\r\n");
      int a=0;
      ex_gest=0; 
    }
    if(gesture_index==1){         // ********mode1
      uLCD.cls();
      uLCD.printf("\r\nbackward\r\n");
      int a=0;
      ex_gest=1; 
    }

  
    if(button3==0){                 //*************if pressed SW3 -> confirm and stop prediction
      confirm=1;}    

  }
  

  

  int kk=ex_gest;     // kk=0 for forward  , =1 for backward
  int play_index=-1;
  int jj=songname+1;  // next song
  int ii=songname-1;  // previous song

  


  if(kk==0){
    if(jj>3){play_index=1;}
    else{play_index=jj;}
  
    uLCD.printf("hello you choose mode0(forward)\r\n");
    
    printf("%d\r\n",play_index);
  
  }


  if(kk==1){
    if(ii<1){play_index=3;}
    else{play_index=ii;}
    uLCD.printf("hello you choose mode1(backward)\r\n");
    
    printf("%d\r\n",play_index);
         
  }
  
  


}

void gogogo()
{
  if(buttontimer.read_ms() > 1000)
  {
    buttontimer.reset();
    uLCD.cls();
    queue.call(cake);
    queue.call(loadSong);

  }

}


void playsong()

{

      if(play==1)
      {

          uLCD.printf("\nnow playing\r\nsongname=%d\n",songname); //Default Green on black text

        
          for(int i = 0; i < 42; i++)
          {

            int length = noteLength[i];

            while(length--)

            { 
            
              queue1.call(playNote, song[i]);  

              if(length <= 1) wait(1.0);

            }

          }
      
        play=0;
        uLCD.cls();



        

      
      }      


  

}


int main()
{
  buttontimer.start();
  /////////////////////////////////////////////////////////////
 
	error_reporter = &micro_error_reporter;
	model = tflite::GetModel(g_magic_wand_model_data);
	if (model->version() != TFLITE_SCHEMA_VERSION)
	{
		error_reporter->Report(
			"Model provided is schema version %d not equal "
			"to supported version %d.",
			model->version(), TFLITE_SCHEMA_VERSION);
		return -1;
	}
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_DEPTHWISE_CONV_2D, tflite::ops::micro::Register_DEPTHWISE_CONV_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_MAX_POOL_2D, tflite::ops::micro::Register_MAX_POOL_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_CONV_2D, tflite::ops::micro::Register_CONV_2D());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_FULLY_CONNECTED, tflite::ops::micro::Register_FULLY_CONNECTED());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_SOFTMAX, tflite::ops::micro::Register_SOFTMAX());
	micro_op_resolver.AddBuiltin(tflite::BuiltinOperator_RESHAPE, tflite::ops::micro::Register_RESHAPE(), 1);
	static tflite::MicroInterpreter static_interpreter(model, micro_op_resolver, tensor_arena, kTensorArenaSize, error_reporter);
	interpreter = &static_interpreter;
	interpreter->AllocateTensors();
	model_input = interpreter->input(0);
	if ((model_input->dims->size != 4) || (model_input->dims->data[0] != 1) ||
		(model_input->dims->data[1] != config.seq_length) ||
		(model_input->dims->data[2] != kChannelNumber) ||
		(model_input->type != kTfLiteFloat32))
	{
		error_reporter->Report("Bad input tensor parameters in model");
		return -1;
	}
	input_length = model_input->bytes / sizeof(float);
	TfLiteStatus setup_status = SetupAccelerometer(error_reporter);
	if (setup_status != kTfLiteOk) {
		error_reporter->Report("Set up failed\n");
		return -1;
	}
	//error_reporter->Report("Set up successful...\n");
  uLCD.printf("Set up successful...\n");
 
//////////////////////////////////////////////////////////////////////////////////////


  green_led = 1;
  
  pc.baud(115200);
  
  t.start(callback(&queue, &EventQueue::dispatch_forever));
  
  t1.start(callback(&queue1, &EventQueue::dispatch_forever));


  uLCD.printf("songlist\r\n");
  uLCD.printf("song1\r\n");
  uLCD.printf("song2\r\n");
  uLCD.printf("song3\r\n");
  uLCD.printf("no song in mbed yet , plz load for playing\r\n");

  button.rise(queue.event(gogogo));
  while(1)
  {
    playsong();
  }

}

