% Find the C code for a color transform
clear all;

% Transformation matrix, real numbers

% % RGB -> YPbPr (ITU-R BT.601)
% I = [0 0 0;
%      1 1 1];
% Obits = 8;
% Oq = [ 16  16  16;
%       235 240 240];
% A = [+0.299    +0.587    +0.114;
%      -0.168736 -0.331264 +0.5;
%      +0.5      -0.418688 -0.081312];
 
% % RGB -> YPbPr (ITU-R BT.709)
% I = [0 0 0;
%      1 1 1];
% Obits = 8;
% Oq = [ 16  16  16;
%       235 240 240];
% A = [+0.2125 +0.7154 +0.0721;
%      -0.115  -0.385  +0.5;
%      +0.5    -0.454  -0.046];
 
% % RGB -> YCoCg (H.264 reversible transform)
% I = [0 0 0;
%      1 1 1];
% Obits = 8;
% Oq = [  0   0   0;
%       255 255 255];
% A = [+0.25 +0.50 +0.25;
%      +0.50 +eps  -0.50;
%      -0.25 +0.5  -0.25];
 
% RGB -> YCxCz (Stereoscopic digital cinema)
I = [0 0 0; 1 1 1]; % real numbers input range: top row is minimum values for each comp, bottom row is maximum values
O = [0 -0.5 -0.5; 1 0.5 0.5]; % real numbers output range (empty is auto): top row is minimum values for each comp, bottom row is maximum values
Obits = 8; % expected number of bits for the output values
Oq = [0 0 0; 255 255 255]; % output precision integer values range: top row is minimum values for each comp, bottom row is maximum values
A = [0.23447500416257   0.52652640688218   0.19768418642745;
    0.10827354260078  -0.13669337549857  -0.02047781544626;
    -0.12210863287956  -0.23047402039993   0.42438048133343]; % color transform coefficients
 
% Conversion coefficients precision
coeffbits = 9;

%%%%%%%%%%%%%%%%%%%%%%%%%%
% Do not modify below!!! %
%%%%%%%%%%%%%%%%%%%%%%%%%%

% Output limits
As   = sign(A);
Omax = I(1 + (As + 1) / 2)';
Omin = I(1 + (1 - As) / 2)';
if isempty(O),
    O = [diag(A*Omin) diag(A*Omax)]'
else,
    O
end;

% Offsets
Oc = ((Oq(2, :) - Oq(1, :)) ./ (O(2, :) - O(1, :)))
Oo = Oq(1, :) - O(1, :) .* Oc


% Find quantized coeffs.
Aq = (repmat(Oc, 3, 1) .* (A .* (2^(coeffbits - 1))) / (2^(Obits) - 1));

% Prepare C code
if (Obits <= 8),
    tipo = 'unsigned char';
elseif (Obits <= 16),
    tipo = 'unsigned short';
else,
    tipo = 'unsigned int';
end;
disp('****** DIRECT ******');
for oo = 1:size(A, 1),

    fprintf(2, 'compo = %3.0f + (', Oo(oo));
    for ii = 1:size(A, 2),
        
        if (round(Aq(oo, ii)) ~= 0),
            fprintf(2, '%+4.0f * x%d ', Aq(oo, ii), ii);
        end;
        
    end;
    fprintf(2, ') >> %d;', (coeffbits - 1));
    disp(' ');
    fprintf(2, 'y%d = (%s) ((compo < 0) ? 0 : ((compo > %d) ? %d : compo))', oo, tipo, 2^Obits - 1, 2^Obits - 1);
    disp(' ' );
    
end;
disp(' ' );

% A inverse
Ai = inv(A)
Aiq = (Ai ./ repmat(Oc, 3, 1)) .* (2^(Obits) - 1) .* (2^(coeffbits - 1))

% We go reverse
disp('****** REVERSE ******');
for oo = 1:size(A, 1),
    fprintf(2, 'yy%d = (int) y%d - %3.0f;', oo, oo, Oo(oo));
    disp(' ');
end;
for oo = 1:size(A, 1),

    fprintf(2, 'compo = (', Oo(oo));
    for ii = 1:size(A, 2),
        
        if (round(Aiq(oo, ii)) ~= 0),
            fprintf(2, '%+4.0f * yy%d ', Aiq(oo, ii), ii);
        end;
        
    end;
    fprintf(2, ') >> %d;', (coeffbits - 1));
    disp(' ');
    fprintf(2, 'x%d = (%s) ((compo < 0) ? 0 : ((compo > %d) ? %d : compo))', oo, tipo, 2^Obits - 1, 2^Obits - 1);
    disp(' ' );
    
end;
disp(' ' );
