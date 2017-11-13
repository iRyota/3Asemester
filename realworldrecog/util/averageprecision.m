function ap = averageprecision(obj_class, image_feature, machine_learning)
% AVERAGEPRECISION calculate average precision from saved prediction and ground truth
% obj_class: class name to be recognized
% image_feature: handle for methods in feature_extraction/*.m
% machine_learning: handle for class in machine_learning/*.m

ap = 0;

% load ground truth
[imgID x1 y1 x2 y2] = textread( ['datasets/test_data/ground_truth_' obj_class '.txt'], '%d %d %d %d %d\n' );

% compute average precision for detection
if sum(x2+y1+y2) ~= 0

    numpos = length(imgID);

    % load detection results
    loaded = load(['results/boxes_' obj_class]);
    if isempty(loaded.results); return; end
    scores = loaded.results(:,6);

    % evaluate detection results
    labels = zeros(size(loaded.results,1),1);
    for i=1:max(imgID)
        % detection results
        inds_b = find(loaded.results(:,1)==i);
        if isempty(inds_b); continue; end
        boxes_ = loaded.results(inds_b, 2:5);
        % ground truth
        inds = find(imgID==i);
        for j=inds'
            x1_ = x1(j);
            y1_ = y1(j);
            x2_ = x2(j);
            y2_ = y2(j);
            % compute overlap
            area = (x2_-x1_+1) * (y2_-y1_+1);
            xx1 = max(boxes_(:,1), x1_);
            yy1 = max(boxes_(:,2), y1_);
            xx2 = min(boxes_(:,3), x2_);
            yy2 = min(boxes_(:,4), y2_);
            w = max(0,xx2-xx1+1);
            h = max(0,yy2-yy1+1);
            overlap =    w .* h / area;
            res = zeros(length(inds_b),1);
            inds = find(overlap > 0.5);
            if ~isempty(inds)
                if length(inds)==1
                    res(inds) = 1;
                else
                    [~, idx] = max(scores(inds_b(inds)),[],1);
                    res(inds(idx)) = 1;
                end
            end
            labels(inds_b) = max(labels(inds_b), res);
        end
    end

    graph_path = ['results/detect_graph_' obj_class];

% compute average precision for classification
else

    labels = x1;
    numpos = length(find(labels==1));

    % scores
    loaded = load(['results/classes_' obj_class]);
    if isempty(loaded.results); return; end
    scores = loaded.results;

    graph_path = ['results/classify_graph_' obj_class];

end

% graph settings
figure;
set(0, 'DefaultAxesLineWidth', 3);
set(0, 'DefaultAxesFontSize', 30);
set(0, 'DefaultTextFontSize', 40);

[~, inds] = sort(-scores);
labels = labels(inds);
labels = cumsum(labels); % RUISEKI
recall = labels / numpos;
precision = labels ./ (1:length(labels))';

plot(recall, precision, 'r', 'linewidth', 3); hold on;

% compute average precision
for t=0:0.1:1
    p=max(precision(recall>=t));
    if isempty(p)
        p=0;
    end
    ap=ap+p/11;
end

fprintf('Average Precision: %f\n',ap);

xlabel('Recall')
ylabel('Precision')
title(['AP = ' num2str(ap)])
saveas(gcf,[graph_path '_' char(image_feature) '_' char(machine_learning)],'png');

end