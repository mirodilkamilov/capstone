import numpy as np
import cv2 as cv
# import motor as mt

ROI_WIDTH_LOWER_BOUND = 0
ROI_WIDTH_UPPER_BOUND = 640
ROI_HEIGHT_LOWER_BOUND = 400
ROI_HEIGHT_UPPER_BOUND = 480


def find_roi_center(roi):
    width = roi.shape[1]
    height = roi.shape[0]

    return int(width / 2), int(height / 2)


def draw_roi_center(frame, roi_center):
    """Draws center of roi in the original frame"""
    x = roi_center[0]
    # center of roi plus cropped height of the frame
    y = roi_center[1] + ROI_HEIGHT_LOWER_BOUND
    red = [0, 0, 255]

    cv.circle(frame, (x, y), 3, red, -1)


cap = cv.VideoCapture('test-tracks/new-full-track.avi')

if not cap.isOpened():
    print("Cannot open camera")
    exit()

while True:
    ret, frame = cap.read()

    if not ret:
        print("Can't receive frame. Exiting ...")
        break

    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    blur = cv.GaussianBlur(gray, (5, 5), 0)
    blur_colored = cv.GaussianBlur(frame, (5, 5), 0)
    canny = cv.Canny(blur, 30, 80)
    hsv = cv.cvtColor(blur_colored, cv.COLOR_BGR2HSV)

    # define range of yellow color in HSV
    # lower_yellow = np.array([50, 46, 112])
    lower_yellow = np.array([40, 0, 115])
    upper_yellow = np.array([63, 255, 255])

    # returns binary image highlighting yellow lines
    mask = cv.inRange(hsv, lower_yellow, upper_yellow)

    # combine canny and hsv binary frames to have more robust detection
    combined_binary = cv.bitwise_or(canny, mask)

    roi = combined_binary[
          ROI_HEIGHT_LOWER_BOUND:ROI_HEIGHT_UPPER_BOUND,
          ROI_WIDTH_LOWER_BOUND:ROI_WIDTH_UPPER_BOUND]

    roi_center = find_roi_center(roi)
    draw_roi_center(frame, roi_center)

    cv.imshow('original', frame)

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
