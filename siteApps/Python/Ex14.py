scores = [120,130,180,150,-200]

#method.1
M, m = scores[0], scores[1]

for score in scores:
    if score > M:
        M = score
    if score < m:
        m = score

print('Max,', M, 'Min', m)


#method.2
scores = [-20, 120,130,180,150]
M, m = None, None

for score in scores:
    if M == None or score > M:
        M = score
    if m == None or score <m:
        m = score

print('Max',M, ', Min', m)


#Mean Normalization

for score_idx in range(len(scores)):
    scores[score_idx] = (scores[score_idx] - m)/(M - m)

print(scores)

M, m = None, None

for score in scores:
    if M == None or score > M:
        M = score
    if m == None or score < m:
        m = score

print('Normalization Max:', M, ", Min", m)