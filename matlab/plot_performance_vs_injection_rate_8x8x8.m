%%this script reads input from
%%\\engnas.bu.edu\users\j\y\jysheng\Documents\dynamic_router_sim\8x8x8 and
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
title_list = {'nearest neighbor ', '3-hop nearest neighbor ', 'cube nearest neighbor ', 'bit complement ', 'transpose ', 'tornado ', 'all to all '};
metric_list = {'total latency', 'avg latency', 'worst case latency', 'avg rcv thruput', 'avg sent thruput', 'max rcv thruput', 'max sent thruput', 'max used VC NUM'};
unit_list = {'cycles','cycles','cycles','flits/node/cycle','flits/node/cycle','flits/node/cycle','flits/node/cycle',' '};
pattern_size = [1 2 4 8 16];
packet_size = [1 2 4 8 16];
injection_gap0 = [0 1 3 7 15];
injection_gap1 = 0:1:4;
injection_gap2 = [0 3 7 15];
base_injection_rate = [6, 4, 6, 1, 1, 1, 6];

marker_list = {'x', 'o', 'V', '^', 'd', '>', '<'};

figure_id = 1;

common_path = 'Z:\Documents\dynamic_router_sim\';
network_size_path = '8x8x8\';


%%plot: x axis is offered injection thruput, y axis is metrics, different
%%line denotes best, avg and worst case for different packet size or data amount
%% plot NN result
offered_injection_ratio = 6 * (1 ./ (1+injection_gap1));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap1, 2):-1:1
                filename = strcat(pattern_list{1}, '_', num2str(pattern_size(j)), '_4_', num2str(pattern_size(j)*injection_gap1(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{1}, '\', filename);
                if i == size(metric_list, 2)
                    [best(1,i,j,k), avg(1,i,j,k), worst(1,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(1,i,j,k), avg(1,i,j,k), worst(1,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap1, 2):-1:1
                filename = strcat(pattern_list{1}, '_16_', num2str(packet_size(j)), '_', num2str(16*injection_gap1(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{1}, '\', filename);
                if i == size(metric_list, 2)
                    [best(1,i,j,k), avg(1,i,j,k), worst(1,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(1,i,j,k), avg(1,i,j,k), worst(1,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{1}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{1}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(1,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(1,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
%        plot(fliplr(offered_injection_ratio'), squeeze(worst(1,i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('NN ',metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

%compute improvement best vs avg
speedup_NN_mat = best(1,:,:,:) ./ avg(1,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_NN(i) = geomean(geomean(speedup_NN_mat(1,i,:,:)));
end
for i = 1 : 1 : size(best(1,8,:,:),3)
    for j = 1 : 1 : size(best(1,8,:,:),4)
        area_speedup_NN(i,j) = xbar_area_estimate(round(best(1,8,i,j))) / xbar_area_estimate(round(avg(1,8,i,j)));
    end
end
speedup_NN(8) = geomean(geomean(area_speedup_NN));

fprintf('NN speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_NN(1),speedup_NN(2),speedup_NN(3),speedup_NN(4),speedup_NN(5),speedup_NN(6),speedup_NN(7),speedup_NN(8));
fprintf('NN done\n');

%% plot 3H_NN result
offered_injection_ratio = 4 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{2}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{2}, '\', filename);
                if i == size(metric_list, 2)
                    [best(2,i,j,k), avg(2,i,j,k), worst(2,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(2,i,j,k), avg(2,i,j,k), worst(2,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{2}, '_8_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{2}, '\', filename);
                if i == size(metric_list, 2)
                    [best(2,i,j,k), avg(2,i,j,k), worst(2,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(2,i,j,k), avg(2,i,j,k), worst(2,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{2}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{2}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(2,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(2,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
 %       plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('3H NN ', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end
%compute improvement best vs avg
speedup_3H_NN_mat = best(2,:,:,:) ./ avg(2,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_3H_NN(i) =abs(1 - geomean(geomean(speedup_3H_NN_mat(1,i,:,:))));
end
for i = 1 : 1 : size(best(2,8,:,:),3)
    for j = 1 : 1 : size(best(2,8,:,:),4)
        area_speedup_3H_NN(i,j) = xbar_area_estimate(round(best(2,8,i,j))) / xbar_area_estimate(round(avg(2,8,i,j)));
    end
end
speedup_3H_NN(8) = 1 - geomean(geomean(area_speedup_3H_NN));

fprintf('3H_NN speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_3H_NN(1),speedup_3H_NN(2),speedup_3H_NN(3),speedup_3H_NN(4),speedup_3H_NN(5),speedup_3H_NN(6),speedup_3H_NN(7),speedup_3H_NN(8));


fprintf('3H NN done\n');

%% plot CUBE_NN result
offered_injection_ratio = 6 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{3}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{3}, '\', filename);
                if i == size(metric_list, 2)
                    [best(3,i,j,k), avg(3,i,j,k), worst(3,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(3,i,j,k), avg(3,i,j,k), worst(3,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{3}, '_4_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{3}, '\', filename);
                if i == size(metric_list, 2)
                    [best(3,i,j,k), avg(3,i,j,k), worst(3,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(3,i,j,k), avg(3,i,j,k), worst(3,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{3}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{3}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(3,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(3,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
 %       plot(fliplr(offered_injection_ratio'), squeeze(worst(i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('CUBE NN ', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

speedup_CUBE_NN_mat = best(3,:,:,:) ./ avg(3,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_CUBE_NN(i) =abs(1 - geomean(geomean(speedup_CUBE_NN_mat(1,i,:,:))));
end

for i = 1 : 1 : size(best(3,8,:,:),3)
    for j = 1 : 1 : size(best(3,8,:,:),4)
        area_speedup_CUBE_NN(i,j) = xbar_area_estimate(round(best(3,8,i,j))) / xbar_area_estimate(round(avg(3,8,i,j)));
    end
end
speedup_CUBE_NN(8) = 1 - geomean(geomean(area_speedup_CUBE_NN));

fprintf('CUBE_NN speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_CUBE_NN(1),speedup_CUBE_NN(2),speedup_CUBE_NN(3),speedup_CUBE_NN(4),speedup_CUBE_NN(5),speedup_CUBE_NN(6),speedup_CUBE_NN(7),speedup_CUBE_NN(8));

fprintf('CUBE done\n');

%% plot bit_complement result
offered_injection_ratio = 1 * (1 ./ (1+injection_gap1));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap1, 2):-1:1
                filename = strcat(pattern_list{4}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap1(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{4}, '\', filename);
                if i == size(metric_list, 2)
                    [best(4,i,j,k), avg(4,i,j,k), worst(4,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(4,i,j,k), avg(4,i,j,k), worst(4,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap1, 2):-1:1
                filename = strcat(pattern_list{4}, '_8_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap1(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{4}, '\', filename);
                if i == size(metric_list, 2)
                    [best(4,i,j,k), avg(4,i,j,k), worst(4,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(4,i,j,k), avg(4,i,j,k), worst(4,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{4}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{4}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(4,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(4,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
%        plot(fliplr(offered_injection_ratio'), squeeze(worst(4,i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('bitcomplement', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end
speedup_bc_mat = best(4,:,:,:) ./ avg(4,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_bc(i) =abs(1 - geomean(geomean(speedup_bc_mat(1,i,:,:))));
end

for i = 1 : 1 : size(best(4,8,:,:),3)
    for j = 1 : 1 : size(best(4,8,:,:),4)
        area_speedup_bc(i,j) = xbar_area_estimate(round(best(4,8,i,j))) / xbar_area_estimate(round(avg(4,8,i,j)));
    end
end
speedup_bc(8) = 1 - geomean(geomean(area_speedup_bc));
fprintf('bc speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_bc(1),speedup_bc(2),speedup_bc(3),speedup_bc(4),speedup_bc(5),speedup_bc(6),speedup_bc(7),speedup_bc(8));


fprintf('bit_complement done\n');

%% plot transpose result
offered_injection_ratio = 1 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{5}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{5}, '\', filename);
                if i == size(metric_list, 2)
                    [best(5,i,j,k), avg(5,i,j,k), worst(5,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(5,i,j,k), avg(5,i,j,k), worst(5,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{5}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{5}, '\', filename);
                if i == size(metric_list, 2)
                    [best(5,i,j,k), avg(5,i,j,k), worst(5,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(5,i,j,k), avg(5,i,j,k), worst(5,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{5}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{5}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(5,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(5,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
%        plot(fliplr(offered_injection_ratio'), squeeze(worst(5,i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('transpose ', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end


speedup_transpose_mat = best(5,:,:,:) ./ avg(5,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_transpose(i) =abs(1 - geomean(geomean(speedup_transpose_mat(1,i,:,:))));
end

for i = 1 : 1 : size(best(5,8,:,:),3)
    for j = 1 : 1 : size(best(5,8,:,:),4)
        area_speedup_transpose(i,j) = xbar_area_estimate(round(best(5,8,i,j))) / xbar_area_estimate(round(avg(5,8,i,j)));
    end
end
speedup_transpose(8) = 1 - geomean(geomean(area_speedup_transpose));
fprintf('transpose speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_transpose(1),speedup_transpose(2),speedup_transpose(3),speedup_transpose(4),speedup_transpose(5),speedup_transpose(6),speedup_transpose(7),speedup_transpose(8));

fprintf('transpose done\n');




%% plot tornado result
offered_injection_ratio = 1 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{6}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{6}, '\', filename);
                if i == size(metric_list, 2)
                    [best(6,i,j,k), avg(6,i,j,k), worst(6,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(6,i,j,k), avg(6,i,j,k), worst(6,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{6}, '_16_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{6}, '\', filename);
                if i == size(metric_list, 2)
                    [best(6,i,j,k), avg(6,i,j,k), worst(6,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(6,i,j,k), avg(6,i,j,k), worst(6,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{6}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{6}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2);
    else
        plot_set_num = size(packet_size, 2);
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(6,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(6,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
%        plot(fliplr(offered_injection_ratio'), squeeze(worst(6,i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('tornado ', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end



speedup_tornado_mat = best(6,:,:,:) ./ avg(6,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    speedup_tornado(i) =abs(1 - geomean(geomean(speedup_tornado_mat(1,i,:,:))));
end
for i = 1 : 1 : size(best(6,8,:,:),3)
    for j = 1 : 1 : size(best(6,8,:,:),4)
        area_speedup_tornado(i,j) = xbar_area_estimate(round(best(6,8,i,j))) / xbar_area_estimate(round(avg(6,8,i,j)));
    end
end
speedup_tornado(8) = 1 - geomean(geomean(area_speedup_tornado));
fprintf('tornado speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_tornado(1),speedup_tornado(2),speedup_tornado(3),speedup_tornado(4),speedup_tornado(5),speedup_tornado(6),speedup_tornado(7),speedup_tornado(8));


fprintf('tornado done\n');

%% plot all_to_all result

offered_injection_ratio = 6 * (1 ./ (1+injection_gap0));
for i = 1:1:size(metric_list, 2)
    if i == 1 %if i == 1 is total latency, the different line means different amout of data been sent
        for j = 1:1:size(pattern_size, 2) - 1 %%use the data of packet size 4 as the data
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{7}, '_', num2str(pattern_size(j)), '_4_', num2str(4*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{7}, '\', filename);
                if i == size(metric_list, 2)
                    [best(7,i,j,k), avg(7,i,j,k), worst(7,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(7,i,j,k), avg(7,i,j,k), worst(7,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j)), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j)), 'kb avg');
%            legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j)), 'kb worst');
        end
    else
        %for other case, the different lines mean different packet size
        for j = 1:1:size(packet_size, 2)
            for k = size(injection_gap0, 2):-1:1
                filename = strcat(pattern_list{7}, '_8_', num2str(packet_size(j)), '_', num2str(packet_size(j)*injection_gap0(k)), '.csv');
                path = strcat(common_path, network_size_path, pattern_list{7}, '\', filename);
                if i == size(metric_list, 2)
                    [best(7,i,j,k), avg(7,i,j,k), worst(7,i,j,k)] = mydata_import(path, i+4);
                else
                    [best(7,i,j,k), avg(7,i,j,k), worst(7,i,j,k)] = mydata_import(path, i+3);
                end
            end
            legend_list{(j-1)*2 + 1} = strcat(num2str(pattern_size(j) / 4.0), 'kb best');
            legend_list{(j-1)*2 + 2} = strcat(num2str(pattern_size(j) / 4.0), 'kb avg');
 %           legend_list{(j-1)*3 + 3} = strcat(num2str(pattern_size(j) / 4.0), 'kb worst');
        end        
    end
    outputplotepsfilename = strcat(common_path, network_size_path, pattern_list{7}, '_', metric_list{i}, 'performance_vs_injection_rate.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, pattern_list{7}, '_', metric_list{i}, 'performance_vs_injection_rate.png');
    cur_fig = figure(figure_id);
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 800 600])
    set(cur_fig, 'visible','off');
    
    if i == 1
        plot_set_num = size(pattern_size, 2) - 1;
    else
        plot_set_num = size(packet_size, 2) - 1;
    end
    for j = 1:1:plot_set_num 
        plot(fliplr(offered_injection_ratio'), squeeze(best(7,i,j,:)), strcat('b-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
        if j == 1
            xlabel('offered injection rate (flits/node/cycle)');
            ylabel(strcat(metric_list{i},'flits/node/cycle'));
            grid on;
            hold on;
        end
        plot(fliplr(offered_injection_ratio'), squeeze(avg(7,i,j,:)), strcat('r-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
%        plot(fliplr(offered_injection_ratio'), squeeze(worst(7,i,j,:)), strcat('k-',marker_list{j}), 'LineWidth', 2, 'MarkerSize', 12);
    end
    
    legend(legend_list);
    title(strcat('All to all ', metric_list{i}));
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    figure_id = figure_id + 1;
end

speedup_ata_mat = best(7,:,:,:) ./ avg(7,:,:,:);
for i = 1 : 1 : size(metric_list, 2) - 1
    for j = 1 : 1 : size(speedup_ata_mat,3)
        for k = 1 : 1 : size(speedup_ata_mat, 4)
            if isnan(speedup_ata_mat(1,i,j,k))
                speedup_ata_mat(1,i,j,k) = 1;
            end
        end
    end
    speedup_ata(i) =abs(1 - geomean(geomean(speedup_ata_mat(1,i,:,:))));
end

for i = 1 : 1 : size(best(7,8,:,:),3)
    for j = 1 : 1 : size(best(7,8,:,:),4)
        if (best(7,8,i,j) ~= 0) & (avg(7,8,i,j) ~= 0)
            area_speedup_ata(i,j) = xbar_area_estimate(round(best(7,8,i,j))) / xbar_area_estimate(round(avg(7,8,i,j)));
        else
            area_speedup_ata(i,j) = 1;
        end
    end
end
speedup_ata(8) = 1 - geomean(geomean(area_speedup_ata));
fprintf('all_to_all speedup\n');
fprintf('total latency\t avg latency\t worst case latency\t avg rcv thruput\t avg sent thruput\t max rcv thruput\t max sent thruput\t max used VC NUM\n');
fprintf('%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n',speedup_ata(1),speedup_ata(2),speedup_ata(3),speedup_ata(4),speedup_ata(5),speedup_ata(6),speedup_ata(7),speedup_ata(8));



fprintf('all_to_all done\n');

%%now subplot all the figures in the same metric except NN
for i = 1 : 1 : size(metric_list, 2) %ignore NN (NN is not interesting in our router)
    outputplotepsfilename = strcat(common_path, network_size_path, metric_list{i}, '.eps');
    outputplotpngfilename = strcat(common_path, network_size_path, metric_list{i}, '.png');
    cur_fig = figure(figure_id);
    %tight_subplot(2,3,[.01 .03],[.1 .01],[.01 .01]) 
    set(gcf, 'PaperPositionMode','auto');
    set(cur_fig, 'Position', [0 0 2000 600])
    if i == 1
        legend_list = {'1kb best', '1kb avg', '2kb best', '2kb avg', '4kb best', '4kb avg', '8kb best', '8kb avg', '16kb best', '16kb avg'};
    else
        legend_list = {'0.25kb best', '0.25kb avg', '0.5kb best', '0.5kb avg', '1kb best', '1kb avg', '2kb best', '2kb avg' ,'4kb best', '4kb avg'};
    end
    for j = 1 : 1 : 4
        if j == 6
            legend_list = legend_list(1:8);
        end
        subplot(1, 4 ,j);
        offered_injection_ratio = base_injection_rate(j) * (1 ./ (1+injection_gap0));
        if i == 1
            plot_set_num = size(pattern_size, 2) - (j == 6);
        else
            plot_set_num = size(packet_size, 2) - (j == 6);
        end
        for k = 1:1:plot_set_num 
            plot(fliplr(offered_injection_ratio'), squeeze(best(j+1,i,k,:)), strcat('b-',marker_list{k}), 'LineWidth', 2, 'MarkerSize', 12);
            if k == 1
                xlabel('offered injection rate (flits/node/cycle)');               
                ylabel(strcat(metric_list{i},'(',unit_list{i},')'));
                grid on;
                hold on;
            end
            plot(fliplr(offered_injection_ratio'), squeeze(avg(j+1,i,k,:)), strcat('r-',marker_list{k}), 'LineWidth', 2, 'MarkerSize', 12);
        end
        legend(legend_list,'Location','best');
        title(strcat(title_list{j+1},'--- ', metric_list{i}));
    end
    print(cur_fig, outputplotepsfilename, '-depsc2');
    print(cur_fig, outputplotpngfilename, '-dpng');
    
    figure_id = figure_id + 1;
end










                
            
        
        
        

