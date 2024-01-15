from flask import Flask, render_template, request, jsonify
import sqlite3
import datetime
import myfunction

ip = '0.0.0.0'  #服务器绑定本地ip的地址
port = '8000'  #服务器绑定端口号

#网页监视所使用的信息（控制码，图片文件，提示信息，车牌号，车主姓名）
info = {'ctrlcode': 0, 'picloc': 'static/default.jpg', 'txt': '无车', 'number': None, 'name': None}

app = Flask(__name__)


#默认监视器页面
@app.route('/')
def index():
    return render_template('index.html')


#服务器与监视器网页通过json传递info中的信息
#打开‘/’网页后会浏览器会每隔0.5向服务器发送‘/info’请求以实时获取服务器上的info变量信息
@app.route('/info')
def ctrlcode():
    global info
    return jsonify(info)


#添加车辆信息
@app.route('/add', methods=['GET', 'POST'])
def add():
    conn = sqlite3.connect("park.db")
    cur = conn.cursor()
    if request.method == 'POST':
        carnumber = request.form.get('carnumber')  #获取表单输入
        name = request.form.get('name')
        cur.execute("select * from carinfo where carnumber='%s'" % (carnumber))  #判断输入的车牌号是否在数据库中
        if not cur.fetchall():
            cur.execute("select * from carinfo")
            id = len(cur.fetchall()) + 1
            cur.execute("insert into carinfo (id, carnumber, name) values (%d, '%s', '%s')" % (id, carnumber, name))
            conn.commit()
            cur.execute("select * from carinfo")
            carinfo = cur.fetchall()
            cur.close()
            conn.close()
            return render_template('add.html', info="车辆信息 (%d, '%s', '%s') 录入成功！" % (id, carnumber, name), carinfo=carinfo)
        else:
            cur.execute("select * from carinfo")
            carinfo = cur.fetchall()
            cur.close()
            conn.close()
            return render_template('add.html', info="车牌号 %s 已存在！" % (carnumber), carinfo=carinfo)
    else:
        cur.execute("select * from carinfo")
        carinfo = cur.fetchall()
        cur.close()
        conn.close()
        return render_template('add.html', carinfo=carinfo)


#查看日志
@app.route('/log')
def log():
    time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  #抓取日志的时间
    conn = sqlite3.connect("park.db")
    cur = conn.cursor()
    cur.execute("select * from log")
    loginfo = cur.fetchall()
    cur.close()
    conn.close()
    return render_template('log.html', loginfo=loginfo, time=time)


#与micro:bit交互的路由
@app.route('/ctrl')
def ctrl():
    global info
    code = info['ctrlcode'] = int(request.args.get('code'))
    if code == 1:
        info['txt'] = '车到，正在识别'
        time = datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')  #获取时间，待会写入日志用
        #调用myfunction中的函数进行拍照识别，得到图片存储位置和车牌号
        info['picloc'], info['number'] = myfunction.main()
        # info['number'] = 'A31'
        conn = sqlite3.connect("park.db")
        cur = conn.cursor()
        cur.execute("select * from carinfo where carnumber='%s'" % (info['number']))  #查询数据库中是否存在该车牌的信息
        searchRes = cur.fetchall()
        if searchRes:
            info['txt'] = '允许通行'  #设置提示信息，待会传递给网页
            info['name'] = searchRes[0][2]  #获取车主信息，待会传递给网页
            cur.execute("insert into log (time, carnumber, picloc, statue) values ('%s', '%s', '%s', '%s')" %
                        (time, info['number'], info['picloc'], 'ACCEPT'))  #写入日志
            conn.commit()
            cur.close()
            conn.close()
            return 'ACCEPT'  #返回给micro:bit以打开闸门
        else:
            info['txt'] = '禁止通行'
            cur.execute("insert into log (time, carnumber, picloc, statue) values ('%s', '%s', '%s', '%s')" %
                        (time, info['number'], info['picloc'], 'REFUSE'))
            conn.commit()
            cur.close()
            conn.close()
            return 'REFUSE'  #返回给micro:bit
    elif code == 0:  #车离开后micro:bit会发出code=0的请求，而后info的某些内容将会被重置
        info['picloc'] = 'static/default.jpg'  #设置默认图片
        info['number'] = None
        info['name'] = None
        info['txt'] = '无车'
        return 'RECEIVED'


if __name__ == '__main__':
    app.run(host=ip, port=port, debug=True)