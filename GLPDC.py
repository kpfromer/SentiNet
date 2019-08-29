import numpy as np
import matplotlib.pyplot as plt
import math

DISCRETIZATION_SIZE=100
PATH_TOL=0.2 #meters

def Bez_Cur(Start,End,Attractor,weight):
	#Start End and Attractor must be x,y pairs, weight [-1,1]
	t=np.arange(DISCRETIZATION_SIZE)/DISCRETIZATION_SIZE
	M=np.zeros([2,len(t)])
	#Generate bezier curve for travel
	for i in range(len(t)):
		s=1-t[i]
		div=s**2+2*weight*s*t[i]+t[i]**2
		M[0][i]=(((s**2)*Start[0]+2*weight*s*t[i]*Attractor[0]+(t[i]**2)*End[0])/div)
		M[1][i]=(((s**2)*Start[1]+2*weight*s*t[i]*Attractor[1]+(t[i]**2)*End[1])/div)

	#generate finite difference velocities
	dM=np.zeros([2,len(t)-1])
	dM[0]=[x-M[0][i-1] for i,x in enumerate(M[0])][1:]
	dM[1]=[x-M[1][i-1] for i,x in enumerate(M[1])][1:]
	return M,dM

def GLPDC(path,pHeadings,position,velocity,backwards):
	#path as 2 by Dis_Size set of discrete path points
	#pHeadings as 2xDis_size-1 set of path headings
	#position as 3x1 x,y,th vector
	#velocity as 2x1 dx,dy vector

	#find closest position on path to determine parameter value [0,1]
	i=np.argmin((path[0]-position[0])**2+(path[1]-position[1])**2)
	t=i/DISCRETIZATION_SIZE

	#determine deviation in heading/desired heading
	if i<DISCRETIZATION_SIZE-1:
		h_dev=np.arctan2(velocity[0]*pHeadings[1,i]-velocity[1]*pHeadings[0,i],velocity[0]*pHeadings[0,i]+velocity[1]*pHeadings[1,i])
	else:
		h_dev=0
	#determine deviation from path wrt allowed error
	p_dev=path[:,i]-position[0:2]
	p_dev_th=np.arctan2(p_dev[1],p_dev[0])
	p_dev_n=np.linalg.norm(p_dev)
	#apply control law on path parameter, heading deviation, path deviation
	throttle=(1-t)*(-1)**backwards

	heading=position[2]
	if backwards:
		if heading>0:
			heading=heading-np.pi
		elif heading<0:
			heading=heading+np.pi
	if p_dev_n>PATH_TOL:
		turn_ratio=(p_dev_th-heading)/np.pi*(-1)**backwards
		throttle=0
		if abs(turn_ratio)<0.1:
			throttle=(-1)**backwards
	elif abs(h_dev)>np.pi/4:
		turn_ratio=h_dev/np.pi*(-1)**backwards
		throttle=0
	else:
		turn_ratio=h_dev/np.pi*(-1)**backwards

	return [throttle, turn_ratio]
start=[0,0]
end=np.array([5,5])
att=[2,-2]
M,dM=Bez_Cur(start,end,att,1)
pos=np.array([3.,0.,0.])
vel=np.array([0.,0.])
pos_cat_x=[]
pos_cat_y=[]
counter=0
while np.linalg.norm(end-pos[0:2])>PATH_TOL and counter<10000:
	counter+=1
	throttle_turn=GLPDC(M,dM,pos,vel,0)
	pos_cat_x.append(pos[0])
	pos_cat_y.append(pos[1])
	vel=np.array([np.cos(pos[2])*0.2*throttle_turn[0],np.sin(pos[2]*0.2*throttle_turn[0])])
	pos+=np.array([vel[0]*0.1,vel[1]*0.1,throttle_turn[1]*0.1])


plt.plot(M[0],M[1])
plt.plot(pos_cat_x,pos_cat_y)
plt.show()