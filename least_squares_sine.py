
import numpy as np
from scipy.optimize import leastsq
import pylab as plt


def sin_estimation(data,t):

    guess_mean = np.mean(data)
    guess_std = 3*np.std(data)/(2**0.5)
    guess_phase = 0

    # we'll use this to plot our first estimate. This might already be good enough for you
    data_first_guess = guess_std*np.sin(t+guess_phase) + guess_mean

    # Define the function to optimize, in this case, we want to minimize the difference
    # between the actual data and our "guessed" parameters
    optimize_func = lambda x: x[0]*np.sin(t+x[1]) + x[2] - data
    est_std, est_phase, est_mean = leastsq(optimize_func, [guess_std, guess_phase, guess_mean])[0]

    # recreate the fitted curve using the optimized parameters
    data_fit = est_std*np.sin(t+est_phase) + est_mean

    return est_std, est_phase, est_mean, data_fit
