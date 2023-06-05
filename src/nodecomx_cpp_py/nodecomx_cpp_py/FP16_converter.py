import numpy as np

class Converter():

    # Methods for converting lists. The input should be in a form of a python list, i.e. [1, 2, 3]. 
    # The method names are self-explanatory. 

    def int8_list_to_FP16(user_input):

        if isinstance(user_input, str):
            converted_list = user_input.strip('][').split(', ')
            converted_list = [eval(i) for i in converted_list]
            user_input = converted_list

        new_val = [Converter.int8_to_FP16(a, b) for a, b in zip(user_input[::2], user_input[1::2])]
        return new_val
    
    def FP16_list_to_int8(user_input):

        if isinstance(user_input, str):
            converted_list = user_input.strip('][').split(', ')
            converted_list = [eval(i) for i in converted_list]
            user_input = converted_list

        new_val = [Converter.FP16_to_int8(a) for a in user_input]
        flattened_list = [num for tup in new_val for num in tup]
        return flattened_list

    def int8_list_to_FP32(user_input):

        if isinstance(user_input, str):
            converted_list = user_input.strip('][').split(', ')
            converted_list = [eval(i) for i in converted_list]
            user_input = converted_list

        new_val = [Converter.int8_to_FP32(a, b, c, d) for a, b, c, d in zip(user_input[::4], user_input[1::4], user_input[2::4], user_input[3::4])]
        return new_val

    
    def FP32_list_to_int8(user_input):

        if isinstance(user_input, str):
            converted_list = user_input.strip('][').split(', ')
            converted_list = [eval(i) for i in converted_list]
            user_input = converted_list

        new_val = [Converter.FP32_to_int8(a) for a in user_input]
        flattened_list = [num for tup in new_val for num in tup]

        return flattened_list


# Single value converters. These are used in the list converters, and work in a similar fassion.
# The inputs can not be strings.

    def FP32_to_int8(user_input):

        # Convert FP32 to a 32bit integer
        x_int = int.from_bytes(np.float32(user_input), byteorder='little', signed=False)

        # convert the 32bit integer to its binary representation
        binary_x = np.binary_repr(x_int, width=32)

        # Splitting up the 16 bits into two bytes and their 8bit integer numbers based on base 2
        byte_1_as_int = int(binary_x[:8], 2)
        byte_2_as_int = int(binary_x[8:16], 2)
        byte_3_as_int = int(binary_x[16:24], 2)
        byte_4_as_int = int(binary_x[24:32], 2)

        # Converting the 8bit integers into 8bit unicode equivalent character
        # myasc1 = chr(byte_1_as_int)
        # myasc2 = chr(byte_2_as_int)

        return (byte_1_as_int, byte_2_as_int, byte_3_as_int, byte_4_as_int)

    def int8_to_FP32(received_byte_1, received_byte_2, received_byte_3, received_byte_4):
        
        # Converting the unicode characters into the unicode code integer
        # myByte = ord(received_character_1)
        # myByte2 = ord(received_character_2)
        
        # Converting the integers into 8bit binary representations, and combining them into a 16bit value as a string
        x_int = (format(received_byte_1, '#010b')[2:] + format(received_byte_2, '#010b')[2:] + 
                    format(received_byte_3, '#010b')[2:] + format(received_byte_4, '#010b')[2:])


        # convert the 16bit value string into a 16bit integer, then into a combination of two bytes.
        byte_str = int(x_int, 2).to_bytes(4, byteorder='little')

        # create a FP32 from the bytes
        x = np.frombuffer(byte_str, dtype=np.float32)[0]
        
        return x


# WIP
    
    def FP16_to_int8(user_input):


        # Convert FP16 to a 16bit integer
        x_int = int.from_bytes(np.float16(user_input), byteorder='little', signed=False)

        # convert the 16bit integer to its binary representation
        binary_x = np.binary_repr(x_int, width=16)

        # Splitting up the 16 bits into two bytes and their 8bit integer numbers based on base 2
        byte_1_as_int = int(binary_x[:8], 2)
        byte_2_as_int = int(binary_x[8:], 2)

        # Converting the 8bit integers into 8bit unicode equivalent character
        # myasc1 = chr(byte_1)
        # myasc2 = chr(byte_2)

        return (byte_1_as_int, byte_2_as_int)

    # RECEIVING PART

    def int8_to_FP16(received_character_1, received_character_2):
        
        # Converting the unicode characters into the unicode code integer
        # myByte = ord(received_character_1)
        # myByte2 = ord(received_character_2)
        
        byte_1 = received_character_1
        byte_2 = received_character_2

        # Converting the integers into 8bit binary representations, and combining them into a 16bit value as a string
        x_int = (format(byte_1, '#010b')[2:] + format(byte_2, '#010b')[2:])


        # convert the 16bit value string into a 16bit integer, then into a combination of two bytes.
        byte_str = int(x_int, 2).to_bytes(2, byteorder='little')

        # create a numpy float16 from the bytes
        float_list_finished = np.frombuffer(byte_str, dtype=np.float16)[0]
        
        return float_list_finished
