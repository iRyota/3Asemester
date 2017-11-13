function classify_objects(obj_class, image_feature, feat_size, isdraw, isreal, threshold)
% CLASSIFY_OBJECTS classifies if image_feature includes obj_class
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

% load object model
learned = load (['models/' obj_class]);
    
if isreal

    % images obtained from web camera will be recognized
    camera = cv.VideoCapture();

    while true

        % read camera image
        im = camera.read;
        % classify if im has obj_class
        clssfys = classify_single_image(im, learned.mlc, image_feature, feat_size);
        % ignore isdraw and show recognition results
        show_result(im, clssfys, obj_class, threshold);

    end

else

    % images in datasets/test_data/ will be recognized
    files = dir(['datasets/test_data/' obj_class '/*.jpg']);
    % allocate memory for recognition results
    results = zeros(length(files),1);

    for f_ind = 1:length(files)

        % load test image
        im = imread(['datasets/test_data/' obj_class '/' files(f_ind).name]);
        % classify if im has obj_class
        clssfys = classify_single_image(im, learned.mlc, image_feature, feat_size);
        if isdraw
            % show recognition results
            show_result(im, clssfys, obj_class, threshold);
        end
        % memorize recognition results
        results(f_ind) = clssfys;

    end

    % save recognition results for evaluation
    save(['results/classes_' obj_class], 'results');

end

end

function clssfys = classify_single_image(im, mlc, image_feature, feat_size)
% classifies if im has obj_class using mlc obtained from machine_learning/*.m
% image_feature: handle for feature_extraction/*.m. first you must complete @tiny_image
% feat_size: original size of image_feature

% start timer
tic
% extract feature
feat = image_feature(double(imresize(im, [(feat_size(1)+2)*8 (feat_size(2)+2)*8])));
% classify using feature
clssfys = mlc.predict(feat(:));
% show elapsed time
toc

end

function show_result(im, clssfys, obj_class, threshold)
% shows classification results with image

% clear current figure window
clf;
% show im in cleared figure
image(im);
% erase axes
axis off;
fprintf('score: %f, threshold: %f ... ', clssfys, threshold );
% if classified as obj_class is included, textbox will be drawn in red, otherwise drawn in blue
if clssfys >= threshold
    fprintf('Detected\n');
    str = obj_class;
    currentcolor = 'red';
else
    fprintf('Not detected\n');
    str = ['not ' obj_class];
    currentcolor = 'blue';
end
annotation('textbox',[.2 .5 .3 .3],'EdgeColor',currentcolor,'Color',currentcolor,'String',str,'FitBoxToText','on');
% small pause to make current figure update its content
pause(0.01);

end