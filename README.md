# ANPR using SRA-Wall-E Board

In this project I have trained my own character recognition model using YOLOv11n. The dataset for training, testing is also from YOLO org
The model detetcts cars/ vehicles, strips down the image to a number plate and then performs the actual OCR on it: giving output of number plate in text.
The model achieved about 98.9% accuracy (~0.123 BOX_LOSS)
Use of sra board: The board generates its own IP local website when powered. Users can send their input image by clicking a picture and clicking upload. 
The image is captured, sent to the SRA board, sent to machine to do the OCR AI and then sent back to the sra board for displaying on the OLED screen.

# 1: Installation:
Requirements- paddleOCR (2.6.2 & 2.7.0 paddlepaddle ), numpy (1.26.x, HIGHER VERSION WILL CRASH), openCV (4.18.x, HIGHER WILL CRASH). You can also use paddleGPU (if Nvidia GPU is available)
