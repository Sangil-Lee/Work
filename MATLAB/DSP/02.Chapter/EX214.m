n=[-10:9];
x = [5,4,3,2,1];
xtilde = x' * ones(1, 4); xtilde = (xtilde(:))';
subplot(2,2,4);
stem(n,xtilde); title('Sequence in Problem 2.1d');
xlabel('n'); ylabel('xtilde(n)');
