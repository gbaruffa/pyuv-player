function ycxcz = rgb2ycxcz(in)
%RGB2YCXCZ Convert RGB values to YCxCz color space.
%   YCXCZMAP = RGB2YCXCZ(RGBMAP) converts the RGB values in the
%   colormap RGBMAP to the YCxCz color space. If RGBMAP is M-by-3 and
%   contains the RGB color values as columns, then YCXCZMAP
%   is an M-by-3 matrix that contains the Y, Cx, and Cz values
%   equivalent to those colors.
%
%   YCXCZ = RGB2YCXCZ(RGB) converts the RGB image to the equivalent
%   image YCxCz.
%
%   Class Support
%   -------------
%   If the input is a RGB image, it can be of class uint8, uint16,
%   or double; the output image is of the same class as the input 
%   image.  If the input is a colormap, the input and output 
%   colormaps are both of class double.

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

% These equations transform RGB in [0, 1] to YCxCz in [-1,1].

% a =
% 
%    1/4  1/2  1/4
%    1/2   0  -1/2
%   -1/4  1/2 -1/4
%
% ai = inv(a)

ai = [
    0.23447500416257   0.52652640688218   0.19768418642745;
    0.10827354260078  -0.13669337549857  -0.02047781544626;
    -0.12210863287956  -0.23047402039993   0.42438048133343
];

ycxcz(:,:,1) = ai(1, 1) * in(:,:,1) + ai(1,2) * in(:,:,2) + ai(1,3) * in(:,:,3);
ycxcz(:,:,2) = ai(2, 1) * in(:,:,1) + ai(2,2) * in(:,:,2) + ai(2,3) * in(:,:,3);
ycxcz(:,:,3) = ai(3, 1) * in(:,:,1) + ai(3,2) * in(:,:,2) + ai(3,3) * in(:,:,3);

switch classin
case 'uint8'
   ycxcz = uint8(ycxcz);
case 'double'
   ycxcz = ycxcz/255;
case 'uint16'
   ycxcz = uint16(ycxcz .* 257);
otherwise
   error('Unsupported output class');
end

if iscolormap
   ycxcz = reshape(ycxcz, [colors 3 1]);
end

% if isa(ycocg,'double')
%     ycocg = min(max(ycocg,0.0),1.0);
% end

