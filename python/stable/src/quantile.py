import numpy as np
import sys
from scipy.interpolate import RegularGridInterpolator
from scipy.optimize import minimize
sys.path.append("/Users/kelemenkf/dev/stable/cpp/build/src")
import stable_cpp

class Quantile():
    def __init__(self, X):
      self.X = X
      self.quant_cpp = stable_cpp.Quantile(self.X)
      self.v_alpha = self.calculate_v_alpha()
      self.v_beta = self.calculate_v_beta()
      self.v_gamma = self.calculate_v_gamma()
      self.v_delta = self.calculate_v_delta()
      self.v_alpha_values, self.alphas = self.load_alpha_table()
      self.v_beta_values, self.betas = self.load_beta_table()
      self.v_gamma_values = self.load_gamma_table()
      self.v_delta_values = self.load_delta_table()
      self.alpha, self.beta = self.estimate_alpha_beta()
      self.gamma = self.estimate_gamma()
      self.delta = self.estimate_delta()


    def get_params(self):
        return self.alpha, self.beta, self.gamma, self.delta


    def calculate_v_alpha(self):
        return  self.quant_cpp.get_v_alpha()
    
    
    def calculate_v_beta(self):
        return self.quant_cpp.get_v_beta()
    

    def calculate_v_gamma(self):
        return self.quant_cpp.get_v_gamma()


    def calculate_v_delta(self):
        return self.quant_cpp.get_v_delta()


    def load_alpha_table(self):
        alpha_table = np.loadtxt("/Users/kelemenkf/dev/stable/cpp/assets/vAlpha_lookup_tables.csv", delimiter=',')
        return alpha_table[1:,1:], alpha_table[1:,0]


    def load_beta_table(self):
        beta_table = np.loadtxt("/Users/kelemenkf/dev/stable/cpp/assets/vBeta_lookup_tables.csv", delimiter=',')
        return beta_table[1:,1:], beta_table[0,1:]
    

    def load_gamma_table(self):
        gamma_table = np.loadtxt("/Users/kelemenkf/dev/stable/cpp/assets/vGamma_lookup_tables.csv", delimiter=',')
        return gamma_table[1:,1:]


    
    def load_delta_table(self):
        delta_table = np.loadtxt("/Users/kelemenkf/dev/stable/cpp/assets/vDelta_lookup_tables.csv", delimiter=',')
        return delta_table[1:,1:]


    def v_alpha_interpolator(self):
        v = RegularGridInterpolator((self.alphas, self.betas), self.v_alpha_values)
        return v 


    def v_beta_interpolator(self):
        v = RegularGridInterpolator((self.alphas, self.betas), self.v_beta_values)
        return v 
    

    def v_gamma_interpolator(self):
        v = RegularGridInterpolator((self.alphas, self.betas), self.v_gamma_values)
        return v


    def v_delta_interpolator(self):
        v = RegularGridInterpolator((self.alphas, self.betas), self.v_delta_values)
        return v
    
 
    def estimate_alpha_beta(self):
        v_alpha = self.v_alpha_interpolator()
        v_beta = self.v_beta_interpolator()

        def error_function(vars):
            alpha, beta = vars
            err1 = (v_alpha((alpha, beta)) - self.v_alpha) ** 2
            err2 = (v_beta((alpha, beta)) - self.v_beta) ** 2
            return err1 + err2

        initial_guess = [np.mean(self.alphas), np.mean(self.betas)]  

        result = minimize(error_function, initial_guess, bounds=[(min(self.alphas), max(self.alphas)), (min(self.betas), max(self.betas))])
        
        return result.x  


    def estimate_gamma(self):
        v_gamma = self.v_gamma_interpolator()

        v_gamma_1 = v_gamma([self.alpha, self.beta])[0]
        gamma = self.v_gamma / v_gamma_1

        return gamma


    def estimate_delta(self): 
        v_delta = self.v_delta_interpolator()

        v_delta_1 = v_delta([self.alpha, self.beta])[0]
        delta = self.gamma * v_delta_1 - self.v_delta

        return delta 