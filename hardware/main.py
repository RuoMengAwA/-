# # 这是一个示例 Python 脚本。
#
# # 按 Shift+F10 执行或将其替换为您的代码。
# # 按 双击 Shift 在所有地方搜索类、文件、工具窗口、操作和设置。
# from serial_ac import meter_serial
#
#
# def ser_str_to_unicode(raw_str):
#     result = ''
#     for c in raw_str:
#         _hex = hex(ord(c))
#         # 处理数字
#         if len(_hex) == 4:
#             _hex = _hex.replace('0x', '00')
#         else:
#             _hex = _hex.replace('0x', '')
#
#         result += _hex.upper()
#     return result
#
# # 按间距中的绿色按钮以运行脚本。
# if __name__ == '__main__':
#     print(len(ser_str_to_unicode('110.420248,25.313388')))
#     while True:
#
#         print(a)
#
#

import  torch