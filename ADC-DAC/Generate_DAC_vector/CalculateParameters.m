clc
clear


%% DAC delay (D)
f0 = 25e3;
fn = 25;
f4dec = 4;
p4period = 20;

nFrequencies = (log10(f0)-log10(fn))*f4dec+1;
frequencyVector = round(logspace(log10(f0),log10(fn), nFrequencies));
DAC_Delay = round(1./(p4period*frequencyVector)*1e6);
DAC_Delay = 2*round(DAC_Delay/2);

%% ADC frequency (F)
ADC_Frequency = 100*round(8*frequencyVector/100);

%% good parameters
goodParamerter = cell(3,1);
% high frequency
goodParamerter{1}.type = 'high';
goodParamerter{1}.minFreq = 10e3;
goodParamerter{1}.A = 1;
goodParamerter{1}.R = 8;
goodParamerter{1}.N = 4000;
% medium frequency
goodParamerter{2}.type = 'medium';
goodParamerter{2}.minFreq = 1e3;
goodParamerter{2}.A = 1;
goodParamerter{2}.R = 8;
goodParamerter{2}.N = 4000;
% low frequency
goodParamerter{3}.type = 'medium';
goodParamerter{3}.minFreq = 1e-6;
goodParamerter{3}.A = 1;
goodParamerter{3}.R = 8;
goodParamerter{3}.N = 4000;


%% make string
Parameters = cell(nFrequencies,1);
fftIndex = cell(nFrequencies,1);

expectedTime = 0;

string = 'A%d R%d N%d F%.3f D%d T0 S0';
for i = 1:nFrequencies
    l = 1;
    while ~(frequencyVector(i) >= goodParamerter{l}.minFreq)
        l = l+1;
    end
    
    fftIndex{i,1} = round((goodParamerter{l}.N / ADC_Frequency(i)) / (DAC_Delay(i)*1e-6 * p4period));
    
    s = sprintf(string, goodParamerter{l}.A, goodParamerter{l}.R, goodParamerter{l}.N, ADC_Frequency(i), DAC_Delay(i));
    Parameters{i,1} = s;  
    
    fprintf(['''', s, '''', ',', '\n'])
    
    expectedTime = expectedTime + goodParamerter{l}.N/ADC_Frequency(i);
    
end

fprintf('\n')
fprintf('%d, ', fftIndex{:})
fprintf('\n')

fprintf('%d min %d sec\n', floor(expectedTime./60),  round(mod(expectedTime,60)))
%%


    
    
