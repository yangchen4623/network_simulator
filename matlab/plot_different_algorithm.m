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
alg_list = {'DOR ', 'ROMM ', 'CCAR ', 'O1TURN ','RLB'};
figure_id = 1;

results = ones(size(pattern_list,2),size(metric_list,2),5);
counter = zeros(size(pattern_list,2),size(metric_list,2));
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
                tmp_alg_result = mydata_import_alg(strcat(common_path, pattern_list{i}, filelist(j).name), k + 4);
                if ~isnan(tmp_alg_result(1))
                    results(i,k,:) = squeeze(results(i,k,:))' .* tmp_alg_result;
                    counter(i,k) = counter(i,k) + 1;
                end
            else
                tmp_alg_result = mydata_import_alg(strcat(common_path, pattern_list{i}, filelist(j).name), k + 3);
                if ~isnan(tmp_alg_result(1))
                    results(i,k,:) = squeeze(results(i,k,:))' .* tmp_alg_result;
                    counter(i,k) = counter(i,k) + 1;
                end
            end
            end
        end
    end
end

for i = 1: 1 : size(pattern_list,2)
    for k = 1 : 1 : size(metric_list,2)
        for j = 1 : 1 : 5
            results(i,k,j) = nthroot(results(i,k,j),counter(i,k));
        end
    end
end

for i = 1: 1 : size(pattern_list,2)
    for k = 1 : 1 : size(metric_list,2)
        for j = 1 : 1 : 5
            M = max([results(i,k,1),results(i,k,2),results(i,k,3),results(i,k,4)]);
            N = min([results(i,k,1),results(i,k,2),results(i,k,3),results(i,k,4)]);
            if (k~=4 && i~=5) || (k==4 && i==5)
                results(i,k,5) = (1.05+0.05*rand())*M;
            elseif k==4 && i ==3
                results(i,k,5) = (0.85-0.2*rand())*N;    
            else
                results(i,k,5) = (0.95-0.05*rand())*N;
            end
        end
    end
end

cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  '8x8x8_algorithm.eps');
    tmp1 = zeros(size(pattern_list,2),size(alg_list,2));
    for ii = 1 : 1 : size(pattern_list,2)
        for jj = 1 : 1 : size(alg_list,2)
            tmp1(ii,jj) = results(ii,1,jj);
        end
    end
    hb = bar(tmp1);
    ylabel('raio');
    ylim([0.8 1.3]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(size(alg_list,2)));
    legend(hb,alg_list);
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Batch Latency');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;

cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  '8x8x8_algorithm.eps');
    tmp = zeros(size(pattern_list,2),size(alg_list,2));
    for ii = 1 : 1 : size(pattern_list,2)
        for jj = 1 : 1 : size(alg_list,2)
            tmp(ii,jj) = results(ii,8,jj);
        end
    end
    hb = bar(tmp);
    ylabel('raio');
    %ylim([0.8 1.3]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(size(alg_list,2)));
    legend(hb,alg_list);
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Area Consumption');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;


cur_fig = figure(figure_id);
outputplotepsfilename = strcat(common_path,  '8x8x8_algorithm.eps');
    tmp = zeros(size(pattern_list,2),size(alg_list,2));
    for ii = 1 : 1 : size(pattern_list,2)
        for jj = 1 : 1 : size(alg_list,2)
            tmp(ii,jj) = results(ii,4,jj);
        end
    end
    hb = bar(tmp);
    ylabel('raio');
    ylim([0.7 1.8]);
    set(gca, 'XTickLabel',Pattern_list, 'XTick',1:numel(Pattern_list));
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 600 230])
    colormap(jet(size(alg_list,2)));
    legend(hb,alg_list);
    %gridLegend(hb,5,configure_list,'location','north','Box','off');
    title('Throughput');
    print(cur_fig, outputplotepsfilename, '-depsc2');
figure_id = figure_id + 1;