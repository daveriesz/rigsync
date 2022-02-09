# rigsync

Rigsync is a utility that keeps the freuency and mode of multiple radios in sync.  Supported radios are all those supported by whatever version of Hamlib to which rigsync is linked.

## Why

I have an SDR dongle connected to the external receiver antenna output of a Kenwood TS-870S and use SDR++ on Linux to view the SDR output.  Tuning SDR++ and the Kenwood separately got to be a drag, so I wrote rigsync to do it for me.  Rigsync handily updates the SDR dongle via the rigctl server in SDR++ whenver the frequency or mode of the Kenwood radio is changed.  I can also click a spot on the spectrum output in SDR++ to tune to a signal and the Kenwood will be updated automatically to match it.

This has also been tested with a Yaesu FT-857D that has an SDR dongle attached to an RF tap point inside the rig.

## Further Development

I'd like to write an SDR++ module to handle this capability.  It would be a cleaner solution than this standalone utility.

For the time being, there are undoubtedly lots of bugs I will need to address.

Additionally, there is an 8.83 MHz IF output on the Kenwood TS-870S (as well as other Kenwood radios) that works better than the external receiver antenna output for the purpose of using an SDR dongle as a panadpater.  There are some problems with this:

1. The IF output is reversed.  The radio and dongle have mirror images of the spectrum.
2. This reversal also means that LSB/USB are reversed.
3. Changing modes between LSB and USB on the radio causes the radio's IF frequency to shift by about +/- 1.25KHz off center.

It would be nice to make a provision to handle this scenario.

## Building

There are no autoconf scripts or cmakefiles.  Just a plain GNU-style Makefile.  You may edit it to suit your needs.  Hamlib and and GCC are expected.  If you use a different compiler you might need to change the ```-O2``` optimization argument.

## Running

Command-line usage can be obtained at any time with:  ```rigsync -h```.  You will see the output:

```
usage:

    ./rigsync <rig definition> <rig definition> ... [-n <rig number>]
        Keep defined rigs in sync.
        Each <rig definition> entry must start with:
             -m <model number>
        and be followed optionally by (depending on the rig):
             -r <port/connection>
             -s <baud rate>
        The "-n <rig number>" argument defines which rig in the list
        the others are synched to on startup.  The default is the first (1).

    ./rigsync -h
        Print this help message.
```

Example 1:

```
  $ ./rigsync \
      -m 2 -r localhost:4532 \
      -m 210 -r /dev/ttyS0 -s 57600
```

The first ```-m``` argument defines a networked rig control server at localhost port 4532.  The second  ```-m``` argument defines a Kenwood TS-870S connected to the serial port /dev/ttyS0 at 57600 baud.  On startup, rigsync will set the TS-870S's frequency and mode to match the rig control server (this is the default since there is no ```-n``` argument and the server is defined firsrt).  Thereafter, it will keep both in sync.

Example 2:

```
  $ ./rigsync \
    -m 2 -r localhost:4532 \
    -m 210 -r /dev/ttyF1 -s 57600 \
    -m 122 -r /dev/ttyF0 -s 38400 \
    -n 2
```

This defines a rig control server, a Kenwood TS-870S, and a Yaesu FT-857D.  The ```-n 2``` argument will, on startup, sync all radios to the TS-870S.

## Issues

Issue?  Bug?  Feature request?  Please submit an [issue](https://github.com/daveriesz/rigsync/issues).

## Contributing

Feel free to submit a pull request if you want to contribute.  I'm not very good at reading other people's code, so please include a verbose description and/or explanation.

## Licensing

This project and its code are all licensed under the GNU LGPL license v2.1.  You all know what this means.  If you don't, read the [license file](https://github.com/daveriesz/rigsync/blob/main/LICENSE).

