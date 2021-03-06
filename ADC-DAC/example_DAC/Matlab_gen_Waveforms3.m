%%
% This script creates a file.h containing the stream of data for the DAC
% and the corresponding varaibles to call in the real .ino file.
% 
% It works both for Octave and Matlab
% in the .ino file add "#include "filename.h""
%%

%% Parameters to set
filename = 'Waveforms_Gen.h';		% name of the header file to create
baseFreq = 1;     	% base frequeny of the arbitrary waveform
L = 1e4;            % length of the multisine (1e4 and 1e5 take 2% and 20% of available memory on T3.6)
nbit = 12;			% resolution of the DAC
nperiods = [10];  	% in the waveform, it is a vector with [1,4] you have two signals with 1 and 4 periods inside

stepTime = ((baseFreq*L)^-1)*1e6;     % in us interrupt delay time (Delay)
maxDig = 2^nbit-1;    % resolution
f = baseFreq*nperiods;  % in Hz

%% Checks
% step time
if stepTime <= 2
	% I could not get good results with too fast output
    error('stepTime is shorter than 2 us')
else
    fprintf('stepTime is %.3f us\n', stepTime )
end

% Calculate frequencies
if max(nperiods) >= L/2 % aliasing
    error('nperiods is too high\n')
else
    for i = 1:length(f)
        fprintf('Frequency\t%d:\t%5.3f Hz\n', i, f(i))
    end
end

% Cutoff frequency
fprintf('Cutoff filter frequency: ~%d Hz\n', round(stepTime^-1*1e6))

% Length of the multisine (500'000 points take almost all the memory)
fprintf('Data length: %d\n', L)

% Filename
fprintf('Filename:\n\t%s\n',filename)

%% Make waveform
Multisine = zeros(1,L);   % arbitrary units
for i = 1:length(nperiods)
	% multisine with random phase
    Multisine = Multisine + sin(2*pi*nperiods(i)*(0:L-1)/L + pi*rand(1));
end
% Convert to integer decimals with the right resolution
Dec = round(maxDig*( Multisine /(2*max(Multisine))+0.5));

%% Plot results
figure(1)
clf
subplot(2,1,1)
plot(Dec)
subplot(2,1,2)
semilogy(abs(fft(Dec))/L,'+-')
xlim([0,2*max(nperiods)])

%% Convert to hexadecimal
Hex = dec2hex(Dec);

%% Make file
fprintf('Writing file\n')
% open file
id = fopen(filename, 'w');

% first lines
firstLines = { ...
    '#ifndef _Waveforms_h_', ...    % do not repeat the #define, common use
    '#define _Waveforms_h_', ...    % common use
    sprintf('#define Delay %f // in us', stepTime) ...  % macros Delay (interrupt delay)
    sprintf('#define maxSamplesNum %d // length', L) ...% macros maxSamplesNum (length of the waveform)
    'static short waveformsTable[maxSamplesNum] = {' ...% varaible waveformsTable (waveform declaration)
    };

% last lines
lastLines = {'};', '#endif'};   

% write file
% first lines
fprintf(id, '%s\n',firstLines{:});

% data
fprintf(id, '\t0x%s,\t', Hex(1,:));
for i = 2:L-1
    fprintf(id, '0x%s,\t', Hex(i,:));
    if mod(i,15) == 0   % new line
        fprintf(id,'\n\t');
    end
end
fprintf(id, '0x%s\t', Hex(end,:));
% last lines
fprintf(id, '%s\n',lastLines{:});

% close file
fclose(id);

%% done
fprintf('\nDone\n')
