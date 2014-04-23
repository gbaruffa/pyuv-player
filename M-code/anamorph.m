% Anamorphic transformation coefficients
clear all;

% parameters
K = 9; % input pixel line
N = 16; % output pixel line

K_limits = [0:1/K:1];
N_limits = [0:1/N:1];
disp(' ');

for n = 1:N,
   
   quota = [];
   quotaw = [];
   quotab = [];
   
   for k = 1:K,
      
      if ( (N_limits(n) >= K_limits(k)) & (N_limits(n + 1) <= K_limits(k + 1)) ),
         % n is completely inside k
         quota = [k];
         quotaw = [1];
      elseif ( (N_limits(n) < K_limits(k)) & (N_limits(n + 1) > K_limits(k)) ),
         % n is amidst two k's
         quota = [(k - 1) k];
         quotaw = [(K_limits(k) - N_limits(n)) (N_limits(n + 1) - K_limits(k))] ./ (1 / N);
      else,
         
      end;
      
      quotab = round(quotaw * 256);
      
   end;
   
   %(n - 1)
   %[(quota - 1);
   %   quotaw;
   %   quotab]
   
   if (length(quota) == 1),
      disp(['dest[' int2str(3 * (n - 1) + 0) '] = orig[' int2str(3 * (quota - 1) + 0) '];']);
      disp(['dest[' int2str(3 * (n - 1) + 1) '] = orig[' int2str(3 * (quota - 1) + 1) '];']);
      disp(['dest[' int2str(3 * (n - 1) + 2) '] = orig[' int2str(3 * (quota - 1) + 2) '];']);
   else,
      disp(['dest[' int2str(3 * (n - 1) + 0) '] = (uc) (((ui) ' ...
         int2str(quotab(1)) ' * (ui) orig[' int2str(3 * (quota(1) - 1) + 0) '] + (ui) ' ...
         int2str(quotab(2)) ' * (ui) orig[' int2str(3 * (quota(2) - 1) + 0) ']) >> 8);']);
      disp(['dest[' int2str(3 * (n - 1) + 1) '] = (uc) (((ui) ' ...
         int2str(quotab(1)) ' * (ui) orig[' int2str(3 * (quota(1) - 1) + 1) '] + (ui) ' ...
         int2str(quotab(2)) ' * (ui) orig[' int2str(3 * (quota(2) - 1) + 1) ']) >> 8);']);
      disp(['dest[' int2str(3 * (n - 1) + 2) '] = (uc) (((ui) ' ...
         int2str(quotab(1)) ' * (ui) orig[' int2str(3 * (quota(1) - 1) + 2) '] + (ui) ' ...
         int2str(quotab(2)) ' * (ui) orig[' int2str(3 * (quota(2) - 1) + 2) ']) >> 8);']);
   end;
   
end;