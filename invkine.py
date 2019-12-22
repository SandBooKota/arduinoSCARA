import math
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()

x = 120
y = 150

l1 = 147
l2 = 147
l3 = 40
offset1 = 2
offset2 = 1
offset3 = -2

theta = math.radians(90)

A = y - l3*math.sin(theta);
B = x - l3*math.cos(theta);
C = (A*A + B*B + l1*l1 - l2*l2)/(2*l1);
th1 = math.atan2(A, B) - math.atan2(+math.sqrt(A*A+B*B-C*C), C);

D = (A*A + B*B - l1*l1 + l2*l2)/(2*l2);
th2 = math.atan2(+math.sqrt(A*A+B*B-C*C), C) - math.atan2(-math.sqrt(A*A+B*B-D*D), D);

th3 = theta - th1 - th2;

print(A,B,C,D)
print(215 - math.degrees(th1) + offset1)
print(125 - math.degrees(th2) + offset2)
print(125 - math.degrees(th3) + offset3)

x1 = l1 * math.cos(th1)
y1 = l1 * math.sin(th1)

x2 = x1 + (l2 * math.cos((th1+th2)))
y2 = y1 + (l2 * math.sin((th1+th2)))

x3 = x2 + (l3 * math.cos((th1+th2+th3)))
y3 = y2 + (l3 * math.sin((th1+th2+th3)))

x0 = [0, x1, x2, x3]
y0 = [0, y1, y2, y3]

plt.plot(x0,y0,"r-")

plt.xlim(-170,170)
plt.ylim(-100,380)

plt.show()
