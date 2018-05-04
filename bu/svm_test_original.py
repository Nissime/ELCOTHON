
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.font_manager
from sklearn import svm
from least_squares_sine import sin_estimation


n_samples = 40
est_std = []
est_phase = []
est_mean  = []


xx, yy = np.meshgrid(np.linspace(-5, 5, 500), np.linspace(-5, 5, 500))


for i in range(0,n_samples):

    N = 1000 # number of data points
    t = np.linspace(0, 2*np.pi, N)
    data = 4.0*np.sin(t+0.001) + 0.5 + np.random.randn(N) # create artificial data with noise

    [est_std_o, est_phase_o, est_mean_o,data_fit] =  sin_estimation(data,t)

    est_std.append(est_std_o)
    est_phase.append(est_phase_o)
    est_mean.append(est_mean_o)
    Sig = data - data_fit
    f5_std = np.std(Sig)


# Generate train data
X = np.array(list(zip(est_std,est_mean)))


X_train = X
# Generate some regular novel observations
X_test = X
# Generate some abnormal novel observations

for i in range(0,n_samples):

    N = 1000 # number of data points
    t = np.linspace(0, 2*np.pi, N)
    data = 10*np.sin(t+0.001) + 0.5 + np.random.randn(N) # create artificial data with noise

    [est_std_o, est_phase_o, est_mean_o,data_fit] =  sin_estimation(data,t)

    est_std.append(est_std_o)
    est_phase.append(est_phase_o)
    est_mean.append(est_mean_o)

# Generate train data
X_outliers = np.array(list(zip(est_std,est_mean)))



# fit the model

clf = svm.OneClassSVM(nu=0.1, kernel="rbf", gamma=0.1)
clf.fit(X_train)
y_pred_train = clf.predict(X_train)
y_pred_test = clf.predict(X_test)
#import pdb; pdb.set_trace()
y_pred_outliers = clf.predict(X_outliers)
n_error_train = y_pred_train[y_pred_train == -1].size
n_error_test = y_pred_test[y_pred_test == -1].size
n_error_outliers = y_pred_outliers[y_pred_outliers == 1].size

# plot the line, the points, and the nearest vectors to the plane
Z = clf.decision_function(np.c_[xx.ravel(), yy.ravel()])
Z = Z.reshape(xx.shape)

plt.title("Novelty Detection")
plt.contourf(xx, yy, Z, levels=np.linspace(Z.min(), 0, 7), cmap=plt.cm.PuBu)
a = plt.contour(xx, yy, Z, levels=[0], linewidths=2, colors='darkred')
plt.contourf(xx, yy, Z, levels=[0, Z.max()], colors='palevioletred')

s = 40
b1 = plt.scatter(X_train[:, 0], X_train[:, 1], c='white', s=s, edgecolors='k')
b2 = plt.scatter(X_test[:, 0], X_test[:, 1], c='blueviolet', s=s,
                 edgecolors='k')
c = plt.scatter(X_outliers[:, 0], X_outliers[:, 1], c='gold', s=s,
                edgecolors='k')
plt.axis('tight')
plt.xlim((-5, 5))
plt.ylim((-5, 5))

plt.legend([a.collections[0], b1, b2, c],
           ["learned frontier", "training observations",
            "new regular observations", "new abnormal observations"],
           loc="upper left",
           prop=matplotlib.font_manager.FontProperties(size=11))
plt.xlabel(
    "error train: %d/200 ; errors novel regular: %d/40 ; "
    "errors novel abnormal: %d/40"
    % (n_error_train, n_error_test, n_error_outliers))
plt.show()
