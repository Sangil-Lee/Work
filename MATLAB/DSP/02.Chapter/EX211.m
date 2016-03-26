n=[-5:5];
x = 2 * impseq(-2,-5,5) - impseq(4,-5,5);
subplot(2,2,1);
stem(n,x); title('Sequence in Problem 2.1a');
xlabel('n'); ylabel('x(n)');
