function cropsize = decide_filter_size(obj_class)
% DECIDE_FILTER_SIZE decides filter size for obj_class

% store sizes of training images
datadir=dir(['datasets/images/' obj_class '/*.jpg']);
img={datadir.name};
h = zeros(1, length(img));
w = zeros(1, length(img));
for i=1:length(img)
    im=imread(['datasets/images/' obj_class '/' img{i}]);
    h(i) = size(im,1);
    w(i) = size(im,2);
end

% pick mode of aspect ratios
xx = -2:.02:2;
filter = exp(-(-100:100).^2/400);
aspects = hist(log(h./w), xx);
aspects = convn(aspects, filter, 'same');
[~, I] = max(aspects);
aspect = exp(xx(I));

% pick 20 percentile area
areas = sort(h.*w);
area = areas(max(floor(length(areas) * 0.2),1));
area = max(min(area, 5000), 3000);

% pick dimensions
w = sqrt(area/aspect);
h = w*aspect;
sbin = 8;

% size of the filter
fsize = [round(h/sbin) round(w/sbin)];

% size of training images (please resize them.)
cropsize = (fsize+2) * sbin

end