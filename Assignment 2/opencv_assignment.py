import cv2
import numpy as np

# task 1: draw a rectanlge
original = cv2.imread('gray_car.png')
image = cv2.imread('gray_car.png')
image = cv2.rectangle(image, (215, 104), (365, 245), (0, 0, 255), 3)

cv2.imwrite('task1.png', image)


# task 2: seperate two cars (using ROI)
cv2.imwrite('task2_car1.png', original[104:245, 215:365])
cv2.imwrite('task2_car2.png', original[18:110, 100:212])


# task 3: create blurred and sharpened images
size = 15
kernel_motion_blur = np.zeros((size, size))
kernel_motion_blur[int((size-1)/2), :] = np.ones(size)
kernel_motion_blur = kernel_motion_blur / size

kernel_sharpen = np.array(
    [[-1, -1, -1],
     [-1, 9, -1],
     [-1, -1, -1]])

blurred = cv2.filter2D(original, -1, kernel_motion_blur)
sharpened = cv2.filter2D(original, -1, kernel_sharpen)

cv2.imwrite('task3_blurred.png', blurred)
cv2.imwrite('task3_sharpened.png', sharpened)


# task 4: increase brightness
img_yuv = cv2.cvtColor(original, cv2.COLOR_BGR2YUV)
img_yuv[:, :, 0] = cv2.equalizeHist(img_yuv[:, :, 0])
brighter = cv2.cvtColor(img_yuv, cv2.COLOR_YUV2BGR)

cv2.imwrite('task4.png', brighter)

cv2.waitKey(1000)
