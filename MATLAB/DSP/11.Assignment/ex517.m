% Example 5.17

x1 = [1,2,2,1]; x2 = [1,-1,-1,1]; x3=conv(x1,x2)

% N = 6:
x4 = circonvt(x1,x2,6)
%Error
err_6 = x4-x3(1:6)

% N = 5:
x4 = circonvt(x1,x2,5)
%Error
err_5 = x4-x3(1:5)

% N = 4:
x4 = circonvt(x1,x2,4)
%Error
err_4 = x4-x3(1:4)
