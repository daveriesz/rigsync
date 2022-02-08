# RIGSYNC

This is a utility that keeps two rigs in sync using Hamlib.  It is initially very basic.  Currently, it is hardcoded for only two rigs:

- rig a: a rigctl server running on localhost port 4352
- rig b: a Yaesu FT-857D

On startup it syncs the server to the FT-857 and from there on, syncs either rig to whichever has been changed most recently.  A sync is performed every 100ms.  It is intended to support my station setup:  An AirspyHF connected to the FT-857 and a Linux computer running SDR++ with a rigctl server.  

Eventually, I may update it to support more flexibility in terms of rigs and behavior.
