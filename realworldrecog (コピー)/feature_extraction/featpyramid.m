function pyra = featpyramid(im, padx, pady, image_feature)
% FEATPYRAMID computes feature pyramid.
% pyra.feat{i} is the i-th level of the feature pyramid.
% pyra.scales{i} is the scaling factor used for the i-th level.
% pyra.feat{i+interval} is computed at exactly half the resolution of feat{i}.
% first octave halucinates higher resolution data.
% im: image
% padx,pady: optionally pads each level of the feature pyramid
% image_feature: handle for feature_extraction/*.m. first you must complete @tiny_image

sbin = 8;
interval = 10;
sc = 2 ^(1/interval);
imsize = [size(im, 1) size(im, 2)];
max_scale = 1 + floor(log(min(imsize)/(5*sbin))/log(sc));
pyra.feat = cell(max_scale, 1);
pyra.scales = zeros(max_scale, 1);
pyra.imsize = imsize;

% our resize function wants floating point values
im = double(im);
for i = 1:interval
    scaled = imresize(im, 1/sc^(i-1));
    pyra.feat{i} = image_feature(scaled);
    pyra.scales(i) = 1/sc^(i-1);
    % remaining interals
    for j = i:interval:max_scale-interval
        scaled = imresize(scaled, 0.5);
        pyra.feat{j+interval} = image_feature(scaled);
        pyra.scales(j+interval) = 0.5 * pyra.scales(j);
    end
end

for i = 1:length(pyra.feat)
    % add 1 to padding because feature generation deletes a 1-cell
    % wide border around the feature map
    pyra.feat{i} = padarray(pyra.feat{i}, [pady+1 padx+1 0], 0);
end
pyra.padx = padx;
pyra.pady = pady;

end