import numpy as np
from .stable import Stable 
from .density import StableDensity 
import statsmodels.api as sm
import statsmodels.formula.api as smf


class StableRegression(Stable):
    def __init__(self, X, y):
        super().__init__()
        self.X = sm.add_constant(X)
        self.y = y


    def first_fit(self):
        results = sm.OLS(self.y, self.X).fit()
        coeffs = results.params

        return coeffs
    
    
    def get_residuals(self, coeffs, x):
        fit_line1 = np.vectorize(lambda x: coeffs[0] + coeffs[1]*x)

        y_fitted = fit_line1(x[:,1])

        return self.y - y_fitted
    

    def trim_data(self):
        residuals = self.get_residuals(self.first_fit(), self.X)
        q_01 = np.quantile(residuals, 0.1)
        q_09 = np.quantile(residuals, 0.9)
        trimmed_x = []
        trimmed_residuals = []
        trimmed_y = []

        for i in range(len(X)): 
            if (residuals[i] < q_09 and residuals[i] > q_01):
                trimmed_y.append(self.y[i])
                trimmed_x.append(self.X[i])
                trimmed_residuals.append(residuals[i])

        return (np.array(trimmed_y), np.array(trimmed_x))
    

    def second_fit(self):
        trimmed_y, trimmed_x = self.trim_data()
        trimmed_x = sm.add_constant(trimmed_x)

        results = sm.OLS(trimmed_y, trimmed_x).fit()
        coeffs = results.params
        
        return coeffs