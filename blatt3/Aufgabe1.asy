pair p = (2, 4);

transform T1 = shift(-1, -1);
transform T2 = rotate(-45);
transform T3 = shift(1, 1);
write(T3*T2*T1*p);

transform T = rotate(-45, (1,1));
write(T*p);
