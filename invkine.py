import math
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()

x = 1
y = 150

l1 = 168
l2 = 168
l3 = 40

theta = math.radians(90)

A = y - l3*math.sin(theta);
B = x - l3*math.cos(theta);
C = (A*A + B*B + l1*l1 - l2*l2)/(2*l1);
th1 = math.atan2(A, B) - math.atan2(+math.sqrt(A*A+B*B-C*C), C);

D = (A*A + B*B - l1*l1 + l2*l2)/(2*l2);
th2 = math.atan2(+math.sqrt(A*A+B*B-C*C), C) - math.atan2(-math.sqrt(A*A+B*B-D*D), D);

th3 = theta - th1 - th2;

print(A,B,C,D)
print(math.degrees(th1))
print(math.degrees(th2))
print(math.degrees(th3))

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
plt.ylim(0,380)

plt.show()
