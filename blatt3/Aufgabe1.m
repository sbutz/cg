#!/usr/bin/octave-cli

p = [2; 4; 1];

t1 = [1  0 -1
      0  1 -1
      0  0  1];

a = -pi/4;
t2 = [cos(a) -sin(a) 0
      sin(a)  cos(a) 0
           0       0 1];

t3 = [1 0 1
      0 1 1
      0 0 1];

t3 * t2 * t1 * p
