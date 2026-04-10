import serial
import time
import os
import cv2
from paddleocr import PaddleOCR

# ================= SERIAL CONFIG =================
PORT = "COM7"          # change if required
BAUD = 115200

# ================= PATHS =================
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
IMAGE_PATH = os.path.join(BASE_DIR, "temp.jpg")

START_MARK = b"<IMAGE>"
END_MARK   = b"</IMAGE>"

# ================= INIT =================
print("Opening serial...")
ser = serial.Serial(PORT, BAUD, timeout=1)
time.sleep(2)
print("Serial connected")

print("Loading PaddleOCR (first run may take time)...")
ocr = PaddleOCR(lang="en")
print("PaddleOCR loaded")

buffer = bytearray()

# ================= MAIN LOOP =================
while True:
    data = ser.read(1024)
    if data:
        print("Receiving data...")
    if not data:
        continue

    buffer.extend(data)

    start = buffer.find(START_MARK)
    end = buffer.find(END_MARK)

    if start != -1 and end != -1 and end > start:
        image_bytes = buffer[start + len(START_MARK):end]
        buffer = buffer[end + len(END_MARK):]

        if len(image_bytes) < 5000:
            print("Image too small, skipping")
            continue

        with open(IMAGE_PATH, "wb") as f:
            f.write(image_bytes)

        print(f"Image received ({len(image_bytes)} bytes)")

        img = cv2.imread(IMAGE_PATH)
        if img is None:
            print("Failed to decode image")
            ser.write(b"<PLATE>N/A</PLATE>\n")
            continue

        # ================= OCR =================
        result = ocr.ocr(img)

        plate_text = "N/A"
        # PaddleOCR 2.7 returns: [[ [[box_coords], ('text', confidence)], ... ]]
        if result and result[0]:
            # Extract just the text strings from the nested list structure
            texts = [line[1][0] for line in result[0]]
            if texts:
                plate_text = texts[0].strip()

        print(f"Detected Plate: {plate_text}")

        # ================= SEND BACK =================
        msg = f"<PLATE>{plate_text}</PLATE>\n"
        ser.write(msg.encode())

        print("Sent to ESP32\n")