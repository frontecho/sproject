# ParkControl
基于micro:bit和AI文字识别的车库闸门管理系统实现

*代码功底不足，词句混乱，还可能包含一些问题，请多包涵*

<strike> **演示视频**：[Click Here!](http://code.vqyt.cn:8001/) </strike>

### 系统功能说明
1. 一套完整的车库闸门管理系统
2. 车到调用摄像头拍摄车牌信息，并使用百度文字识别api识别号码
3. Flask与JavaScript的联动实现在网页上的数据实时监视
4. 已学网络、数据库知识的简单应用
5. ……


### 系统使用说明
1.	修改microbit.py文件中的ip, port, ssid, password后烧录
2.	修改server.py文件中的ip, port为本机服务器的信息
3.	默认```park.db```文件由```createdb.py```文件生成
4.	需要额外安装的库有opencv-python, flask
```pip install opencv-python flask -i https://pypi.tuna.tsinghua.edu.cn/simple```
5.	摄像头要接在服务器上，使用前可能要禁用已有的多余摄像头，否则可能无法正常拍照，cv2将调用本机默认摄像头拍照
