import numpy as np

a = np.array([2,3,4,5,6,7])


print(a.data)


# @ operator is a matrix multiplication or .dot()

#reshape is returned, resize is in place

#hstack add them side by side, vstack add then on top of each other

#array split unequal size splitting

#e[..., 1] all : until last 1

#slice by array, can take individial elemnts and put them in a new shaped array - have to index with a numpy array

#can cretae boolean masks, e.g. a[a>4]


#python is always pass by reference!!

#b = a is not a copy, b refers to a

#c = a.view() - c is not a, changing c doesnt change a, share same data
#deep copy - .copy dont share data

#scipy .eig will return h-stacked eigenvectors and a list of eigenvaluues