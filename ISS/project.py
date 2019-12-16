import matplotlib
matplotlib.use('Agg')
from scipy.io import wavfile
import numpy as np
import matplotlib.pyplot as plt
import soundfile as sf
import IPython
from scipy.signal import spectrogram, lfilter, freqz, tf2zpk
import scipy

data, fs = sf.read('sa1.wav')
data = data[:250000]
time = np.arange(data.size) / fs



f, t, sgr =spectrogram(data, fs, nperseg=400, noverlap=240, nfft=512)


sgr_log = 10 * np.log10(sgr+1e-20) 

new_sgr = np.zeros([sgr.shape[0] // 16, sgr.shape[1]])

temp = sucet = 0
tempstop= 15
for i in range(len(sgr_log[0])):
    for j in range(len(sgr_log)):
        sucet += sgr_log[j][i]
        if j == tempstop:
            new_sgr[temp][i] = sucet
            tempstop += 16
            temp += 1
            sucet = 0
    temp =0
    tempstop = 15
    
#plt.pcolormesh(new_sgr)
#plt.show()
#plt.savefig('save1.pdf')


#zroobim query
query, fs = sf.read('qa1.wav')
query = query[:250000]
time1 = np.arange(query.size) / fs


f, t1, sgr1 =spectrogram(query, fs, nperseg=400, noverlap=240, nfft=512)


query_log = 10 * np.log10(sgr1+1e-20) 

new_query = np.zeros([sgr1.shape[0] // 16, sgr1.shape[1]])

temp = sucet = 0
tempstop= 15
for i in range(len(query_log[0])):
    for j in range(len(query_log)):
        sucet += query_log[j][i]
        if j == tempstop:
            new_query[temp][i] = sucet
            tempstop += 16
            temp += 1
            sucet = 0
    temp =0
    tempstop = 15



plt.pcolormesh(new_query)
plt.show()
plt.savefig('save.pdf')

A=new_query.transpose()
B=new_sgr.transpose()



C=B.shape[0]-A.shape[0]
vysledky=np.zeros(B.shape[0]-A.shape[0])
vysledok= 0
for i in range(B.shape[0]-A.shape[0]):
    rating=0
    for j in range(A.shape[0]):
        rating ,dudu=scipy.stats.pearsonr(A[j],B[i+j])
        vysledok+=rating
    vysledky[i]=vysledok/A.shape[0]
    vysledok= 0
    
#query2
query1, fs = sf.read('qa22.wav')
query1 = query1[:250000]
time2 = np.arange(query.size) / fs

f, t2, sgr2 =spectrogram(query1, fs, nperseg=400, noverlap=240, nfft=512)


query_log1 = 10 * np.log10(sgr2+1e-20) 

new_query1 = np.zeros([sgr2.shape[0] // 16, sgr2.shape[1]])

temp = sucet = 0
tempstop= 15
for i in range(len(query_log1[0])):
    for j in range(len(query_log1)):
        sucet += query_log1[j][i]
        if j == tempstop:
            new_query1[temp][i] = sucet
            tempstop += 16
            temp += 1
            sucet = 0
    temp =0
    tempstop = 15



plt.pcolormesh(new_query1)
plt.show()
plt.savefig('save.pdf')


C=new_query1.transpose()





vysledky1=np.zeros(B.shape[0]-C.shape[0])
vysledok= 0
for i in range(B.shape[0]-C.shape[0]):
    rating=0
    for j in range(C.shape[0]):
        rating ,dudu=scipy.stats.pearsonr(C[j],B[i+j])
        vysledok+=rating
    vysledky1[i]=vysledok/A.shape[0]
    vysledok= 0

#print(vysledky,vysledky1)
fig, axs = plt.subplots(3)

axs[0].set_title('government and millionaires vs. sx167')
axs[0].plot(time, data)
axs[0].margins(0)
axs[0].set_ylabel('signal')
axs[1].set_xticklabels(['0', '1', '2', '3', '4', '5', '6', '7'])
axs[1].pcolormesh(new_sgr)
axs[1].set_ylabel('features')
axs[1].margins(0)

axs[2].set_xlabel('sec')
axs[2].set_ylabel('scores')
axs[2].margins(0)
axs[2].plot(vysledky, label="government")
axs[2].plot(vysledky1, label="millionaires")
axs[2].legend(loc="lower right")
plt.show()
plt.savefig('save.pdf')