%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% This script is used to convert Planar YUV 4:2:0 material to other formats %%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;

% Original 420 planar sequence 111111
filename420 = '../sequences/short_352x288_25Hz_P420.yuv';
width420 = 352;
height420 = 288;

% Destination 444 planar sequence
filename444 = '../sequences/short_352x288_25Hz_P444.yuv';

% Destination 422 planar sequence
filename422 = '../sequences/short_352x288_25Hz_P422.yuv';

% Destination 411 planar sequence
filename411 = '../sequences/short_352x288_25Hz_P411.yuv';

% Destination 410 planar sequence
filename410 = '../sequences/short_352x288_25Hz_P410.yuv';

% Destination 444 interleaved sequence
filename444i = '../sequences/short_352x288_25Hz_I444.yuv';

% Destination 422 interleaved sequence
filename422i = '../sequences/short_352x288_25Hz_I422.yuv';

% Destination 411 interleaved sequence
filename411i = '../sequences/short_352x288_25Hz_I411.yuv';

% Destination 420 interleaved sequence
filename420i = '../sequences/short_352x288_25Hz_I420.yuv';

% Destination 420 UYVY interleaved sequence
filename420iuyvy = '../sequences/short_352x288_25Hz_UYVY.yuv';

% Destination 410 interleaved sequence
filename410i = '../sequences/short_352x288_25Hz_I410.yuv';

% Destination 420 few-bits sequence
filename420lb = '../sequences/short_352x288_25Hz_P420_5b.yuv';

% Destination 420 many-bits sequence
filename420hb = '../sequences/short_352x288_25Hz_P420_10b.yuv';

% Destination 420 16-bits sequence
filename42016b = '../sequences/short_352x288_25Hz_P420_16b.yuv';

% Destination 420 few-bits signed sequence
filename420lbs = '../sequences/short_352x288_25Hz_P420_-5b.yuv';

% Destination 420 many-bits signed sequence
filename420hbs = '../sequences/short_352x288_25Hz_P420_-10b.yuv';

% Destination 444 RGB sequence
filename444rgb = '../sequences/short_352x288_25Hz_P444.rgb';

% Destination 444 XYZ sequence
filename444xyz = '../sequences/short_352x288_25Hz_P444.xyz';

% Destination 444 HSV sequence
filename444hsv = '../sequences/short_352x288_25Hz_P444.hsv';

% Destination 444 YIQ sequence
filename444yiq = '../sequences/short_352x288_25Hz_P444.yiq';

% Destination 400 BW sequence
filename444bw = '../sequences/short_352x288_25Hz_P400.bw';

% Destination 444 YCoCg sequence
filename444ycocg = '../sequences/short_352x288_25Hz_P444.ycocg';

% Destination anamorphic RGB sequence
filename1781 = '../sequences/short_198x288_25Hz_P444_anamorphic.rgb';

% Destination interlaced RGB sequence
filename444i = '../sequences/short_352x288_50Hz_P444i.rgb';

% Destination odd sized RGB sequence
filename444odd = '../sequences/short_351x287_50Hz_P444.rgb';

% Destination 444 YCxCz sequence
filename444ycxcz = '../sequences/short_352x288_25Hz_P444.ycxcz';

% Prepare some info on source file
aaa = dir(filename420);
length420 = aaa.bytes;
clear aaa;
area420 = width420 * height420;
framesize420 = area420 * 1.5;
frames420 = length420 / framesize420;

% Prepare destination file
frame444 = zeros(height420, width420, 3);

% Open source file
fid420 = fopen(filename420, 'rb');

% Open destination files
% fid444 = fopen(filename444, 'wb');
% fid422 = fopen(filename422, 'wb');
% fid411 = fopen(filename411, 'wb');
% fid410 = fopen(filename410, 'wb');
% fid444i = fopen(filename444i, 'wb');
% fid422i = fopen(filename422i, 'wb');
% fid411i = fopen(filename411i, 'wb');
% fid420i = fopen(filename420i, 'wb');
% fid420iuyvy = fopen(filename420iuyvy, 'wb');
% fid410i = fopen(filename410i, 'wb');
% fid420lb = fopen(filename420lb, 'wb');
% fid420hb = fopen(filename420hb, 'wb');
% fid42016b = fopen(filename42016b, 'wb');
% fid420lbs = fopen(filename420lbs, 'wb');
% fid420hbs = fopen(filename420hbs, 'wb');
% fid444rgb = fopen(filename444rgb, 'wb');
% fid444xyz = fopen(filename444xyz, 'wb');
% fid444hsv = fopen(filename444hsv, 'wb');
% fid444yiq = fopen(filename444yiq, 'wb');
% fid444bw = fopen(filename444bw, 'wb');
% fid444ycocg = fopen(filename444ycocg, 'wb');
% fid1781 = fopen(filename1781, 'wb');
% fid444i = fopen(filename444i, 'wb');
% fid444odd = fopen(filename444odd, 'wb');
fid444ycxcz = fopen(filename444ycxcz, 'wb');

% Cycle over all source frames
for framenum = 1:frames420,
    
    % Read a 420 planar frame
    frame420 = fread(fid420, framesize420, 'uchar') ./ 255;
    
        % Load 444 planar Y
        frame444(:, :, 1) = reshape(frame420(1:area420), width420, height420)';
        
        % Load 444 planar U
        frame444(:, :, 2) = imresize(reshape(frame420((area420 + 1):(area420 * 1.25)), width420/2, height420/2)', ...
                                      [height420 width420], 'bicubic');
        %%frame444(1:2:end, 1:2:end, 2) = reshape(frame420((area420 + 1):(area420 * 1.25)), width420/2, height420/2)';
        %%frame444(2:2:end, 1:2:end, 2) = frame444(1:2:end, 1:2:end, 2);
        %%frame444(1:2:end, 2:2:end, 2) = frame444(1:2:end, 1:2:end, 2);
        %%frame444(2:2:end, 2:2:end, 2) = frame444(1:2:end, 1:2:end, 2);
        
        % Load 444 planar V
        frame444(:, :, 3) = imresize(reshape(frame420((area420 * 1.25 + 1):end), width420/2, height420/2)', ...
                                      [height420 width420], 'bicubic');
        %%frame444(1:2:end, 1:2:end, 3) = reshape(frame420((area420 * 1.25 + 1):end), width420/2, height420/2)';
        %%frame444(2:2:end, 1:2:end, 3) = frame444(1:2:end, 1:2:end, 3);
        %%frame444(1:2:end, 2:2:end, 3) = frame444(1:2:end, 1:2:end, 3);
        %%frame444(2:2:end, 2:2:end, 3) = frame444(1:2:end, 1:2:end, 3);
    
    % Dump a 444 planar frame
    
%         % Dump 444 planar Y
%         fwrite(fid444, round(frame444(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar U
%         fwrite(fid444, round(frame444(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar V
%         fwrite(fid444, round(frame444(:, :, 3)' .* 255), 'uchar');
            
    % Dump a 422 planar frame
    
%         % Dump 422 planar Y
%         fwrite(fid422, round(frame444(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 422 planar U
%         fwrite(fid422, round(frame444(:, 1:2:end, 2)' .* 255), 'uchar');
%             
%         % Dump 422 planar V
%         fwrite(fid422, round(frame444(:, 1:2:end, 3)' .* 255), 'uchar');
            
    % Dump a 411 planar frame
    
%         % Dump 411 planar Y
%         fwrite(fid411, round(frame444(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 411 planar U
%         fwrite(fid411, round(frame444(:, 1:4:end, 2)' .* 255), 'uchar');
%             
%         % Dump 411 planar V
%         fwrite(fid411, round(frame444(:, 1:4:end, 3)' .* 255), 'uchar');
            
    % Dump a 410 planar frame
    
%         % Dump 410 planar Y
%         fwrite(fid410, round(frame444(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 410 planar U
%         fwrite(fid410, round(frame444(1:2:end, 1:4:end, 2)' .* 255), 'uchar');
% 
%         % Dump 410 planar V
%         fwrite(fid410, round(frame444(1:2:end, 1:4:end, 3)' .* 255), 'uchar');
            
    % Dump a 444 interleaved frame
%     for (row = 1:height420),
%         for (col = 1:width420),
%             for (comp = 1:3),
%                 fwrite(fid444i, round(frame444(row, col, comp) .* 255), 'uchar');
%             end;
%         end;
%     end;
    
    % Dump a 422 interleaved frame
%     for (row = 1:height420),
%         for (col = 1:2:width420),
%             fwrite(fid422i, round(frame444(row, col, 1) .* 255), 'uchar');
%             fwrite(fid422i, round(frame444(row, col, 2) .* 255), 'uchar');
%             fwrite(fid422i, round(frame444(row, col + 1, 1) .* 255), 'uchar');
%             fwrite(fid422i, round(frame444(row, col + 1, 3) .* 255), 'uchar');
%         end;
%     end;
    
    % Dump a 411 interleaved frame
%     for (row = 1:height420),
%         for (col = 1:4:width420),
%             fwrite(fid411i, round(frame444(row, col, 1) .* 255), 'uchar');
%             fwrite(fid411i, round(frame444(row, col, 2) .* 255), 'uchar');
%             fwrite(fid411i, round(frame444(row, col + 1, 1) .* 255), 'uchar');
%             fwrite(fid411i, round(frame444(row, col + 2, 1) .* 255), 'uchar');
%             fwrite(fid411i, round(frame444(row, col + 2, 3) .* 255), 'uchar');
%             fwrite(fid411i, round(frame444(row, col + 3, 1) .* 255), 'uchar');
%         end;
%     end;

    % Dump a 420 interleaved frame
%     for (row = 1:2:height420),
%         for (col = 1:2:width420),
%             fwrite(fid420i, round(frame444(row, col, 1) .* 255), 'uchar');
%             fwrite(fid420i, round(frame444(row, col, 2) .* 255), 'uchar');
%             fwrite(fid420i, round(frame444(row, col + 1, 1) .* 255), 'uchar');
%         end;
%         for (col = 1:2:width420),
%             fwrite(fid420i, round(frame444(row + 1, col, 1) .* 255), 'uchar');
%             fwrite(fid420i, round(frame444(row + 1, col, 3) .* 255), 'uchar');
%             fwrite(fid420i, round(frame444(row + 1, col + 1, 1) .* 255), 'uchar');
%         end;
%     end;

%     % Dump a 420 UYVY interleaved frame
%     for (row = 1:height420),
%         for (col = 1:2:width420),
%             fwrite(fid420iuyvy, round(frame444(row, col, 2) .* 255), 'uchar');
%             fwrite(fid420iuyvy, round(frame444(row, col, 1) .* 255), 'uchar');
%             fwrite(fid420iuyvy, round(frame444(row, col, 3) .* 255), 'uchar');
%             fwrite(fid420iuyvy, round(frame444(row, col + 1, 1) .* 255), 'uchar');
%         end;
%     end;

    % Dump a 410 interleaved frame
%     for (row = 1:2:height420),
%         for (col = 1:4:width420),
%             fwrite(fid410i, round(frame444(row, col, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row, col, 2) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row, col + 1, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row, col + 2, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row, col + 3, 1) .* 255), 'uchar');
%         end;
%         for (col = 1:4:width420),
%             fwrite(fid410i, round(frame444(row + 1, col, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row + 1, col, 3) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row + 1, col + 1, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row + 1, col + 2, 1) .* 255), 'uchar');
%             fwrite(fid410i, round(frame444(row + 1, col + 3, 1) .* 255), 'uchar');
%         end;
%     end;

%     % Dump a 420 few bit planar frame
%      numbits = 5;
%    
%         % Dump 420 planar Y
%         fwrite(fid420lb, round(frame444(:, :, 1)' .* (pow2(numbits) - 1)), 'uchar');
%             
%         % Dump 420 planar U
%         fwrite(fid420lb, round(frame444(1:2:end, 1:2:end, 2)' .* (pow2(numbits) - 1)), 'uchar');
%             
%         % Dump 420 planar V
%         fwrite(fid420lb, round(frame444(1:2:end, 1:2:end, 3)' .* (pow2(numbits) - 1)), 'uchar');
%     
%     % Dump a 420 many bit planar frame
%     numbits = 10;
%     
%         % Dump 420 planar Y
%         fwrite(fid420hb, round(frame444(:, :, 1)' .* (pow2(numbits) - 1)), 'uint16');
%             
%         % Dump 420 planar U
%         fwrite(fid420hb, round(frame444(1:2:end, 1:2:end, 2)' .* (pow2(numbits) - 1)), 'uint16');
%             
%         % Dump 420 planar V
%         fwrite(fid420hb, round(frame444(1:2:end, 1:2:end, 3)' .* (pow2(numbits) - 1)), 'uint16');
%     
%     % Dump a 420 many bit planar frame
%     numbits = 16;
%     
%         % Dump 420 planar Y
%         fwrite(fid42016b, round(frame444(:, :, 1)' .* (pow2(numbits) - 1)), 'uint16');
%             
%         % Dump 420 planar U
%         fwrite(fid42016b, round(frame444(1:2:end, 1:2:end, 2)' .* (pow2(numbits) - 1)), 'uint16');
%             
%         % Dump 420 planar V
%         fwrite(fid42016b, round(frame444(1:2:end, 1:2:end, 3)' .* (pow2(numbits) - 1)), 'uint16');
%    
%     % Dump a 420 few bit planar signed frame
%     numbits = 5;
%     
%         % Dump 420 planar Y
%         fwrite(fid420lbs, round(frame444(:, :, 1)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'schar');
%             
%         % Dump 420 planar U
%         fwrite(fid420lbs, round(frame444(1:2:end, 1:2:end, 2)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'schar');
%             
%         % Dump 420 planar V
%         fwrite(fid420lbs, round(frame444(1:2:end, 1:2:end, 3)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'schar');
%     
%     % Dump a 420 many bit planar signed frame
%     numbits = 10;
%     
%         % Dump 420 planar Y
%         fwrite(fid420hbs, round(frame444(:, :, 1)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'int16');
%             
%         % Dump 420 planar U
%         fwrite(fid420hbs, round(frame444(1:2:end, 1:2:end, 2)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'int16');
%             
%         % Dump 420 planar V
%         fwrite(fid420hbs, round(frame444(1:2:end, 1:2:end, 3)' .* (pow2(numbits) - 1)) - pow2(numbits - 1), 'int16');
    
    % Convert YUV to RGB
    framergb = ycbcr2rgb(frame444);
    
    % Dump a 444 planar RGB frame
    
%         % Dump 444 planar R
%         fwrite(fid444rgb, round(framergb(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar G
%         fwrite(fid444rgb, round(framergb(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar B
%         fwrite(fid444rgb, round(framergb(:, :, 3)' .* 255), 'uchar');

    % Convert RGB to XYZ
%     framexyz = rgb2xyz(framergb);
    
    % Dump a 444 planar XYZ frame
    
%         % Dump 444 planar X
%         fwrite(fid444xyz, round(framexyz(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar Y
%         fwrite(fid444xyz, round(framexyz(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar Z
%         fwrite(fid444xyz, round(framexyz(:, :, 3)' .* 255), 'uchar');

%     % Convert RGB to YCoCg
%     frameycocg = rgb2ycocg(framergb);
%     
%     % Dump a 444 planar YCoCg frame
%     
%         % Dump 444 planar Y
%         fwrite(fid444ycocg, round(frameycocg(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar Co
%         fwrite(fid444ycocg, 128 + round(frameycocg(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar Cg
%         fwrite(fid444ycocg, 128 + round(frameycocg(:, :, 3)' .* 255), 'uchar');

    % Convert RGB to HSV
%     framehsv = rgb2hsv(framergb);
    
    % Dump a 444 planar HSV frame
    
%         % Dump 444 planar H
%         fwrite(fid444hsv, round(framehsv(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar S
%         fwrite(fid444hsv, round(framehsv(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar V
%         fwrite(fid444hsv, round(framehsv(:, :, 3)' .* 255), 'uchar');

    % Convert RGB to YIQ
%     frameyiq = rgb2ntsc(framergb);
    
    % Dump a 444 planar YIQ frame
    
%         % Dump 444 planar Y
%         fwrite(fid444yiq, round(frameyiq(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar I
%         fwrite(fid444yiq, round(frameyiq(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar Q
%         fwrite(fid444yiq, round(frameyiq(:, :, 3)' .* 255), 'uchar');

    % Convert RGB to BW
    %framebw = (framergb(:, :, 1) + framergb(:, :, 2) + framergb(:, :, 3)) / 3;
%     framebw = frame444(:, :, 1);
    
    % Dump a 400 planar BW frame
    
        % Dump 400 planar BW
%         fwrite(fid444bw, round(framebw' .* 255), 'uchar');
            
    % Convert 4:3 to 16:9 anamorphic
%       rows1781 = size(framergb, 1);
%       cols1781 = size(framergb, 2);
%       frame1781 = imresize(framergb, [rows1781 cols1781*9/16], 'bilinear');
%       frame1781(frame1781 < 0.0) = 0.0;
%       frame1781(frame1781 > 1.0) = 1.0;
    
    % Dump a 444 anamorphic frame
    
%         % Dump 444 planar R
%         fwrite(fid1781, round(frame1781(:, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar G
%         fwrite(fid1781, round(frame1781(:, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar B
%         fwrite(fid1781, round(frame1781(:, :, 3)' .* 255), 'uchar');

    % Dump a 444 interlaced frame
    
%         % Create bottom field, in case
%         if (~exist('frameold')),
%             frameold = zeros(size(framergb));
%         end;
% 
%         % Dump 444 planar R, top field
%         fwrite(fid444i, round(framergb(1:2:end, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar G, top field
%         fwrite(fid444i, round(framergb(1:2:end, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar B, top field
%         fwrite(fid444i, round(framergb(1:2:end, :, 3)' .* 255), 'uchar');
% 
%         % Dump 444 planar R, bottom field
%         fwrite(fid444i, round(frameold(2:2:end, :, 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar G, bottom field
%         fwrite(fid444i, round(frameold(2:2:end, :, 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar B, bottom field
%         fwrite(fid444i, round(frameold(2:2:end, :, 3)' .* 255), 'uchar');
%         
%         % Save current frame
%         frameold = framergb;

    % Dump a 444 planar odd RGB frame
    
%         % Dump 444 planar R
%         fwrite(fid444odd, round(framergb(1:(end - 1), 1:(end - 1), 1)' .* 255), 'uchar');
%             
%         % Dump 444 planar G
%         fwrite(fid444odd, round(framergb(1:(end - 1), 1:(end - 1), 2)' .* 255), 'uchar');
%             
%         % Dump 444 planar B
%         fwrite(fid444odd, round(framergb(1:(end - 1), 1:(end - 1), 3)' .* 255), 'uchar');

    % Convert RGB to YCxCz
    frameycxcz = rgb2ycxcz(framergb);
    
    % Dump a 444 planar YCxCz frame
    
        % Dump 444 planar Y
        fwrite(fid444ycxcz, round(frameycxcz(:, :, 1)' .* 255) + 0, 'uchar');
            
        % Dump 444 planar Cx
        fwrite(fid444ycxcz, 128 + round(frameycxcz(:, :, 2)' .* 255), 'uchar');
            
        % Dump 444 planar Cz
        fwrite(fid444ycxcz, 128 + round(frameycxcz(:, :, 3)' .* 255), 'uchar');

    % Display what has been read
    figure(1);
    image(framergb);
    axis image;
    axis off;
    pause(0.04);
    
end;

% Close all files
fclose('all');