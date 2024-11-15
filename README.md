guitartuner
===========

Simple guitar tuner sample app with BSD license.

There are plenty of tuners out there. This one is designed to be easy to understand and follow. You can read more about how it works here](http://bjorg.bjornroche.com/audio/2024/11/15/frequency-tracking-fft.html)
It should also be easy to port to other platforms, though right now it compiles only on OS X and Linux.

To Compile:
-----------

0. OS X and Linux only :(patches for other apps accepted!)
1. Download and install portaudio.
   - For OS X, use homebrew or macports to install -- `port install portaudio`.
   - For Ubuntu, use `apt-get install portaudio19-dev`.
2. run "make"
3. the output is ./tuner

Copyright
---------

Tuner Copyright (C) 2012 by Bjorn Roche

FFT Copyright (C) 1989 by Jef Poskanzer

Permission to use, copy, modify, and distribute this software and its documentation for any purpose and without fee is hereby granted, provided that the above copyright notice appear in all copies and that both that copyright notice and this permission notice appear in supporting documentation. This software is provided "as is" without express or implied warranty.
