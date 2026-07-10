import sys
import serial
from PyQt5.QtWidgets import QApplication, QWidget, QGridLayout, QPushButton
from PyQt5.QtGui import QColor

SERIAL_PORT = 'COM3'  
BAUD_RATE = 115200

class MateRixController(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("MateRix Cyberdeck")

        self.ser = None

        try:
            self.ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0)
        except:
            print(f"Warning: Could not open {SERIAL_PORT}. Please check the connection.")
        self.pixels = [[(0, 0, 0) for _ in range(10)] for _ in range(10)]
        self.buttons = {}

        self.initUI()

    def initUI(self):
        grid = QGridLayout()
        grid.setSpacing(0)

        for y in range(10):
            for x in range(10):
                btn = QPushButton()
                btn.setFixedSize(30, 30)
                btn.setStyleSheet("background-color: rgb(0, 0, 0); border: 1px solid #000;")
                btn.clicked.connect(lambda checked, bx=x, by=y: self.paint_pixel(bx, by))
                self.buttons[(x, y)] = btn
                grid.addWidget(btn, y, x)
        self.setLayout(grid)

    def paint_pixel(self, x, y):
        current_color = self.pixels[y][x]
        new_color = (0, 255, 255) if current_color == (0, 0, 0) else (0, 0, 0)
        self.pixels[y][x] = new_color

        self.buttons[(x, y)].setStyleSheet(f"background-color: rgb({new_color[0]},{new_color[1]},{new_color[2]});")

        self.send_frame()

    def send_frame(self):
        if not self.ser: return

        frame_data = bytearray()
        for y in range(10):
            for x in range(10):
                r, g, b = self.pixels[y][x]
                frame_data.extend([r, g, b])
        self.ser.write(frame_data)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = MateRixController()
    ex.show()
    sys.exit(app.exec_())
    