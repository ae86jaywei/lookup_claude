import sqlite3
import json
import os

# 数据库文件路径
db_path = r"C:\Users\ming\AppData\Roaming\Trae CN\ModularData\ckg_server\env_codekg.db"

# 连接到数据库
conn = sqlite3.connect(db_path)
cursor = conn.cursor()

# 获取所有表名
cursor.execute("SELECT name FROM sqlite_master WHERE type='table';")
tables = cursor.fetchall()

print("数据库中的表:")
for table in tables:
    table_name = table[0]
    print(f"\n表名: {table_name}")
    
    # 获取表结构
    cursor.execute(f"PRAGMA table_info({table_name})")
    columns = cursor.fetchall()
    print("表结构:")
    for col in columns:
        print(f"  列名: {col[1]}, 类型: {col[2]}")
    
    # 获取前几行数据
    try:
        cursor.execute(f"SELECT * FROM {table_name} LIMIT 5")
        rows = cursor.fetchall()
        print("前5行数据:")
        for row in rows:
            # 如果列是JSON字符串，尝试解析
            formatted_row = []
            for col in row:
                try:
                    # 尝试解析JSON
                    parsed = json.loads(col)
                    formatted_row.append(json.dumps(parsed, indent=2, ensure_ascii=False))
                except:
                    formatted_row.append(str(col))
            print(formatted_row)
    except Exception as e:
        print(f"读取表数据时出错: {e}")

conn.close()