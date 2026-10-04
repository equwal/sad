#define RESAMPLEQUALITY SOXR_QQ
#define BITRATE 48000
#define BITDEPTH 16
#define CHANNELS 2
#define SOUNDSYS ALSA

/*
   Only need to edit below this line if you have multiple sound backends.
   Only the backend selected by SOUNDSYS is linked; see OUTOBJ in Makefile.
*/

Outputcfg outputcfgs[] = {
#if SOUNDSYS == SNDIO
    {.name = "sndio",
     .fmt = {.bits = BITDEPTH, .rate = BITRATE, .channels = CHANNELS},
     .enabled = 1,
     .output = &sndiooutput},
#endif
#if SOUNDSYS == ALSA
    {.name = "alsa",
     .fmt = {.bits = BITDEPTH, .rate = BITRATE, .channels = CHANNELS},
     .enabled = 1,
     .output = &alsaoutput},
#endif
#if SOUNDSYS == OSS
    {.name = "oss",
     .fmt = {.bits = BITDEPTH, .rate = BITRATE, .channels = CHANNELS},
     .enabled = 1,
     .output = &ossoutput},
#endif
    {.name = "fifo",
     .fmt = {.bits = BITDEPTH, .rate = BITRATE, .channels = CHANNELS},
     .enabled = SOUNDSYS == FIFO,
     .output = &fifooutput},
};
