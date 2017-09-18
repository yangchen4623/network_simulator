function alg_result = mydata_import_alg(filename, metric_id)
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
            if line_counter == 1
                best = nan;
                avg = nan;
                worst = nan;
                for i = 1 : 1 : 4
                    alg_result(i) = nan;
                end
                return;
            end
            break;
        end
        line_elements = textscan(tline, '%f','Delimiter',',');
        line_elements_array =  line_elements{1};
        results_array(line_counter) = line_elements_array(metric_id);
        results_array_total_latency(line_counter) = line_elements_array(4);
        tline = fgets(fp);
        line_counter = line_counter + 1;
    end
    
    if line_counter ~= 16
        for i = 1 : 1 : 5
            alg_result(i) = nan;
        end
                            
        fclose(fp);
        return
    else
        for i = 1 : 1 : 5
            alg_result(i) = (results_array(3*i-2) + results_array(3*i-1) + results_array(3*i))/3;
        end
    end
    
    %normalize
    baseline = alg_result(1)
    for i = 1 : 1 : 5
        
        alg_result(i) = alg_result(i)/baseline;
    end
                        
    fclose(fp);
    return
    
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
            if results_array_total_latency(i) ~= 10000
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