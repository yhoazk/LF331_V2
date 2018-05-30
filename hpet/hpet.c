#include <stdio.h>
#include <stdlib.h> // include atoi
#include <unistd.h> // includes getpid, pause, close
#include <fcntl.h>
#include <time.h> // clock_gettime, CLOCK_REALTIME
#include <signal.h>
#include <stdint.h> // for uint64_t kind of types
#include <sys/ioctl.h>
#include <linux/hpet.h>


static uint64_t hpet_sigio_count;
static uint64_t secs;
/**
 * Function to be executed on the timer interruption
 * */
static void hpet_alarm(int val)
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);

  if(!secs)
  {
    secs = t.tv_sec;
  }

  fprintf(stderr, "Alarm CAlled Iter: %2d secs: %ld, nsecs: %ld \n",
        hpet_sigio_count, (t.tv_sec - secs), t.tv_sec * 100000 + t.tv_nsec);
  hpet_sigio_count++;
}



int main(int argc, const char** argv)
{
  struct sigaction old_act, new_act;
  struct hpet_info info;
  int frequency;
  int iterations;
  int retval = 0;
  int fd;
  int r,i,value;

  if (argc != 3)
  {
    fprintf(stderr, "Usage: %s freq(1-64) iter (10-99)\n", argv[0]);
    retval = -1;
  }
  else
  {
    frequency = atoi(argv[1]);
    iterations = atoi(argv[2]);

    if ( (frequency > 64) || (frequency < 1) )
    {
      fprintf(stderr, "Error: invalid value for freq");
      retval = -1;
    }
    
    else if( (iterations < 10) || (iterations > 99) )
    {
      fprintf(stderr, "Error: invalid value for iterations");
      retval = -1;
    }
    else
    {
      hpet_sigio_count = 0;

      sigemptyset(&new_act.sa_mask);
      new_act.sa_flags = 0;
      new_act.sa_handler = hpet_alarm;

      sigaction(SIGIO, NULL, &old_act);
      sigaction(SIGIO, &new_act, NULL);

      fd = open("/dev/hpet", O_RDONLY);

      if(fd < 0)
      {
        fprintf(stderr, "Error: failed to open /dev/hpet\n");
        retval = -1;
      }
      else
      {
        if( (fcntl(fd, F_SETOWN, getpid()) == 1) || \
          ((value = fcntl(fd, F_GETFL)) == 1) || \
          (fcntl(fd, F_SETFL, value | O_ASYNC) == 1) )
        {
          fprintf(stderr, "Error: fnctl failed\n");
          retval = -1;
          goto fail;
        }
        if(ioctl(fd, HPET_IRQFREQ, frequency) < 0)
        {
          fprintf(stderr, "Error: Could ont set /dev/hpet to have %2dHz timer\n", frequency);
          retval = 2;
          goto fail;
        }

        if(ioctl(fd, HPET_INFO, &info) < 0)
        {
          fprintf(stderr, "Error: failed to get info\n");
          retval = 3;
          goto fail;
        }

        fprintf(stdout, "\n hi_irqfreq: 0x%lx hi_flags: %0x%lx hi_hpet: 0x%x hi_timer: 0x%x\n\n",
            info.hi_ireqfreq, info.hi_flags, info.hi_hpet, info.hi_timer);

        r = ioctl(fd, HPET_EPI, 0);
        if( info.hi_flags && ( r < 0))
        {
          fprintf(stderr, "Error: HPET_EPI failed\n");
          retval = 4;
          goto fail;
        }

        if( ioctl(fd, HPET_IE_ON, 0) < 0)
        {
          fprintf(stderr, "Error: HPET_IE_ON failed\n");
          retval = 5;
          goto fail;
        }

        for(i=0; i < iterations; ++i)
        {
          (void) pause();
        }

        if(ioctl(fd, HPET_IE_OFF, 0) < 0)
        {
          fprintf(stderr, "Error: HPET_IE_OFF failed\n");
          retval = 6;
        }
      }


    }
  }

fail: 
  sigaction(SIGIO, &old_act, NULL);
  if(fd > 0)
  {
    close(fd);
  }

  return retval;
}
