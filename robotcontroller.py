from flask import Flask, render_template, request, send_file
import io
import os
#import picamera
app = Flask(__name__)
teensy = None
camera = None

@app.route('/')
def hello_world():
    return render_template('robotcontroller.html')

def get_image_bytes():
    try:
        b = io.BytesIO()
        with open('C:/Users/Doskir/Dropbox/Projects/robotcontroller/static/cam.jpg', "rb") as f:
            while True:
                data = f.read(1024)
                if not data:
                    break
                b.write(data)
    except Exception as e:
        print e
    b.seek(0)
    return b

@app.route('/cam.jpg')
def cam_jpg():
    try:
        img = get_image_bytes()
        return send_file(img, attachment_filename='cam.jpg', mimetype='image/jpeg')
    except Exception as e:
        print e
    return img

    camera.capture()
    return app.send_static_file('cam.jpg')


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
        return None
        while not self.serial:
            try:
                self.serial = serial.Serial(
                    port='/dev/serial/by-id/usb-Teensyduino_USB_Serial_12345-if00',
                    baudrate=9600)
                print "Connected to Teensy"
            except:
                pass

    def send_motor_powers(self, motor_powers):
        print motor_powers
        return None
        for key, value in motor_powers.items():
            self.serial.write("{0}{1}\r\n".format(key, value))

def calculate_motor_powers(vector):
    motor_powers = {'FL': round(max(-1.0, min(1.0, vector[1] + vector[0])), 2),
                    'FR': round(max(-1.0, min(1.0, vector[1] - vector[0])), 2),
                    'BL': round(max(-1.0, min(1.0, vector[1] + vector[0])), 2),
                    'BR': round(max(-1.0, min(1.0, vector[1] - vector[0])), 2)}
    return motor_powers

if __name__ == '__main__':
    teensy = Teensy()
    #camera = picamera.PiCamera()
    #camera.resolution = (640, 480)
    #camera.rotation = 180
    app.run()
