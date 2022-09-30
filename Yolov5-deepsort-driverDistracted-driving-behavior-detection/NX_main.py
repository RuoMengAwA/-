# -*- coding: utf-8 -*-
# 主函数
import sys
import cv2
from PySide2 import QtWidgets, QtCore, QtGui
from PySide2.QtWidgets import QMainWindow, QFileDialog, QMessageBox
from PySide2.QtCore import QDir, QTimer, Slot
from PySide2.QtGui import QPixmap, QImage
from ui_mainwindow_ import Ui_MainWindow
import time
from transfer import Service
import myframe

# 定义变量

# 眼睛闭合判断
EYE_AR_THRESH = 0.20  # 眼睛长宽比
EYE_AR_CONSEC_FRAMES = 2  # 闪烁阈值

# 嘴巴开合判断
MAR_THRESH = 0.65  # 打哈欠长宽比
MOUTH_AR_CONSEC_FRAMES = 3  # 闪烁阈值

last_time = 0
last1_time = 0
last_action = ''

# 定义检测变量，并初始化
COUNTER = 0  # 眨眼帧计数器
TOTAL = 0  # 眨眼总数
mCOUNTER = 0  # 打哈欠帧计数器
mTOTAL = 0  # 打哈欠总数
ActionCOUNTER = 0  # 分心行为计数器器

# 疲劳判断变量
# Perclos模型
# perclos = (Rolleye/Roll) + (Rollmouth/Roll)*0.2
Roll = 0  # 整个循环内的帧技术
Rolleye = 0  # 循环内闭眼帧数
Rollmouth = 0  # 循环内打哈欠数


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        # 打开文件类型，用于类的定义
        self.f_type = 0
         # Ui_MainWindow.printf(window, "正在打开摄像头请稍后...")
        # 设置时钟
        self.v_timer = QTimer()
        # 打开摄像头
        self.cap = cv2.VideoCapture(0)
        # 设置定时器周期，单位毫秒
        self.v_timer.start(20)
        # 连接定时器周期溢出的槽函数，用于显示一帧视频
        self.v_timer.timeout.connect(self.show_pic)
        # 在前端UI输出提示信息
        #  Ui_MainWindow.printf(window, "载入成功，开始运行程序")
        #  Ui_MainWindow.printf(window, "")
        #  window.statusbar.showMessage("正在使用摄像头...")

        self.t_p = UploadThread("phone", "phone.jpg")
        self.t_d = UploadThread("drink", "drink.jpg")
        self.t_s = UploadThread("smoke", "smoke.jpg")
        self.t_f = UploadThread("fatigue", "fatigue.jpg")


    def window_init(self):
        # 设置控件属性
        # 设置label的初始值
        self.label.setText("请打开摄像头")
        self.label_2.setText("疲劳检测：")
        self.label_3.setText("眨眼次数：0")
        self.label_4.setText("哈欠次数：0")
        self.label_5.setText("行为检测：")
        self.label_6.setText("手机")
        self.label_7.setText("抽烟")
        self.label_8.setText("喝水")
        self.label_9.setText("是否存在分心行为")
        self.label_10.setText("是否为疲劳状态")
        self.menu.setText("打开")
        # 菜单按钮 槽连接 到函数
        self.menu.clicked.connect(CamConfig_init)
        # 自适应窗口缩放
        self.label.setScaledContents(True)


    def show_pic(self):
        # 全局变量
        # 在函数中引入定义的全局变量
        global EYE_AR_THRESH, EYE_AR_CONSEC_FRAMES, MAR_THRESH, MOUTH_AR_CONSEC_FRAMES, COUNTER, TOTAL, mCOUNTER, mTOTAL, ActionCOUNTER, Roll, Rolleye, Rollmouth, data, last_time, last_action, last1_time
        # 读取摄像头的一帧画面
        success, frame = self.cap.read()
        frame = cv2.flip(frame, 1)
        if success:
            # 检测
            # 将摄像头读到的frame传入检测函数myframe.frametest()
            origin_frame = frame.copy()
            ret, frame = myframe.frametest(frame)
            lab, eye, mouth = ret
            # ret和frame，为函数返回
            # ret为检测结果，ret的格式为[lab,eye,mouth],lab为yolo的识别结果包含'phone' 'smoke' 'drink',eye为眼睛的开合程度（长宽比），mouth为嘴巴的开合程度
            # frame为标注了识别结果的帧画面，画上了标识框

            # 分心行为判断
            # 分心行为检测以15帧为一个循环
            ActionCOUNTER += 1

            # 如果检测到分心行为u
            # 将信息返回到前端ui，使用红色字体来体现
            # 并加ActionCOUNTER减1，以延长循环时间
            if ActionCOUNTER > 0:  # 检测到分心行为后5帧不再进行行为检测
                for i in lab:
                    if (i == "phone"):
                        if last_action == "phone" and time.time() - last_time < 5:

                            window.label_6.setText("<font color=red>正在用手机</font>")
                           
                            window.label_9.setText("<font color=red>请不要分心</font>")
                        
                            continue
                        last_action = "phone"
                        last_time = time.time()
                        print(last_time)
                        cv2.imwrite("phone.jpg", origin_frame)
                        api.newevent("phone", "phone.jpg")

                        self.t_p.start()
                        if ActionCOUNTER > 0:
                            ActionCOUNTER = -5
                    elif (i == "smoke"):
                        if last_action == "smoke" and time.time() - last_time < 5:

                            window.label_7.setText("<font color=red>正在抽烟</font>")
                            window.label_9.setText("<font color=red>请不要分心</font>")
                        
                            continue
                        last_action = "smoke"
                        last_time = time.time()
                        cv2.imwrite("smoke.jpg", origin_frame)
                        # api.newevent("smoke", "smoke.jpg")

                        self.t_s.start()
                        if ActionCOUNTER > 0:
                            ActionCOUNTER = -5
                    elif i == "drink":
                        if last_action == "drink" and time.time() - last_time < 5:

                            window.label_8.setText("<font color=red>正在用喝水</font>")
                            window.label_9.setText("<font color=red>请不要分心</font>")
                            
                            continue
                        last_action = "drink"
                        last_time = time.time()
                        cv2.imwrite("drink.jpg", origin_frame)
                        # api.newevent("drink", "drink.jpg")

                        self.t_d.start()
                        if ActionCOUNTER > 0:
                            ActionCOUNTER = -5

            # 如果超过15帧未检测到分心行为，将label修改为平时状态
            if ActionCOUNTER == 5:  # 改为5帧
                window.label_6.setText("手机")
                window.label_7.setText("抽烟")
                window.label_8.setText("喝水")
                window.label_9.setText("")
                ActionCOUNTER = 0

            # 疲劳判断
            # 眨眼判断
            if eye < EYE_AR_THRESH:
                # 如果眼睛开合程度小于设定好的阈值
                # 则两个和眼睛相关的计数器加1
                COUNTER += 1
                Rolleye += 1
            else:
                # 如果连续2次都小于阈值，则表示进行了一次眨眼活动
                if COUNTER >= EYE_AR_CONSEC_FRAMES:
                    TOTAL += 1
                    window.label_3.setText("眨眼次数：" + str(TOTAL))
                    # 重置眼帧计数器
                    COUNTER = 0

            # 哈欠判断，同上
            if mouth > MAR_THRESH:
                mCOUNTER += 1
                Rollmouth += 1
            else:
                # 如果连续3次都小于阈值，则表示打了一次哈欠
                if mCOUNTER >= MOUTH_AR_CONSEC_FRAMES:
                    mTOTAL += 1
                    window.label_4.setText("哈欠次数：" + str(mTOTAL))
                    # 重置嘴帧计数器
                    mCOUNTER = 0

            # 将画面显示在前端UI上
            show = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            showImage = QImage(show.data, show.shape[1], show.shape[0], QImage.Format_RGB888)
            window.label.setPixmap(QPixmap.fromImage(showImage))

            # 疲劳模型
            # 疲劳模型以150帧为一个循环
            # 每一帧Roll加1
            Roll += 1
            # 当检测满150帧时，计算模型得分
            if Roll == 30:
                # 计算Perclos模型得分
                perclos = (Rolleye / Roll) + (Rollmouth / Roll) * 0.2
                # 在前端UI输出perclos值

                # Ui_MainWindow.printf(window, "过去150帧中，Perclos得分为" + str(round(perclos, 3)))
                # 当过去的150帧中，Perclos模型得分超过0.38时，判断为疲劳状态
                if perclos > 0.38 and time.time() - last1_time > 5:
                    Ui_MainWindow.printf(window, "当前处于疲劳状态")
                    window.label_10.setText("<font color=red>疲劳！！！</font>")
                    Ui_MainWindow.printf(window, "")
                    last1_time = time.time()
                    cv2.imwrite("fatigue.jpg", origin_frame)
                    api.newevent("fatigue", "fatigue.jpg")

                    self.t_f.start()
                else:
                    Ui_MainWindow.printf(window, "当前处于清醒状态")
                    window.label_10.setText("清醒")
                    Ui_MainWindow.printf(window, "")

                # 归零
                # 将三个计数器归零
                # 重新开始新一轮的检测
                Roll = 0
                Rolleye = 0
                Rollmouth = 0
                Ui_MainWindow.printf(window, "重新开始执行疲劳检测...")


class UploadThread(QtCore.QThread):

    def __init__(self, type, image_path):
        super().__init__()
        self.type = type
        self.image_path = image_path
        self.flag = True

    def run(this):
        api.newevent(this.type, this.image_path)
        this.stop()

    def stop(self):
        self.exit()


def CamConfig_init():
    window.f_type = MainWindow()


if __name__ == '__main__':
    enID = 'j1wbLNFholppgePX/VpOryVMYDgfNA1Xf+f2Bn9sSpyKiU7ztJGD4B8AjYa1d7OG3IV4L96XNwl4uTn6u52ZkLfHdXXjGl40/HJgBSDsykkASQPiEeynmssfUrfoW5Yl/VYcBFUdKupbaLoBm30f4e3KzeBQg8y0dQuuRDD2SsE='
    api = Service(enID)
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.window_init()
    window.show()
    sys.exit(app.exec_())
