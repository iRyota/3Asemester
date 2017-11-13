classdef random_guess < base_learn
    
    properties
        feat_dim
    end

    methods

        % construct instance doing nothing
        function obj = random_guess(ml_params)
        end

        % learn just feature dimension        
        function fit(obj, X, Y)
            obj.feat_dim = size(X,1);
        end

        % predict class label with random scores
        function pred_Y = predict(obj, test_X)
            pred_Y = rand(1, size(test_X,2));
        end

        % return filter and bias with random values
        function [filter, bias] = get_filter(obj)
            filter = rand(obj.feat_dim,1);
            bias = rand(1);
        end

    end
    
end

