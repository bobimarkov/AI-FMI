import math
import random
import matplotlib.pyplot as plt
import colorsys

def distance(point: tuple, center: tuple):
    return math.sqrt((point[0]-center[0])**2+(point[1]-center[1])**2)

def meanPointOfPoints(points: list[tuple]):
    x = sum([x[0] for x in points])/len(points)
    y = sum([x[1] for x in points])/len(points)

    return (x,y)
    

def generateCentroids(k: int, data: list[tuple]):
    centroids = []
    chosenIndexes = []
    for i in range(k):
        randomIndex = random.randint(0, len(data))
        while randomIndex in chosenIndexes:
            randomIndex = random.randint(0, len(data))
        chosenIndexes.append(randomIndex)

        chosenPoint = data[randomIndex]
        centroids.append(chosenPoint)

    return centroids

def drawClusters(clusters, centroids):
    clusterIndex = 0
    plt.rcParams['figure.facecolor'] = 'gray'
    fig, ax = plt.subplots()
    for cluster in clusters:
        hsv = (math.fmod(clusterIndex * 0.618033988749895, 1.0), 0.5, math.sqrt(1.0 - math.fmod(clusterIndex * 0.618033988749895, 0.5)))
        ax.plot([x[0] for x in cluster], [x[1] for x in cluster], "o", markersize=2, color=colorsys.hsv_to_rgb(*hsv))
        ax.plot(centroids[clusterIndex][0], centroids[clusterIndex][1], "x", markersize=30, color=colorsys.hsv_to_rgb(*hsv))
        clusterIndex += 1
    plt.savefig("result.png", bbox_inches='tight')
    plt.show()

def calculateWSSD(clusters, centroids):
    totalWSSD = 0
    for clusterIndex in range(len(clusters)):
        currentClusterWSSD = 0
        for point in clusters[clusterIndex]:
            currentClusterWSSD += distance(point, centroids[clusterIndex])
        totalWSSD += currentClusterWSSD
    return totalWSSD

def kMeans(k: int, data: list[tuple]):
    minWSSD = float('inf')
    bestClusters = []
    bestCentroids = []

    for restart in range(30):
        centroids = generateCentroids(k, data)
        clusters = [[] for i in range(k)]
        changeCentroids = True

        iterations = 1
        while changeCentroids and iterations <= 100:
            changeCentroids = True
            clusters = [[] for i in range(k)]
            for point in data:
                minDistance = distance(point, centroids[0])
                minCentroid = 0
                for centroidIndex in range(1, len(centroids)):
                    currentDistance = distance(point, centroids[centroidIndex])
                    if currentDistance < minDistance:
                        minDistance = currentDistance
                        minCentroid = centroidIndex
                clusters[minCentroid].append(point)

            for clusterIndex in range(len(clusters)):
                if clusters[clusterIndex] != []:
                    currentClusterMean = meanPointOfPoints(clusters[clusterIndex])
                    changeCentroids = currentClusterMean[0] != centroids[clusterIndex][0] or currentClusterMean[1] != centroids[clusterIndex][1]
                    centroids[clusterIndex] = currentClusterMean

            # drawClusters(clusters, centroids)
            iterations += 1
        drawClusters(clusters, centroids)
        WSSD = calculateWSSD(clusters, centroids)
        print(WSSD)
        if WSSD < minWSSD:
            minWSSD = WSSD
            bestClusters = clusters
            bestCentroids = centroids 
        
    return (bestClusters, bestCentroids)
            
    

if __name__ == "__main__":
    # file = open("./normal/normal.txt", "r").readlines()
    # data = [*map(lambda x: tuple([float(i) for i in x[:-1].split('\t')]), file)]
    
    file = open("./unbalance/unbalance.txt", "r").readlines()
    data = [*map(lambda x: tuple([float(i) for i in x[:-1].split(' ')]), file)]

    k = 3
    clusters, centroids = kMeans(k, data)
    # drawClusters(clusters, centroids)

