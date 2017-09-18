for i = 1 : 1 : 9
    area(i) = xbar_area_estimate(i);
end
cur_fig = figure(10000);
set(gcf, 'PaperPositionMode','auto');
set(cur_fig, 'Position', [0 0 400 300]);
VC = 1 : 1 : 9;
plot(VC,area, 'LineWidth', 2);
grid on
xlabel('VC num');
ylabel('area size (ALMs)');
title('router area vs VC num');