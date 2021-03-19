import scipy.optimize

banana = lambda x: 100*(x[1]-x[0]**2)**2+(1-x[0])**2

xopt = scipy.optimize.fmin(func=banana, x0=[-1.2,1])

print(xopt)