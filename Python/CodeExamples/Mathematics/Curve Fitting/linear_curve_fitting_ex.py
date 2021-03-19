from scipy.optimize import curve_fit

x = [0, 1, 2, 3, 4, 5]
y = [15, 10, 9, 6, 2, 0]


def linear_model(x, a, b):
    return a * x + b

popt, pcov = curve_fit(linear_model, x, y)

print(popt)