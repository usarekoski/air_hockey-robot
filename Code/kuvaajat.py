#Simppeli laskutapa kulmien ratkaisuun.

import math
import numpy as np
import matplotlib.pyplot as plt

s = 180 #varsi
l = 300 #mottoriväli
# 10mm aidoilla ja 60mm mailalla:
# Xmin=65mm, Xmax=235, Ymin=30, Ymax=140mm

pi = 3.141592653

x=float(input('''anna x\n''')) #koordinaatit
#y=float(input('''anna y\n'''))

#Määritetään y:n määrittelyalue
#x:n ja y:n voi vaihtaa toisteppäin, jos vaihtaa kummalle otetaan alkuarvo
#ja minkä suhteen plotataan
#x = np.linspace(65,235)
y = np.linspace(30,140)

y2 = y #toinen puoli lasketaan peilikuvana
x2 = l-x
######### vasen moottori
theta11 = np.arctan2(y,x) #koordinaattivektorin kulma
theta11 = theta11*180 #muunnos asteiksi
theta11 = theta11/pi
#
a1 = np.hypot(x,y) #koordinaattivektorin pituus
theta12 = np.arccos(a1/(2*s)) #kulma koordinaattivektorista varteen

theta12 = theta12*180 #muunnos asteiksi
theta12 = theta12/pi
#
theta1 = theta11 + theta12 #haettu kulma on näiden summa
######### oikea moottori
theta21 = np.arctan2(y2,x2)
theta21 = theta21*180
theta21 = theta21/pi
#
a2 = np.hypot(x2,y2)
theta22 = np.arccos(a2/(2*s))

theta22 = theta22*180
theta22 = theta22/pi
#
theta2 = theta21 + theta22
########
#print(theta1)
#print(theta2)

#piirretään kuvaajat tietyllä x:n arvolla
#vaaka-akselilla y
plt.plot(y,theta1)
plt.plot(y,theta2)
plt.show()
