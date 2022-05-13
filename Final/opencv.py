import numpy as np
import cv2 as cv
import glob

# termination criteria
criteria = (cv.TERM_CRITERIA_EPS + cv.TERM_CRITERIA_MAX_ITER, 30, 0.001)

# prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
objp = np.zeros((6 * 7, 3), np.float32)
objp[:, :2] = np.mgrid[0:7, 0:6].T.reshape(-1, 2)

# Arrays to store object points and image points from all the images.
objpoints = []  # 3d point in real world space
imgpoints = []  # 2d points in image plane.

# Make a list of calibration images
images = glob.glob('images/calibrate/*')

for i, fname in enumerate(images):
    img = cv.imread(fname)
    gray = cv.cvtColor(img, cv.COLOR_BGR2GRAY)

    # Find the chess board corners
    ret, corners = cv.findChessboardCorners(gray, (7, 6), None)

    # If found, add object points, image points (after refining them)
    if ret:
        objpoints.append(objp)
        corners2 = cv.cornerSubPix(gray, corners, (11, 11), (-1, -1), criteria)
        imgpoints.append(corners)

        # Draw and display the corners
        img = cv.drawChessboardCorners(img, (7, 6), corners2, ret)

    if cv.waitKey(1) == ord('q'):
        break

cv.destroyAllWindows()

ret, mtx, dist, rvecs, tvecs = cv.calibrateCamera(objpoints, imgpoints, gray.shape[::-1], None, None)

print("Camera Matrix:")
print(mtx)
print("\nDistortion Coefficients:")
print(dist)


def undistort(img, mtx, dist):
    undist = cv.undistort(img, mtx, dist, None, mtx)
    return undist


def add_lines(img, src):
    img2 = np.copy(img)
    color = [255, 0, 0]  # Red
    thickness = 2
    x0, y0 = src[0]
    x1, y1 = src[1]
    x2, y2 = src[2]
    x3, y3 = src[3]
    cv.line(img2, (x0, y0), (x1, y1), color, thickness)
    cv.line(img2, (x1, y1), (x2, y2), color, thickness)
    cv.line(img2, (x2, y2), (x3, y3), color, thickness)
    cv.line(img2, (x3, y3), (x0, y0), color, thickness)
    return img2


def add_points(img, src):
    img2 = np.copy(img)
    color = [255, 0, 0]  # Red
    thickness = -1
    radius = 3
    x0, y0 = src[0]
    x1, y1 = src[1]
    x2, y2 = src[2]
    x3, y3 = src[3]

    cv.circle(img2, (x0, y0), radius, color, thickness)
    cv.circle(img2, (x1, y1), radius, color, thickness)
    cv.circle(img2, (x2, y2), radius, color, thickness)
    cv.circle(img2, (x3, y3), radius, color, thickness)
    return img2


# Points for the original image
src = np.float32([
    [0, 410],
    [110, 330],
    [470, 330],
    [600, 400]
])
src_int = np.int32(src)

# Points for the new image
dst = np.float32([
    [220, 480],
    [220, 0],
    [640 - 220, 0],
    [640 - 220, 480]
])
dst_int = np.int32(dst)


def warper(img):
    # Compute and apply perspective transform
    img_size = (img.shape[1], img.shape[0])
    M = cv.getPerspectiveTransform(src, dst)
    warped = cv.warpPerspective(img, M, img_size, flags=cv.INTER_NEAREST)  # keep same size as input image

    return warped


def unwarp(img):
    # Compute and apply inverse perspective transform
    img_size = (img.shape[1], img.shape[0])
    Minv = cv.getPerspectiveTransform(dst, src)
    unwarped = cv.warpPerspective(img, Minv, img_size, flags=cv.INTER_NEAREST)

    return unwarped


cap = cv.VideoCapture('road2-light.avi')

if not cap.isOpened():
    print("Cannot open camera")
    exit()

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # if frame is read correctly ret is True
    if not ret:
        print("Can't receive frame. Exiting ...")
        break

    # Our operations on the frame come here
    gray = cv.cvtColor(frame, cv.COLOR_BGR2GRAY)
    blur = cv.GaussianBlur(gray, (5, 5), 0)
    blur_colored = cv.GaussianBlur(frame, (5, 5), 0)
    canny = cv.Canny(blur, 30, 80)
    # bgr_temp = cv.cvtColor(blur, cv.COLOR_GRAY2BGR)
    hsv = cv.cvtColor(blur_colored, cv.COLOR_BGR2HSV)

    # define range of blue color in HSV
    # lower_yellow = np.array([50, 46, 112])
    lower_yellow = np.array([40, 0, 115])
    upper_yellow = np.array([63, 255, 255])
    # Threshold the HSV image to get only blue colors
    mask = cv.inRange(hsv, lower_yellow, upper_yellow)

    # combine these layers
    combined_binary = cv.bitwise_or(canny, mask)

    cv.imshow('canny', canny)
    cv.imshow('mask', mask)
    cv.imshow('combined', combined_binary)

    # show one frame at a time
    key = cv.waitKey(0)
    while key not in [ord('q'), ord(' ')]:
        key = cv.waitKey(0)

    # Quit when 'q' is pressed
    if key == ord('q'):
        break

# When everything done, release the capture
cap.release()
cv.destroyAllWindows()
