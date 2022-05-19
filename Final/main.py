import numpy as np
import cv2 as cv
import constants as constant
import roi_operations as ro
import motor as mt

cap = cv.VideoCapture('test-tracks/new-full-track.avi')

if not cap.isOpened():
    print('Cannot open camera')
    exit()

while True:
    ret, frame = cap.read()

    if not ret:
        print('Can\'t receive frame. Exiting ...')
        break

    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    blur = cv.GaussianBlur(gray, (5, 5), 0)
    # canny = cv.Canny(blur, 30, 80)
    colored_blur = cv.GaussianBlur(frame, (5, 5), 0)
    hsv = cv.cvtColor(colored_blur, cv.COLOR_BGR2HSV)

    # define range of yellow color in HSV
    lower_yellow = np.array([28, 89, 165])
    upper_yellow = np.array([63, 255, 255])

    # returns binary image highlighting yellow lines
    mask = cv.inRange(hsv, lower_yellow, upper_yellow)

    # combine canny and hsv binary frames to have more robust detection (in our case hsv was working perfect)
    # combined_binary = cv.bitwise_or(canny, mask)

    roi = mask[
          constant.ROI_HEIGHT_LOWER_BOUND:constant.ROI_HEIGHT_UPPER_BOUND,
          constant.ROI_WIDTH_LOWER_BOUND:constant.ROI_WIDTH_UPPER_BOUND]

    ro.draw_legend(frame)
    roi_center = ro.find_roi_center()
    ro.draw_roi_center(frame, roi_center)

    roi_lane_center = ro.find_roi_lane_center(roi, roi_center)
    ro.draw_roi_lane_center(frame, roi_lane_center)

    direction = mt.handle_motor_operations(roi_center, roi_lane_center)
    ro.draw_direction_text(frame, direction)

    cv.imshow('original', frame)
    cv.imshow('roi', roi)

    # show one frame at a time (press space to show next frame)
    key = cv.waitKey(0)
    while key not in [ord('q'), ord(' ')]:
        key = cv.waitKey(0)

    # Quit when 'q' is pressed
    if key == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()
