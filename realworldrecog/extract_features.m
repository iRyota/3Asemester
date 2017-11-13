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

N = 1;
% allocate variables for feature vectors and class labels
train_data = zeros(feature_dim, N*(length(posimg)+length(negimg)));
labels = zeros(1, N*(length(posimg)+length(negimg)));

% for j=1:N
% % extract hog from positive training images
% for i=1:length(posimg)
%   fprintf('(pos) %d / %d\n',i,length(posimg));
%   % load an image
%   im = imread(['datasets/train_data/' obj_class '/pos/' posimg{i}]);
%   im = imrotate(im, 5 * (j-1),'bilinear','crop');
%   % extract an image feature
%   feat = image_feature(double(im));
%   % store feature vector and class label

%   train_data(:,i+(j-1)*(length(posimg)+length(negimg))) = feat(:);
%   labels(i+(j-1)*(length(posimg)+length(negimg))) = true;
% end

% % extract hog from negative training images
% for i=1:length(negimg)
%   fprintf('(neg) %d / %d\n',i,length(negimg));
%   % load an image
%   im = imread(['datasets/train_data/' obj_class '/neg/' negimg{i}]);
%   im = imrotate(im, 5 * (j-1),'bilinear','crop');
%   % extract an image feature
%   feat = image_feature(double(im));
%   % store feature vector and class label
%   train_data(:,i+length(posimg)+(j-1)*(length(posimg)+length(negimg))) = feat(:);
%   labels(i+length(posimg)+(j-1)*(length(posimg)+length(negimg))) = false;
% end

% end
for j=1:N
% extract hog from positive training images
for i=1:length(posimg)
  fprintf('(pos) %d / %d\n',i,length(posimg));
  % load an image
  im = imread(['datasets/train_data/' obj_class '/pos/' posimg{i}]);
  im = imrotate(im, 5,'bilinear','crop');
  % extract an image feature
  feat = image_feature(double(im));
  % store feature vector and class label

  train_data(:,i+(j-1)*(length(posimg)+length(negimg))) = feat(:);
  labels(i+(j-1)*(length(posimg)+length(negimg))) = true;
end

% extract hog from negative training images
for i=1:length(negimg)
  fprintf('(neg) %d / %d\n',i,length(negimg));
  % load an image
  im = imread(['datasets/train_data/' obj_class '/neg/' negimg{i}]);
  im = imrotate(im, 5,'bilinear','crop');
  % extract an image feature
  feat = image_feature(double(im));
  % store feature vector and class label
  train_data(:,i+length(posimg)+(j-1)*(length(posimg)+length(negimg))) = feat(:);
  labels(i+length(posimg)+(j-1)*(length(posimg)+length(negimg))) = false;
end

end

% feature size
feat_size = size(feat);

end

