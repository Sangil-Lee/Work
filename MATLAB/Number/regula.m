function a = regula(fun, range_min, range_max, err, iter)
 %Regula Falsi method
 %Parameters
  % fun: input function: ex)fun = @(x) x^3 + 2*x^2 - 1;
  % range_min: Range minimum
  % range_max: Range maximum
  % err: Error rate allowed
  % iter: Iterlation loop count to find regula falsi algorithm

 format long;
 f = fun;

 %R = input ('[ x_min, x_max] of roots:\n');
 R = [range_min, range_max];

 % range
 [ rn , rm ] = size(R);
 if rn ~= 1 || rm~= 2
 	disp( 'Input error: [x_min, x_max]')
 	return
 end

% if root lies in the boundary
 if feval( f , R( 1,1) )* feval( f , R(1,2)) == 0
 if feval( f , R( 1,1) ) == 0
 	R(1,1)
 	return
 else
 	feval( f , R(1,2)) == 0
	 R(1,2)
 	return
 end
 end

 %error allowed in final answer
 tol = abs(err);

 % Loop
 n = iter;

 %initializing the value of k and matrix X
 k=1;
 X= zeros(n+1,3);

disp(sprintf('\t [iterate] \t [value of x] \t [error]'));

x0= R(1,1); x1= R(1,2); x_disp= x0; err = x1-x0;
disp(sprintf ('\t %3d \t %11.5f \t %11.5f ', 0, x_disp,err));

while k <=n && abs(err) > tol 
 %Regula falsi formula
 x = x1 - (x1-x0)/( feval(f,x1)-feval(f,x0) ) *feval(f,x1);
 if feval(f , x0) * feval(f , x) == 0
 x
 return
 else
 if feval(f,x0) * feval(f,x) <0
 err = x - x1;
 x1 = x;
 x_disp=x1;
 X(k,2) = x1;
 else
 err = x-x0;
 x0 = x;
 x_disp = x0;
 X(k,2) = x0;
 end 
 end 

 % storing values in the form of matrix
 X(k,1) = k;
 X(k,3) = abs(err);
 disp(sprintf ('\t %3d \t %11.5f \t %11.5f ', k, x_disp,err));
 k = k + 1;
end
if abs(err) > tol
	disp('More iteration loop') 
else
 	disp(sprintf ('Final result: [%3d] iterations, [%10.10f] with an error: [%10.10f] \n' , (k-1) , X((k-1),2),X((k-1),3)))
a = X((k-1),2);
end
