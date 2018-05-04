import numpy as np
import matplotlib.pyplot as plt
import matplotlib.font_manager
from sklearn import svm
from least_squares_sine import sin_estimation

class elAp(object):

    def __init__(self,ID,n_samples):
        self.ID = ID
        self.state = 'train'
        self.clf=None
        self.X_train = []
        self.n_samples=n_samples
        self.n_current = 0


    def addTrainData(self,x):
        #x=x.reshape((-1,1))
        #import pdb; pdb.set_trace()
        if self.X_train == []:
            self.X_train = [[max(x)-min(x),int(np.mean(x))]]
        else:
            self.X_train.append([max(x)-min(x),int(np.mean(x))])
        self.n_current+=1

        if self.n_current >= self.n_samples:
            self.clf = svm.OneClassSVM(nu=0.1, kernel="rbf", gamma=0.1)
            #self.clf = svm.OneClassSVM(nu=0.2, kernel="sigmoid", gamma=0.1)
            self.clf.fit(np.array(self.X_train).reshape((-1,2)))
            self.state='monitor'

    def clsfy(self,x):
        if self.state=='monitor':
            return self.clf.predict(np.array([[max(x)-min(x),int(np.mean(x))]]).reshape((-1,2)))
        else:
            return 1


#if __name__ == __main__:
#    elAp.addTrainData
