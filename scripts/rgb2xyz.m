function xyz = rgb2xyz(in)
%RGB2XYZ Convert RGB values to XYZ color space.
%   XYZMAP = RGB2XYZ(RGBMAP) converts the RGB values in the
%   colormap RGBMAP to the XYZ color space. If RGBMAP is M-by-3 and
%   contains the RGB color values as columns, then XYZMAP
%   is an M-by-3 matrix that contains the X, Y, and Z values
%   equivalent to those colors.
%
%   XYZ = RGB2XYZ(RGB) converts the RGB image to the equivalent
%   image XYZ.
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

% These equations transform RGB in [0, 1] to XYZ in [0,1].

% a =
% 
%    3.40479000000000  -1.53715000000000  -0.49853500000000
%   -0.96925600000000   1.87599200000000   0.04155600000000
%    0.05564800000000  -0.20404300000000   1.05731100000000
%
% ai = inv(a)

ai = [
   0.38627512289306   0.33488427283928   0.16897130319214;
   0.19917304440773   0.70345694075710   0.06626420898270;
   0.01810671359830   0.11812969367380   0.94969014122942
];

xyz(:,:,1) = ai(1, 1) * in(:,:,1) + ai(1,2) * in(:,:,2) + ai(1,3) * in(:,:,3);
xyz(:,:,2) = ai(2, 1) * in(:,:,1) + ai(2,2) * in(:,:,2) + ai(2,3) * in(:,:,3);
xyz(:,:,3) = ai(3, 1) * in(:,:,1) + ai(3,2) * in(:,:,2) + ai(3,3) * in(:,:,3);

switch classin
case 'uint8'
   xyz = uint8(xyz);
case 'double'
   xyz = xyz/255;
case 'uint16'
   xyz = uint16(xyz .* 257);
otherwise
   error('Unsupported output class');
end

if iscolormap
   xyz = reshape(xyz, [colors 3 1]);
end

if isa(xyz,'double')
    xyz = min(max(xyz,0.0),1.0);
end

