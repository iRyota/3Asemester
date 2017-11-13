classdef knn_classification < base_learn
    
    properties
        k
        train_num
        X
        Y
    end
    
    methods

        function obj = knn_classification(ml_params)
            if isfield(ml_params, 'k')
                obj.k = ml_params.k;
            else
                obj.k = 5;
            end
        end
        
        function fit(obj, X, Y)
            obj.X = X;
            obj.train_num = numel(Y);
            obj.Y = [1-Y; Y];
        end

        function pred_Y = predict(obj, test_X)
            test_num = size(test_X, 2);
            [~, nearest_X] = sort(bsxfun(@minus, sum(obj.X.*obj.X)', 2*obj.X'*test_X));
            affinity = zeros(obj.train_num, test_num);
            affinity(bsxfun(@plus, nearest_X(1:obj.k,:), (0:test_num-1)*obj.train_num)) = 1;
            pred_Y = obj.Y(2,:) * affinity - obj.Y(1,:) * affinity;
        end

        function [filter, bias] = get_filter(obj)
            filter = [];
            bias = [];
        end
        
    end
    
end

