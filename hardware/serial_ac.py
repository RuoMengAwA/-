# # -*- coding: utf-8 -*-
# """
# @Time: 2022/4/10 19:31
# @Author: Veera
# @Version: 3.9.0
# @File: serial_ac.py.py
# @Software: PyCharm
# """
# # -*- coding: utf-8 -*-
# import serial
# import json
# import time
#
#
# class Alcohol_serial(object):
#     def __init__(self, port, baudrate=9600, readtimeout=0.5):
#         self._serial = None
#         self.port = port
#         self.baudrate = baudrate
#         self.readtimeout = readtimeout
#         self.parity = 'N'
#         self.ser_connect()
#
#     def ser_connect(self):
#         """
#         Connect to the stm32
#         :return: none
#         """
#         if self._serial is not None:
#             self.ser_disconnect()
#         try:
#             self._serial = serial.Serial(parity=self.parity, port=self.port,
#                                          baudrate=self.baudrate, timeout=self.readtimeout)
#         except serial.SerialException as e:
#             print("Failed to connect to the meter "
#                   'due to: %s' % e)
#
#     def ser_disconnect(self):
#         """
#         Disconnects from the serial port
#         :return: none
#         """
#         if self._serial is None:
#             return
#         self._serial.close()
#
#     def ser_send_cmd(self, cmd_num):
#         """
#         Send command form port to stm32
#         发送命令
#         { 'cmd': 1 }
#         :return: none
#         """
#         # cmd = {"cmd": cmd_num}
#         # data = json.dumps(cmd)
#         # byte_cmd = bytes( data,encoding = 'utf-8')
#         # self._serial.write(byte_cmd)
#         self._serial.write(bytes(json.dumps({'cmd': cmd_num}), encoding='utf-8'))
#         self._serial.write(bytes("ATD15376557299;"))
#         self._serial.write(bytes("AT+CMEDPLAY=1,C:\\User\\1.amr,0,100"))
#         self._serial.write(bytes("AT+"))
#
#     def ser_rev_inf(self):
#         """
#         Recive inf form stm32
#         :return: js_data(json)
#         """
#         if self._serial.in_waiting is not 0:
#             data = str(self._serial.readline(),encoding='utf-8')
#             return data
#             #print(st1)
#             # if data is {}:
#             #     return None
#             # if data is not None:
#             #     js_data = json.dumps(st1)
#             #     print(type(js_data))
#             #     print(js_data)
#             #     return js_data
#
#     def flush_output(self):
#         """
#         清空接收缓冲区
#         :return: none
#         """
#         self._serial.reset_output_buffer()
#
#
#     def ser_iter_scan(self):
#         """
#         生成器获取json数据
#         :return: js_data(json)
#         """
#         while True:
#             data = self._read_response()
#             yield data
#
#     def ser_clean_input(self):
#         pass
#
#     def ser_stop(self):
#         """
#         关闭串口
#         :return:
#         """
#         self._serial.close()
#
#     def ser_run(self):
#
#     def ser_test(self):
#         """
#         测试函数
#         :return: data
#         """
#         global data
#         data = self._serial.readline()
#         return data
#
#
# if __name__ == '__main__':
#     ser = Alcohol_serial(port='com40')
#     # while True:
#     #     # data = test.ser_test()
#     #     # print(data)
#     #     # test._serial.write(data)
#     # cmd = {"cmd": 1}
#     # data = json.dumps(cmd)
#     # print(cmd)
#     # print(data)
#     # while True:
#     # #     data = test.ser_test()
#     #     ser.ser_rev_inf()
#     #     #print(data)
#     #     time.sleep(2)
#     #     ser.ser_send_cmd(1)
#     # # data = "b'sdsd'"
#     # # print(data[2:-1])
#
# -*- coding: utf-8 -*-
"""
@Time: 2022/4/10 19:31
@Author: Veera
@Version: 3.9.0
@File: serial_ac.py.py
@Software: PyCharm
"""
# -*- coding: utf-8 -*-
import serial
import json
import time


class meter_serial(object):
    def __init__(self, port, baudrate=9600, readtimeout=0.5):
        self._serial = None
        self.port = port
        self.baudrate = baudrate
        self.readtimeout = readtimeout
        self.parity = 'N'
        self.ser_connect()

    @staticmethod
    def ser_str_to_unicode(raw_str):
        result = ''
        for c in raw_str:
            _hex = hex(ord(c))
            # 处理数字
            if len(_hex) == 4:
                _hex = _hex.replace('0x', '00')
            else:
                _hex = _hex.replace('0x', '')

            result += _hex.upper()
        return result

    def ser_connect(self):
        """
        Connect to the stm32
        :return: none
        """
        if self._serial is not None:
            self.ser_disconnect()
        try:
            self._serial = serial.Serial(parity=self.parity, port=self.port,
                                         baudrate=self.baudrate, timeout=self.readtimeout)
        except serial.SerialException as e:
            print("Failed to connect to the meter "
                  'due to: %s' % e)

    def ser_disconnect(self):
        """
        Disconnects from the serial port
        :return: none
        """
        if self._serial is None:
            return
        self._serial.close()

    def ser_send_cmd(self, inf: object) -> object:
        """
        send inf to port
        :return: none
        """
        if type(inf) is bytes:
            self._serial.write(inf)
        else:
            self._serial.write(inf.encode())

    def ser_send_loca_cmf(self, ph_num, inf):
        pass

    def ser_send_English_mes_cmd(self, ph_num="15376557299", inf= "Hello"):
        """
        发送短信
        ph_num : 发送的电话号
        inf ：短信内容
        :return: none
        """
        self.ser_send_cmd("AT+CMGF=1\r\n")
        self.ser_send_cmd("AT+CMGS=\""+ph_num+"\"\r\n")
        time.sleep(0.5)
        self.ser_send_cmd(inf)
        time.sleep(1)
        self.ser_send_cmd(b'\x1a')
        time.sleep(1)

    def ser_send_Unicode_mes_cmd(self, ph_num="15376557299", inf= "Hello"):
        """
        发送短信
        ph_num : 发送的电话号
        inf ：短信内容
        :return: none
        """
        self.ser_send_cmd("AT+CMGF=1\r\n")
        self.ser_send_cmd("AT+CSMP=17,167,1,8\r\n")
        self.ser_send_cmd("AT+CSCS=\"UCS2\"\r\n")
        ph_num = self.ser_str_to_unicode(ph_num)
        self.ser_send_cmd("AT+CMGS=\""+ph_num+"\"\r\n")
        time.sleep(0.5)
        inf = self.ser_str_to_unicode(inf)
        self.ser_send_cmd(inf)
        time.sleep(1)
        print(list(self.ser_rev_inf()))
        self.ser_send_cmd(b'\x1a')
        time.sleep(1)
        print(list(self.ser_rev_inf()))

    def ser_send_loca_mes_cmd(self):

        self.ser_send_cmd('AT+SAPBR=3,1,"CONTYPE","GPRS"\r\n')
        print("1")
        print(ser.ser_rev_inf())
        self.ser_send_cmd('AT+SAPBR=3,1,"APN","CMNET"\r\n')
        print("2")
        print(ser.ser_rev_inf())
        self.ser_send_cmd('AT+SAPBR=1,1\r\n')
        time.sleep(0.5)
        print("3")
        print(ser.ser_rev_inf())
        self.ser_send_cmd('AT+SAPBR=2,1\r\n')
        time.sleep(0.5)
        print("4")
        print(ser.ser_rev_inf())
        self.ser_send_cmd('AT+CLBS=1,1\r\n')
        time.sleep(0.5)
        print("end")
        data = self.ser_rev_inf()
        print(data)
        print(len(data))
        print(type(data))
        print(data)
        # 24 - 43
        if len(data)>50:
            print(data[23:43])
            loca = 'https://uri.amap.com/marker?position='+data[23:43]
        print(loca)
        print(type(loca))
        self.ser_send_mes_cmd(inf=loca)

    def ser_send_call_cmd(self, phone_num='15376557299;'):
        num_inf = 'ATD' + phone_num + '\r\n'
        self.ser_send_cmd(num_inf)
        print(self.ser_rev_inf())

    def
    def ser_send_AC_cmd(self, cmd_num):
        """
        Send command form port to stm32
        发送酒精获取命令
        { 'cmd': 1 }
        :return: none
        """
        self.ser_send_cmd(json.dumps({'cmd': cmd_num}))

    def ser_rev_inf(self):
        """
        Recive inf form stm32/GSM sim800c
        :return: js_data(json)
        """
        time.sleep(0.5)
        # if self._serial.in_waiting is not 0:
        #     data = str(self._serial.readline(), encoding='utf-8')
        #     return data
        # else:
        #     print("Nothing")
        if self._serial.in_waiting is not 0:
            data = str(self._serial.read(self._serial.in_waiting), encoding='utf-8')
            return data
        else:
            print("Nothing")

    def flush_output(self):
        """
        清空接收缓冲区
        :return: none
        """
        self._serial.reset_output_buffer()

    def ser_iter_scan(self):
        """
        生成器获取json数据
        :return: js_data(json)
        """
        while True:
            data = self._read_response()
            yield data

    def ser_clean_input(self):
        pass

    def ser_stop(self):
        """
        关闭串口
        :return:
        """
        self._serial.close()

    def ser_run(self):
        pass

    def ser_test(self):
        """
        测试函数
        :return: data
        """
        global data
        data = self._serial.readline()
        return data


if __name__ == '__main__':
    ser = meter_serial(port='com30', baudrate=115200)
    ser.ser_send_Unicode_mes_cmd(inf="https://reurl.cc/WraRvL")

    # inf = 'AT\r\n'
    # while True:
    #     ser.ser_send_cmd(inf)
    #     print(ser.ser_rev_inf())
    # ser.ser_send_mes_cmd(ph_num="15376557299", inf="test")
    #ser.ser_send_loca_cmf(ph_num="15266377925", inf="HELLO,SIM")
    # ser.ser_send_cmd("AT+CMGF=1\r\n")
    # ser.ser_send_cmd("AT+CMGS=\"15376557299\"\r\n")
    # time.sleep(0.5)
    # ser.ser_send_cmd("Helloasd")
    # time.sleep(0.5)
    # ser.ser_send_cmd(b'\x1a')
    #ser.ser_send_mes_cmd(inf='asdas')



    # ser.ser_send_cmd('AT+SAPBR=3,1,"CONTYPE","GPRS"\r\n')
    # print("1")
    # print(ser.ser_rev_inf())
    # ser.ser_send_cmd('AT+SAPBR=3,1,"APN","CMNET"\r\n')
    # print("2")
    # print(ser.ser_rev_inf())
    # ser.ser_send_cmd('AT+SAPBR=1,1\r\n')
    # time.sleep(0.5)
    # print("3")
    # print(ser.ser_rev_inf())
    # ser.ser_send_cmd('AT+SAPBR=2,1\r\n')
    # time.sleep(0.5)
    # print("4")
    # print(ser.ser_rev_inf())
    # ser.ser_send_cmd('AT+CLBS=1,1\r\n')
    # time.sleep(0.5)
    # print("end")
    # data = ser.ser_rev_inf()
    # print(data)
    # print(len(data))
    # print(type(data))
    # print(data)
    # # 24 - 43
    # if len(data)>50:
    #     print(data[23:43])
    #     loca = 'https://uri.amap.com/marker?position='+data[23:43]
    # print(loca)
    # print(type(loca))
    # #ser.ser_send_mes_cmd(inf=loca)
    # a = 'helloasdasdsasaddasdsaddasdasdasdsaasdasdasdsadasdasdsadasdas'
    # print(type(a))
    # ser.ser_send_mes_cmd(inf=a)





