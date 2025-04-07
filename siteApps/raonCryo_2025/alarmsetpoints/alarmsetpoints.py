numbers = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]
even_numbers = [x for x in numbers if x % 2 == 0]
print(even_numbers)  #even

numbers = [1, 2, 3, 4, 5]
squared_numbers = [x**2 for x in numbers]
print(squared_numbers)  #square

words = ["apple", "banana", "cherry", "date"]
word_lengths = [len(word) for word in words]
print(word_lengths)  # word length

numbers = [1, 2, 3, 4, 5]
result = ["even" if x % 2 == 0 else "odd" for x in numbers]
print(result)  #


nested_list = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
flattened_list = [x for sublist in nested_list for x in sublist]
print(flattened_list)  #

numbers = [1, 2, 2, 3, 4, 4, 5]
unique_numbers = list(set(numbers))
print(unique_numbers)  # unique value

words = ['apple', 'banana', 'cherry', 'date', 'elderberry']
filtered_words = [word for word in words if 'a' in word]
print(filtered_words) # 'a' in word

for i in range(5):  # 
    print(i)

total = 0
for i in range(1, 11):  # 1부터 10까지
    total += i
print(total)  # 


for i in range(10, 1000, 10):
    print(i)  


my_list = ["apple", "banana", "cherry"]
for i in range(len(my_list)):
    print(i, my_list[i])


for i in range(100, 0, -10):
    print(i)

for i in range(1, 5, 0.1):
    print(i)


my_list = [10, 20, 30, 40, 50]
for i in range(1, 4):  
    print(my_list[i])  # 20, 30, 40


matrix = [[1, 2, 3], [4, 5, 6], [7, 8, 9]]
for i in range(len(matrix)):
    for j in range(len(matrix[i])):
        print(matrix[i][j])


numbers = [1, 2, 3, 4, 5]
squared_dict = {x: x**2 for x in numbers}
print(squared_dict)  #{1: 1, 2: 4, 3: 9, 4: 16, 5: 25}


words = ["apple", "banana", "cherry", "date"]
word_length_dict = {word: len(word) for word in words}
print(word_length_dict)  # {'apple': 5, 'banana': 6, 'cherry': 6, 'date': 4}


numbers = [1, 2, 3, 4, 5]
result_dict = {x: "even" if x % 2 == 0 else "odd" for x in numbers}
print(result_dict)  #{1: 'odd', 2: 'even', 3: 'odd', 4: 'even', 5: 'odd'}

my_dict = {"a": 1, "b": 2, "c": 3}
reversed_dict = {value: key for key, value in my_dict.items()}
print(reversed_dict)  # reverse key, value

text = "hello world hello python"
word_counts = {word: text.split().count(word) for word in set(text.split())}
print(word_counts)  # {'hello': 2, 'world': 1, 'python': 1}

my_dict = {"a": 1, "b": 2, "c": 3, "d": 4}
filtered_dict = {key: value for key, value in my_dict.items() if value > 2}
print(filtered_dict)  # {'c': 3, 'd': 4}

my_list = ['apple', 'banana', 'cherry']
indexed_dict = {value: index for index, value in enumerate(my_list)}
print(indexed_dict) # {'apple': 0, 'banana': 1, 'cherry': 2}

keys = ['a', 'b', 'c']
values = [1, 2, 3]
combined_dict = {keys[i]: values[i] for i in range(len(keys))}
print(combined_dict) # {'a': 1, 'b': 2, 'c': 3}


