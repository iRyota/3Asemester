function mlc = train_detector(obj_class, train_data, labels, ml_class, ml_params)
% TRAIN_DETECTOR traines ml_class
% obj_class: class name to be recognized
% train_data: matrix having image features in each column
% labels: binary vector having one if each image includes obj_class and zero otherwise
% ml_class: handle for class in machine_learning/*.m
% ml_params: structure having parameters for ml_class

% initialize
fprintf(['training ' char(ml_class) '...\n']);
mlc = ml_class(ml_params);

% train parameters
mlc.fit(train_data, labels)

% save parameters
save(['models/' obj_class], 'mlc');

end