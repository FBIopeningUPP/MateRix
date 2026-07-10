import sys
import serial
import threading
import pyautogui
from PyQt5.QtWidgets import (QApplication, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QGridLayout, QColorDialog, QCheckBox)
from PyQt5.QtCore import Qt, pyqtSignal, QObject

SERIAL_PORT = 'COM3'  
BAUD_RATE = 115200

class SerialListener(QObject):
    telemetry_received = pyqtSignal(str)

    def __init__(self, ser):
        super().__init__()
        self.ser = ser
        self.running = True
    
    def listen(self):
        while self.running and self.ser:
            if self.ser.in_waiting:
                try:
                    data = self.ser.readline().decode('utf-8').strip()
                    self.telemetry_received.emit(data)
                except:
                    pass

class CyberdeckStudio(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Cyberdeck Studio")
        self.setFixedSize(700, 450)

        self.ser = None
        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        except: 
            print(f"Warning: could not open{SERIAL_PORT}.")
        
        self.controller_mode = False
        self.initUI()
        self.start_serial_thread()
    
    def initUI(self):
        main_layout = QHBoxLayout()

        preview_layout = QGridLayout()
        preview_layout.setSpacing(10)
        self.pixels = [[(0, 0, 0) for _ in range(10)] for _ in range(10)]
        self.buttons = {}

        for y in range(10):
            for x in range(10):
                btn = QPushButton()
                btn.setFixedSize(35, 35)
                btn.setStyleSheet("background-color: black; border: 1px solid #333;")
                btn.clicked.connect(lambda checked, bx=x, by=y: self.paint_pixel(bx, by))
                preview_layout.addWidget(btn, y, x)
                self.buttons[(x, y)] = btn
        
        control_layout = QVBoxLayout()

        self.telemetry_label = QLabel("Telemetry: N/A")
        self.telemetry_label.setAlignment(Qt.AlignCenter)
        self.telemetry_label.setStyleSheet("font-size: 16px; font-weight: bold; color: #00FFFF; background-color: #222; padding: 10px;")

        self.color_btn = QPushButton("Select Color")
        self.color_btn.setFixedHeight(40)
        self.color_btn.setStyleSheet("background-color: #444; color: #FFF; font-size: 14px;")
        self.color_btn.clicked.connect(self.choose_color)

        self.gamepad_toggle = QCheckBox("Controller Mode")
        self.gamepad_toggle.setStyleSheet("color: #FFF; font-size: 14px;")
        self.gamepad_toggle.stateChanged.connect(self.toggle_gamepad)

        control_layout.addWidget(self.telemetry_label)
        control_layout.addSpacing(20)
        control_layout.addWidget(self.color_btn)
        control_layout.addSpacing(20)
        control_layout.addWidget(self.gamepad_toggle)
        control_layout.addStretch()

        main_layout.addLayout(preview_layout)
        main_layout.addSpacing(20)
        main_layout.addLayout(control_layout)

        self.setLayout(main_layout)
        self.setStyleSheet("background-color: #111;")

    def start_serial_thread(self):
        if self.ser:
            self.listener = SerialListener(self.ser)
            self.listener.telemetry_received.connect(self.handle_telemetry)

            self.thread = threading.Thread(target=self.listener.listen, daemon=True)
            self.thread.start()

    def paint_pixel(self, x, y):
        print(f"Painted {x}, {y}")
    
    def choose_color(self):
        color = QColorDialog.getColor()
        if color.isValid():
            self.selected_color = (color.red(), color.green(), color.blue())
            print(f"Selected color: {self.selected_color}")

    def toggle_gamepad(self, state):
        self.controller_mode = state == Qt.Checked
        print(f"Controller mode: {self.controller_mode}")

    def handle_telemetry(self, data):
        self.telemetry_label.setText(f"Telemetry: {data}")

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = CyberdeckStudio()
    window.show()
    sys.exit(app.exec_())