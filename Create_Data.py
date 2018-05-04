import numpy as np
import matplotlib.pyplot as plt


data_length = 200 # data points

x = np.linspace(0,2*np.pi,data_length)
x_noise = np.random.randn(data_length)

plt.plot(x,np.sin(x)*10+x_noise)
plt.show()
