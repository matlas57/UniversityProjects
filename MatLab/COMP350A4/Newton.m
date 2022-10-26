function r = Newton(f,fd,x, r,xtol,ftol,nmax,display)
% Newton's method for solving f(x)=0.
% r = newton(f,fd,x,r,xtol,ftol,n_max,display)
% input:  f is the handle of the function f(x).
%         fd is the handle of the derivative f'(x).
%         x is the initial point
%         r is the root in [0, 1] found using roots
%         xtol and ftol are termination tolerances
%         nmax is the maximum number of iterations
%         display = 1 if step-by-step display is desired,
%                 = 0 otherwise
% output: root is the computed root of f(x)=0
%
n = 0;
fx = f(x);  

if display 
   disp('   n             x                  |x - r|/|r|                f(x)')
   disp('----------------------------------------------------------------------------')
   fprintf('%4d %23.15e %23.15e %23.15e\n', n, x, abs(x-r)/ abs(r), fx)
end
if abs(fx) <= ftol
   r = x;
   return
end

for n = 1:nmax
    fdx = fd(x); 
    d = fx/fdx;   
    x = x - d;
    fx = f(x);  
    if display 
       fprintf('%4d %23.15e %23.15e %23.15e\n', n, x, abs(x-r)/ abs(r), fx), end
    if abs(d) <= xtol || abs(fx) <= ftol
       r = x;
       return
    end
end
r = x;

end