import imdata

from PIL import Image
from sklearn import svm
from sklearn import preprocessing
import os
import numpy
import pylab
import warnings
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D


types = ['clutch','hobo','flats','pumps']
trainingData = []
tuningData = []
testingData = []

#Get labels for training, tuning, or testing sets
def getLabel(dir, clothing, classificationGroup):
     y = []

     if (classificationGroup == 'test'):
          for subdir, dirs, files in os.walk(dir+"/Testing"):
               for file in files:
                    if 'clutch' in file:
                         y.append(0)
                    elif 'hobo' in file:
                         y.append(1)
                    elif 'flats' in file:
                         y.append(2)
                    else:
                         y.append(3)

     elif (classificationGroup == 'train'):
          for subdir, dirs, files in os.walk(dir+"/Training"):
               for file in files:
                    if clothing in file:
                         y.append(1)
                    else:
                         y.append(-1)
     else:
           for subdir, dirs, files in os.walk(dir+"/Tuning"):
               for file in files:
                    if clothing in file:
                         y.append(1)
                    else:
                         y.append(-1)
     return y

#get representation of training images
def getTrainingData(dir, dataType):

    for subdir, dirs, files in os.walk(dir+"/Training"):
        for file in files:
             if (dataType == "bin"):
                  trainingData.append(imdata.pixelBin(dir+"/Training/"+file))
             else:
                  trainingData.append(imdata.getThumbnail(dir+"/Training/"+file))

#get representation of tuning images
def getTuningData(dir, dataType):

    for subdir, dirs, files in os.walk(dir+"/Tuning"):
        for file in files:
             if (dataType == "bin"):
                  tuningData.append(imdata.pixelBin(dir+"/Tuning/"+file))
             else:
                  tuningData.append(imdata.getThumbnail(dir+"/Tuning/"+file))

#get representation of testing images
def getTestingData(dir, dataType):

     for subdir, dirs, files in os.walk(dir+"/Testing"):
          for file in files:
               if (dataType == "bin"):
                    testingData.append(imdata.pixelBin(dir+"/Testing/"+file))
               else:
                    testingData.append(imdata.getThumbnail(dir+"/Testing/"+file))

#find how well parameters work
def tune(dir,label,clothing,clf):
    correct = 0;
    total = 0;
    tuningLabel = getLabel(dir,clothing,'tune')
    sz = len(tuningData[0])

    for i in range(0,len(tuningData)):
         if clf.predict_proba(tuningData[i])[0][1] >= 0.5:
              decision = 1
         else:
              decision = -1

         if decision == 1 and tuningLabel[i] == 1:
              correct += 1
         elif decision == -1 and tuningLabel[i] == -1:
              correct += 1
         total += 1

    print 'C = ' + str(clf.C) + ', gamma = '+str(clf.gamma)+ ' -- ' + str(correct)+'/'+str(total)+' on ' + clothing
    return correct


#search for parameters/fit data
#currently it will just print outputs of c and gamma values for each of four image types
def trainSVMs(dir,dataType, kernel):
     getTrainingData(dir,dataType)
     getTuningData(dir,dataType)

     c = [1,10,100,1000,10000,100000] #parameters to test
     g = [0.0001,0.001,0.01,0.1,1]

     svmArr = [] #array to best SVMs of each image type

     for i in range(0,4):
          label = getLabel(dir,types[i],'train')
	  correct = -1
          bestSVM = svm.SVC()

          for j in range(0,6):
               clf = svm.SVC()
               clf.C = c[j]
               clf.kernel = kernel
               clf.probability = True
               if kernel == 'rbf':
                    for k in range(0,5):
                         clf.gamma = g[k]
                         clf.fit(trainingData,label)
                         numCorrect = tune(dir,label,types[i],clf)
                         if numCorrect > correct:
                              bestSVM = clf
			      correct = numCorrect
               else:
                    clf.fit(trainingData,label)
                    numCorrect = tune(dir,label,types[i],clf)
                    if numCorrect > correct:
                         bestSVM = clf
			 correct = numCorrect
          svmArr.append(bestSVM)
          print

     print svmArr
     getTestingData(dir,dataType)
     classifier(dir,svmArr)




#classify each image in test data as image type with largest probability
def classifier(dir, svmArr):
     y = getLabel(dir,'assd','test') #labels of test set
     confusionMatrix = numpy.zeros((4,4))
     correctnessArr = [0,0,0,0]
     wrongPics = [] #index, predicted, actual

     for i in range(0,len(y)):
          prob = -1
          classType = -1
          for j in range(0,4):
               tempProb = svmArr[j].predict_proba(testingData[i])[0][1]
               if tempProb > prob:
                    classType = j
                    prob = tempProb
          if classType == y[i]:
               correctnessArr[classType] += 1

          else:
               confusionMatrix[y[i]][classType] += 1
               wrongPics.append([i,classType,y[i]])


     print correctnessArr
     print confusionMatrix
     #getWrongPics(dir,wrongPics)

#print names of misclassified images
def getWrongPics(dir, wrongPics):
     fileNames = []
     for subdir, dirs, files in os.walk(dir+"/Testing"):
          for file in files:
              fileNames.append(file)

     for i in range(0,len(wrongPics)):
          print fileNames[wrongPics[i][0]] + " predicted: " + types[wrongPics[i][1]]


with warnings.catch_warnings():
    warnings.simplefilter("ignore")
    directory = raw_input()
    dataType = raw_input() #32x32 ('anything not bin keyword') or hist ('bin' keyword)
    kernel = raw_input()
    trainSVMs(directory,dataType,kernel)
