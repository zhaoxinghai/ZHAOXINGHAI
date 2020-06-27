#include <stdio.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <locale.h>
#include <alsa/asoundlib.h>
#include <assert.h>
#include <termios.h>
#include <signal.h>
#include <sys/poll.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <endian.h>

void help()
{
    printf("./alsatest name rate channel \n");
    printf("eg: \n");
    printf("./alsatest plughw:4,0 48000 6 \n");
}

int main(int argc, char *argv[])
{
    if(argc < 4)
    {
        help();
        return 0;
    }

    snd_pcm_t *handle_CAPTURE = NULL;
    snd_pcm_t *handle_PLAYBACK = NULL;
    char *pcm_name = argv[1];
    int rate = atoi(argv[2]);
    int channels = atoi(argv[3]);
    printf("device_name = %s \n", pcm_name);
    printf("rate        = %d \n", rate);
    printf("channels    = %d \n", channels);
    printf("format      = SND_PCM_FORMAT_FLOAT_LE \n");
    printf("access      = SND_PCM_ACCESS_RW_NONINTERLEAVED \n");

    int err = 0;
    err = snd_pcm_open(&handle_CAPTURE, pcm_name, SND_PCM_STREAM_CAPTURE, 0);
    if (err < 0)
    {
        perror("snd_pcm_open CAPTURE");
        return -1;
    }
    err = snd_pcm_open(&handle_PLAYBACK, pcm_name, SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0)
    {
        perror("snd_pcm_open PLAYBACK");
        return -1;
    }
    err = snd_pcm_set_params(handle_CAPTURE,
            SND_PCM_FORMAT_FLOAT_LE,
            SND_PCM_ACCESS_RW_NONINTERLEAVED,
            channels,            //Channels
            rate,               //Rate
            1,			// Allow software resampling
            32000);		// u seconds per buffer
    if (err < 0)
    {
        perror("snd_pcm_set_params CAPTURE");
        return -1;
    }
    err = snd_pcm_set_params(handle_PLAYBACK,
            SND_PCM_FORMAT_FLOAT_LE,
            SND_PCM_ACCESS_RW_NONINTERLEAVED,
            channels,            //Channels
            rate,               //Rate
            1,			// Allow software resampling
            32000);		// u seconds per buffer
    if (err < 0)
    {
        perror("snd_pcm_set_params CAPTURE");
        return -1;
    }

    int rc = 0;
    int i = 0;
    while (1)
    {
        int bitByte = 1;
        snd_pcm_sframes_t frames = 480;
        float pcm[bitByte * frames * channels];
        void *bufs[channels];
        for(i=0;i<channels;++i)
        {
            bufs[i] = pcm + (bitByte * frames * i);
        }
        rc = snd_pcm_readn(handle_CAPTURE, bufs, frames);
        if (rc == -EPIPE)
        {
            snd_pcm_prepare(handle_CAPTURE);
        }
        else if (rc != frames)
        {
            printf("CAPTURE snd_pcm_readn miss %d \n", rc);
        }
        rc = snd_pcm_writen(handle_PLAYBACK, bufs, frames);
        if (rc == -EPIPE)
        {
            snd_pcm_prepare(handle_PLAYBACK);
        }
        else if (rc != frames)
        {
            printf("PLAYBACK snd_pcm_readn miss %d \n", rc);
        }
    }

    return 0;
}
