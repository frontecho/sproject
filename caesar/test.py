from caesar import *

text = read_file('origin.txt')

text_dict = {}
text_dict['origin'] = text

# 读取 academic news novel 三个文件夹下的所有文件， 以文件名（不包含后缀）作为键， 文件内容作为值，存储在字典text_dict中
import os
types = ['academic', 'news', 'novel']
topics = []
for folder in types:
    for file in os.listdir(folder):
        if file.endswith('.txt'):
            key = file.split('.')[0]
            topics.append(key)
            text = read_file(os.path.join(folder, file))
            text_dict[key] = text

# 从topics中删除types的内容
topics = list(set(topics) - set(types))

# 输出字典中所有键的列表，及其文件内容的长度
for key in text_dict:
    print(key, len(text_dict[key]))

# 加密
OFFSET = 3
TOL = 1
encrypted_text_dict = {}

# (1)分析不同错误容忍度（程序中tolerance超参数）对破解精度的影响。
print('>>> tolerance')
tolerance_accuracy_list = []
tolerance_list = [0, 1, 2, 3, 4, 5]
encrypted_text_dict["origin"] = encrypt(text_dict["origin"], OFFSET)
for tol in tolerance_list:
    en_letter_frequency = get_letter_frequency(encrypted_text_dict['origin'])
    accuracy = get_accuracy(text_dict['origin'], encrypted_text_dict['origin'], en_letter_frequency, tol)
    tolerance_accuracy_list.append(accuracy)
    print('tolerance:', tol, 'accuracy:', accuracy)
print('<<< tolerance')


# (2)分析不同的加密文本长度对破解精度的影响
# >>> length
print('>>> length')
length_accuracy_list = []
lengths = [10, 50, 100, 500, 1000, 5000, 10000, 50000]
for length in lengths:
    text_dict[length] = text_dict['origin'][:length]
    encrypted_text_dict[length] = encrypt(text_dict[length], OFFSET)
    en_letter_frequency = get_letter_frequency(encrypted_text_dict[length])
    accuracy = get_accuracy(text_dict[length], encrypted_text_dict[length], en_letter_frequency, tol=TOL)
    length_accuracy_list.append(accuracy)
    print('length:', length, 'accuracy:', accuracy)
print('<<< length')


# (3)分析不同类型的文本（学术、小说、新闻…）对破解精度的影响
# >>> type
print('>>> type')
type_accuracy_list = []

# get shortest text length in types
type_length = min([len(text_dict[type]) for type in types])
print('length:', type_length)

for type in types:
    encrypted_text_dict[type] = encrypt(text_dict[type][:type_length], OFFSET)
    en_letter_frequency = get_letter_frequency(encrypted_text_dict[type])
    accuracy = get_accuracy(text_dict[type][:type_length], encrypted_text_dict[type], en_letter_frequency, tol=TOL)
    type_accuracy_list.append(accuracy)
    print('type:', type, 'accuracy:', accuracy)
print('<<< type')


# (4)分析同一类型中不同主题的文本（如新闻文本包含科技、政治、体育等不同主题）对破解精度的影响。
# >>> topic
print('>>> topic')
topic_accuracy_list = {}
type_topics = {}
for type in types:
    print('    >>> type:', type)
    topic_accuracy_list[type] = []
    type_topics[type] = [topic for topic in topics if topic[:len(type)] == type]
    topic_length = min([len(text_dict[topic]) for topic in type_topics[type]])
    print('    length:', topic_length)
    for topic in type_topics[type]:
        encrypted_text_dict[topic] = encrypt(text_dict[topic][:topic_length], OFFSET)
        en_letter_frequency = get_letter_frequency(encrypted_text_dict[topic])
        accuracy = get_accuracy(text_dict[topic][:topic_length], encrypted_text_dict[topic], en_letter_frequency, tol=TOL)
        topic_accuracy_list[type].append(accuracy)
        print('    topic:', topic, 'accuracy:', accuracy)
    print('    <<< type:', type)
print('<<< topic')



# 准确率可视化 为tolerance、length、type 以及每个type下的一组topic的数据绘图
import matplotlib.pyplot as plt

#设置中文字体为微软雅黑
plt.rcParams['font.sans-serif'] = ['SimHei']
plt.rcParams['axes.unicode_minus'] = False


# tolerance
plt.figure()
plt.plot(tolerance_list, tolerance_accuracy_list)
plt.xlabel('tolerance')
plt.ylabel('accuracy')
plt.title('tolerance vs accuracy')
plt.show()

# length
plt.figure()
plt.plot(lengths, length_accuracy_list)
plt.xlabel('length')
plt.ylabel('accuracy')
plt.title('length vs accuracy')
plt.show()

# type
plt.figure()
plt.bar(types, type_accuracy_list)
plt.xlabel('type')
plt.ylabel('accuracy')
plt.title('type vs accuracy')
plt.show()

# topic
for type in types:
    plt.figure()
    plt.bar(type_topics[type], topic_accuracy_list[type])
    plt.xlabel('topic')
    plt.ylabel('accuracy')
    plt.title('topic vs accuracy in type: ' + type)
    plt.show()
