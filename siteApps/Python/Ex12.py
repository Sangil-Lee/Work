
scores = [10,20,30,40,50,60]

sum_scores = 0
n_student = 0
for score in scores:
    sum_scores += score
    n_student += 1

mean_score = sum_scores/n_student
print(mean_score)

## Assign other list
ms_list = list()
for score_idx in range(len(scores)):
    ms_list.append(scores[score_idx] - mean_score)

print(ms_list)

sum_scores = 0
for ms in ms_list:
    sum_scores += ms

print(sum_scores)

## Adjust origin list

sum_scores = 0
for score_idx in range(len(scores)):
    scores[score_idx] -= mean_score
    sum_scores += scores[score_idx]


print(scores, ', Sum of Scores:', sum_scores)

math_scores = [40,60,80]
eng_scores = [30,40,50]

n_class = 2
n_student = len(math_scores)

score_sums = list()
score_means = list()

for _ in range(n_class):
    score_sums.append(0)

for score_idx in range(n_student):
    score_sums[0] += math_scores[score_idx]
    score_sums[1] += eng_scores[score_idx]

print('sums of scores:', score_sums)


for class_idx in range(n_class):
    class_mean = score_sums[class_idx]/n_student
    score_means.append(class_mean)


print('means of scores:', score_means)

for student_idx in range(n_student):
    math_scores[student_idx] -= score_means[0]
    eng_scores[student_idx] -= score_means[1]

print('math score:', math_scores, 'eng scores:', eng_scores)

