import numpy as np 
import nodecomx.gradient_hessian_calculator  as gh_calc
from datetime import datetime
import sympy as sym
import os 

log = np.log # The natural logarithm
log2 = np.log2 # Log base 2

class NRC_algo():
    initialized = False

    def __init__(self):

        if not NRC_algo.initialized:
                
                self.B = 12000   # Bandwidth
                self.R_c = 0.5   # Coding rate
                self.p_c = 0.25  # Cyclic prefix fraction
                self.t_oh = 0.1  # Overhead (preamble, propagation...)
                self.R_n = 0.5   # 0<R_n<1 A part of the carrier waves 
                self.r = 1000    # r = 100-4000m -emil
                self.c1 = 1500   # Speed of sound, 343 m/s
                self.t_d = self.r/self.c1  # Transmission delay t_d = r/c

                # dimension of hessian / gradient. ex. 3x3 and 3x1
                self.n = 3 

                # Number of out neighbors. Since this simulation is just using two nodes, they each have one neighbor.
                self.out_neigh = 1 

                # Value used in order to increase the basin of attraction and the robustness of the algorithm
                self.c = 1E-8

                # bb multiplier (used in the barrier method) /// NOT USED IN THIS VERSION
                # self.mu = 2
                
                # Number of sequences before increasing bb /// NOT USED IN THIS VERSION
                # self.gamma = 20

                # How exact one wants before the simulation stops. A lower tolerance value => more exact /// NOT USED IN THIS VERSION
                # self.tolerance = 1e-3

                # The bb value is also used as the "t" parameter. This is the parameter used in the barrier method / interior point method.
                # It is important to not start with a too big bb value. /// NOT USED IN THIS VERSION
                # self.bb = 5

                # Max iterations (if it never converges) /// NOT USED IN THIS VERSION
                # max_iter = 1000

                # We set a very low epsilon value first iteration to not get a big jump in the beginning
                self.epsilon = 0.00001

               

                # Initial estimate for the global optimization. Arbitrary values, recommended by Emil
                self.xi = np.array([ 
                            [500], # N_start
                            [4],  # M_start
                            [5]])  # m_start

                self.yi, self.gi, self.gi_old = np.zeros((self.n, 1)), np.zeros((self.n, 1)), np.zeros((self.n, 1))   # if n=3 -> they are 3x1 zero matrices
                self.zi, self.hi, self.hi_old = np.identity(self.n), np.identity(self.n), np.identity(self.n)         # if n=3 -> they are 3x3 identity matrices
                self.sigmai_y, self.sigmai_z = np.zeros((self.n, 1)), np.zeros((self.n, self.n))                 # 3x1 and 3x3. Gradient is 3x1 and hessian is 3x3

                # Each np.zeros is for node j1, j2. Will make more np.zeros if it has more neighbors...
                self.rhoi_y = np.zeros((self.n, 1))
                # Each np.zeros is for node j1, j2. Will make bigger if it has more neighbors.
                # The rhoi_z is as array that contains nxn matrices (the hessians of node j's). ex. rhoi_z[j] = [3x3,3x3,3x3....]
                self.rhoi_z = np.zeros((self.n, self.n))

                self.flag_reception, self.flag_update, self.flag_transmission, self.flag_update2, self.flag_reception2, self.flag_transmission2 = 0, 0, 0, 0, 0, 0
                self.flag_update = 1

                self.sigmai_y, self.sigmai_z = np.zeros((self.n, 1)), np.zeros((self.n, self.n)) # skal egentlig komme fra node j!
                self.sigmaj_y, self.sigmaj_z = np.zeros((self.n, 1)), np.zeros((self.n, self.n)) # skal egentlig komme fra node j!

                # x[2] = m, x[1] = M, x[0] = N, cost function definition from Emil paper
                # self.path = os.path.join(os.getcwd(), "log_to_plot.txt")

                # with open(self.path, mode="w") as f:
                #     f.write(f'\n{str(datetime.now().strftime("%H:%M:%S"))}')


                NRC_algo.initialized = True

    def data_reception(self, input_array):

        # Should get these values from node j: transmitter_node_ID, sigmaj_y and sigmaj_z 

        self.sigmaj_y =  np.transpose(np.array([input_array[:3]]))
        self.sigmaj_z = np.transpose(np.array([input_array[3:12]])).reshape(3,3)
      
        self.yi = self.yi + self.sigmaj_y - self.rhoi_y
        self.zi = self.zi + self.sigmaj_z - self.rhoi_z

        self.rhoi_y = self.sigmaj_y 
        self.rhoi_z = self.sigmaj_z

        self.estimate_update()

    def data_transmission(self):
       
        # Push sum consensus
        self.yi = (1/(self.out_neigh + 1)) * self.yi
        self.zi = (1/(self.out_neigh + 1)) * self.zi

        # These sigmas are broadcasted to the neighbors (just setting them global in the simulation)
        self.sigmai_y = self.sigmai_y + self.yi
        self.sigmai_z = self.sigmai_z + self.zi
       
        output_array = (self.sigmai_y.flatten().tolist()) + (self.sigmai_z.flatten().tolist())
        return output_array
    
    def estimate_update(self): 

        # NOT USED IN THIS VERSION[
        # Every something iteration, increase bb. In the real world synchronous clocks are used for now, not based on iterations. 
        # if i % gamma == 0:
        #     bb = bb*mu
        # print("bb: ", bb)
        # ]NOT USED IN THIS VERSION 

        # In order to increase the basin of attraction and the robustness of the algorithm, it is suitable to force: hessian >= c*I

        if (np.abs(np.linalg.eigvals(self.zi)) < self.c).all():
            self.zi = self.c*np.identity(self.n)
    
        # Newton-Raphson Consensus
        self.xi = (1-self.epsilon) * self.xi + self.epsilon*np.linalg.inv(self.zi) @ self.yi
        self.epsilon = 0.2

        # For plotting the xi[
        # with open(self.path, mode="a") as f:
        #     f.write(f'{self.xi.flatten().tolist()} \n')
        # ]For plotting the xi

        gi_old = self.gi
        hi_old = self.hi
        self.gradient, self.hi = gh_calc.gradient_hessian_calculator(self.xi)
        self.gi = self.hi @ self.xi - self.gradient

        self.yi = self.yi + self.gi - gi_old
        self.zi = self.zi + self.hi - hi_old 



        




    