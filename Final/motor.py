import constants as constant


def handle_motor_operations(roi_center, roi_lane_center):
    roi_center_x = roi_center[0]
    roi_lane_center_x = roi_lane_center[0]
    center_difference = roi_center_x - roi_lane_center_x

    direction = 'Stop'
    direction_keys = constant.DIRECTION_THRESHOLDS.keys()

    for direction_key in direction_keys:
        if center_difference in constant.DIRECTION_THRESHOLDS.get(direction_key):
            direction = direction_key
            break

    # if direction == 'Forward':
    #     go_forward()
    # elif direction == 'Right':
    #     turn_right()
    # elif direction == 'Left':
    #     turn_left()
    # else:
    #     stop_motor()

    return direction


# TODO: Uncomment to use motors
"""
from gpiozero import Motor

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
    motor3.forward(0.45 / 8)
    motor4.forward(0)


def turn_left():
    motor1.forward(0.45 / 8)
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

"""
