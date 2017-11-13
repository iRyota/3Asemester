classdef nearest_class_mean < base_learn
    
    properties
        weight
        bias
    end
    
    methods

        function obj = nearest_class_mean(ml_params)
        end
        
        function fit(obj, X, Y)
            train_num = numel(Y);
            sparse_Y = [1-Y; Y];
            class_means = bsxfun(@rdivide, X * sparse_Y', sum(sparse_Y, 2)');
            obj.weight = class_means(:,2) - class_means(:,1);
            obj.bias = (sum(class_means(:,1).^2) - sum(class_means(:,2).^2)) / 2;
        end
        
        function pred_Y = predict(obj, test_X)
            pred_Y = obj.weight'*test_X + obj.bias;
        end

        function [filter, bias] = get_filter(obj)
            filter = obj.weight;
            bias = obj.bias;
        end

    end
    
end

