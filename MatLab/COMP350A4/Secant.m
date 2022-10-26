function r = Secant(f, x0, x1, r, xtol, ftol, nmax, display)
% Secant method for solving f(x)=0.
% r = Secant(f,x0,x1,xtol,ftol,n_max,display)
% input:  f is the handle of the function f(x).
%         x0 is the first initial point
%         x1 is the second initial point
%         r is the root in [0, 1] found using roots
%         xtol and ftol are termination tolerances
%         nmax is the maximum number of iterations
%         display = 1 if step-by-step display is desired,
%                 = 0 otherwise
% output: root is the computed root of f(x)=0
%
n=0;
fx0 = f(x0);
fx1 = f(x1);

if display 
   disp('   n             x1                  |x1 - r|/|r|                f(x)')
   disp('----------------------------------------------------------------------------')
   fprintf('%4d %23.15e %23.15e %23.15e\n', n, x1, abs(x1-r)/ abs(r), fx1)
end
if abs(fx1) <= ftol
   r = x1;
   return
end
for n = 1:nmax
    d = ((x1-x0)/(fx1-fx0))*fx1;
    x0 = x1;
    fx0 = fx1;
    x1 = x1 - d;
    fx1 = f(x1);
    if display 
       fprintf('%4d %23.15e %23.15e %23.15e\n', n, x1, abs(x1-r)/ abs(r), fx1), end
    if abs(d) <= xtol || abs(fx1) <= ftol
       r = x1;
       return
    end
end
r = x1;

end