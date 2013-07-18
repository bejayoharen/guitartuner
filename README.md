guitartuner
===========

Simple guitar tuner sample app with BSD license.

There are plenty of tuners out there, this one is designed to be easy to understand and follow.
It should also be easy to port to other platforms, though right now it compiles only on OS X.

To Compile:
==

0. OS X only :(patches for other apps accepted!)
1. First, you need to download and compile portaudio from PortAudio.com.
2. Grab libportaudio.a from lib/.libs in portaudio and put it in libs/
3. run "make"
4. the output is ./tuner


Copyright
==

Tuner Copyright (C) 2012 by Bjorn Roche

FFT Copyright (C) 1989 by Jef Poskanzer

Permission to use, copy, modify, and distribute this software and its documentation for any purpose and without fee is hereby granted, provided that the above copyright notice appear in all copies and that both that copyright notice and this permission notice appear in supporting documentation.  This software is provided "as is" without express or implied warranty.

More Info
==

This tuner app works by calculating the magnatude of the FFT, which is not the ideal method, but
it works well enough.

You can find more info about this code on Bjorn Roche's blog:
http://blog.bjornroche.com/2012/07/frequency-detection-using-fft-aka-pitch.html
