# -*- coding: utf-8 -*-
"""
Spyder Editor

This is a temporary script file.
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

fig, ax = plt.subplots(figsize=(10,8))

ax.set_xlim(0, 195)
ax.set_ylim(0, 34)

ax.xaxis.set_major_locator(MultipleLocator(25))
ax.yaxis.set_major_locator(MultipleLocator(5))

ax.xaxis.set_minor_locator(AutoMinorLocator(4))
ax.yaxis.set_minor_locator(AutoMinorLocator(4))

ax.grid(which='minor', linestyle=':')
ax.grid(which='major', linestyle='--')

#importing the dataset
dataset = pd.read_csv('./BI_Map_1.csv')
X = dataset.iloc[:, 1:2].values
y = dataset.iloc[:, 2].values

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=1, random_state=0)

lin_reg = LinearRegression()
lin_reg.fit(X, y)

#fitting polynomial regression to the dataset
poly_reg = PolynomialFeatures(degree=4)
x_poly = poly_reg.fit_transform(X)
pol_reg = LinearRegression()
pol_reg.fit(X_poly, y)

f = open('SCL3_HWR_BI_Map.txt', mode='wt', encoding='utf-8')

def QM_BI_polymonial_smooth():
    X_grid = np.arange(min(X), max(X), 0.1)
    X_grid = X_grid.reshape(len(X_grid), 1)
    
    Y_grid = pol_reg.predict(poly_reg.fit_transform(X_grid))
    
    plt.scatter(X, y, color='red')
    plt.plot(X_grid, Y_grid, color='blue')
    plt.title('test')    
    plt.xlabel('CV Track')
    plt.ylabel('Kv-Value')
    plt.grid(True)
    plt.tight_layout()
    plt.savefig('test-mj.png')
    
    i = 0
    X_grid = X_grid.reshape(len(X_grid), 1).flatten()
    while X_grid[i] < X_grid.size:
        f.write("["+str(format(float(X_grid[i]),".1f"))+"]\t[" +str(format(float(str(Y_grid[i])),".3f"))+"]\n")
        i = i + 1
        if(i==X_grid.size): break
    return

QM_BI_polymonial_smooth()
f.close()

lin_reg.predict([[5.5]])
pol_reg.predict(poly_reg.fit_transform([[5.5]]))
    
    
    

    
    
    
    