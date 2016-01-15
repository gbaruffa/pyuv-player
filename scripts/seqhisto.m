% Copyright (C) 2005 - 2014 Giuseppe Baruffa
% 
% This file is part of PYUV.
% 
% PYUV is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
% 
% PYUV is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
% 
% You should have received a copy of the GNU General Public License
% along with PYUV.  If not, see <http://www.gnu.org/licenses/>.

% This is used to find the histogram of a raw sequence
clear all;

% We begin only with 3 comps, YUV 4:2:0 sources
% filename = 'short_352x288_25Hz_P420.yuv';
% width = 352;
% height = 288;
% numframes = 5;
filename = 'foreman.cif';
width = 352;
height = 288;
numframes = Inf;

% Prepare some info on source file
aaa = dir(filename);
filelength = aaa.bytes;
clear aaa;
area = width * height;
framesize = area * 1.5;
numframes = min(floor(filelength / framesize), numframes);

% Open source file
fid = fopen(filename, 'rb');

% Load frames
disp(' ');
acchistY = zeros(1, 256);
acchistU = zeros(1, 256);
acchistV = zeros(1, 256);
for ff = 1:numframes,
    disp(['** Frame #' int2str(ff - 1) ' **']);
    
    % Read a 420 planar frame
    frame = fread(fid, framesize, 'uchar');
    frameY = frame(1:area);
    frameU = frame((area + 1):(area + area / 4));
    frameV = frame((area + area / 4 + 1):end);
    
    % Do stats
    X = [0:255];
    histY = hist(frameY, X);
    histU = hist(frameU, X);
    histV = hist(frameV, X);
    
    % Accum stats
    acchistY = acchistY + histY;
    acchistU = acchistU + histU;
    acchistV = acchistV + histV;
    
    % Draw plots
    figure(1);
    clf;
    subplot(311);
    bar(X, histY ./ (area));
    title(['Single #' int2str(ff - 1)]);
    subplot(312);
    bar(X, histU ./ (area / 4));
    subplot(313);
    bar(X, histV ./ (area / 4));
    
    figure(2);
    clf;
    subplot(311);
    bar(X, acchistY ./ (ff * area));
    title('Accumulated');
    subplot(312);
    bar(X, acchistU ./ (ff * area / 4));
    subplot(313);
    bar(X, acchistV ./ (ff * area / 4));
    drawnow;
    
end;

% Normalize stats
% acchistY = acchistY ./ (numframes * area);
% acchistU = acchistU ./ (numframes * area / 4);
% acchistV = acchistV ./ (numframes * area / 4);


% Close all files
fclose('all');