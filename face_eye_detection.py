import cv2
import requests
import time
import speech_recognition as sr

# NodeMCU server IP address (replace with your NodeMCU IP)
NODEMCU_IP = "http://192.168.0.172"

# Initialize the face and eye detectors
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")
eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_eye.xml")

# Speech recognizer
recognizer = sr.Recognizer()

# Start video capture
cap = cv2.VideoCapture(0)

led_on = False

def listen_for_command():
    # Listen for a voice command and return the recognized text.
    with sr.Microphone() as source:
        print("Listening for a command...")
        try:
            audio = recognizer.listen(source, timeout=5)
            command = recognizer.recognize_google(audio)
            return command.lower()
        except sr.UnknownValueError:
            print("Sorry, I couldn't understand the command.")
        except sr.RequestError as e:
            print(f"Speech Recognition error: {e}")
        except sr.WaitTimeoutError:
            print("No command detected.")
        return None

try:
    while True:
        # Capture frame-by-frame
        ret, frame = cap.read()
        if not ret:
            print("Failed to grab frame")
            break

        # Convert to grayscale for detection
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

        # Detect faces
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

        face_detected = False
        eye_detected = False

        # Process detected faces
        for (x, y, w, h) in faces:
            # Draw rectangles around detected faces
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
            face_detected = True

            # Region of interest for eyes
            roi_gray = gray[y:y + h, x:x + w]
            roi_color = frame[y:y + h, x:x + w]

            # Detect eyes within the face region
            eyes = eye_cascade.detectMultiScale(roi_gray, scaleFactor=1.1, minNeighbors=10, minSize=(15, 15))
            for (ex, ey, ew, eh) in eyes:
                cv2.rectangle(roi_color, (ex, ey), (ex + ew, ey + eh), (0, 255, 0), 2)
                eye_detected = True

        # Print status of detections
        if face_detected:
            print("Face Detected")
        if eye_detected:
            print("Eye Detected")

        # If a face is detected, turn on the LED
        if face_detected and not led_on:
            requests.get(f"{NODEMCU_IP}/on")
            print("LED ON (Face Detected)")
            led_on = True
        elif not face_detected and led_on:
            requests.get(f"{NODEMCU_IP}/off")
            print("LED OFF (No Face Detected)")
            led_on = False

        # Check for voice commands
        if cv2.waitKey(1) & 0xFF == ord("c"):
            print("Voice command mode enabled.")
            command = listen_for_command()
            if command:
                if "turn on" in command and not led_on:
                    requests.get(f"{NODEMCU_IP}/LED=ON")
                    print("LED ON (Voice Command)")
                    led_on = True
                elif "turn off" in command and led_on:
                    requests.get(f"{NODEMCU_IP}/LED=OFF")
                    print("LED OFF (Voice Command)")
                    led_on = False
                else:
                    print("Invalid command. Please say 'turn on' or 'turn off'.")

        # Display the resulting frame
        cv2.imshow("Face and Eye Detection", frame)

        # Break loop on 'q' key press
        if cv2.waitKey(1) & 0xFF == ord("q"):
            break

except Exception as e:
    print(f"Error: {e}")

finally:
    cap.release()
    cv2.destroyAllWindows()
    # Ensure LED is turned off when exiting
    try:
        requests.get(f"{NODEMCU_IP}/off")
    except:
        pass
