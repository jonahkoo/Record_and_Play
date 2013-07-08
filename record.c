#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <alsa/asoundlib.h>	//alsa sound

#define SAMPLE_SIZE 80

//global vars
FILE *sound_raw;		//write for sound file
snd_pcm_t *handle;		//pcm handle

//func protype
void handle_sigint (int);	//signal handle func

int
main (int argc, char **argv)
{

  unsigned char buffer[SAMPLE_SIZE];	//buffer for keep sound

  // Sigint handler code catch ctrl+c event
  struct sigaction sigIntHandler;
  sigIntHandler.sa_handler = handle_sigint;
  sigemptyset (&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;
  sigaction (SIGINT, &sigIntHandler, NULL);

  sound_raw = fopen ("sound.raw", "w+");	//open a sound file
  if (sound_raw == NULL)
    {
      printf ("Failed to open sound.raw\n");
      return -1;
    }

  //setup_device
  if (snd_pcm_open (&handle, "default", SND_PCM_STREAM_CAPTURE, 0) < 0)
    {
      printf ("snd_pcm_open -- failed to open device\n");
      return -1;
    }

  //set device parameter
  if (snd_pcm_set_params (handle, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1,	// channels
			  8000,	// sample rate
			  1,	// allow resampling
			  500000	// .5 seconds
      ) < 0)
    {
      printf ("snd_pcm_set_params -- failed to set parameters\n");
      return -1;
    }

  //in loop, save the sound and increase the node into linklist queue
  //until press ctrl+c gen  end signal 
  while (1)
    {
      snd_pcm_readi (handle, buffer, sizeof (char) * 80);
      fwrite (buffer, sizeof (char), 80, sound_raw);
    }

  return 0;

}

void
handle_sigint (int s)
{				//signal responed

  fclose (sound_raw);		//close file
  snd_pcm_close (handle);	//close device
  exit (0);
}
