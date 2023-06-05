# Pseudo code for node i 

import random 
import numpy as np 
import numdifftools as nd
import matplotlib.pyplot as plt
import sympy as sym
import sys 

"""
The values we are actually optimizing: 

N = optimizable - Subcarriers 
M = optimizable - Modulation order
m = optimizable - Symbols per packet
"""


# Number of out neighbors. Since this simulation is just using two nodes, they each have one neighbor.
out_neigh = 1 
out_neigh2 = 1

# How exact one wants before the simulation stops. A lower tolerance value => more exact
tolerance = 1e-3

# We set a very low epsilon value first iteration to not get a big jump in the beginning
epsilon = 0.00001

# Max iterations (if it never converges)
max_iter = 50


# -----------------Initialization node i--------------------

# Initial estimate for node i for the global optimization. Arbitrary values.
ox1 = 100
ox2 = 10

flag_reception, flag_update, flag_transmission, flag_update2, flag_reception2, flag_transmission2 = 0, 0, 0, 0, 0, 0
flag_transmission = 1

# Used for when plotting N, M and m.
#node i
ox_list = np.array(ox1)

#node j
ox_list2 = np.array(ox2)

sigma_ox1 = 0
sigma_ox2 = 0

rho_ox1 = 0
rho_ox2 = 0

i = 1
#---------------------------------------------------------------------------------------

while i < max_iter: 
# ------Data transmission node i-------

    if flag_transmission == 1:

        transmitter_node_ID = "i" # This is what will be transmitted (but not really in this simulation)
        
        # Push sum consensus
        ox1 = (1/(out_neigh + 1))*ox1
        
        # These sigmas are broadcasted to the neighbors (just setting them global in the simulation)
        sigma_ox1 = sigma_ox1 + ox1

        flag_reception, flag_update, flag_transmission, flag_update2, flag_reception2, flag_transmission2 = 0, 0, 0, 0, 0, 0
        flag_reception2 = 1
        
# ------Data Reception node i------

    if flag_reception == 1:

        # Should get these values from node j: transmitter_node_ID, sigmaj_y and sigmaj_z 

        ox1 = ox1 + sigma_ox2 - rho_ox1

        ox_list = np.append(ox_list, ox1)

        rho_ox1 = sigma_ox2

        flag_reception, flag_update, flag_transmission, flag_update2, flag_reception2, flag_transmission2 = 0, 0, 0, 0, 0, 0
        flag_transmission = 1
    
#-----------------------------------------------------------------------------------------------------------------------------------------------------------------

    # ------Data transmission node j-------

    if flag_transmission2 == 1:
        transmitter_node_ID = "j" # This is what will be transmitted (but not really in this simulation)
        
        # Push sum consensus
        ox2 = (1/(out_neigh2 + 1))*ox2
        
        # These sigmas are broadcasted to the neighbors (just setting them global in the simulation)
        sigma_ox2 = sigma_ox2 + ox2

        flag_reception, flag_update, flag_transmission, flag_update2, flag_reception2, flag_transmission2 = 0, 0, 0, 0, 0, 0
        flag_reception = 1

    # ------Data Reception node j------

    if flag_reception2 == 1:

        # Should get values from node j: transmitter_node_ID, sigmaj_y and sigmaj_z
        if i == 1:
            ox2 = (1/(out_neigh + 1))*ox2
        
        ox2 = ox2 + sigma_ox1 - rho_ox2

        ox_list2 = np.append(ox_list2, ox2)

        rho_ox2 = sigma_ox1 

        i += 1 

        flag_reception, flag_update, flag_transmission, flag_update2, flag_reception2, flag_transmission2 = 0, 0, 0, 0, 0, 0
        flag_transmission2 = 1


# ------------------------Plotting------------------------------

figure, (ax1) = plt.subplots(1,1, figsize=(10, 8))

ax1.plot(np.arange(i-1), ox_list, "b", label="oxygen - node 1")
ax1.plot(np.arange(i), ox_list2, "c", label="oxygen - node 2")

ax1.grid()

ax1.legend(loc="lower right")

ax1.set_xlabel('Iterations')

figure.suptitle('Consensus algorithm with two nodes - Forced sequence', fontsize=16)

print(f"ox1: {ox_list[-1]} & ox2: {ox_list2[-1]}")

plt.show()