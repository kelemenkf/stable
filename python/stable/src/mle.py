import numpy as np
import time
from .stable import Stable
from .density import StableDensity
from .quantile import Quantile
import sys
sys.path.append("/Users/kelemenkf/dev/stable/cpp/build/src")
import stable_cpp



class MLE(Stable):
    def __init__(self, X):
        self.X = np.sort(np.array(X))
        self.alpha, self.beta, self.gamma, self.delta = self.get_initial_estimates()
        self.alpha_lower = 0
        self.alpha_upper = 2.0
        self.beta_lower = -1
        self.beta_upper = 1
        self.gamma_lower = 0
        self.newton()


    def get_initial_estimates(self):
        start = time.time()

        quant = Quantile(self.X)

        end = time.time()

        print("Initial estimates in ", end - start, " ", quant.get_params())

        return quant.get_params()


    def get_params(self):
        return (self.alpha, self.beta, self.gamma, self.delta)


    def loglikelihood(self, params):
        alpha, beta, gamma, delta = params
        pdf = stable_cpp.Density(self.X, alpha, beta, gamma, delta).get_pdf()

        logL = np.sum(np.log(pdf))

        return logL 


    def gradient(self, eps=10e-6):
        start = time.time()

        theta = np.array([self.alpha, self.beta, self.gamma, self.delta])

        grad = []

        for t in range(theta.size):
            forward = theta.copy()
            backward = theta.copy()
            forward[t] += eps
            backward[t] -= eps
            df_dx = (self.loglikelihood(forward) - self.loglikelihood(backward))  /  (2 * eps)
            grad.append(df_dx)

        end = time.time()

        print("Gradient ", end - start)

        return np.array(grad)

    
    def hessian(self, eps=10e-6):
        start = time.time()

        theta = np.array([self.alpha, self.beta, self.gamma, self.delta])
        start = time.time()
        f_theta = self.loglikelihood(theta)
        end = time.time()
        print(end - start, " function evaluation time")

        hess = np.zeros((theta.size, theta.size))

        function_counter = 0

        for i in range(theta.size):
            for j in range(i, theta.size):
                if i == j:
                    forward = theta.copy()
                    backward = theta.copy()
                    forward[j] += eps
                    backward[j] -= eps
                    df2_dx2 = (self.loglikelihood(forward) - (2 * f_theta) + self.loglikelihood(backward)) / (eps**2)
                    function_counter += 2
                    hess[i, j] = df2_dx2
                else:
                    theta_ij1 = theta.copy()
                    theta_ij2 = theta.copy()
                    theta_ij3 = theta.copy()
                    theta_ij4 = theta.copy()

                    theta_ij1[i] += eps
                    theta_ij1[j] += eps

                    theta_ij2[i] += eps
                    theta_ij2[j] -= eps

                    theta_ij3[i] -= eps
                    theta_ij3[j] += eps

                    theta_ij4[i] -= eps
                    theta_ij4[j] -= eps

                    f1 = self.loglikelihood(theta_ij1)
                    f2 = self.loglikelihood(theta_ij2)
                    f3 = self.loglikelihood(theta_ij3)
                    f4 = self.loglikelihood(theta_ij4)

                    function_counter += 4

                    hess[i, j] = (f1 - f2 - f3 + f4) / (4 * eps ** 2) 
                    hess[j, i] = hess[i, j] 
        
        end = time.time()

        print("Hessian ", end - start, " with ", function_counter, " evaluations")
        
        return hess
    

    def clamp_parameters(self, params):
        params[0] = np.clip(params[0], 0, 2)
        params[1] = np.clip(params[1], -1, 1)

        return params


    def newton(self, eps=10e-8):
        G = self.gradient()

        while (np.linalg.norm(G , 2) > eps):
            G = self.gradient()

            print(G)

            if np.linalg.norm(G, 2) < eps: 
                break

            H = self.hessian()
            x_k = np.array([self.alpha, self.beta, self.gamma, self.delta])
            start = time.time()
            H_inverse = np.linalg.inv(H)
            end = time.time()
            print("Inverse ", end - start)
            x_k = x_k - (H_inverse @ G)
            x_k = self.clamp_parameters(x_k)
            self.alpha, self.beta, self.gamma, self.delta = x_k

