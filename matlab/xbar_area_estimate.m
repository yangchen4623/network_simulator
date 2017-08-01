function area = xbar_area_estimate(vc_num)
three_to_one_base = 330;
two_to_one_base = 170;
rest = 8000;


%VC_num = 9 2x3x3x3
%VC_num = 7&8 2x2x2x2x3
%VC_num = 5&6 2x2x3x3
%VC_num = 4 2x2x2x3
%VC_num = 3 2x3x3
%VC_num = 2 2x2x3
%VC_num = 1 2x3


if vc_num == 1 
    area = 6*(three_to_one_base * 2 + two_to_one_base) + rest;
elseif vc_num == 2
    area = 6*(4*three_to_one_base+2*two_to_one_base + two_to_one_base) + rest;
elseif vc_num == 3
    area = 6*(6*three_to_one_base+2*three_to_one_base + two_to_one_base) + rest;
elseif vc_num == 4
    area = 6*(8*three_to_one_base+4*two_to_one_base + 2*two_to_one_base+ two_to_one_base) + rest;
elseif (vc_num == 5) | (vc_num == 6)
    area = 6*(12*three_to_one_base+4*three_to_one_base + 2*two_to_one_base+ two_to_one_base) + rest;
elseif (vc_num == 7) | (vc_num == 8)
    area = 6*(16*three_to_one_base+8*two_to_one_base + 4* two_to_one_base + 2*two_to_one_base+ two_to_one_base) + rest;
else
    area = 6*(18*three_to_one_base + 6 * three_to_one_base + 2 * three_to_one_base + two_to_one_base) + rest;
end


    

