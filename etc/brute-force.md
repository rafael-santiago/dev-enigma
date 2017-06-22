# How long does it takes to attack Enigma by brute force?

Let's quantify the total of possible settings that should be tested on a brute force attack against the Enigma.

The machine is configured with three rotors in any of three "slots", so the Equation 1 denotes the possible arrangements.

![Equation-1](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-1.png)

Each of the three rotors also can assume twenty-six initial positions. Equation 2 denotes the total of possible positions of each rotor.

![Equation-2](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-2.png)

Due to the twenty-six shift levels offered by the rings, the Equation 2 should be updated to Equation (3).

![Equation-3](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-3.png)

With the plug-board ten letters can be swapped, the Equation 4 denotes the swaps possibilities.

![Equation-4](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-4.png)

Three rotors are used during the encryption but they are chose from a set of eight rotors, the Equation 5 denotes it.

![Equation-5](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-5.png)

The Equation (1) and (5) can be combined as shown in Equation (6).

![Equation-6](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-6.png)

Finally, the two possible reflectors possibilities follows denoted by Equation (7)

![Equation-7](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-7.png)

The final security offered by a cryptosystem that is a composite of two or more cryptosystems is the product of the isolated security offered
by each one. Thus, the Equation 8 denotes the final security offered by the Enigma cipher.

![Equation-8](https://github.com/rafael-santiago/dev-enigma/blob/master/etc/Equation-8.png)

The Equation (8) expresses the search space of a brute force attack against Enigma. This is the maximum number of unique possible settings
offered by this machine.
