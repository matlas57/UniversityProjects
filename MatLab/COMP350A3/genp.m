function x = genp(A,b)
%
% genp.m  Gaussian elimination (no pivoting)
% input:  A is an n x n nonsingular matrix
%         b is an n x 1 vector
% output: x is the solution of Ax=b.
%
n = length(b);
flops = 0;
for k = 1:n-1  
    i = k+1:n;  
    mult = A(i,k)/A(k,k); 
    A(i,i) = A(i,i) - mult*A(k,i); 
    b(i) = b(i) - mult*b(k);  
end
x = zeros(n,1);
for k = n:-1:1  
    x(k) = (b(k) - A(k,k+1:n)*x(k+1:n))/A(k,k);
    flops = flops + 3;
end
disp(flops);