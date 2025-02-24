import numpy as np
import sys
sys.path.append("/Users/kelemenkf/dev/stable/cpp/build/src")
import stable_cpp

class Quantile():
    def __init__(self, X):
      self.X = X
      self.quant_cpp = stable_cpp.Quantile(self.X)
      self.v_alpha = self.calculate_v_alpha()
      self.v_beta = self.calculate_v_beta()
      self.v_gamma = self.calculate_v_gamma()
      self.alpha_values, self.alphas = self.load_alpha_table()
      self.beta_values, self.betas = self.load_beta_table()


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
    

    def estimate_gamma(self):
        pass

    
    def estimate_delta(self): 
        pass