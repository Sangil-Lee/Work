	real function fortfunc_multiply(ii,ff)
	integer ii
	real*4 ff

	PRINT *,'Hello Fortran:','ii:',ii,'ff:',ff
	fortfunc_multiply = ii * ff

	PRINT *,'Result:', fortfunc_multiply

	return
	end
