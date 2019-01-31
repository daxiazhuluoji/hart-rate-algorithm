
clear all;
clc;
close all;

%% 取数据
fid = fopen('222.dat');
a=fread(fid,'uint16');
b=a(114500:121800);
fclose(fid);

figure(1);
plot(b);

data_ori=b;
data_len = length(data_ori);

fid = fopen('data.txt','w');
fprintf(fid,'{');
for i=1:data_len
    if(mod(i,8)==1)
        fprintf(fid,'\n');
    end 
    if(i==data_len)
        fprintf(fid,'%d}',data_ori(i));
    else
        fprintf(fid,'%d, ',data_ori(i));
    end
end
fclose(fid);

%%算法部分
wsize=43;
find_fifo=zeros(wsize,1);
peak_index=1;

for i=1:1:length(b)-wsize
    find_fifo=b(i:i+wsize-1);
    [max_v, max_index]=max(find_fifo);    %%找最大值
    if(max_index==(floor(wsize/2)+1))     %%最大值在窗口正中间，就是心率波峰
        peak(peak_index)=i+(floor(wsize/2));  %%记录波峰出现时刻
        peak_index=peak_index+1;
    end
end

pulse=diff(peak); %%相邻波峰出现时刻相减就是心率周期

for i=3:1:length(pulse)            %%中值滤波平滑
    yy=medfilt1(pulse(i-2:i),3);
    pulse_smooth1(i-2)=yy(2);
end

for i=1:1:length(pulse_smooth1)-2    %%平均滤波平滑
    pulse_smooth2(i)=sum(pulse_smooth1(i:i+2))/3;
end

figure(2)
plot(pulse)

figure(3)
plot(pulse_smooth1)

figure(4)
plot(pulse_smooth2)
