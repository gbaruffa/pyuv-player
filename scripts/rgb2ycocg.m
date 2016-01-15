function ycocg = rgb2ycocg(in)
%RGB2YCOCG Convert RGB values to YCoCg color space.
%   YCOCGMAP = RGB2YCOCG(RGBMAP) converts the RGB values in the
%   colormap RGBMAP to the YCoCb color space. If RGBMAP is M-by-3 and
%   contains the RGB color values as columns, then YCOCGMAP
%   is an M-by-3 matrix that contains the Y, Co, and Cg values
%   equivalent to those colors.
%
%   YCOCG = RGB2YCOCG(RGB) converts the RGB image to the equivalent
%   image YCoCg.
%
%   Class Support
%   -------------
%   If the input is a RGB image, it can be of class uint8, uint16,
%   or double; the output image is of the same class as the input 
%   image.  If the input is a colormap, the input and output 
%   colormaps are both of class double.

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

if ndims(in)~=3 | size(in,3)~=3
   if ndims(in)==2 & size(in,2)==3 % a colormap
      iscolormap=1;
      colors = size(in,1);
      in = reshape(in, [colors 1 3]);
   else
      error('Invalid RGB input image');
   end
else
   iscolormap=0;
end

classin = class(in);
switch classin
case 'uint8'
   in = double(in);
case 'double'
   in = in*255;
case 'uint16'
   in = double(in)/257;
otherwise
   error('Unsupported input class');
end

% These equations transform RGB in [0, 1] to YCoCg in [0,1].

% a =
% 
%    1/4  1/2  1/4
%    1/2   0  -1/2
%   -1/4  1/2 -1/4
%
% ai = inv(a)

ai = [
   0.25   0.5   0.25;
   0.5    0.0  -0.5;
  -0.25   0.5  -0.25
];

ycocg(:,:,1) = ai(1, 1) * in(:,:,1) + ai(1,2) * in(:,:,2) + ai(1,3) * in(:,:,3);
ycocg(:,:,2) = ai(2, 1) * in(:,:,1) + ai(2,2) * in(:,:,2) + ai(2,3) * in(:,:,3);
ycocg(:,:,3) = ai(3, 1) * in(:,:,1) + ai(3,2) * in(:,:,2) + ai(3,3) * in(:,:,3);

switch classin
case 'uint8'
   ycocg = uint8(ycocg);
case 'double'
   ycocg = ycocg/255;
case 'uint16'
   ycocg = uint16(ycocg .* 257);
otherwise
   error('Unsupported output class');
end

if iscolormap
   ycocg = reshape(ycocg, [colors 3 1]);
end

% if isa(ycocg,'double')
%     ycocg = min(max(ycocg,0.0),1.0);
% end

