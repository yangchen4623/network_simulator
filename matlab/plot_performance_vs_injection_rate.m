%%this script reads input from
%%\\engnas.bu.edu\users\j\y\jysheng\Documents\dynamic_router_sim\4x4x4 and
%%8x8x8
%%The results are located in subdir with the pattern name
%%The filename is [pattern name]_[pattern_size]_[packet_size]_[injection_gap]
%%pattern size means how many patterns are repeated
%%injection gap means the number of cycles gap between two packets
%% merics to evaluate:
%% max VC number utilization
%% total latency, 
%% average latency, worst-case latency
%% avg send latency, avg rcvd latency, max send latency, max rcvd latency
clear;
pattern_list = {'NN', '3H_NN', 'CUBE_NN', 'bit_complement', 'transpose', 'tornado', 'all_to_all'};
metric_list = {'total latency', 'avg latency', 'worst case latency', 'avg rcv thruput', 'avg sent thruput', 'max rcv thruput', 'max sent thruput', 'max used VC NUM'};
pattern_size = [1 2 4 8 16];
packet_size = [1 2 4 8 16];
injection_gap0 = 0 : 1 : 9;
injection_gap1 = [0 1 3 7 15];

marker_list = {'x', 'o', '-', '^', 'd', '>', '<'};

figure_id = 1;

common_path = 'Z:\Documents\dynamic_router_sim\';
network_size_path = '4x4x4\';


%%plot: x axis is offered injection thruput, y axis is metrics, different
%%line denotes best, avg and worst case for different packet size or data amount
%% plot NN result
offered_injection_ratio = 6 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{1}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{1}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{1}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{1}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{1}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{1}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    %set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(metric_list{i});
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('NN ',metric_list{i},'comparison between best and avg performance of different router configurations'));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

%% plot 3H_NN result
offered_injection_ratio = 4 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{2}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{2}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{2}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{2}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{2}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{2}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    %set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(metric_list{i});
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('3H NN ', metric_list{i},'comparison between best and avg performance of different router configurations'));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

%% plot CUBE_NN result
offered_injection_ratio = 6 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{3}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{3}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{3}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{3}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{3}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{3}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    %set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(metric_list{i});
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('CUBE NN ', metric_list{i},'comparison between best and avg performance of different router configurations'));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

%% plot bit_complement result
offered_injection_ratio = 1 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{4}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{4}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{4}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{4}, '\', filename);
                if i == size(metric_list, 2)
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+4);
                else
                    [best(i,j,k), avg(i,j,k), worst(i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*3 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*3 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{4}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{4}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    %set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(metric_list{i});
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('bitcomplement', metric_list{i},'comparison between best and avg performance of different router configurations'));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end







                
            
        
        
        

