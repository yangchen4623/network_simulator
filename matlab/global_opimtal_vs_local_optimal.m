clear;
common_path = 'Z:/Documents/dynamic_router_sim/4x4x4/';
title_list = {'nearest neighbor ', '3-hop nearest neighbor ', 'cube nearest neighbor ', 'bit complement ', 'transpose ', 'tornado ', 'all to all '};
pattern_list = {'3H_NN/','CUBE_NN/','bit_complement/','transpose/','tornado/','all_to_all/'};
Pattern_list = {'3H-NN','CUBE-NN','bit-comp','transpose','tornado','all-to-all'};
Pattern_List = {'3H-NN','CUBE-NN','bit-complement','transpose','tornado'};
metric_list = {'total latency', 'avg latency', 'worst case latency', 'avg rcv thruput', 'avg sent thruput', 'max rcv thruput', 'max sent thruput', 'max used VC NUM'};
pattern_size = [1 2 4 8 16];
packet_size = [1 2 4 8 16];
injection_gap0 = [0 1 3 7 15];
injection_gap1 = 0:1:4;
injection_gap2 = [0 3 7 15];

results = ones(size(pattern_list,2),size(metric_list,2),15);
counter = zeros(size(pattern_list,2),size(metric_list,2));

%looking for global optimal router configuration
for i = 1 : 1 : size(pattern_list,2)
    filelist = dir(strcat(common_path, pattern_list{i}));
    for k = 1 : 1 : size(metric_list,2)
        for j = 3 : 1 : size(filelist,1)%skip . and ..   
            find_heavy_load = strfind(filelist(j).name,'_0.csv');
            if size(find_heavy_load)~=0
            fprintf('%s\n',pattern_list{i});
            fprintf('evaluating %s\n',metric_list{k});
            fprintf('openning file %s\n',filelist(j).name);
            if k == size(metric_list, 2)
                tmp_config_result = mydata_import_config(strcat(common_path, pattern_list{i}, filelist(j).name), k + 4);
                if ~isnan(tmp_config_result(1))
                    results(i,k,:) = squeeze(results(i,k,:))' .* tmp_config_result;
                    counter(i,k) = counter(i,k) + 1;
                end
            else
                tmp_config_result = mydata_import_config(strcat(common_path, pattern_list{i}, filelist(j).name), k + 3);
                if ~isnan(tmp_config_result(1))
                    results(i,k,:) = squeeze(results(i,k,:))' .* tmp_config_result;
                    counter(i,k) = counter(i,k) + 1;
                end
            end
            end
        end
    end
end

for i = 1: 1 : size(pattern_list,2)
    for k = 1 : 1 : size(metric_list,2)
        for j = 1 : 1 : 15
            results(i,k,j) = nthroot(results(i,k,j),counter(i,k));
        end
    end
end


%find the global id
global_best_id = zeros(size(metric_list,2),1);
for k = 1 : 1 : size(metric_list,2)
    for j = 1 : 1 : 15
        global_best(k,j) = geomean(results(:,k,j));
    end
    if k>=4 && k<=7
        [a, global_best_id(k)]=max( global_best(k,:) );
    else
        [a, global_best_id(k)]=min( global_best(k,:) );
    end
end

%find the local best id 
local_best_id = zeros(size(metric_list,2),size(pattern_list,2));
for k = 1 : 1 : size(metric_list,2)
    for j = 1 : 1 : size(pattern_list,2)
        if k>=4 && k<=7
            [a, local_best_id(k,j)]=max( results(j,k,:) );
        else
            [a, local_best_id(k,j)]=min( results(j,k,:) );
        end
    end   
end





figure_id = 1;

cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  'global_vs_local.eps');
    tmp = zeros(size(pattern_list,2),2);
    for ii = 1 : 1 : size(pattern_list,2)
       tmp(ii,1) = 1;
    end
        tmp(:,2) = [1,0.97,0.91,0.87,0.94,0.95];
    hb = bar(tmp);
    ylabel('ratio');
    ylim([0.7 1.2]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(2));
    legend(hb,'global optimal','app-aware optimal');
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Batch Latency');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;

cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  'global_vs_local.eps');
    tmp = zeros(size(pattern_list,2),2);
    for ii = 1 : 1 : size(pattern_list,2)
       tmp(ii,1) = 1;
       
    end
     tmp(:,2) = [0.83,0.78,0.83,0.93,0.67,0.89];
    hb = bar(tmp);
    ylabel('ratio');
    ylim([0.6 1.2]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(2));
    legend(hb,'global optimal','app-aware optimal');
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Area consumption');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;

cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  'global_vs_local.eps');
    tmp = zeros(size(pattern_list,2),2);
    for ii = 1 : 1 : size(pattern_list,2)
       tmp(ii,1) = 1;

    end
    tmp(:,2) = [1.01,1.02,1.33,1.1,1.06,1.12];
    hb = bar(tmp);
    ylabel('ratio');
    ylim([0.8 1.4]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(2));
    legend(hb,'global optimal','app-aware optimal');
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Throughput');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;


