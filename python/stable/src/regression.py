import numpy as np
from .stable import Stable 
from .density import StableDensity 
from .mle import MLE
from .quantile import Quantile
import statsmodels.api as sm
import statsmodels.formula.api as smf


class StableRegression(Stable):
    def __init__(self, X, y):
        super().__init__()
        self.X = sm.add_constant(X)
        self.y = y
        self.trimmed_y, self.trimmed_x = self.trim_data()
        self.distribution_params = self.calculate_initial_distribution_parameters()
        print("Initial dist params ", self.distribution_params)
        self.linear_params = self.second_fit()
        self.newton()


    def get_distribution_params(self):
        return self.distribution_params


    def get_linear_params(self):
        return self.linear_params


    def loglikelihood(self, params):
        alpha, beta, gamma, delta = params[:4] 

        residuals_full = self.y - self.X @ params[4:] 

        pdf = StableDensity(residuals_full, alpha, beta, gamma, delta).get_pdf()

        pdf = np.maximum(pdf, np.finfo(float).eps) 

        logL = -np.sum(np.log(pdf))

        return logL
    

    def first_fit(self):
        results = sm.OLS(self.y, self.X).fit()
        coeffs = results.params

        return coeffs
    
    
    def get_residuals(self, coeffs, y, x):
        fit_line1 = np.vectorize(lambda x: coeffs[0] + coeffs[1]*x)

        y_fitted = fit_line1(x[:,1])

        return y - y_fitted
    

    def trim_data(self):
        residuals = self.get_residuals(self.first_fit(), self.y, self.X)
        q_01 = np.quantile(residuals, 0.1)
        q_09 = np.quantile(residuals, 0.9)
        trimmed_x = []
        trimmed_y = []

        for i in range(len(self.X)): 
            if (residuals[i] < q_09 and residuals[i] > q_01):
                trimmed_y.append(self.y[i])
                trimmed_x.append(self.X[i])

        return (np.array(trimmed_y), np.array(trimmed_x))
    

    def second_fit(self):
        self.trimmed_x = sm.add_constant(self.trimmed_x)

        results = sm.OLS(self.trimmed_y, self.trimmed_x).fit()
        #Initial coefficient values
        coeffs = results.params
        
        return coeffs
    

    def trimmed_residuals(self):
        coeffs = self.second_fit()
        residuals = self.get_residuals(coeffs, self.trimmed_y, self.trimmed_x)

        return residuals


    def calculate_initial_distribution_parameters(self):
        coeffs = self.first_fit()
        residuals = self.get_residuals(coeffs, self.y, self.X)

        initial_distribution_parameters = Quantile(residuals).get_params() 

        return np.array(initial_distribution_parameters)


    def gradient(self, eps=10e-6):
        theta = np.concatenate((self.distribution_params, self.linear_params))

        grad = []

        for t in range(theta.size):
            forward = theta.copy()
            backward = theta.copy()
            forward[t] += eps
            forward = self.clamp_parameters(forward)
            backward[t] -= eps
            backward = self.clamp_parameters(backward)
            df_dx = (self.loglikelihood(forward) - self.loglikelihood(backward))  /  (2 * eps)
            print(df_dx, theta[t])
            grad.append(df_dx)

        return np.array(grad)

    

    def hessian(self, eps=10e-6):
        theta = np.concatenate((self.distribution_params, self.linear_params))

        f_theta = self.loglikelihood(theta)

        hess = np.zeros((theta.size, theta.size))

        function_counter = 0

        for i in range(theta.size):
            for j in range(i, theta.size):
                if i == j:
                    forward = theta.copy()
                    backward = theta.copy()
                    forward[j] += eps
                    forward = self.clamp_parameters(forward)
                    backward[j] -= eps
                    backward = self.clamp_parameters(backward)
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

                    theta_ij1 = self.clamp_parameters(theta_ij1)

                    theta_ij2[i] += eps
                    theta_ij2[j] -= eps

                    theta_ij2 = self.clamp_parameters(theta_ij2)

                    theta_ij3[i] -= eps
                    theta_ij3[j] += eps

                    theta_ij3 = self.clamp_parameters(theta_ij3)

                    theta_ij4[i] -= eps
                    theta_ij4[j] -= eps

                    theta_ij4 = self.clamp_parameters(theta_ij4)

                    f1 = self.loglikelihood(theta_ij1)
                    f2 = self.loglikelihood(theta_ij2)
                    f3 = self.loglikelihood(theta_ij3)
                    f4 = self.loglikelihood(theta_ij4)

                    function_counter += 4

                    hess[i, j] = (f1 - f2 - f3 + f4) / (4 * eps ** 2) 
                    hess[j, i] = hess[i, j] 
        
        return hess


    def clamp_parameters(self, params):
        params[0] = np.clip(params[0], 0, 2)
        params[1] = np.clip(params[1], -1, 1)
        params[2] = np.clip(params[2], 0, None)

        return params
    

    def reflect_gradient_at_boundary(self, params, grad):
        normal = np.zeros_like(params)

        print("Unreflected step ", params)

        bounds = [(0,2), (-1,1)]
        for i in range(len(bounds)):
            lower, upper = bounds[i]
            print(params[i])
            if params[i] > upper: 
                normal[i] = 1
            elif params[i] < lower: 
                normal[i] = -1

        grad_perp = np.dot(grad, normal) * normal
        grad_parallel = grad - grad_perp
        return grad_parallel - grad_perp


    
    def newton(self, eps=10e-6):
        G = self.gradient()

        while (np.linalg.norm(G , 2) > eps):
            G = self.gradient()

            print("Gradient ", G)

            if np.linalg.norm(G, 2) < eps: 
                break

            H = self.hessian()
            x_k = np.concatenate((self.distribution_params, self.linear_params))
            if (np.linalg.det(H) == 0):
                print(H)
                print(self.distribution_params, self.linear_params)
            H_inverse = np.linalg.inv(H)
            x_k = x_k - (H_inverse @ G)
            x_k = self.clamp_parameters(x_k)
            self.distribution_params = x_k[:4]
            self.linear_params = x_k[4:]