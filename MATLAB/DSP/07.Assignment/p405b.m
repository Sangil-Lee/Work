b = [3 (3*sin(0.3*pi)-2.25*cos(0.3*pi))]; a = [1 -1.5*cos(0.3*pi) 0.5625];
[delta,n1] = impseq(0,0,7); xb1 = filter(b,a,delta); [u,n2] = stepseq(0,0, 7);
xb2 = 3*(((0.75).^n2).*cos(0.3*pi*n2)).*u+4*(((0.75).^n2).*sin(0.3*pi*n2)).*u;
error = max(abs(xb1-xb2))

Hf_1 = figure; set(Hf_1,'NumberTitle','off','Name','P0403b');
[Hz,Hp,Hl] = zplane(b,a); set(Hz,'linewidth',1); set(Hp,'linewidth',1);
title('Pole-Zero plot');
