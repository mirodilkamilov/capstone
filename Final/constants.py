ROI_WIDTH_LOWER_BOUND = 0
# In the test video, x = 638 has white line (didn't want to include that)
ROI_WIDTH_UPPER_BOUND = 637
ROI_HEIGHT_LOWER_BOUND = 400
ROI_HEIGHT_UPPER_BOUND = 480

RED_COLOR = [0, 0, 255]  # ROI center color
GREEN_COLOR = [0, 255, 0]  # Lane center color
WHITE_COLOR = [255, 255, 255]  # Legend color

DIRECTION_THRESHOLDS = {
    'Forward': range(-20, 20),
    'Right': range(-ROI_WIDTH_UPPER_BOUND, -20),
    'Left': range(20, ROI_WIDTH_UPPER_BOUND)
}
