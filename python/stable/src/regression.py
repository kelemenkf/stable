import numpy as np
from .stable import Stable 
from .density import StableDensity 
from .mle import MLE
import statsmodels.api as sm
import statsmodels.formula.api as smf


class StableRegression(Stable):
    def __init__(self, X, y):
        super().__init__()
        self.X = sm.add_constant(X)
        self.y = y
        self.trimmed_y, self.trimmed_x = self.trim_data()
        self.distribution_params = self.calculate_initial_distribution_parameters()


    def get_distribution_params(self):
        return self.distribution_params


    def get_linear_params(self):
        pass

    def loglikelihood(self, params):
        pass


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

        print(coeffs)
        
        return coeffs
    

    def trimmed_residuals(self):
        coeffs = self.second_fit()
        residuals = self.get_residuals(coeffs, self.trimmed_y, self.trimmed_x)

        return residuals


    def calculate_initial_distribution_parameters(self):
        trimmed_residuals = self.trimmed_residuals()

        initial_distribution_parameters = MLE(trimmed_residuals, 1.8, 0, 1, 0).get_params() 

        self.distribution_params = initial_distribution_parameters


    def gradient(self):
        pass


    def hessian(self):
        pass


    def newton(self):
        pass