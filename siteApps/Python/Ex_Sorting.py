

scores = [40,20,30,10,50,-20,100,150,-40]

sort_scores = list()

for _ in range(len(scores)):
    len_scores = len(scores)
    M, M_idx = scores[0], 0

    for score_idx in range(len_scores):
        if scores[score_idx] < M:
            M = scores[score_idx]
            M_idx = score_idx

    
    tmp_scores = list()
    for score_idx in range(len_scores):
        if score_idx == M_idx:
            sort_scores.append(scores[score_idx])
        else:
            tmp_scores.append(scores[score_idx])
    
    scores = tmp_scores

    print(tmp_scores, sort_scores)


# Accuracy
predictions = [0,1,0,2,1,2,0]
labels      = [1,1,0,0,1,2,1]

n_pred = len(predictions)
n_correct = 0

for pred_idx in range(n_pred):
    if predictions[pred_idx] == labels[pred_idx]:
        n_correct += 1

print('Accuracy[%]:', n_correct/n_pred *100, '[%]')

# Confusion Vector(Accuracy according to class(0,1,2))
predictions = [0,1,0,2,1,2,0]
labels      = [1,1,0,0,1,2,1]

M_class = None

for label in labels:
    if M_class == None or label > M_class:
        M_class = label

M_class += 1
print(M_class)

class_cnts, correct_cnts, confusion_vec = list(), list(), list()

for _ in range(M_class):
    class_cnts.append(0)
    correct_cnts.append(0)
    confusion_vec.append(None)

for pred_idx in range(len(predictions)):
    pred = predictions[pred_idx]
    label = labels[pred_idx]

    class_cnts[label] += 1
    if pred == label:
       correct_cnts[label] += 1

for class_idx in range(M_class):
    confusion_vec[class_idx] = correct_cnts[class_idx]/class_cnts[class_idx]


print('confusion vect:', confusion_vec)