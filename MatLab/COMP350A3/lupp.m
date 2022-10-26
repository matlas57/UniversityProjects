function determinant = lupp(Asize)
% lupp: LU factorization with partial pivoting
% 
% input:  A  
% output: L, U and P such that PA = LU
%
% create the matrix of size n = 1000, 1200
A = zeros(Asize);
for i=1:Asize
    for j=1:Asize
        A(i,j) = abs(i - j);
    end
end
n = size(A,1);
P = eye(n);
detmodifier=1;
for k = 1:n-1
   [maxval, maxindex] = max(abs(A(k:n,k)));
   q = maxindex + k - 1;
   if maxval == 0, error('A is singular'), end
   if q ~= k
       A([k,q],:) = A([q,k],:); 
       P([k,q],:) = P([q,k],:);
       detmodifier = detmodifier * -1;
   end
   i = k+1:n;
   A(i,k) = A(i,k)/A(k,k);
   A(i,i) = A(i,i) - A(i,k)*A(k,i); 
end

L = tril(A,-1) + eye(n);
U = triu(A);
% because all of the diagnoal entries of L = 1 the determinant = 1.
% This is because the determinant of a upper or lower triangular matrix is
% the product of its diagonals
detL=1;
for i=1:Asize
    detL = detL* L(i,i);
end
% to avoid overflow when calculating the product of all the diagonals of A
% I divided the product into two products: one for the first Asize/2
% entries and one for the second Asize/2 entries
detU1=1;
for i=1:Asize/2
    detU1 = detU1* U(i,i);
end
detU2=1;
for i=Asize/2 + 1:Asize
    detU2 = detU2* U(i,i);
end
% then to get the final determinant all that needs to be done is to
% multiply the two portions of the determinant, but this causes overflow
% when done with traditional multiplication.
% instead I extracted the decimal and exponent components of the two values
% and multiplied the decimal components and summed the exponent components,
% lastly I output the final determinant as a string so that a useful result
% is obtained
stringU1 = sprintf('%.4e', detU1);
stringPartsU1 = strsplit(stringU1, 'e');
decimalU1 = str2double(stringPartsU1(1));
exponentU1 = str2double(stringPartsU1(2));

stringU2 = sprintf('%.4e', detU2);
stringPartsU2 = strsplit(stringU2, 'e');
decimalU2 = str2double(stringPartsU2(1));
exponentU2 = str2double(stringPartsU2(2));

decimalU = decimalU1 * decimalU2 * detL * detmodifier;
exponentU = exponentU1 + exponentU2;

determinant = "The determinant of A at size " + Asize + " is " + decimalU + " times 10 to the power " + exponentU; 
