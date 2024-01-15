from microbit import *
import Servo  #电机库

sv = Servo(pin1)
sv.angle(90)  #闸门复位至关闭


class Obloq(object):

    def connectWifi(self, SSID, PASSWORD, TIME):
        uart.write('AT+CWJAP="%s","%s"\r\n' % (SSID, PASSWORD))  # 设置wifi模块连接无线网络
        display.scroll("wait...")
        display.show(Image.HAPPY)
        uart.write('AT+CIPMUX=0\r\n')
        sleep(500)  # 设置WIFI模块单路链接模式
        uart.write('AT+CIPSTART="TCP","0",0\r\n')
        sleep(TIME)
        if uart.any():  #第二次读，返回服务器IP设置消息
            data = str(uart.readall(), "UTF-8")
            sleep(1000)
        uart.write('AT+CIFSR\r\n')
        sleep(2000)
        data = 0  # 设置这个后 读串口返回绑定ip
        if uart.any():
            data = str(uart.readall(), "UTF-8")
            temp = data.split("\"", 5)
            self.ip_address = temp[1]
            if self.ip_address[0] == '1' and len(self.ip_address) > 10:
                return True
            else:
                display.show(".")
                sleep(3000)
                return False
        else:
            display.show(".")
            sleep(3000)
            return False

    def ifconfig(self):
        return self.ip_address

    def httpSet(self, IP, PORT):
        uart.write('AT+CIPSTART="TCP","%s",%s\r\n' % (IP, PORT))
        sleep(6000)
        uart.write('AT+CIPMODE=1\r\n')  # 设置WIFI模块为透穿模式
        sleep(500)
        uart.write('AT+CIPSEND\r\n')  # 在透穿模式下开始发送数据
        sleep(3000)
        if uart.any():  # 如果串口有值
            self.res = str(uart.readall(), "UTF-8")
            self.res = 0

    def get(self, url, time):
        uart.write('GET /' + url + '\r\n\r\n')
        sleep(1000)
        if uart.any():  # 如果串口有值
            res = str(uart.readall(), "UTF-8")
            return (200, res)
        return (404, "NOT FOUND")


Obloq = Obloq()

IP = "192.168.0.100"  #改为服务器ip
PORT = "8080"  #改为服务器端口
SSID = "406"  #设置WIFI的SSID，确保与可与服务器通信
PASSWORD = "134679258"  #设置WIFI的密码

uart.init(baudrate=115200, bits=8, parity=None, stop=1, tx=pin12, rx=pin8)

while Obloq.connectWifi(SSID, PASSWORD, 10000) != True:
    display.show(".")

display.scroll(Obloq.ifconfig)
Obloq.httpSet(IP, PORT)

defaultVal = pin0.read_analog()  #设置默认光照强度
flag = False  #flag控制只在车离开后发送控制代码0（表示无车）
while True:
    if defaultVal - pin0.read_analog() >= 20:  #光强变化超过阈值时发生动作
        display.scroll("!")
        flag = True
        erron, resp = Obloq.get('ctrl?code=1', 10000)  #向服务器返回控制代码1（表示车到）
        if erron == 200:
            if resp == "ACCEPT":  #允许通行
                sv.angle(0)  #抬起闸门
                while defaultVal - pin0.read_analog() >= 20:  #直到车离开后才进行下一步动作
                    display.show(Image.YES, delay=300, clear=True)  #"YES"标志闪烁
                sleep(1000)
                sv.angle(90)  #闸门复位
            elif resp == "REFUSE":  #禁止通行
                while defaultVal - pin0.read_analog() >= 20:
                    display.show(Image.NO, delay=300, clear=True)  #"NO"标志闪烁
    elif flag == True:
        defaultVal = pin0.read_analog()
        erron, resp = Obloq.get('ctrl?code=0', 10000)  #向服务器返回控制代码0
        flag = False
    sleep(1000)