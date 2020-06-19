

For this, Need packages follows;

1. Qt.5.12.2(https://www.qt.io/download-open-source)
2. opencv2(https://github.com/opencv/opencv)
3. tesseract(https://github.com/tesseract-ocr/tesseract)
4. loptonica(https://github.com/DanBloomberg/leptonica)

5. $>sudo aptitude install tesseract-ocr libtesseract4 tesseract-ocr-eng

//OCR Machine Learning DATA Path, into bashrc
6. export TESSDATA_PREFIX=/usr/share/tesseract-ocr/4.00/tessdata

//Expected Result
OCR Out Text: 
SRF-TF:CM-001:Sensor001_Temp 134.34
SRF-TF:CM-001:Sensor002_Temp 4.34
SRF-TF:CM-001:Sensor003_Temp 34.402
SRF-TF:CM-001:Sensor004_Temp 45.67
SRF-TF:CM-001:Sensor005_Temp 74.23
SRF-TF:CM-001:Sensor006_Temp 56.912
SRF-TF:CM-001:Sensor007_Temp 86.23
SRF-TF:CM-001:Sensor008_Temp 1.0345
SRF-TF:CM-001:Sensor009_Temp 0.765

=>Next steps: 
OCR EPICS IOC with Asyn, This is for interface between independent networks.

