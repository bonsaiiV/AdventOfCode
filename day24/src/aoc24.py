#!/usr/bin/python3
 
#If standing on a hail we will see the rock travel in a straight line,
#pass through us and two other points on two other pieces of hail that zip by
#there must be two vectors from our hail to the other two collisions (v1 and v2)
#such that v1 = m * v2 where m is some unknown scalar multiplier.
#we can make v1 = v2 by dividing one of the x,y or z components by itself to ensure
#it is equal to 1. Then solve.
 
#Select three hail so that the relative, x, y or z is all zero
#Hail 0
#176253337504656, 321166281702430, 134367602892386 @ 190, 8, 338
#Hail 1
#308344120080284, 193172753823598, 249535698501761 @ 60, 8, 134
#Hail 2
#307032218923206, 220427490765998, 286976738475573 @ 29, 8, 58
"""
255167466745174, 320609684190143, 294940767243330 @ 16, 9, -11
224010996119509, 140759350243343, 288485281691650 @ 121, 9, -351
201073555066112, 234125526022447, 407102419342779 @ 118, 9, -316
"""
 
vx0 = 16
vy0 = 9
vz0 = -11
 
vx1 = 121
vy1 = 9
vz1 = -351
 
vx2 = 118
vy2 = 9
vz2 = -316
 
x0 = 255167466745174
y0 = 320609684190143
z0 = 294940767243330
 
x1 = 224010996119509
y1 = 140759350243343
z1 = 288485281691650
 
x2 = 201073555066112
y2 = 234125526022447
z2 = 407102419342779
"""
255167466745174, 320609684190143, 294940767243330 @ 16, 9, -11
224010996119509, 140759350243343, 288485281691650 @ 121, 9, -351
201073555066112, 234125526022447, 407102419342779 @ 118, 9, -316
"""
 
#calculate relative velocities of hail 1 and 2 to hail 0
#the y component is zero due to selection of hail
vxr1 = vx1 - vx0
vzr1 = vz1 - vz0
vxr2 = vx2 - vx0
vzr2 = vz2 - vz0
 
#relative initial position of hail 1
xr1 = x1 - x0
yr1 = y1 - y0
zr1 = z1 - z0
 
#relative initial position of hail 2
xr2 = x2 - x0
yr2 = y2 - y0
zr2 = z2 - z0
 
# 1st Hail equations
# x = xr1 + vxr1*t1
# y = yr1
# z = zr1 + vzr1*t1
 
# 2nd Hail equations
# x = xr2 + vxr2*t2
# y = yr2
# z = zr2
 
#divide all equations by the y component to make y = 1 and ensure both vectors are the same
# 1st Set results
# x = (xr1+vxr1*t1)/yr1
# y = 1
# z = (zr1+vzr1*t1)/yr1
 
#2nd Set results
# x = (xr2+vxr2*t2)/yr2
# y = 1
# z = (zr2+vzr2*t2)/yr2
 
#Solve set of two linear equations x=x and z=z
num = (yr2*xr1*vzr1)-(vxr1*yr2*zr1)+(yr1*zr2*vxr1)-(yr1*xr2*vzr1)
den = yr1*((vzr1*vxr2)-(vxr1*vzr2))
t2 = num / den
 
#Substitute t2 into a t1 equation
num = (yr1*xr2)+(yr1*vxr2*t2)-(yr2*xr1)
den = yr2*vxr1
t1 = num / den
print('t1 time of first vector @ collision', t1)
print('t2 time of second vector @ collision', t2)
 
#calculate collision position at t1 and t2 of hail 1 and 2 in normal frame of reference
cx1 = x1 + (t1*vx1)
cy1 = y1 + (t1*vy1)
cz1 = z1 + (t1*vz1)
 
cx2 = x2 + (t2*vx2)
cy2 = y2 + (t2*vy2)
cz2 = z2 + (t2*vz2)
print('collision one occurs @', cx1, cy1, cz1)
print('collision two occurs @', cx2, cy2, cz2)
 
#calculate the vector the rock travelled between those two collisions
xm = (cx2-cx1)/(t2-t1)
ym = (cy2-cy1)/(t2-t1)
zm = (cz2-cz1)/(t2-t1)
print('rock vector', xm, ym, zm)
 
#calculate the initial position of the rock based on its vector
xc = cx1 - (xm*t1)
yc = cy1 - (ym*t1)
zc = cz1 - (zm*t1)
print('rock inital position', xc, yc, zc)
print('answer', int(xc+yc+zc))
