function x = hessenberggepp(A,b)
% gepp.m    Gauss elimination for a hessenberg matrix with partial pivoting
% 
% input:  A is an n x n nonsingular matrix
%         b is an n x 1 vector
% output: x is the solution of Ax=b.
%

n = length(b);
for k = 1:n-1  
   % part 1: determine if the entry below A(k,k) is greater, 
   % if so, swap the rows
   [~, maxindex] = max([abs(A(k,k)), abs(A(k+1,k))]);
   if maxindex ~= 0  
       A([k,k+1],k:n) = A([k+1,k],k:n);
       b([k,k+1]) = b([k+1,k]);
   end
   mult = A(k+1,k)/A(k,k);
   i = k:n;  % traverse the row and apply the multiplier
   A(k+1,i) = A(k+1,i) - mult*A(k,i); % apply the multiplier to the row below
   b(k+1) = b(k+1) - mult*b(k); % apply the multiplier to the b vector entry
end
x = zeros(n,1);
for k = n:-1:1  
  x(k) = (b(k) - A(k,k+1:n)*x(k+1:n))/A(k,k);
end

