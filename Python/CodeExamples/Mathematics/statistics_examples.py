import statistics as st

data = [-1.0, 2.5, 3.25, 5.75]

#Mean or Average
m = st.mean(data)
print(m)

# Standard Deviation
st_dev = st.stdev(data)
print(st_dev)

# Median
med = st.median(data)
print(med)

# Variance
var = st.variance(data)
print(var)
