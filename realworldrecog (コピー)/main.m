% IMPORTANT! experimental settings
% object to be recognized. first you can choose 'bicycle', 'motorbike', or 'horse'
obj_class = 'bicycle'
% recognition type. first you must choose @classify_objects. @detect_objects will be implemented on the 2nd day.
recognize = @detect_objects
% handle for feature_extraction/*.m. first you must complete @tiny_image  */
image_feature = @hog_desc
% handle for machine_learning/*.m. first you can choose only @random_guess  */
machine_learning = @sgd_svm
% parameters for machine learning. used for train_detector
ml_params = struct()
% threshold to classify/detect objects. lower value makes objects easy to be detected
threshold = 1
% if true, images and their recognition results are visualized
is_draw = false
% if true, images in datasets/test_data/ will be recognized
% otherwise, images captured by web camera will be recognized
is_real = true


% initialize environment
addpath('util')
addpath('mexopencv')
addpath('feature_extraction')
addpath('machine_learning')
import cv.*
initrand();

% preprocessing
preprocess_images(obj_class)

% extract features and class labels
[train_data, labels, feat_size] = extract_features(obj_class, image_feature);

% train mlc (machine learning classifier) using train_data and labels
mlc = train_detector(obj_class, train_data, labels, machine_learning, ml_params);

% recognize objects (classification / detection)
recognize(obj_class, image_feature, feat_size, is_draw, is_real, threshold)

% evaluate results
averageprecision(obj_class, image_feature, machine_learning)
