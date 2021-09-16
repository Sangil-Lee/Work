import tensorflow as tf


sess = tf.Session()

hello = tf.constant('Hello, TensorFlow!!')
print(sess.run(hello))

node1 = tf.constant(3.0, tf.float32)
node2 = tf.constant(4.0)
node3 = tf.add(node1, node2)

print('Tensor Add', sess.run(node3))

a = tf.placeholder(tf.float32)
b = tf.placeholder(tf.float32)
adder = a+b

print('Adder:', sess.run(adder, feed_dict={a:3, b:4}))
print('Adder List:', sess.run(adder, feed_dict={a:[1,2,3], b:[4,5,6]}))


#H(x) = Wx + b => Hypothesis 
x_train = [1, 2, 3]
y_train = [1, 2, 3]

W = tf.Variable(tf.random_normal([1]), name = 'weight')
b = tf.Variable(tf.random_normal([1]), name = 'bias')

#Our hypothesis 

hypothesis = x_train*W + b

#cost/loss function
cost = tf.reduce_mean(tf.square(hypothesis - y_train))

#GrdientDescent
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.01)
train = optimizer.minimize(cost)

sess.run(tf.global_variables_initializer())

for step in range(20001):
    sess.run(train)
    if step % 200 == 0:
        print(step, sess.run(cost), sess.run(W), sess.run(b))
