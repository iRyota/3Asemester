function boxes = nms(boxes, overlap)
% NMS (Non-Maximum Suppression) suppress boxes 
% by greedily selecting high-scoring detections and skip detections
% that are significantly covered by a previously selected detection.
% boxes: detected boxes to be suppressed
% overlap: overlap threshold to reduce boxes

% default value of overlap
if nargin < 2
    overlap = 0.5;
end

if isempty(boxes)
    pick = [];
else
    x1 = boxes(:,1); % x-coordinates of upperleft corner
    y1 = boxes(:,2); % y-coordinates of upperleft corner
    x2 = boxes(:,3); % x-coordinates of lowerright corner
    y2 = boxes(:,4); % y-coordinates of lowerright corner
    s = boxes(:,5); % scores for each detected box
    area = (x2-x1+1) .* (y2-y1+1); % compute areas of detected box

    % sort scores in ascending order
    [~, I] = sort(s); 
    % define list for selected boxes
    pick = [];
    while ~isempty(I)
        % s(last) means max(s)
        last = length(I);
        i = I(last);
        % add i to pick list
        pick = [pick; i];
        % define list for removing boxes from I
        suppress = [last];
        for pos = 1:last-1
            j = I(pos);
            xx1 = max(x1(i), x1(j));
            yy1 = max(y1(i), y1(j));
            xx2 = min(x2(i), x2(j));
            yy2 = min(y2(i), y2(j));
            w = xx2-xx1+1;
            h = yy2-yy1+1;
            if w > 0 && h > 0
                % compute overlap 
                o = w * h / area(j);
                if o > overlap
                    % add pos to suppress list
                    suppress = [suppress; pos];
                end
            end
        end
        % remove elements in suppress positions
        I(suppress) = [];
    end  
end

% select boxes in pick positions
boxes = boxes(pick,:);