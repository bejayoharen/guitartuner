#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include "libfft.h"
#include "portaudio.h"

/* -- some basic parameters -- */
#define SAMPLE_RATE (8000)
#define FFT_SIZE (4096)
#define FFT_EXP_SIZE (12)
#define NUM_SECONDS (20)

/* -- functions declared and used here -- */
void buildHammingWindow( float *window, int size );
void applyWindow( float *window, float *data, int size );
//a must be of length 2, anbd b must be of length 3
void computeSecondOrderLowPassParameters( float srate, float f, float *a, float *b );
//mem must be of length 4.
float processSecondOrderFilter( float x, float *mem, float *a, float *b );
void signalHandler( int signum ) ;

bool running = true;

/* -- main function -- */
int main( int argc, char **argv ) {
   PaStreamParameters inputParameters;
   float a[2], b[3], mem1[4], mem2[4];
   float *data = NULL;
   float *datai = NULL;
   float *window = NULL;
   void * fft = NULL;
   PaStream *stream = NULL;
   PaError err = 0;
   int size ;
   struct sigaction action;

   size = FFT_SIZE * 4;

   // create buffers of the required size
   data = (float *) calloc( 1, size );
   datai = (float *) calloc( 1, size );
   window = (float *) calloc( 1, size );

   // add signal listen so we know when to exit:
   action.sa_handler = signalHandler;
   sigemptyset (&action.sa_mask);
   action.sa_flags = 0;
     
   sigaction (SIGINT, &action, NULL);
   sigaction (SIGHUP, &action, NULL);
   sigaction (SIGTERM, &action, NULL);

   // build the window, fft, etc
   /* test our window
   buildHammingWindow( window, 30 );
   for( int i=0; i<30; ++i ) {
      for( int j=0; j<window[i]*50; ++j )
         printf( "*" );
      printf("\n");
   }
   */

   buildHammingWindow( window, FFT_SIZE );
   fft = initfft( FFT_EXP_SIZE );
   computeSecondOrderLowPassParameters( SAMPLE_RATE, 330, a, b );
   mem1[0] = 0; mem1[1] = 0; mem1[2] = 0; mem1[3] = 0;
   mem2[0] = 0; mem2[1] = 0; mem2[2] = 0; mem2[3] = 0;

   // initialize portaudio
   err = Pa_Initialize();
   if( err != paNoError ) goto error;

   inputParameters.device = Pa_GetDefaultInputDevice();
   inputParameters.channelCount = 1;
   inputParameters.sampleFormat = paFloat32;
   inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultHighInputLatency ;
   inputParameters.hostApiSpecificStreamInfo = NULL;

   printf( "Opening %s\n",
           Pa_GetDeviceInfo( inputParameters.device )->name );

   err = Pa_OpenStream( &stream,
                        &inputParameters,
                        NULL, //no output
                        SAMPLE_RATE,
                        FFT_SIZE,
                        paClipOff,
                        NULL,
                        NULL );
   if( err != paNoError ) goto error;

   err = Pa_StartStream( stream );
   if( err != paNoError ) goto error;

   // this is the main loop where we listen to and
   // process audio.
   while( running )
   {
      // read some data
      err = Pa_ReadStream( stream, data, FFT_SIZE );
      if( err ) goto error; //FIXME: we don't want to err on xrun

      for( int j=0; j<FFT_SIZE; ++j ) {
         data[j] = processSecondOrderFilter( data[j], mem1, a, b );
         data[j] = processSecondOrderFilter( data[j], mem2, a, b );
      }
      applyWindow( window, data, FFT_SIZE );
      applyfft( fft, data, datai, false );

      float maxVal = -1;
      int maxIndex = -1;
      for( int j=0; j<FFT_SIZE; ++j ) {
         float v = data[j] * data[j] + datai[j] * datai[j] ;
         if( v > maxVal ) {
            maxVal = v;
            maxIndex = j;
         }
      }

      int res = fseek( stdin, 1, SEEK_END );

      printf("\033[2J\033[1;1H"); //clear screen, go to top left
      fflush(stdout);
      printf( "Tuner listening. Control-C to exit.\n" );
      printf( "%d - %f %d\n", maxIndex, maxVal, res );

   }
   err = Pa_StopStream( stream );
   if( err != paNoError ) goto error;

   // cleanup
   free( data );
   free( datai );
   free( window );
   destroyfft( fft );
   Pa_Terminate();

   return 0;
 error:
   if( stream ) {
      Pa_AbortStream( stream );
      Pa_CloseStream( stream );
   }
   free( data );
   free( datai );
   free( window );
   destroyfft( fft );
   Pa_Terminate();
   fprintf( stderr, "An error occured while using the portaudio stream\n" );
   fprintf( stderr, "Error number: %d\n", err );
   fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
   return 1;
}

void buildHammingWindow( float *window, int size )
{
   for( int i=0; i<size; ++i )
      window[i] = .54 - .46 * cos( 2 * M_PI * i / (float) size );
}
void applyWindow( float *window, float *data, int size )
{
   for( int i=0; i<size; ++i )
      data[i] *= window[i] ;
}
void computeSecondOrderLowPassParameters( float srate, float f, float *a, float *b )
{
   float a0;
   float w0 = 2 * M_PI * f/srate;
   float cosw0 = cos(w0);
   float sinw0 = sin(w0);
   float alpha = sinw0/2;

   a0   = 1 + alpha;
   a[0] = (-2*cosw0) / a0;
   a[1] = (1 - alpha) / a0;
   b[0] = ((1-cosw0)/2) / a0;
   b[1] = ( 1-cosw0) / a0;
   b[2] = b[0];
}
float processSecondOrderFilter( float x, float *mem, float *a, float *b )
{
    float ret = b[0] * x + b[1] * mem[0] + b[2] * mem[1]
                         - a[0] * mem[2] - a[1] * mem[3] ;

		mem[1] = mem[0];
		mem[0] = x;
		mem[3] = mem[2];
		mem[2] = ret;

		return ret;
}
void signalHandler( int signum ) { running = false; }
