"""
All roi operations are handled here:
1) finding center of roi and drawing it
2) finding lane center and drawing it
3) put text about which direction to go

roi (Region Of Interest) - cropped binary frame for detecting the lane
"""

import cv2 as cv
import constants as constant


def find_roi_center():
    width = constant.ROI_WIDTH_UPPER_BOUND - constant.ROI_WIDTH_LOWER_BOUND
    height = constant.ROI_HEIGHT_UPPER_BOUND - constant.ROI_HEIGHT_LOWER_BOUND
    x, y = int(width / 2), int(height / 2)

    return x, y


def draw_roi_center(frame, roi_center):
    """Draws center of roi in the original frame"""
    x = roi_center[0]
    # center of roi plus cropped height of the frame
    y = roi_center[1] + constant.ROI_HEIGHT_LOWER_BOUND

    cv.circle(frame, (x, y), 4, constant.RED_COLOR, -1)


def find_roi_lane_center(roi, roi_center):
    roi_center_x, roi_center_y = roi_center

    # if no lane detected in both sides roi's right and left boundaries are set, respectively
    right_lane_x = constant.ROI_WIDTH_UPPER_BOUND
    left_lane_x = constant.ROI_WIDTH_LOWER_BOUND

    is_right_lane_detected = False
    is_left_lane_detected = False

    is_stepped = False
    # check if kit is stepped on the lane
    if roi[roi_center_y, roi_center_x] == 255:
        is_stepped = True
        print('stepped on the lane')

    # finding right lane: looks through middle row of roi from the center to end
    for i in range(roi_center_x, constant.ROI_WIDTH_UPPER_BOUND - 1):
        # calculate width of the lane from right side of roi center
        if is_stepped and roi[roi_center_y, i] != 255:
            lane_width_from_right = i - roi_center_x
            break

        if not is_stepped and roi[roi_center_y, i] == 255:
            is_right_lane_detected = True
            right_lane_x = i
            break

    # finding left lane: looks through middle row of roi from the center to beginning
    for i in range(roi_center_x, constant.ROI_WIDTH_LOWER_BOUND, -1):
        # calculate width of the lane from left side of roi center
        if is_stepped and roi[roi_center_y, i] != 255:
            lane_width_from_left = roi_center_x - i
            break

        if not is_stepped and roi[roi_center_y, i] == 255:
            is_left_lane_detected = True
            left_lane_x = i
            break

    # which direction should go if stepped
    if is_stepped:
        if lane_width_from_right >= lane_width_from_left:
            # turn maximum left
            is_right_lane_detected = True
            right_lane_x = roi_center_x
        else:
            # turn maximum right
            is_left_lane_detected = True
            left_lane_x = roi_center_x

    # If only left lane detected increase imaginary right lane position
    if is_left_lane_detected and not is_right_lane_detected:
        right_lane_x += left_lane_x

    # If only right lane detected move imaginary left lane towards negative axes
    if not is_left_lane_detected and is_right_lane_detected:
        left_lane_x = -(constant.ROI_WIDTH_UPPER_BOUND - right_lane_x)

    # left_lane_x can be positive: to move the center to right and
    #             can be negative: to move the center to left
    half_distance_between_lanes = int((right_lane_x - left_lane_x) / 2)
    roi_lane_center = left_lane_x + half_distance_between_lanes, roi_center_y

    return roi_lane_center


def draw_roi_lane_center(frame, roi_lane_center):
    """Draws center of lane in the original frame"""
    x = roi_lane_center[0]
    # center of roi plus cropped height of the frame
    y = roi_lane_center[1] + constant.ROI_HEIGHT_LOWER_BOUND

    cv.circle(frame, (x, y), 5, constant.GREEN_COLOR, 2)


def draw_direction_text(frame, direction):
    cv.putText(frame, text='Direction: ' + direction, org=(10, 40), fontFace=cv.FONT_HERSHEY_SIMPLEX,
               fontScale=0.8, color=constant.WHITE_COLOR, thickness=2)


def draw_legend(frame):
    center_circle = cv.circle(frame, (16, 60), 4, constant.RED_COLOR, -1)
    cv.putText(frame, text='- Frame center', org=(30, 67), fontFace=cv.FONT_HERSHEY_SIMPLEX,
               fontScale=0.8, color=constant.WHITE_COLOR, thickness=2)

    lane_center_circle = cv.circle(frame, (16, 85), 5, constant.GREEN_COLOR, 2)
    cv.putText(frame, text='- Lane center', org=(30, 93), fontFace=cv.FONT_HERSHEY_SIMPLEX,
               fontScale=0.8, color=constant.WHITE_COLOR, thickness=2)
