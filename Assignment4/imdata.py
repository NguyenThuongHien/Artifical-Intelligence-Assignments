from PIL import Image
import numpy

#after the resizing, if applicable
def isNonColorPic(pic):
     if type(pic[0]) is int:
          return True
     return False

#return numpy array of pixels for grayscale/color
def getImageData(im):
      picture = list(im.getdata())

      if isNonColorPic(picture):
           adjustedPic = []
           for i in range(0,len(picture)):
                color = picture[i]
                adjustedPic.append((color,color,color))
           return adjustedPic

      else:
           return picture

#return list of normalized values from 8x8x8 histogram
def pixelBin(dir):
     im = Image.open(dir)
     rgb = getImageData(im)
     bin = numpy.zeros((8,8,8))
     m,n = im.size
     pixels = m*n

     for vals in rgb:
          x = vals[0]/32
          y = vals[1]/32
          z = vals[2]/32
          bin[x,y,z] += 1
     bin /= pixels
     return bin.flatten()


#numpy array of RGB values for 32x32x3 image
def getThumbnail(dir):
     im = Image.open(dir)
     im.thumbnail([32,32],Image.ANTIALIAS)
     data = getImageData(im)
     data = [x for xs in data for x in xs]
     return data
