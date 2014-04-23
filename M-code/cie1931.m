% CIE 1931 chromaticity diagram

% settings
x_range = linspace(0, 1.0, 100);
y_range = linspace(0, 1.0, 100);

% processing
[X, Y] = meshgrid(x_range, y_range);
Z = 1 - (X + Y);
Z(find(Z > 1.0)) = 1.0;
Z(find(Z < 0.0)) = 0.0;


figure(1);
surf(X, Y, Z);
shading flat

chdi = zeros(length(x_range), length(y_range), 3);
chdi(:, :, 1) = flipud(X);
chdi(:, :, 2) = flipud(Y);
chdi(:, :, 3) = flipud(Z);

figure(2);
imshow(chdi);

maxvals = max(max(X, Y), Z);
Xm = X ./ maxvals;
Ym = Y ./ maxvals;
Zm = Z ./ maxvals;

figure(3);
surf(Xm, Ym, Zm);
shading flat

chdim = zeros(length(x_range), length(y_range), 3);
chdim(:, :, 1) = flipud(Xm);
chdim(:, :, 2) = flipud(Ym);
chdim(:, :, 3) = flipud(Zm);

figure(4);
imshow(chdim);


var_X = X*0.95047;        %Where X = 0 ÷  95.047
var_Y = Y*1.0;        %Where Y = 0 ÷ 100.000
var_Z = Z*1.08883;        %Where Z = 0 ÷ 108.883

var_R = var_X *  3.2406 + var_Y * -1.5372 + var_Z * -0.4986;
var_G = var_X * -0.9689 + var_Y *  1.8758 + var_Z *  0.0415;
var_B = var_X *  0.0557 + var_Y * -0.2040 + var_Z *  1.0570;

findtemp = find(var_R > 0.0031308);
var_R(findtemp) = 1.055 * ( var_R(findtemp) .^ ( 1 / 2.4 ) ) - 0.055;
findtemp = find(var_R <= 0.0031308);
var_R(findtemp) = 12.92 * var_R(findtemp);

findtemp = find(var_G > 0.0031308);
var_G(findtemp) = 1.055 * ( var_G(findtemp) .^ ( 1 / 2.4 ) ) - 0.055;
findtemp = find(var_G <= 0.0031308);
var_G(findtemp) = 12.92 * var_G(findtemp);

findtemp = find(var_B > 0.0031308);
var_B(findtemp) = 1.055 * ( var_B(findtemp) .^ ( 1 / 2.4 ) ) - 0.055;
findtemp = find(var_B <= 0.0031308);
var_B(findtemp) = 12.92 * var_B(findtemp);

var_R(find(var_R > 1.0)) = 1.0;
var_G(find(var_G > 1.0)) = 1.0;
var_B(find(var_B > 1.0)) = 1.0;

var_R(find(var_R < 0.0)) = 0.0;
var_G(find(var_G < 0.0)) = 0.0;
var_B(find(var_B < 0.0)) = 0.0;

maxvals = max(max(var_R, var_G), var_B);
var_Rm = var_R ./ maxvals;
var_Gm = var_G ./ maxvals;
var_Bm = var_B ./ maxvals;

figure(5);
surf(var_Rm, var_Gm, var_Bm);
shading flat

chdimm = zeros(length(x_range), length(y_range), 3);
chdimm(:, :, 1) = flipud(var_Rm);
chdimm(:, :, 2) = flipud(var_Gm);
chdimm(:, :, 3) = flipud(var_Bm);

figure(6);
imshow(chdimm);

