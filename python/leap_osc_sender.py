"""
Leap Motion OSC Sender

This script captures hand tracking data from a Leap Motion Controller
and sends it via OSC to a receiver (typically Troikatronix Isadora and Blender).

Protocol:
    - Position: /hand/[left|right]/position [x, y, z]
    - Orientation: /hand/[left|right]/orientation [w, x, y, z]

Requirements:
    - Ultraleap Gemini SDK
    - python 3.8
    - Python-OSC
    - leap-motion-python-bindings from github.com/ultraleap/leapc-python-bindings

Setup Instructions:
    1. Install Ultraleap Hand Tracking Software:
       Download from developer.leapmotion.com/get-started

    2. Install Python Dependencies either in your venv or globally:
       git clone https://github.com/ultraleap/leapc-python-bindings.git
       cd leapc-python-bindings
       pip install -r requirements.txt
       pip install -e leapc-python-api

    3. Run this script:
       python leap_osc_sender.py

Data Format:
    - Position: [x, y, z] in millimeters
    - Orientation: Quaternion [w, x, y, z]
    - Update rate: ~100Hz
    - Port: 1235 (localhost)
"""

import leap
import time
from pythonosc import udp_client


class LeapOSCListener(leap.Listener):
    """
    Listener class that receives Leap Motion tracking data and forwards it via OSC.

    Sends hand position and orientation data for both hands at ~100Hz.
    Position data is in millimeters, orientation in quaternions.
    """

    def __init__(self):
        """Initialize the listener and create OSC client."""
        super().__init__()
        # Initialize OSC client (localhost, port 12345)
        self.client = udp_client.SimpleUDPClient("127.0.0.1", 12345)

    def on_connection_event(self, event):
        """Called when the Leap Motion service connection changes."""
        print("Connected to Leap Motion")

    def on_device_event(self, event):
        """Called when a Leap Motion controller is connected/disconnected."""
        try:
            with event.device.open():
                info = event.device.get_info()
                print(f"Found device {info.serial}")
        except leap.LeapCannotOpenDeviceError:
            print("Could not open device")

    def on_tracking_event(self, event):
        """
        Process tracking frame data and send via OSC.

        For each hand detected:
        - Determines hand type (left/right)
        - Sends position as [x, y, z]
        - Sends orientation as quaternion [w, x, y, z]

        Args:
            event: Leap.TrackingEvent containing hand data
        """
        for hand in event.hands:
            # Determine hand type and construct base OSC address
            hand_type = "left" if str(hand.type) == "HandType.Left" else "right"

            # Extract and send position data [x, y, z]
            position = [hand.palm.position.x, hand.palm.position.y, hand.palm.position.z]
            self.client.send_message(f"/hand/{hand_type}/position", position)

            # Extract and send orientation as quaternion [w, x, y, z]
            orientation = [
                hand.palm.orientation.w,  # w component first
                hand.palm.orientation.x,
                hand.palm.orientation.y,
                hand.palm.orientation.z,
            ]
            self.client.send_message(f"/hand/{hand_type}/orientation", orientation)


def main():
    """
    Main execution function.

    Sets up Leap Motion connection and runs continuous tracking loop.
    Sends OSC data until interrupted.
    """
    # Create and setup listener
    listener = LeapOSCListener()
    connection = leap.Connection()
    connection.add_listener(listener)

    # Start tracking
    with connection.open():
        connection.set_tracking_mode(leap.TrackingMode.Desktop)
        print("Started sending OSC messages to localhost:1235")
        try:
            while True:
                time.sleep(0.01)  # ~100Hz update rate
        except KeyboardInterrupt:
            print("\nStopping OSC sender...")


if __name__ == "__main__":
    main()
