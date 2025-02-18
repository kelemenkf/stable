import numpy as np
import math
from .stable import Stable
from .density import StableDensity


class MLE(Stable):
    def __init__(self, X, alpha, beta, gamma, delta):
        self.X = np.sort(np.array(X))
        self.alpha = alpha 
        self.beta = beta
        self.gamma = gamma 
        self.delta = delta


    def loglikelihood(self, params):
        alpha, beta, gamma, delta = params 
        pdf = StableDensity(self.X, alpha, beta, gamma, delta).get_pdf()

        logL = np.sum(np.log(pdf))

        return logL 


    def gradient(self, eps=10e-5):
        theta = np.array([self.alpha, self.beta, self.gamma, self.delta])

        grad = []

        for t in range(theta.size):
            forward = theta.copy()
            backward = theta.copy()
            forward[t] += eps
            backward[t] -= eps
            df_dx = (self.loglikelihood(forward) - self.loglikelihood(backward))  /  (2 * eps)
            grad.append(df_dx)

        return np.array(grad)

    
    def hessian(self):
        pass


    def newton(self, eps=10e-6):
        while (np.linalg.norm(G , 2) > eps):
            G = self.gradient()

            if np.linalg.norm(G, 2) < eps: 
                break

            H = self.hessian()
            x_k = np.array([self.alpha, self.beta, self.gamma, self.delta])
            H_inverse = np.linalg.inv(H)

            x_k = x_k - (H_inverse @ G)
            self.alpha, self.beta, self.gamma, self.delta = x_k

