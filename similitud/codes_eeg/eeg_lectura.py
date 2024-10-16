# https://stackoverflow.com/questions/874461/read-mat-files-in-python
# https://docs.scipy.org/doc/scipy/reference/generated/scipy.io.loadmat.html

import sys
import numpy
import scipy.io as sio

if len(sys.argv) != 2:
  print("Error! en la cantidad de argumentos.")
  print(sys.argv[0], "<filename.mat>")
  sys.exit()

print("file:", sys.argv[1])

mat = sio.loadmat(sys.argv[1])


# Print the keys of the MATLAB dictionary
print("--------------------------------------")
print(mat.keys())
print("--------------------------------------")
print("shape:", mat['EEG'].shape)
print("--------------------------------------")
print("dtype:", mat['EEG'].dtype)
print("--------------------------------------")
print("setname:", mat['EEG'][0]['setname'][0].shape)
print("--------------------------------------")
print("filename:", mat['EEG'][0]['filename'][0].shape)
print("--------------------------------------")
print("filepath:", mat['EEG'][0]['filepath'][0].shape)
print("--------------------------------------")
print("subject:", mat['EEG'][0]['subject'][0].shape)
print("--------------------------------------")
print("group:", mat['EEG'][0]['group'][0].shape)
print("--------------------------------------")
print("condition:", mat['EEG'][0]['condition'][0].shape)
print("--------------------------------------")
print("session:", mat['EEG'][0]['session'][0].shape)
print("--------------------------------------")
print("comments:", mat['EEG'][0]['comments'][0].shape)
print("--------------------------------------")
print("nbchan:", mat['EEG'][0]['nbchan'][0].shape)
print("--------------------------------------")
print("trials:", mat['EEG'][0]['trials'][0].shape)
print("--------------------------------------")
print("pnts:", mat['EEG'][0]['pnts'][0].shape)
print("--------------------------------------")
print("srate:", mat['EEG'][0]['srate'][0].shape)
print("--------------------------------------")
print("xmin:", mat['EEG'][0]['xmin'][0].shape)
print("--------------------------------------")
print("xmax:", mat['EEG'][0]['xmax'][0].shape)
print("--------------------------------------")
print("times:", mat['EEG'][0]['times'][0].shape)
print("--------------------------------------")
print("data:", mat['EEG'][0]['data'][0].shape)
print("--------------------------------------")
print("icaact:", mat['EEG'][0]['icaact'][0].shape)
print("--------------------------------------")
print("icawinv:", mat['EEG'][0]['icawinv'][0].shape)
print("--------------------------------------")
print("icasphere:", mat['EEG'][0]['icasphere'][0].shape)
print("--------------------------------------")
print("icaweights:", mat['EEG'][0]['icaweights'][0].shape)
print("--------------------------------------")
print("icachansind:", mat['EEG'][0]['icachansind'][0].shape)
print("--------------------------------------")
print("chanlocs:", mat['EEG'][0]['chanlocs'][0].shape)
print("--------------------------------------")
print("urchanlocs:", mat['EEG'][0]['urchanlocs'][0].shape)
print("--------------------------------------")
print("chaninfo:", mat['EEG'][0]['chaninfo'][0].shape)
print("--------------------------------------")
print("ref:", mat['EEG'][0]['ref'][0].shape)
print("--------------------------------------")
print("event:", mat['EEG'][0]['event'][0].shape)
print("--------------------------------------")
print("urevent:", mat['EEG'][0]['urevent'][0].shape)
print("--------------------------------------")
print("eventdescription:", mat['EEG'][0]['eventdescription'][0].shape)
print("--------------------------------------")
print("epoch:", mat['EEG'][0]['epoch'][0].shape)
print("--------------------------------------")
print("epochdescription:", mat['EEG'][0]['epochdescription'][0].shape)
print("--------------------------------------")
print("reject:", mat['EEG'][0]['reject'][0].shape)
print("--------------------------------------")
print("stats:", mat['EEG'][0]['stats'][0].shape)
print("--------------------------------------")
print("specdata:", mat['EEG'][0]['specdata'][0].shape)
print("--------------------------------------")
print("specicaact:", mat['EEG'][0]['specicaact'][0].shape)
print("--------------------------------------")
print("splinefile:", mat['EEG'][0]['splinefile'][0].shape)
print("--------------------------------------")
print("icasplinefile:", mat['EEG'][0]['icasplinefile'][0].shape)
print("--------------------------------------")
print("dipfit:", mat['EEG'][0]['dipfit'][0].shape)
print("--------------------------------------")
print("history:", mat['EEG'][0]['history'][0].shape)
print("--------------------------------------")
print("saved:", mat['EEG'][0]['saved'][0].shape)
print("--------------------------------------")
print("etc:", mat['EEG'][0]['etc'][0].shape)
print("--------------------------------------")


print(mat['EEG'][0]['data'][0].shape)
print("data dtype:", mat['EEG'][0]['data'][0].dtype)
print("data[0]:")
for i in range(100):
  print("\t",mat['EEG'][0]['data'][0][0][i])
print("chanlocs shape:",mat['EEG'][0]['chanlocs'][0][0].shape)
print("chanlocs[0] shape:",mat['EEG'][0]['chanlocs'][0][0][0].shape)
for i in range(66):
  print("\t",i,"chanloc tag:",mat['EEG'][0]['chanlocs'][0][0][i][0])
# Print the type of the value corresponding to the key 'EEG'
print(type(mat['EEG']))

# Print the shape of the value corresponding to the key 'EEG'
print(numpy.shape(mat['EEG']))

print(type(mat['EEG'][0]))
print(type(mat['EEG'][0][0]))
print(numpy.shape(mat['EEG'][0][0]))
print(numpy.size(mat['EEG'][0][0]))
