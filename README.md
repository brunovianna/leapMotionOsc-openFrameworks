# leapMotionOsc



Simple openFrameworks app to read OSC messages from hand tracking device LeapMotion.



Notes:

* I got it working on Windows 11, Of 0.12.1, LeapMotion 1, UltraLeap Gemini (version 6.2.0 of the LeapSDK)
* The OSC sender was written by user [jstep](https://community.troikatronix.com/user/jtsteph), you can get it from [here](https://community.troikatronix.com/topic/9080/ultraleap-motion-2-gemini-to-osc) (although I added it to this repo just in case).
* This example only gets orientation and position of the hands, but you can change the Python script to get all fingers information



#### Install



1. Install Ultraleap hyperion v6  https://www.ultraleap.com/downloads/leap-controller/
2. Git clone their Python bindings  https://github.com/ultraleap/leapc-python-bindings.git
3. I had to compile the leap\_cffi module to get it to work https://github.com/ultraleap/leapc-python-bindings?tab=readme-ov-file#missing-compiled-module
4. Install Python-OSC: "pip install Python-OSC"



#### Run

* Go into the python folder in this repo (under the root folder) and run "python leap_osc_sender.py". The script will start sending OSC to the localhost on port 12345.
