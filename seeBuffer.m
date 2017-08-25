% copy the output of teensymonitor into monitorBuffer.txt
% change the format if you need

clear

fid = fopen('monitorBuffer.txt');
dataArray = textscan(fid, '%d', 'Delimiter', '', 'WhiteSpace', '', 'EmptyValue' ,NaN, 'CommentStyle', 'Current Buffer index DMA is writing to:');
% dataArray = textscan(fid, '[%d, %d]', 'Delimiter', '', 'WhiteSpace', '', 'EmptyValue' ,NaN, 'ReturnOnError', false);

data = zeros(length(dataArray{1,1}),length(dataArray));

for i=1:length(dataArray)
    data(:,i) = dataArray{1,i};
end


figure(1)
plot(data)

figure(2)
plot(data(1:min(1500,length(data))), '+-')

figure(3)
clf
hold on
semilogy(abs(fft(double(data)))/length(data))
w = blackman (length(data), "periodic");
w = w/trapz(w);
semilogy(abs(fft(w.*double(data)))/length(data),'g')
hold off

tic
FFT = fft(data);
toc
