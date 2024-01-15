import sqlite3
#车辆信息和日志数据库的创建
conn = sqlite3.connect("park.db")
cur = conn.cursor()
cur.execute("create table carinfo(id int, carnumber text, name text)")
cur.execute("create table log(time text, carnumber text, picloc text, statue text)")
#输入两条默认的信息
cur.execute("insert into carinfo (id, carnumber, name) values (1, 'A31', 'Alice')")
cur.execute("insert into carinfo (id, carnumber, name) values (2, 'B57', 'Bob')")
conn.commit()
cur.close()
conn.close()