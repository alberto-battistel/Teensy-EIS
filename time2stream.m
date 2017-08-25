
clear
nData = [275, 900, 527, 800, 1023, 4549, 3416, 14898, 5098, 8644];
time2Stream = [19996, 74001, 42000, 65000, 84000, 390000, 292000, 1287000, 441000, 751002];

% fitting
P = polyfit (nData, time2Stream, 1)
x = [min(nData),max(nData)];
y = P(1)*x + P(2);

% plot
figure(1)
plot(nData, time2Stream, '+', x, y, 'r-')
xlabel('number of data to stream')
ylabel('time required to stream / us')
legend('data', sprintf('Fitting: %.0f us per data point', P(1)))