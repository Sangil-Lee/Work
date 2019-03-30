function pzplotz(b,a)
%  pzplotz(b,a) 
%  b 
%  a
%  a,b
N = length(a); M = length(b); pz = []; zz = []; 
if (N > M)
     zz = zeros((N-M),1);
elseif (M > N)
     pz = zeros((M-N),1); 
end
pz = [pz;roots(a)]; zz = [zz;roots(b)];
pzr = real(pz)'; pzi = imag(pz)';
zzr = real(zz)'; zzi = imag(zz)';
rzmin = min([pzr,zzr,-1])-0.5; rzmax = max([pzr,zzr,1])+0.5;
izmin = min([pzi,zzi,-1])-0.5; izmax = max([pzi,zzi,1])+0.5;
zmin = min([rzmin,izmin]); zmax = max([rzmax,izmax]); zmm = max(abs([zmin,zmax]));
%
uc=exp(j*2*pi*[0:1:500]/500);
plot(real(uc),imag(uc),'w',[-zmm,zmm],[0,0],'w',[0,0],[-zmm,zmm],'w');
axis([-zmm,zmm,-zmm,zmm]);axis('square');hold
plot(zzr,zzi,'go',pzr,pzi,'yx');hold
text(zmm*1.1,zmm*0.95,'z-plane')
xlabel('imaginary');ylabel('real')
title('Pole-Zero')

