# -*- coding: utf-8 -*-
"""
Created on Fri Dec 21 18:59:49 2018

@author: Nhan Tran
"""

import numpy as np
import matplotlib.pyplot as plt
import pandas as pd

# Fitting Linear Regression to the dataset
from sklearn.model_selection import train_test_split 
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
from matplotlib.ticker import (AutoMinorLocator, MultipleLocator)

fig, ax = plt.subplots(figsize=(10,8))

#ax.set_xlim(0, 6)
#ax.set_ylim(0, 0.035)
#
#ax.set_xlim(0, 9)
#ax.set_ylim(0, 0.035)

#ax.set_xlim(0, 190)
#ax.set_ylim(0, 32)

ax.set_xlim(0, 195)
ax.set_ylim(0, 34)

#ax.set_xlim(0,102)
#ax.set_ylim(0,3)

ax.xaxis.set_major_locator(MultipleLocator(25))
ax.yaxis.set_major_locator(MultipleLocator(5))

ax.xaxis.set_minor_locator(AutoMinorLocator(4))
ax.yaxis.set_minor_locator(AutoMinorLocator(4))

ax.grid(which='minor', linestyle=':')
ax.grid(which='major', linestyle='--')

# Importing the dataset
#dataset = pd.read_csv('./QM_B-I_curves.csv')
#dataset = pd.read_csv('./CV_KvValue.csv')
dataset = pd.read_csv('./BI_Map_1.csv')
#dataset = pd.read_csv('./BI_Map_2.csv')
#dataset = pd.read_csv('./SCL3_QWR_Steering_BI_Map_3.csv')
#dataset = pd.read_csv('./SCL3_HWR_Steering_BI_Map_4.csv')
X = dataset.iloc[:, 1:2].values
y = dataset.iloc[:, 2].values

# Splitting the dataset into the Training set and Test set
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=1, random_state=0)

lin_reg = LinearRegression()
lin_reg.fit(X, y)

# Fitting Polynomial Regression to the dataset
poly_reg = PolynomialFeatures(degree=4)
X_poly = poly_reg.fit_transform(X)
pol_reg = LinearRegression()
pol_reg.fit(X_poly, y)

f = open('SCL3_HWR_BI_Map.txt', mode='wt', encoding='utf-8')
# Additional feature
# Making the plot line (Blue one) more smooth
def QM_BI_polymonial_smooth():
    #X_grid = np.arange(min(X), max(X), 0.01)
    X_grid = np.arange(min(X), max(X), 0.1)
    X_grid = X_grid.reshape(len(X_grid), 1)
        
    # Visualizing the Polymonial Regression results
    Y_grid = pol_reg.predict(poly_reg.fit_transform(X_grid))
    plt.scatter(X, y, color='red')
    #plt.plot(X_grid, pol_reg.predict(poly_reg.fit_transform(X_grid)), color='blue')
    plt.plot(X_grid, Y_grid , color='blue')
    plt.title('CV Track- Kv Value Regression(Smooth)')
    plt.xlabel('CV Track')
    plt.ylabel('Kv-Value')
    plt.grid(True)
    plt.tight_layout()
    #plt.show()
    plt.savefig('CV-Kv_Value.png')
 
    i = 0
    X_grid = X_grid.reshape(len(X_grid), 1).flatten()
    while X_grid[i] < X_grid.size:
        f.write("["+ str(format(float(X_grid[i]),".1f"))+"]\t[" +str(format(float(str(Y_grid[i])),".3f")) + "]\n")
        #f.write(str(Y_grid[i])+'\t' +str(X_grid[i]))
        #f.write('\n')
        i = i + 1
        if(i==X_grid.size): break
    
    return

QM_BI_polymonial_smooth()
f.close()

# Predicting a new result with Linear Regression
lin_reg.predict([[5.5]])
#output should be 249500
# Predicting a new result with Polymonial Regression
pol_reg.predict(poly_reg.fit_transform([[5.5]]))
#output should be 132148.43750003
