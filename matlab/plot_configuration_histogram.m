%plot the histogram of different patterns
clear;
common_path = 'Z:/Documents/dynamic_router_sim/4x4x4/';
pattern_list = {'3H_NN/','CUBE_NN/','bit_complement/','transpose/','tornado/','all_to_all/'};
title_list = {'3-hop diagonal nearest neighbor','Cube neareast neighbor','bit complement','transpose','tornado','all to all'};
metric_list = {'total latency', 'avg latency', 'worst case latency', 'avg rcv thruput', 'avg sent thruput', 'max rcv thruput', 'max sent thruput', 'max used VC NUM'};
metric_label_list = {'total latency', 'avg latency', 'worst case latency', 'thruput', 'max nonidle VC NUM'};

routing_algorithm_list = {'DOR','ROMM','RCA','O1TURN','RLB'};
SA_list = {'FF','OF','MIX'};
hist = zeros(size(pattern_list,2),size(metric_list,2),size(routing_algorithm_list,2)*size(SA_list,2));

for i = 1 : 1 : size(routing_algorithm_list,2)
    for j =1 : 1 : size(SA_list,2)
        configure_list{(i-1)*size(SA_list,2)+j} = strcat(routing_algorithm_list{i},'-',SA_list{j});
    end
end

figure_id = 1;

for i = 1 : 1 : size(pattern_list,2)
   
    filelist = dir(strcat(common_path, pattern_list{i}));
    for k = 1 : 1 : size(metric_list,2)
        
        for j = 3 : 1 : size(filelist,1)%skip . and ..   
            fprintf('%s\n',pattern_list{i});
            fprintf('evaluating %s\n',metric_list{k});
            fprintf('openning file %s\n',filelist(j).name);
            if k == size(metric_list, 2)
                [cur_best_id, cur_worst_id] = mydata_import_id(strcat(common_path, pattern_list{i}, filelist(j).name), k + 4);
            else
                [cur_best_id, cur_worst_id] = mydata_import_id(strcat(common_path, pattern_list{i}, filelist(j).name), k + 3);
            end
            if cur_best_id ~= -1
                hist(i,k,cur_best_id) = hist(i,k,cur_best_id) + 1;
            end
        end
        total = 0;
        for m = 1: 1: size(routing_algorithm_list,2)*size(SA_list,2)
            total = total + hist(i,k,m);
        end
        for m = 1: 1: size(routing_algorithm_list,2)*size(SA_list,2)
            hist(i,k,m) = hist(i,k,m) / total * 100;
        end
        
        
        
%        cur_fig = figure(figure_id);
%        set(gcf, 'PaperPositionMode','auto');
%        set(cur_fig, 'Position', [0 0 800 600])
        %set(cur_fig, 'visible','off');
     %   bar(categorical(configure_list),squeez(hist(i,k,:)));
%        bar(1:1:15,squeeze(hist(i,k,:)));
%        title(strcat(pattern_list{i},' ', metric_list{k}));
%       figure_id = figure_id + 1;
    end
    cur_fig = figure(figure_id);
    outputplotepsfilename = strcat(common_path, title_list{i}, '_histogram.eps');
    tmp = zeros(size(metric_label_list,2),size(routing_algorithm_list,2)*size(SA_list,2));
    for ii = 1 : 1 : size(metric_label_list,2)
        for jj = 1 : 1 : size(routing_algorithm_list,2)*size(SA_list,2)
            if ii <= 4
                tmp(ii,jj) = hist(i,ii,jj);
            else
                tmp(ii,jj) = hist(i,ii + 3,jj);
            end
        end
    end
    hb = bar(tmp);
    ylabel('percent');
    xlabel('metrics');
    set(gca, 'XTickLabel',metric_label_list, 'XTick',1:numel(metric_label_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 1200 500])
    colormap(parula(size(routing_algorithm_list,2)*size(SA_list,2)));
    %legend(hb,configure_list);
    gridLegend(hb,5,configure_list,'location','north','Box','off');
    title(strcat(title_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    figure_id = figure_id + 1;
end

