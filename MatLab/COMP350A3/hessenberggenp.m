function x = hessenberggenp(A,b)
%
% hessenberggenp.m  Gaussian elimination for a hessenberg matrix (no pivoting)
% input:  A is an n x n nonsingular matrix
%         b is an n x 1 vector
% output: x is the solution of Ax=b.
%
n = length(b);
for k = 1:n-1  
    mult = A(k+1,k)/A(k,k); % the multiplier only needs to be calculated once since only the next row will be modified at each step
    i = k:n;  % traverse the row and apply the multiplier
    A(k+1,i) = A(k+1,i) - mult*A(k,i); % apply the multiplier to the row below
    b(k+1) = b(k+1) - mult*b(k); % apply the multiplier to the b vector entry
end
x = zeros(n,1);
for k = n:-1:1  
    x(k) = (b(k) - A(k,k+1:n)*x(k+1:n))/A(k,k);
end
