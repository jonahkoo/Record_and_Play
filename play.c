#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <alsa/asoundlib.h>	//use ALSA driver

#define BUF_SIZE 16000

int
main (int argc, char **argv)
{
  FILE *fp;
  snd_pcm_t *handle;

  if (argc < 2)
    {
      printf ("usage:play filename\n");
      return -1;
    }

  fp = fopen (argv[1], "r");

  if (fp == NULL)
    {
      printf ("Failed to open file\n");
      return -1;
    }


  if (snd_pcm_open (&handle, "default", SND_PCM_STREAM_PLAYBACK, 0) < 0)
    {
      printf ("snd_pcm_open -- failed to open device\n");
      return -1;
    }

  if (snd_pcm_set_params (handle, SND_PCM_FORMAT_U8, SND_PCM_ACCESS_RW_INTERLEAVED, 1,	// channels
			  8000,	// sample rate
			  1,	// allow resampling
			  50000	// .5 seconds
      ) < 0)
    {
      printf ("snd_pcm_set_params -- failed to set parameters\n");
      return -1;
    }

  //buffer
  char *buffer = (char *) malloc (sizeof (char) * BUF_SIZE);
  size_t read;

  do
    {
      read = fread (buffer, sizeof (char), BUF_SIZE, fp);
      snd_pcm_writei (handle, buffer, read);	//put buffer into device
    }
  while (read > 0);		//hit the end of the file

  snd_pcm_close (handle);	//close audio device

  return 1;
}
