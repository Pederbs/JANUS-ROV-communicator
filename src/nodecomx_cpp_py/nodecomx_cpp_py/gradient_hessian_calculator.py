import sympy as sym
import numpy as np
import sys

import datetime
import threading



bb = 1

# def bb_increase():
#     global bb
#     current_minute = datetime.datetime.now().minute
#     minutes_until_next_multiple_of_two = 2 - current_minute % 2
#     seconds_until_next_multiple_of_two_minutes = minutes_until_next_multiple_of_two * 60 - datetime.datetime.now().second
#     threading.Timer(seconds_until_next_multiple_of_two_minutes, bb_increase).start()
#     bb = bb * 5
#     print(f"bb: {bb} ", datetime.datetime.now())

# bb_increase()


def gradient_hessian_calculator(xi):

    R_c = 0.5 # Coding rate
    B = 12000 # Bandwidth
    p_c = 0.25 # Cyclic prefix fraction
    t_oh = 0.1 # Overhead (preamble, propagation...)
    R_n = 0.5 # 0<R_n<1 A part of the carrier waves 
    r = 1000 # r = 100-4000m -emil
    c1 = 1500 # Speed of sound, 343 m/s
    t_d = r/c1 # Transmission delay t_d = r/c
    p_lt = 0.001 # Target packet loss ratio
    gamma = 31.62 # The average SNR at the receiver, 0 < gamma_dB < 30

    N, M, m = sym.symbols('N M m')

    # Constraints for the cost function. constraint_N/M/m is really 2 constraints. This means we have 7 constraints here.
    constraint_N = sym.log(-(N - 2000)) + sym.log(-(400 - N)) # 400 < N < 2000
    constraint_M = sym.log(-(2 - M)) + sym.log(-(M - 64)) # 2 < M < 64
    constraint_m = sym.log(-(1 - m)) + sym.log(-(m - 40)) #  1 < m < 40
    constraint_PLR = sym.log(-(sym.log(m)+sym.log(R_n) + sym.log(N) + sym.log(sym.log(M,2)) + (1/R_c)*(sym.log(0.2)-(3*gamma)/(2*(M-1))) - sym.log(p_lt) )  ) # PLR - packet loss ratio

    # Defining the cost function
    function_without_constraint = -(sym.log(m)+ sym.log(R_c) + sym.log(B) + sym.log(R_n) + sym.log(N) + sym.log(sym.log(M, 2)) - sym.log(m*(1+p_c)*N + B*(t_oh + t_d)))
    
    # Defining the objective function we want to minimize, notice using the bb notation instead of t
    function = bb*( function_without_constraint ) - (constraint_N+constraint_M+constraint_m+constraint_PLR) 

    # Objective function value at xi
    function_value = function.evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})

    # Derivating the objective function for N, M and m (algebraic answer, without values)
    der_x1 = function.diff(N)
    der_x2 = function.diff(M)
    der_x3 = function.diff(m)

    # Putting xi values into the derivatives
    der_x1_values = function.diff(N).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_x2_values = function.diff(M).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_x3_values = function.diff(m).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})

    # Putting the derivatives together in a matrix so we get the gradient of the objective function
    gradient_values = np.array([
        [der_x1_values],
        [der_x2_values],
        [der_x3_values]
    ], dtype=np.float32)

    # Derivating the objective function further to get the hessian
    der_x1x1_values = der_x1.diff(N).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_crossx1x2_values = der_x1.diff(M).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_x2x2_values = der_x2.diff(M).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_crossx1x3_values = der_x1.diff(m).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_x3x3_values = der_x3.diff(m).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})
    der_crossx2x3_values = der_x2.diff(m).evalf(subs={N: xi[0][0], M: xi[1][0], m: xi[2][0]})

    # Putting the derivatives together in a matrix so we get the hessian of the objective function
    hessian_values = np.array([
        [der_x1x1_values, der_crossx1x2_values, der_crossx1x3_values],
        [der_crossx1x2_values, der_x2x2_values, der_crossx2x3_values],
        [der_crossx1x3_values, der_crossx2x3_values, der_x3x3_values]
    ], dtype=np.float32)

    # Check if the answer is feasible. For example a too big starting t value can cause numerical difficulties 
    if sym.im(function_value) != 0:
        print('=========COMPLEX ANSWER, NOT FEASIBLE, MAYBE DECREASE t =============')
        print("f(x): ", function_value)
        sys.exit()

    return gradient_values, hessian_values