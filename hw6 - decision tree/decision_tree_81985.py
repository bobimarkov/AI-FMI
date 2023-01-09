from math import log2

def calculateProbability (attr, value):
    return len(list(filter(lambda x: x == value, attr)))/len(attr)

def calculateEntropySingleAttr (attr):
    allValues = set(attr)
    entropy = 0
    for i in allValues:
        probability = calculateProbability(attr, i)
        entropy += (-probability*log2(probability))
    return entropy

def calculateEntropyTwoAttr (attr1, attr2):
    attr2Values = set(attr2)
    z = list(zip(attr1, attr2))
    entropy = 0

    for i in attr2Values:
        a = list(map(lambda y: y[0], list(filter(lambda x: x[1] == i, z))))
        entropy += calculateEntropySingleAttr(a)*calculateProbability(attr2, i)
    return entropy

def calculateInformationGain (attr1, attr2):
    return calculateEntropySingleAttr(attr1) - calculateEntropyTwoAttr(attr1, attr2)

def findMostCommonClassInSet(classes):
    availableValues = set(classes)
    mostCommonClass = ""
    maxCountClass = 0
    for value in availableValues:
        count = len([*filter(lambda x: x == value, classes)])
        if count > maxCountClass:
            maxCountClass = count
            mostCommonClass = value

    return mostCommonClass

def ID3(attributes: list[dict], class_: list, fullData: list[dict]):
    if calculateEntropySingleAttr(class_) == 0:
        return class_[0]

    indexes = [*attributes[0]]

    maxGain = 0
    maxGainIndex = indexes[0]
    
    for i in indexes:
        attr = list(map(lambda x: x[i], attributes))
        currentGain = calculateInformationGain(class_, attr)
        
        if currentGain > maxGain:
            maxGain = currentGain
            maxGainIndex = i
    
    maxGainAttrValues = set(map(lambda x: x[maxGainIndex], fullData))
    filteredData = {}
    for i in maxGainAttrValues:
        filteredData[i] = list(filter(lambda x: x[0][maxGainIndex] == i, zip(attributes, class_)))

    subTree = {}
    for data in [*filteredData]:
        filteredAttr = list(map(lambda x: x[0], filteredData[data]))
        classes = list(map(lambda x: x[1], filteredData[data]))

        for j in filteredAttr:
            j.pop(maxGainIndex)

        filteredAttr2 = [[i[v] for v in i] for i in filteredAttr]
        unique_data = [list(x) for x in set(tuple(x) for x in filteredAttr2)]

        if classes != []:
            if len(unique_data) == 1:
                if calculateEntropySingleAttr(classes) == 1:
                    result = findMostCommonClassInSet(class_)
                else:
                    result = findMostCommonClassInSet(classes)
            else:
                result = ID3(filteredAttr, classes, fullData)
        else:
            result = findMostCommonClassInSet(class_)

        subTree[data] = result
    
    return {maxGainIndex:subTree}

def classifier(trainingSet, tree):
    subtree = tree
    current = [*subtree][0]
    while type(subtree) != str:
        current = [*subtree][0]
        subtree = subtree[current][trainingSet[current]]
    
    return subtree

def calculateAccuracy(resultSet, initialSet):
    countSuccess = 0
    for i in range(len(resultSet)):
        if resultSet[i] == initialSet[i]:
            countSuccess += 1
    return countSuccess/len(initialSet)

def resolveQuestionValues(data):
    newData = data
    for i in range(len(newData[0])):
        currentAttr = list(map(lambda x: x[i], newData))
        
        attrWOQuestions = list(filter(lambda x: x != '?', currentAttr))
        
        if len(attrWOQuestions) != len(currentAttr):
            learningData = list(filter(lambda x: x[i] != '?', newData))
            learningData = list(map(lambda x: x[:i]+x[i+1:], learningData))
            
            trainingData = list(filter(lambda x: x[i] == '?', newData))
            trainingData = list(map(lambda x: x[:i]+x[i+1:], trainingData))

            attributesWIndex = []
            for x in learningData:
                current = {}
                for j in range(len(x)):
                    current[j] = x[j]
                attributesWIndex.append(current)
            
            tree = ID3(attributesWIndex, attrWOQuestions, learningData)

            results = []
            for trainData in trainingData:
                classified = classifier(trainData, tree)
                results.append(classified)

            counter = 0
            for j in range(len(newData)):
                if newData[j][i] == '?':
                    newData[j][i] = results[counter]
                    counter += 1

if __name__ == "__main__":
    file = open("./breast-cancer.data", "r").readlines()
    data = list(map(lambda x: x[:-1].split(','), file))

    classIndex = 0

    resolveQuestionValues(data)

    dataAttributes = list(map(lambda x: x[:classIndex]+x[classIndex+1:], data))
   
    splits = list(range(0,len(data), len(data)//10))[1:-1] + [len(data)]

    prev = 0
    accuracySum = 0

    for split in range(len(splits)):
        from_ = prev
        to = splits[split]
        prev = splits[split]

        learningClass = list(map(lambda x: x[classIndex], data[:from_]+data[to:]))
        trainingClass = list(map(lambda x: x[classIndex], data[from_:to]))
        learningAttributes = list(map(lambda x: x[:classIndex]+x[classIndex+1:], data[:from_]+data[to:]))
        trainingAttributes = list(map(lambda x: x[:classIndex]+x[classIndex+1:], data[from_:to]))

        attributesWIndex = []
        for i in learningAttributes:
            current = {}
            for j in range(len(i)):
                current[j] = i[j]
            attributesWIndex.append(current)

        tree = ID3(attributesWIndex, learningClass, dataAttributes)
        print(tree)

        result = []
        for i in trainingAttributes:
            classified = classifier(i, tree)
            result.append(classified)

        acs = calculateAccuracy(result, trainingClass)
        accuracySum += acs
        print(f"Split {split + 1} ({len(trainingAttributes)} records): {acs}")
    
    print(f"Average accuracy: {accuracySum/len(splits)}")
