function [best, avg, worst] = mydata_import(filename, metric_id)
    fp = fopen(filename);
    if fp == -1
        fprintf('failed to open %s\n',filename);
    end
    tline = fgets(fp);
    tline = fgets(fp);
    tline = fgets(fp);
    %skip first three lines
    line_counter = 1;

    
    while line_counter <= 15
        tline = fgets(fp);
        if tline == -1
            break;
        end
        line_elements = textscan(tline, '%f','Delimiter',',');
        line_elements_array =  line_elements{1};
        results_array(line_counter) = line_elements_array(metric_id);
        tline = fgets(fp);
        line_counter = line_counter + 1;
    end
    
    if metric_id == 4 %total latency
        best = 1000000;
        avg = 0;
        worst = 0;
        counter = 0;
        for i = 1:1:size(results_array, 2)
            if results_array(i) ~= 10000
                if results_array(i) > worst
                    worst = results_array(i);
                end
                if results_array(i) < best
                    best = results_array(i);
                end
                avg = (avg * counter + results_array(i))/(counter + 1);
                counter = counter + 1;
            end
        end  
    elseif metric_id == 5 || metric_id == 6 || metric_id == 12
        best = 1000000;
        avg = 0;
        worst = 0;
        counter = 0;
        for i = 1:1:size(results_array, 2)
            if results_array(i) > worst
                worst = results_array(i);
            end
            if results_array(i) < best
                best = results_array(i);
            end
            avg = (avg * counter + results_array(i))/(counter + 1);
            counter = counter + 1;
        
        end
    else
        best = 0;
        avg = 0;
        worst = 100000;
        counter = 0;
        for i = 1:1:size(results_array, 2)
            if results_array(i) < worst
                worst = results_array(i);
            end
            if results_array(i) > best
                best = results_array(i);
            end
            avg = (avg * counter + results_array(i))/(counter + 1);
            counter = counter + 1;
        
        end
    end
        
     
                
                    
    fclose(fp);
    
        

end