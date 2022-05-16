from gpiozero import Motor
from time import sleep

# pins for backward are empty
motor1 = Motor(forward=18, backward=17)
motor2 = Motor(forward=23, backward=22)
motor3 = Motor(forward=24, backward=10)
motor4 = Motor(forward=25, backward=9)


def go_forward():
    motor1.forward(0.45)
    motor2.forward(0)
    motor3.forward(0.60)
    motor4.forward(0)


def turn_right():
    motor1.forward(0.60)
    motor2.forward(0)
    motor3.forward(0.45/8)
    motor4.forward(0)


def turn_left():
    motor1.forward(0.45/8)
    motor2.forward(0)
    motor3.forward(0.60)
    motor4.forward(0)

# same pins for forward and backward
def go_backward():
    motor1.forward(0)
    motor2.forward(0.45)
    motor3.forward(0)
    motor4.forward(0.60)


def stop_motor():
    motor1.forward(0)
    motor2.forward(0)
    motor3.forward(0)
    motor4.forward(0)
