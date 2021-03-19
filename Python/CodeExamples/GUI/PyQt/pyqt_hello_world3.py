from PyQt5.QtWidgets import QApplication, QPushButton
app = QApplication([])
app.setStyleSheet("QPushButton { margin: 10ex; }")
button = QPushButton('Hello World')
button.show()
app.exec_()
