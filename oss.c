#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/soundcard.h>

#include <err.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <unistd.h>

#include "sad.h"

static int ossfd = -1;
int ossvolstatus = DEFAULTVOL;

static int ossvol(int vol) {
  ossvolstatus = vol;
  return 0;
}

static int ossopen(Format *fmt) {
  int afmt = AFMT_S16_LE, chans = fmt->channels, rate = fmt->rate;

  ossfd = open("/dev/dsp", O_WRONLY);
  if (ossfd < 0) {
    warn("open /dev/dsp");
    return -1;
  }
  if (ioctl(ossfd, SNDCTL_DSP_SETFMT, &afmt) < 0 ||
      ioctl(ossfd, SNDCTL_DSP_CHANNELS, &chans) < 0 ||
      ioctl(ossfd, SNDCTL_DSP_SPEED, &rate) < 0) {
    warn("ioctl /dev/dsp");
    goto err0;
  }
  if (afmt != AFMT_S16_LE || chans != (int)fmt->channels ||
      rate != (int)fmt->rate) {
    warnx("unsupported audio params");
    goto err0;
  }
  return 0;

err0:
  close(ossfd);
  ossfd = -1;
  return -1;
}

static int ossplay(void *buf, size_t nbytes) {
  return write(ossfd, buf, nbytes);
}

static int ossclose(void) {
  if (ossfd != -1)
    close(ossfd);
  ossfd = -1;
  return 0;
}

Output ossoutput = {
    .volstatus = &ossvolstatus,
    .vol = ossvol,
    .open = ossopen,
    .play = ossplay,
    .close = ossclose,
};
