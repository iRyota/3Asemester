classdef sgd_svm < base_learn
    
    properties
        train_num
        lambda
        iter_num
        weights
        learning_rate
        hyper_param
    end
    
    methods

        function obj = sgd_svm(ml_params)
            if isfield(ml_params, 'lambda')
                obj.lambda = ml_params.lambda;
            else
                obj.lambda = 0.1;
            end
            if isfield(ml_params, 'iter_num')
                obj.iter_num = ml_params.iter_num;
            else
                obj.iter_num = 10;
            end
            if isfield(ml_params, 'learning_rate')
                obj.learning_rate = ml_params.learning_rate;
            else
                obj.learning_rate = 10^-3;
            end
        end
        
        function fit(obj, X, Y)
            obj.weights = zeros(size(X,1)+1, 1);
            obj.hyper_param = 10^-7 + zeros(size(X,1)+1, 1);
            

            data_num = numel(Y);
            if obj.lambda < 0
                obj.lambda = 1/data_num;
            end

            for iter = 1:obj.iter_num
                for train_ind = randperm(data_num)

                    x = [X(:,train_ind); 1];
                    y = sign(Y(train_ind)-0.5);
                    prediction = x'*obj.weights;
                    hingeloss = max(0, 1 - y * prediction);

                    if hingeloss == 0
                        obj.hyper_param(1:end-1) = obj.hyper_param(1:end-1) + obj.lambda^2*obj.weights(1:end-1).^2;
                    else
                        obj.hyper_param(1:end-1) = obj.hyper_param(1:end-1) + (obj.lambda*obj.weights(1:end-1)-y*x(1:end-1)).^2;
                        obj.hyper_param(end) = obj.hyper_param(end) + y^2;
                    end
                    learning_rate = obj.learning_rate * sqrt(obj.hyper_param).^-1;

                    % regularization
                    obj.weights(1:end-1) = obj.weights(1:end-1) - obj.lambda * obj.weights(1:end-1).*learning_rate(1:end-1);

                    if hingeloss == 0
                        continue
                    end

                    % update
                    obj.weights = obj.weights + y * x.*learning_rate;

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

