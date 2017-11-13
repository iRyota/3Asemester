classdef (Abstract) base_learn < handle
    %BASE_LEARN  Basic class for machine learning methods for Real World Recognition
    
    properties
    end

    methods(Abstract)
        % learn classfier using features X and class labels Y
        fit(obj, X, Y)
        % predict classes of test_X using scores for positive / negative class
        pred_Y = predict(obj, test_X)
        % obtain filter from weight vector and bias
        [filter, bias] = get_filter(obj)
    end
    
end

