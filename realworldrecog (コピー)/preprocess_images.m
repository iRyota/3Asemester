function preprocess_images( obj_class )
% PREPROCESS_IMAGES prepares positive and negative images for obj_class

% return if already prepared
posdir = ['datasets/train_data/' obj_class '/pos'];
if exist(posdir, 'dir');
    return
else
    mkdir(posdir)
end
negdir = ['datasets/train_data/' obj_class '/neg'];
if exist(negdir, 'dir')
    return
else
    mkdir(negdir)
end

% define size for cropping object from images
cropsize = decide_filter_size(obj_class);

% resize and save positive images
datadir=dir(['./datasets/images/' obj_class '/*.jpg']);
img={datadir.name};
for i=1:length(img)
    im=imread(['./datasets/images/' obj_class '/' img{i}]);
    im=imresize(im,cropsize);
    imwrite(im,[posdir '/' img{i}]);  
end

% randomly crop and save negative images
datadir = dir('./datasets/images/background/*.jpg');
img = {datadir.name};
for i = 1:length(img)
    % read image
    im_path = ['datasets/images/background/' img{i}];
    fprintf('%d / %d: %s\n',i,length(img),im_path);
    im = imread(im_path);
    % skip too small images
    xmax = size(im,2) - cropsize(2) + 1;
    ymax = size(im,1) - cropsize(1) + 1;
    if (xmax<1) || (ymax<1)
        continue;
    end
    % crop random subimages
    for j = 1:10
        % decide upperleft corner
        x = random('unid', xmax);
        y = random('unid', ymax);
        % allocate image with cropsize
        cropped_im = zeros([cropsize size(im,3)]);
        % call imcrop, matlab native function
        window = imcrop(im,[x,y,cropsize(2)-1,cropsize(1)-1]);
        % pad with zeros
        cropped_im(1:size(window,1),1:size(window,2),:) = window;
        % save
        imwrite(cropped_im,[negdir '/' num2str(j) '_' img{i}]);  
    end
end

end
