unitsize(1cm);

transform T1 = scale(5)*shift(3,0);
transform T2 = shift(3,0)*scale(5);

transform R1 = scale(5)*shift(3,0)*rotate(35);
transform R2 = shift(3,0)*rotate(35)*scale(5);
transform R3 = rotate(35)*shift(3,0)*scale(5);

draw((0,0)--(20,0), Arrow);
draw((0,0)--(0,9), Arrow);
for (int i = 0; i < 9; ++i) draw((0,i)--(.1,i));
for (int i = 0; i < 20; ++i) draw((i,0)--(i,.1));

draw(R1 * unitsquare, red);
draw(R2 * unitsquare, green);
draw(R3 * unitsquare, blue);
