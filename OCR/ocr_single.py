import sys
from paddleocr import PaddleOCR

# Read image path
if len(sys.argv) < 2:
    print("NO_IMAGE")
    sys.exit(1)

img_path = sys.argv[1]

# Initialize OCR
ocr = PaddleOCR(lang='en')

# Run OCR
result = ocr.ocr(img_path)

# Check result
if not result or not result[0]:
    print("NOT_DETECTED")
    sys.exit(0)

# Extract text from PaddleOCR 2.x format
texts = []

for line in result[0]:
    text = line[1][0]   # actual detected text
    texts.append(text)

if texts:
    # Join all detected texts
    plate = "".join(texts)

    # Clean formatting
    plate = plate.replace(" ", "").strip()

    print(plate)

else:
    print("NOT_DETECTED")