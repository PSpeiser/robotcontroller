from flask import Flask, render_template, request, send_file
import io

app = Flask(__name__)
teensy = None
serial_port_string = "/dev/serial/by-id/usb-Teensyduino_USB_Serial_12345-if00"

FL = 0
FR = 1
BL = 2
BR = 3

@app.route('/')
def hello_world():
    return render_template('robotcontroller.html')

@app.route('/motor_vector', methods=["POST"])
def motor_vector():
    x = float(request.form['x'])
    y = float(request.form['y'])
    vector = (x, y)
    motor_powers = calculate_motor_powers(vector)
    teensy.send_motor_powers(motor_powers)
    return "Success"


class Teensy():
    def __init__(self):
        print "Connecting to Teensy..."
        import serial
        self.serial = None
        while not self.serial:
            try:
                self.serial = serial.Serial(
                    port=serial_port_string,
                    baudrate=9600,
                    timeout=0.01)
                print "Connected to Teensy"
            except Exception as e:
                pass


    def send_motor_powers(self, motor_powers):
        for key, value in motor_powers.items():
            motor_select = key
            motor_power_int = self.float_to_int(value) + 128
            self.serial.write(chr(motor_select))
            self.serial.write(chr(motor_power_int))
            self.serial.flush()


    def float_to_int(self, f):
        return int(max(min(f * 128, 127), -128))



def calculate_motor_powers(vector):
    motor_powers = {FL: round(max(-1.0, min(1.0, vector[1] + vector[0])), 2),
                    FR: round(max(-1.0, min(1.0, vector[1] - vector[0])), 2),
                    BL: round(max(-1.0, min(1.0, vector[1] + vector[0])), 2),
                    BR: round(max(-1.0, min(1.0, vector[1] - vector[0])), 2)}
    return motor_powers


if __name__ == '__main__':
    teensy = Teensy()
    app.run(host="0.0.0.0")
