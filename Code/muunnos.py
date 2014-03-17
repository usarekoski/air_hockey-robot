#Simppeli laskutapa kulmien ratkaisuun.

import math

s = 180 #varsi
l = 300 #mottoriväli
# 10mm aidoilla ja 60mm mailalla:
# Xmin=65mm, Xmax=235, Ymin=30, Ymax=140mm

pi = 3.141592653

x=float(input('''anna x\n''')) #koordinaatit
y=float(input('''anna y\n'''))

y2 = y #toinen puoli lasketaan peilikuvana
x2 = l-x
######### vasen moottori
theta11 = math.atan2(y,x) #koordinaattivektorin kulma
theta11 = theta11*180 #muunnos asteiksi
theta11 = theta11/pi
#
a1 = math.hypot(x,y) #koordinaattivektorin pituus
theta12 = math.acos(a1/(2*s)) #kulma koordinaattivektorista varteen

theta12 = theta12*180 #muunnos asteiksi
theta12 = theta12/pi
#
theta1 = theta11 + theta12 #haettu kulma on näiden summa
######### oikea moottori
theta21 = math.atan2(y2,x2)
theta21 = theta21*180
theta21 = theta21/pi
#
a2 = math.hypot(x2,y2)
theta22 = math.acos(a2/(2*s))

theta22 = theta22*180
theta22 = theta22/pi
#
theta2 = theta21 + theta22
########
print(theta1)
print(theta2)

