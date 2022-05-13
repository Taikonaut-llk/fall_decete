#完整独立运行的Pose，在Python_doc的pipe.py
import cv2
import mediapipe as mp
#import time

myPose = mp.solutions.pose  # 求肢体的解决方法
pose = myPose.Pose()        # 获得肢体的坐标
mpDraw= mp.solutions.drawing_utils  # 画出坐标的方法
# cap = cv2.VideoCapture("D:\QQ\pic\group.mp4")


def my_funtion(img):

	imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
	results = pose.process(imgRGB)   #获取坐标，转换坐标格式，存放在results
	flag = 0
	num = []
	newList2 = []
	target = [11, 13, 23, 25, 27]
	#if results.pose_landmarks:       # if results里有坐标， 就画
	mpDraw.draw_landmarks(img, results.pose_landmarks, myPose.POSE_CONNECTIONS)  # draw出来

	for id, lm in enumerate(results.pose_landmarks.landmark):   # 对于每一个点

		h, w, c = img.shape
		cx, cy = int(lm.x * w), int(lm.y * h) # 显示的lm.x lm.y是在1里面的比例
		print(id, cx, cy)
		
		if id in target or flag == 1:                      #把点的.(x, y)放到一个数据结构里
			if flag == 1:
				num[num.size() - 2] = num[num.size() - 2] + cx / 2
				num[num.size() - 1] = num[num.size() - 1] + cy / 2
			else:
				num.append(cx / 2)
				num.append(cy / 2)

			if id + 1 in target:
				flag = 1
			else :
				flag = 0
			# print(id, cx, cy)

	print("python里面的数据有几个：")
	print(num.size())

	for i in range(0, num.size(), 2):
		for j in range(i + 2, num.size(), 2):
			newList2.append(float(num[i] / num[j]))
			newList2.append(float(num[i + 1] / num[j + 1]))
			

		cv2.imshow('img', img)
		# if cv2.waitKey(1): 
		# 	print("字体字体字体字体字体字体字体字体字体字体字体")
		# 	break
	return newList2
