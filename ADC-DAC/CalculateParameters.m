clc
clear


%% DAC delay (D)
f0 = 25e3;
fn = 2.5;
f4dec = 5;
p4period = 20;

nFrequencies = round((log10(f0)-log10(fn))*f4dec+1);
frequencyVector = logspace(log10(f0),log10(fn), nFrequencies);
DAC_Delay = round(1./(p4period*frequencyVector)*1e6);
DAC_Delay = 2*round(DAC_Delay/2);

%% ADC frequency (F)
preMultiplier = 50;
minOverSampling = 8;
ADC_Frequency = minOverSampling*preMultiplier*round(frequencyVector/preMultiplier);

minADC_Frequency = 1000; % the PDB cannot go below a certain frequency

for i = 1:nFrequencies
    ADC_Frequency(i) = max(ADC_Frequency(i), minADC_Frequency);
end

%% good parameters
goodParamerter = cell(3,1);
% high frequency
goodParamerter{1}.type = 'high';
goodParamerter{1}.minFreq = 5e3;
goodParamerter{1}.A = 1;
goodParamerter{1}.R = 8;
goodParamerter{1}.N = 1000;
goodParamerter{1}.minNPeriod = 1500;
% medium frequency
goodParamerter{2}.type = 'medium';
goodParamerter{2}.minFreq = 500;
goodParamerter{2}.A = 4;
goodParamerter{2}.R = 16;
goodParamerter{2}.N = 1000;
goodParamerter{2}.minNPeriod = 1500;
% low frequency
goodParamerter{3}.type = 'low';
goodParamerter{3}.minFreq = 1e-9;
goodParamerter{3}.A = 32;
goodParamerter{3}.R = 16;
goodParamerter{3}.N = 1000;
goodParamerter{3}.minNPeriod = 2*6;



%% make string
Parameters = cell(nFrequencies,1);
fftIndex = cell(nFrequencies,1);

expectedTime = zeros();

string = 'A%d R%d N%d F%.3f D%d T0 S0';
for i = 1:nFrequencies
    l = 1;
    while ~(frequencyVector(i) >= goodParamerter{l}.minFreq)
        l = l+1;
    end
    
    tAquisition = max(goodParamerter{l}.N / ADC_Frequency(i), goodParamerter{l}.minNPeriod / frequencyVector(i));
    nSamples = round(tAquisition*ADC_Frequency(i));
    
    % number_of_samples / ADC_frequency / (DAC_dealy(us) * points_per_period)
    fftIndex{i,1} = round((nSamples / ADC_Frequency(i)) / (DAC_Delay(i)*1e-6 * p4period));
    
    s = sprintf(string, goodParamerter{l}.A, goodParamerter{l}.R, nSamples, ADC_Frequency(i), DAC_Delay(i));
    Parameters{i,1} = s;  
    
    fprintf(['''', s, '''', ',', '\n'])
    
    expectedTime(i,1) = nSamples/ADC_Frequency(i);
    
end

totalTime = sum(expectedTime);

fprintf('\n')
for i = 1:nFrequencies
    fprintf('%d, ', fftIndex{i,1})
    if (i/10 == round(i/10))
        fprintf('\n')
    end
end
fprintf('\n')
fprintf('\n')

for i = 1:nFrequencies
        fprintf('%d, ', frequencyVector(i))
    if (i/10 == round(i/10))
        fprintf('\n')
    end
end
fprintf('\n')
fprintf('\n')
    
fprintf('%d min %d sec\n', floor(totalTime./60),  round(mod(totalTime,60)))

for i = 1:nFrequencies
    fprintf('Expected time: %.3f s\t', expectedTime(i)); 
    fprintf('for f: %.3f\n', frequencyVector(i));
end



    
    
