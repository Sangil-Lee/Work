b = [0 -8 0 -1.5 0 -1/16]; a = [1 0 3/16 0 3/256 0 1/(256*16)];
[delta,n1] = impseq(0,0,9); xb1 = filter(b,a,delta);
[u,n2] = stepseq(0,0,9);xb2 = (((n2-3).*((1/4).^(n2-2))).*cos((pi/2)*(n2-1))).*u;
error = max(abs(xb1-xb2))
[Hz,Hp,Hl] = zplane(b,a); set(Hz,'linewidth',1); set(Hp,'linewidth',1);
title('Pole-Zero plot');
