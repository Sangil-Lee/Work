
score1, score2, score3 = 10, 20, 30
n_student = 3


mean_score = (score1+score2+score3)/n_student
square_mean = mean_score**2

mean_of_square = (score1**2 + score2**2+score3**2)/n_student
print(mean_score, square_mean, mean_of_square)

score_var = mean_of_square - square_mean
score_std = score_var**0.5

print(score_var, score_std)


score1 = (score1 - mean_score)/score_std
score2 = (score2 - mean_score)/score_std
score3 = (score3 - mean_score)/score_std


mean_score = (score1+score2+score3)/n_student
square_mean = mean_score**2
mean_of_square = (score1**2 + score2**2 + score3**2)/n_student
print(mean_score, square_mean, mean_of_square)

score_var = mean_of_square - square_mean
score_std = score_var**0.5
print(score_var, score_std)

numbers = list()
#for num in range(101):
for _ in range(101):
    #numbers.append(2*num) # even number list
    #numbers.append(2*num+1) # odd number list
    numbers.append(0) # initialize

print(numbers)

scores = [10,20,30,40,50]

for score in scores:
    print(score)

for score_idx in range(len(scores)):
    scores[score_idx] += 10
    print(score_idx, scores[score_idx])

print(scores)


list1 = [10,20,30,40,50]
list2 = [100,200,300,400,500]

for list_idx in range(len(list1)):
    print(list1[list_idx], list2[list_idx], list1[list_idx]+list2[list_idx])

score_num = 0
n_student = 0
for score in scores:
    score_num += score
    n_student += 1

score_num /= n_student
print(score_num)

score_num = 0
for score_idx in range(len(scores)):
    score_num += scores[score_idx]

score_num /= len(scores)

print(score_num)