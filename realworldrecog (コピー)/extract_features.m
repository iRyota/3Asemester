function [ train_data, labels, feat_size ] = extract_features( obj_class, image_feature )
% EXTRACT_FEATURES extracts image features in datasets/train_data/obj_class
% image_feature: handle for methods in feature_extraction/*.m

% define paths for positive and negative images
posdir=dir(['datasets/train_data/' obj_class '/pos/*.jpg']);
posimg={posdir.name};
negdir=dir(['datasets/train_data/' obj_class '/neg/*.jpg']);
negimg={negdir.name};

% obtain feature dimension
im = imread(['datasets/train_data/' obj_class '/pos/' posimg{1}]);
feature_dim = numel(image_feature(double(im)));

% allocate variables for feature vectors and class labels
train_data = zeros(feature_dim, length(posimg)+length(negimg));
labels = zeros(1, length(posimg)+length(negimg));

% extract hog from positive training images
for i=1:length(posimg)
  fprintf('(pos) %d / %d\n',i,length(posimg));
  % load an image
  im = imread(['datasets/train_data/' obj_class '/pos/' posimg{i}]);
  % extract an image feature
  feat = image_feature(double(im));
  % store feature vector and class label
  train_data(:,i) = feat(:);
  labels(i) = true;
end

% extract hog from negative training images
for i=1:length(negimg)
  fprintf('(neg) %d / %d\n',i,length(negimg));
  % load an image
  im = imread(['datasets/train_data/' obj_class '/neg/' negimg{i}]);
  % extract an image feature
  feat = image_feature(double(im));
  % store feature vector and class label
  train_data(:,i+length(posimg)) = feat(:);
  labels(i+length(posimg)) = false;
end

% feature size
feat_size = size(feat);

end

