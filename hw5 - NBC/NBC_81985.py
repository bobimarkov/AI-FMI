from sklearn.metrics import accuracy_score
from math import log2

data = open(".\house-votes-84.data", "r").readlines()
dt = list(map(lambda x: x[:-1].split(','),data))
splits = list(range(0,len(dt), 43))[1:-1] + [len(dt)]

prev = 0
sum_of_accuracy = 0

for index in range(len(splits)):
    result = []

    from_ = prev
    to = splits[index]
    prev = splits[index]

    training_set = dt[from_:to]
    
    training_labels = list(map(lambda x: x[0], training_set))
    training_set = list(map(lambda x: x[1:], training_set))

    learning_set = dt[:from_] + dt[to:]

    parties = list(map(lambda x: x[0], learning_set))
    parties_d = list(filter(lambda x: x[0] == 'democrat', learning_set))
    parties_r = list(filter(lambda x: x[0] == 'republican', learning_set))
    parties_d_count = len(parties_d)
    parties_r_count = len(parties_r)

    col_dy = []
    col_dn = []
    col_dq = []
    col_ry = []
    col_rn = []
    col_rq = []

    for i in range(len(learning_set[0])):
        current_dy_count = len(list(filter(lambda x: x[i] == 'y', parties_d)))
        current_dn_count = len(list(filter(lambda x: x[i] == 'n', parties_d)))
        current_dq_count = len(list(filter(lambda x: x[i] == '?', parties_d)))
        current_ry_count = len(list(filter(lambda x: x[i] == 'y', parties_r)))
        current_rn_count = len(list(filter(lambda x: x[i] == 'n', parties_r)))
        current_rq_count = len(list(filter(lambda x: x[i] == '?', parties_r)))

        if i != 0:
            col_dy.append(current_dy_count)
            col_dn.append(current_dn_count)
            col_dq.append(current_dq_count)
            col_ry.append(current_ry_count)
            col_rn.append(current_rn_count)
            col_rq.append(current_rq_count)

    # prob_r = log2(parties_r_count/len(dt))
    # prob_d = log2(parties_d_count/len(dt))

    # Normal NBC
    # for i in range(len(dt_training)):
    #     current_prob_ry = log2(col_ry[i]/parties_r_count)
    #     current_prob_rn = log2(col_rn[i]/parties_r_count)
    #     current_prob_rq = log2(col_rq[i]/parties_r_count)
    #     current_prob_dy = log2(col_dy[i]/parties_d_count)
    #     current_prob_dn = log2(col_dn[i]/parties_d_count)
    #     current_prob_dq = log2(col_dq[i]/parties_d_count)

    #     #print(current_prob_ry, current_prob_rn, current_prob_dy, current_prob_dn)

    #     prob_r += [[current_prob_rq, current_prob_rn][dt_training[i] == 'n'], current_prob_ry][dt_training[i] == 'y']
    #     prob_d += [[current_prob_dq, current_prob_dn][dt_training[i] == 'n'], current_prob_dy][dt_training[i] == 'y']

    #Laplace Smoothing

    lbd = 1

    prob_l_r = log2((parties_r_count + lbd)/(len(dt) + 2*lbd))
    prob_l_d = log2((parties_d_count + lbd)/(len(dt) + 2*lbd))

    for dt_training in training_set:
        for i in range(len(dt_training)):
            current_prob_ry = log2((col_ry[i] + lbd)/(parties_r_count + len(dt_training)*lbd))
            current_prob_rn = log2((col_rn[i] + lbd)/(parties_r_count + len(dt_training)*lbd))
            current_prob_rq = log2((col_rq[i] + lbd)/(parties_r_count + len(dt_training)*lbd))
            current_prob_dy = log2((col_dy[i] + lbd)/(parties_d_count + len(dt_training)*lbd))
            current_prob_dn = log2((col_dn[i] + lbd)/(parties_d_count + len(dt_training)*lbd))
            current_prob_dq = log2((col_dq[i] + lbd)/(parties_d_count + len(dt_training)*lbd))

            prob_l_r += [[current_prob_rq, current_prob_rn][dt_training[i] == 'n'], current_prob_ry][dt_training[i] == 'y']
            prob_l_d += [[current_prob_dq, current_prob_dn][dt_training[i] == 'n'], current_prob_dy][dt_training[i] == 'y']

        prob_l_r = 2**prob_l_r
        prob_l_d = 2**prob_l_d

        if (prob_l_r > prob_l_d):
            result.append('republican')
        else: result.append('democrat')

    current_accuracy = accuracy_score(training_labels, result)
    print(f"Split {index + 1} ({len(training_set)} records): {current_accuracy}")
    sum_of_accuracy += current_accuracy

print(f"Average accuracy: {sum_of_accuracy/10}")
