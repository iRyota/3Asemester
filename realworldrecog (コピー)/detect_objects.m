function detect_objects(obj_class, image_feature, feat_size, isdraw, isreal, threshold)
% DETECT_OBJECTS detects if image_feature includes obj_class
% feat_size: original size of image_feature
% isdraw: if true, recognition results will be shown. if isreal==true, isdraw will be forced to be true
% isreal: if ture, images obtained from web camera will be recognized. otherwise, images in datasets/test_data/ will be recognized
% threshold: threshold to classify/detect objects. lower value makes objects easy to be detected

% default value of isdraw
if nargin < 4
    isdraw = true;
end
% default value of isreal
if nargin < 5
    isreal = false;
end
% default value of threshold
if nargin < 6
    threshold = 0;
end

% input parameters for image_feature
sbin = 8;

% load object model
learned = load (['models/' obj_class]);
% obtain filter and bias for detection
[f, b] = learned.mlc.get_filter();
% reshape f (column vector) to original feat_size
weight = reshape(f, feat_size);
% flip value order to use convn
filter = zeros(size(weight));
for i=1:size(weight,3)
    filter(:,:,i) = rot90(weight(:,:,end+1-i),2);
end
    
if isreal

    % images obtained from web camera will be recognized
    camera = cv.VideoCapture();

    while true

        % read camera image
        im = camera.read;
        % detect if im has obj_class
        dets = detect_single_image(im, filter, b, image_feature, sbin, threshold);
        % ignore isdraw and show recognition results
        show_result(im, dets);

    end

else

    % images in datasets/test_data/ will be recognized
    files = dir(['datasets/test_data/' obj_class '/*.jpg']);
    % define variable for recognition results
    results = [];

    for f_ind = 1:length(files)

        % load test image
        im = imread(['datasets/test_data/' obj_class '/' files(f_ind).name]);
        % detect if im has obj_class
        dets = detect_single_image(im, filter, b, image_feature, sbin, threshold);
        if isdraw
            % show recognition results
            show_result(im, dets);
        end
        % memorize recognition results
        dets = [repmat(f_ind,size(dets,1),1) dets];
        results = [results; dets];

    end

    % save recognition results for evaluation
    save(['results/boxes_' obj_class], 'results');

end

end

function dets = detect_single_image(im, filter, bias, image_feature, sbin, threshold)
% detects if im has obj_class using filter and bias obtained from machine_learning/*.m
% image_feature: handle for feature_extraction/*.m. first you must complete @tiny_image
% sbin: size of each cell in image for image_feature
% threshold: threshold to classify/detect objects. lower value makes objects easy to be detected

% extract feature pyramid
pyra = featpyramid(im, size(filter,2), size(filter,1), image_feature);

tic
dets = [];
rmap_max = -inf;
for i = 1:length(pyra.feat)
    % extract response map
    rmap = convn(pyra.feat{i}, filter, 'valid');
    % find indexes where rmap + bias are bigger than threshold
    [y x] = find(rmap + bias > threshold);
    % update rmap_max
    rmap_max = max(rmap_max, max(rmap(:)));
    % skip if no objects are found
    if isempty(x)
        continue;
    end
    % x coodinate of upperleft corner
    d(:,1) = (x-size(filter,2)-1) * sbin / pyra.scales(i) + 1;
    % y coodinate of upperleft corner
    d(:,2) = (y-size(filter,1)-1) * sbin / pyra.scales(i) + 1;
    % x coodinate of lowerright corner
    d(:,3) = d(:,1) + size(filter,2) * sbin / pyra.scales(i);
    % y coodinate of lowerright corner
    d(:,4) = d(:,2) + size(filter,1) * sbin / pyra.scales(i);
    % absolute coodinate of rmap
    d(:,5) = rmap((x-1)*size(rmap,1)+y);
    dets = [dets; d];
    clear d;
end
toc
fprintf('max score: %f, threshold: %f ... ', rmap_max + bias, threshold );
if isempty(dets)
    fprintf('Not detected\n');
else
    fprintf('Detected\n');
    % fit in image
    dets(:,1) = max(1,dets(:,1));
    dets(:,2) = max(1,dets(:,2));
    dets(:,3) = min(size(im,2),dets(:,3));
    dets(:,4) = min(size(im,1),dets(:,4));

    % Non-maximum suppression
    dets = nms( dets );
end

end

function show_result(im, dets)
% draw the boxes with the detection window on top

% clear current figure window
clf;
% show im in cleared figure
image(im);
% erase axes
axis off;
% if detected, bounding boxes are drawn
if ~isempty(dets)
    x1 = dets(:,1);
    y1 = dets(:,2);
    x2 = dets(:,3);
    y2 = dets(:,4);
    line([x1 x1 x2 x2 x1]', [y1 y2 y2 y1 y1]', 'color', [160/255 0 0], 'linewidth', 3, 'linestyle', '-');
end
% small pause to make current figure update its content
pause(0.01);

end