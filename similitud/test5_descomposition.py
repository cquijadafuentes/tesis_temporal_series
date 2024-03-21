from random import randrange
from pandas import Series
from statsmodels.tsa.seasonal import seasonal_decompose
series = [i+randrange(10) for i in range(1,100)]
result = seasonal_decompose(series, model='additive', period=7)
#print(help(result))

print("observed", result.observed)
print("observed_size:", result.observed.size)
print("seasonal", result.seasonal)
print("seasonal_size:", result.seasonal.size)
print("trend", result.trend)
print("trend_size:", result.trend.size)
print("resid", result.resid)
print("resid_size:", result.resid.size)
print("weights", result.weights)
print("weights_size:", result.weights.size)

print("sumando para obtener los resultados...")

x = result.trend + result.seasonal + result.resid

for z in range(0,result.observed.size):
	print(z, "|||||", result.trend[z], result.seasonal[z], result.resid[z], result.weights[z], "|||||", result.observed[z], x[z])



print(help(result))