# Automated Taylor Spatial Frame
Code for the 2019 Fracture Fixation Project, University of Southampton GDP group 47. The frame was designed with one BLDC motor embedded in each strut.

## Kinematics
The kinematics solves for the change in length required for each strut of a hexapod robot to realign itself from an unaligned start position.

## Accelerometer Code
The accelerometer script was used to monitor vibrations transmitted across an artificial fracture site for comparison with more expensive accelerometers.

## Vibration Analysis
The python script analyses the output of the accelerometers by performing a fourier transform and plotting the result. A change in the frequency profile over time was hoped to correlate with fracture healing, but we found the cheap accelerometers were not sensitive enough.
