<<<<<<< HEAD
# VOYAGER1-Project
This is for a class, I'm emulating voyager ones data collection and compression algorithm. The decompression algorithims are freely available online at https://planetarydata.jpl.nasa.gov/img/data/vg2-u-iss-2-edr-v1.0/vg_0001/software/ 

## Notes
cdcomp.c needs to be ported for modern systems.




## Important Links
Stats on the sensors 
https://pds-ppi.igpp.ucla.edu/mission/Voyager/Imaging_Science_Subsystem_-_Wide_Angle


Camera Stats

Instrument Information
======================
Instrument Id : ISSW
Instrument Host Id : VG2
Pi Pds User Id : BASMITH
Naif Data Set Id : UNK
Instrument Name : IMAGING SCIENCE SUBSYSTEM - WIDE
ANGLE
Instrument Type : VIDICON CAMERA
Build Date : 1976-12-17
Instrument Mass : 13.300000
Instrument Length : 0.550000
Instrument Width : 0.200000
Instrument Height : 0.200000
Instrument Serial Number : SN04
Instrument Manufacturer Name : JET PROPULSION LABORATORY

Instrument Description
======================
The Voyager Imaging Science Subsystem (ISS) is a modified
version of the slow scan vidicon camera designs that were used
in the earlier Mariner flights. The system consists of two
cameras, a high resolution Narrow Angle (NA) camera and a lower
resolution, more sensitive Wide Angle (WA) camera. Unlike the
other on board instruments, operation of the cameras is not
autonomous, but is controlled by an imaging parameter table
residing in one of the spacecraft computers, the Flight Data
Subsystem (FDS) (Science and Mission Systems Handbook, 1987,
JPL D-498, an internal JPL document available from JPL vellum
files).

The original mission was to Jupiter and Saturn. Voyager
surpassed expectations and went on to encounter Uranus and
Neptune. As the Voyager mission progressed the objects
photographed were further from the sun so they appeared more
faint even though longer exposures were used. As the Voyager
spacecrafts' distance from the Earth increases, the
telecommunications capability at each encounter decreases. The
difference in capabilities from the Jupiter and Saturn
encounters and that at Uranus and Neptune was considerable.
The reduced telecommunications capability limits the number of
data modes that imaging can use. Because of the diminished
brightness of the objects being photographed, longer exposure
times were used, many beyond the stated maximum of 15.360
seconds. Longer exposure times were all 48-second increments
added to the maximum. In addition, the camera was slewed in
order to avoid smeared imaging. The light flood state (on/off)
was independent of the instrument mode.



Description
-----------
The sensor used in the Voyager Imaging Science Subsystem ISS
camera system is a 25mm diameter magnetic deflection vidicon
(number B41-003, General Electro-dynamics Co.). The vidicon
storage surface (target) is selenium sulphur and can store a
high resolution (1500 TV lines) picture for over 100 s at
room temperature. The active image area on the target is
11.14 x 11.14 mm. Each frame consists of 800 lines with 800
picture elements (pixels) per line, i.e., 1 pixel = 14
microns. One frame requires 48 s for electronic readout. In
addition to the normal frame readout of 48 s (1:1), four
extended frame-time modes of 2:1, 3:1, 5:1, and 10:1 are
available by command. Following readout, light flooding is
used to remove any residual image that might remain from the
previous frame. At the end of light flooding, 14 erase
frames are used to stabilize and prepare the vidicon target
for the next exposure sequence (VGR ISS Calibration Report,
1978, an internal JPL document available from JPL vellum
files).


Description
-----------
The Imaging Science Subsystem (ISS) electronics consist of
the vidicon support circuits and the signal chain. The
vidicon support circuits are the vertical and horizontal
sweep circuits, and the various power supplies for the
vidicon filament, and the focus and alignment coils. The
signal chain consists of the analog signal amplifiers,
bandpass filters, and an eight bit analog-to-digital
converter. The digital output is sent to the Flight Data
Subsystem (FDS) for editing.



CCSDS packet standards
https://public.ccsds.org/Pubs/133x0b2e2.pdf
https://docs.ccsdspy.org/en/latest/user-guide/ccsds.html 
