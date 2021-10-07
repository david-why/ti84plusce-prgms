from threading import Thread
from PyQt5.QtWidgets import *


def myfun(*args):
    print(args)
    QMessageBox.information(None, 'Test infobox', 'Hello!')


class MainWindow(QMainWindow):
    def __init__(self) -> None:
        super().__init__()
        self.btn = QPushButton('Click', self)
        self.btn.clicked.connect(self.start_myfun_callback)

    def start_myfun_callback(self):
        eg = Thread(target=myfun, args=(1, 2, self))
        eg.start()


app = QApplication([])
win = MainWindow()
win.show()
app.exec_()
