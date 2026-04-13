# ANPR using SRA-Wall-E Board

In this project I have trained my own character recognition model using YOLOv11n. The dataset for training, testing is also from YOLO org
The model detetcts cars/ vehicles, strips down the image to a number plate and then performs the actual OCR on it: giving output of number plate in text.
The model achieved about 98.9% accuracy (~0.123 BOX_LOSS)
Use of sra board: The board generates its own IP local website when powered. Users can send their input image by clicking a picture and clicking upload. 
The image is captured, sent to the SRA board, sent to machine to do the OCR AI and then sent back to the sra board for displaying on the OLED screen.

# 1: Installation:
Requirements- paddleOCR (2.6.2 & 2.7.0 paddlepaddle ), numpy (1.26.x, HIGHER VERSION WILL CRASH), openCV (4.18.x, HIGHER WILL CRASH). You can also use paddleGPU (if Nvidia GPU is available)

# 2: Usage:
> 1. ANPR-
cd path/to/OCR_folder/
python ocr_single.py

IMP: Make sure that whatever image you want to run the number plate recognition of is saved as temp.jpg in the folder

> 2. Full usage (SRA board):

1. Build the .ino and flash into the sra board
   Required: Board should be set to ESP32 Dev Module/ Kit
   Note: Depending on machine, COMx is different so make sure to change it properly in the serial_receiver.py code (in my case it's COM7 maybe not yours)
         And the IP and password can be different too.

2. After flashing run serial_receiver.py
   It should say PaddleOCR loaded (waiting for image...)

3. Connect to ESP WiFi through device (make sure no other connection is enabled on the device) and enter the IP address in any brpwser
4. Take photo of numberplate, click upload!
