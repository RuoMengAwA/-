# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainwindow.ui'
##
## Created by: Qt User Interface Compiler version 5.15.2
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide2.QtCore import *
from PySide2.QtGui import *
from PySide2.QtWidgets import *


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        """

        :type MainWindow: object
        """
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
            MainWindow.setObjectName(u"MainWindow")
            MainWindow.resize(1147, 685)
            MainWindow.setStyleSheet("")
            self.centralwidget = QWidget(MainWindow)
            self.centralwidget.setObjectName("centralwidget")
            self.gridLayout = QGridLayout(self.centralwidget)
            self.gridLayout.setContentsMargins(12, 12, 12, 12)
            self.gridLayout.setHorizontalSpacing(8)
            self.gridLayout.setVerticalSpacing(0)
            self.gridLayout.setObjectName("gridLayout")

            self.label = QLabel(self.centralwidget)
            self.label.setStyleSheet("border-radius: 12px;background-color: rgb(221, 221, 221);")
            self.label.setFrameShape(QFrame.StyledPanel)
            self.label.setText("")
            self.label.setObjectName("label")

            self.gridLayout.addWidget(self.label, 0, 1, 1, 1)
            self.frame_2 = QFrame(self.centralwidget)
            self.frame_2.setMinimumSize(QSize(380, 496))
            self.frame_2.setStyleSheet("border-radius: 12px;\n"
                                       "background-color: rgb(255, 255, 255);")
            self.frame_2.setFrameShape(QFrame.StyledPanel)
            self.frame_2.setFrameShadow(QFrame.Raised)
            self.frame_2.setObjectName("frame_2")
            self.horizontalLayout_2 = QHBoxLayout(self.frame_2)
            self.horizontalLayout_2.setObjectName("horizontalLayout_2")
            self.leftTopLayout = QVBoxLayout()
            self.leftTopLayout.setContentsMargins(24, 24, 24, 24)
            self.leftTopLayout.setSpacing(12)
            self.leftTopLayout.setObjectName("leftTopLayout")
            self.horizontalLayout_4 = QHBoxLayout()
            self.horizontalLayout_4.setObjectName("horizontalLayout_4")
            self.label_2 = QLabel(self.frame_2)
            self.label_2.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_2.setObjectName("label_22")
            self.horizontalLayout_4.addWidget(self.label_2)
            self.label_10 = QLabel(self.frame_2)
            self.label_10.setStyleSheet("font: 16pt \"黑体\";\n"
                                        "color: rgb(0,0,0);")
            self.label_10.setObjectName("label_10")
            self.horizontalLayout_4.addWidget(self.label_10)
            spacerItem = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_4.addItem(spacerItem)
            self.leftTopLayout.addLayout(self.horizontalLayout_4)
            self.horizontalLayout_8 = QHBoxLayout()
            self.horizontalLayout_8.setObjectName("horizontalLayout_8")
            self.label_3 = QLabel(self.frame_2)
            self.label_3.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_3.setObjectName("label_3")
            self.horizontalLayout_8.addWidget(self.label_3)
            spacerItem1 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_8.addItem(spacerItem1)
            self.leftTopLayout.addLayout(self.horizontalLayout_8)
            self.horizontalLayout_9 = QHBoxLayout()
            self.horizontalLayout_9.setObjectName("horizontalLayout_9")
            self.label_4 = QLabel(self.frame_2)
            self.label_4.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_4.setObjectName("label_4")
            self.horizontalLayout_9.addWidget(self.label_4)
            spacerItem2 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_9.addItem(spacerItem2)
            self.leftTopLayout.addLayout(self.horizontalLayout_9)
            self.line_2 = QFrame(self.frame_2)
            self.line_2.setMaximumSize(QSize(16777215, 1))
            self.line_2.setStyleSheet("background-color: rgb(153, 153, 153);\n"
                                      "color: rgb(153, 153, 153);")
            self.line_2.setFrameShape(QFrame.HLine)
            self.line_2.setFrameShadow(QFrame.Sunken)
            self.line_2.setObjectName("line_2")
            self.leftTopLayout.addWidget(self.line_2)
            self.horizontalLayout_10 = QHBoxLayout()
            self.horizontalLayout_10.setObjectName("horizontalLayout_10")
            self.label_5 = QLabel(self.frame_2)
            self.label_5.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_5.setObjectName("label_5")
            self.horizontalLayout_10.addWidget(self.label_5)
            self.label_9 = QLabel(self.frame_2)
            self.label_9.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_9.setObjectName("label_9")
            self.horizontalLayout_10.addWidget(self.label_9)
            spacerItem3 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_10.addItem(spacerItem3)
            self.leftTopLayout.addLayout(self.horizontalLayout_10)
            self.horizontalLayout_13 = QHBoxLayout()
            self.horizontalLayout_13.setObjectName("horizontalLayout_13")
            self.label_6 = QLabel(self.frame_2)
            self.label_6.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_6.setObjectName("label_6")
            self.horizontalLayout_13.addWidget(self.label_6)
            spacerItem4 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_13.addItem(spacerItem4)
            self.leftTopLayout.addLayout(self.horizontalLayout_13)
            self.horizontalLayout_11 = QHBoxLayout()
            self.horizontalLayout_11.setObjectName("horizontalLayout_11")
            self.label_7 = QLabel(self.frame_2)
            self.label_7.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_7.setObjectName("label_7")
            self.horizontalLayout_11.addWidget(self.label_7)
            spacerItem5 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_11.addItem(spacerItem5)
            self.leftTopLayout.addLayout(self.horizontalLayout_11)
            self.horizontalLayout_12 = QHBoxLayout()
            self.horizontalLayout_12.setObjectName("horizontalLayout_12")
            self.label_8 = QLabel(self.frame_2)
            self.label_8.setStyleSheet("font: 16pt \"黑体\";\n"
                                       "color: rgb(0,0,0);")
            self.label_8.setObjectName("label_8")
            self.horizontalLayout_12.addWidget(self.label_8)
            spacerItem6 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_12.addItem(spacerItem6)
            self.leftTopLayout.addLayout(self.horizontalLayout_12)
            self.horizontalLayout_6 = QHBoxLayout()
            self.horizontalLayout_6.setSpacing(0)
            self.horizontalLayout_6.setObjectName("horizontalLayout_6")
            self.textBrowser = QTextBrowser(self.frame_2)
            self.textBrowser.setStyleSheet("background-color: rgb(244,244,244);")
            self.textBrowser.setObjectName("textBrowser")
            self.horizontalLayout_6.addWidget(self.textBrowser)
            self.leftTopLayout.addLayout(self.horizontalLayout_6)
            self.horizontalLayout_7 = QHBoxLayout()
            self.horizontalLayout_7.setObjectName("horizontalLayout_7")
            spacerItem7 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_7.addItem(spacerItem7)
            self.menu = QPushButton(self.frame_2)
            self.menu.setStyleSheet("font: 20pt \"黑体\";\n"
                                    "color: rgb(43,158,255);")
            self.menu.setObjectName("menu")
            self.horizontalLayout_7.addWidget(self.menu)
            spacerItem8 = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
            self.horizontalLayout_7.addItem(spacerItem8)
            self.leftTopLayout.addLayout(self.horizontalLayout_7)
            self.horizontalLayout_2.addLayout(self.leftTopLayout)
            self.gridLayout.addWidget(self.frame_2, 0, 0, 2, 1)
            self.gridLayout.setColumnStretch(0, 7)
            self.gridLayout.setColumnStretch(1, 15)
            self.gridLayout.setRowStretch(0, 9)
            MainWindow.setCentralWidget(self.centralwidget)
#########################################################################
        self.retranslateUi(MainWindow)
        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        # self.actionOpen_camera.setText(QCoreApplication.translate("MainWindow", u"Open camera", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_10.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_3.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_4.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_5.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_9.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_6.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_7.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        self.label_8.setText(QCoreApplication.translate("MainWindow", u"TextLabel", None))
        # self.textBrowser.setText(QCoreApplication.translate("MainWindow", u"textBrowser", None))
        # self.menu.setTitle(QCoreApplication.translate("MainWindow", u"Open", None))
    # retranslateUi

    # 消息框显示函数
    def printf(self, mes):
        self.textBrowser.append(mes)  # 在指定的区域显示提示信息
        self.cursot = self.textBrowser.textCursor()
        self.textBrowser.moveCursor(self.cursot.End)


