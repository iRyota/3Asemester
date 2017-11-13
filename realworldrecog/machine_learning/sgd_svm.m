classdef sgd_svm < base_learn
    
    properties
        train_num
        lambda
        iter_num
        weights
        learning_rate
    end
    
    methods

        function obj = sgd_svm(ml_params)
            if isfield(ml_params, 'lambda')
                obj.lambda = ml_params.lambda;
            else
                obj.lambda = -1;
            end
            if isfield(ml_params, 'iter_num')
                obj.iter_num = ml_params.iter_num;
            else
                obj.iter_num = 10;
            end
            if isfield(ml_params, 'learning_rate')
                obj.learning_rate = ml_params.learning_rate;
            else
                obj.learning_rate = 0.00052;
            end
        end
        
        function fit(obj, X, Y)
            obj.weights = zeros(size(X,1)+1, 1);

            data_num = numel(Y);
            if obj.lambda < 0
                obj.lambda = 1/data_num;
            end

            for iter = 1:obj.iter_num

                for train_ind = randperm(data_num)

                    x = [X(:,train_ind); 1];
                    y = sign(Y(train_ind)-0.5);
                    prediction = obj.weights'*x;
                    hingeloss = max(0, 1-y*prediction);

                    % regularization
                    obj.weights(1:end-1) = (1 - obj.learning_rate * obj.lambda) * obj.weights(1:end-1);

                    if hingeloss == 0
                        continue
                    end

                    % update
                    obj.weights = obj.weights + obj.learning_rate * y * x;

                end
                
            end

        end

        function pred_Y = predict(obj, X)
            pred_Y = obj.weights'*[X;ones(1,size(X,2))];
        end

        function [filter, bias] = get_filter(obj)
            filter = obj.weights(1:end-1);
            bias = obj.weights(end);
        end
        
    end
    
end

