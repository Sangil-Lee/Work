% Example 8.2

N = 3; OmegaC = 0.5;
[b,a] = u_buttap(N,OmegaC);
[C,B,A] = sdir2cas(b,a)

