import cv2
import numpy as np
import bluetooth

# Initialize Bluetooth communication
bd_addr = "00:11:22:33:44:55"  # Replace with your Bluetooth module's MAC address
port = 1
sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)
sock.connect((bd_addr, port))

def detect_arrow_direction(frame):
    # Convert frame to HSV color space
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)

    # Define color ranges for black and red
    lower_black = np.array([0, 0, 0])
    upper_black = np.array([180, 255, 30])
    lower_red1 = np.array([0, 100, 100])
    upper_red1 = np.array([10, 255, 255])
    lower_red2 = np.array([160, 100, 100])
    upper_red2 = np.array([180, 255, 255])

    # Threshold the image to get only black and red colors
    mask_black = cv2.inRange(hsv, lower_black, upper_black)
    mask_red1 = cv2.inRange(hsv, lower_red1, upper_red1)
    mask_red2 = cv2.inRange(hsv, lower_red2, upper_red2)
    mask_red = cv2.bitwise_or(mask_red1, mask_red2)

    # Find contours for black and red masks
    contours_black, _ = cv2.findContours(mask_black, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours_red, _ = cv2.findContours(mask_red, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

    # Process contours to detect arrow shapes and directions
    for cnt in contours_black + contours_red:
        approx = cv2.approxPolyDP(cnt, 0.03 * cv2.arcLength(cnt, True), True)
        if len(approx) == 7:  # Heuristic for detecting arrow shape
            arrow_direction = determine_direction(approx)
            return arrow_direction
    return None

def determine_direction(approx):
    # Simple heuristic to determine direction based on contour shape
    # This is a placeholder; you need to implement actual logic
    # For example, calculate the angle or compare relative positions of vertices
    return 'F'  # Placeholder to always return forward

def capture_image_from_arduino():
    sock.send('C')  # Send capture command to Arduino
    data = sock.recv(320 * 240)  # Receive image data
    frame = np.frombuffer(data, dtype=np.uint8).reshape((240, 320))
    return frame

# Main loop
while True:
    frame = capture_image_from_arduino()
    direction = detect_arrow_direction(frame)
    if direction:
        sock.send(direction)  # Send direction command to Arduino

sock.close()
