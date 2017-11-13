function ti_feat = tiny_image( img )
% TINY_IMAGE extracts image feature (tiny_image) from img

ti_feat = imresize(img,[floor(size(img,1)/8),floor(size(img,2)/8)]);
ti_feat = double(ti_feat(2:end-1, 2:end-1, :));

end
