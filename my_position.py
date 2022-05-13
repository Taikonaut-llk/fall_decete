import cv2
import mediapipe as mp
#import time

myPose = mp.solutions.pose  # 求肢体的解决方法
pose = myPose.Pose()        # 获得肢体的坐标
mpDraw= mp.solutions.drawing_utils  # 画出坐标的方法
# cap = cv2.VideoCapture("D:\QQ\pic\group.mp4")


def my_funtion(img):
	# while True:
	#success, img = cap.read()
	# fx = 0.5 
	# fy = 0.5  
	# 
	# img = cv2.resize(img, (0, 0), fx=fx, fy=fy, interpolation=cv2.INTER_CUBIC)  

	# with mp_pose.Pose(
	# 		min_detection_confidence=0.5, 
	# 		min_tracking_confidence=0.5) as pose:
	imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
	results = pose.process(imgRGB)   #获取坐标，转换坐标格式，存放在results
	newList2 = []
	target = [13, 14, 15, 16, 23, 24, 25, 26, 27, 28]
	#if results.pose_landmarks:       # if results里有坐标， 就画
	mpDraw.draw_landmarks(img, results.pose_landmarks, myPose.POSE_CONNECTIONS)  # draw出来

	for id, lm in enumerate(results.pose_landmarks.landmark):   # 对于每一个点

		h, w, c = img.shape
		cx, cy = int(lm.x * w), int(lm.y * h) # 显示的lm.x lm.y是在1里面的比例
		# print(id, cx, cy)
		if id in target:                      #把点的.(x, y)放到一个数据结构里
			newList2.append(cx)
			newList2.append(cy)
			# print(id, cx, cy)
			
		#print(id, cx, cy)
		#cv2.circle(img, (cx, cy), 5, (255, 0, 0), cv2.FILLED)
	#return 这个.(x,y)的数组
		cv2.imshow('img', img)
		# if cv2.waitKey(1): 
		# 	print("字体字体字体字体字体字体字体字体字体字体字体")
		# 	break
	return newList2

	# if cv2.waitKey(1) == 27: 
	# 	break
# cap.release()
# destoryAllwindows()

#fun()