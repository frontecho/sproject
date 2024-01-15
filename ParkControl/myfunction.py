import cv2
import time
import urllib.request, urllib.parse
import base64


#拍照，返回图像文件路径
def shot():
    try:
        cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
        f, frame = cap.read()
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        ticks = str(time.time())
        cv2.imwrite('static/' + ticks + '.jpg', gray)
        cap.release()
        return ('static/' + ticks + '.jpg')
    except:     
        return None


#识别，返回识别内容
'''
这里为了测试方便并保证准确度，使用的是百度通用文字识别
https://ai.baidu.com/tech/ocr_others/handwriting
https://aip.baidubce.com/rest/2.0/ocr/v1/handwriting
https://aip.baidubce.com/rest/2.0/ocr/v1/general
当然实际生活中可使用专用的车牌识别api
https://ai.baidu.com/tech/ocr_cars/plate
'''
def recognize(location):
    try:
        request_url = "https://aip.baidubce.com/rest/2.0/ocr/v1/general"
        # 二进制方式打开图片文件
        f = open(location, 'rb')
        img = base64.b64encode(f.read())

        params = urllib.parse.urlencode({"image": img})
        params = bytes(params, encoding='utf-8')
        access_token = '24.6cae50746dbdfad0e561497cdab254a7.2592000.1654104212.282335-16936649'
        request_url = request_url + "?access_token=" + access_token
        headers = {'content-type': 'application/x-www-form-urlencoded'}
        request = urllib.request.Request(request_url, data=params, headers=headers, method="POST")
        response = urllib.request.urlopen(request)
        if response:
            data = response.read().decode('utf-8')
            return eval(data)
    except:
        return None


#根据api返回的位置信息在原图像上加矩形框以标示文字位置
def main():
    picLocation = shot()
    if picLocation:
        originalResult = recognize(picLocation)
        print(originalResult)
        if len(originalResult['words_result']) > 0:
            textLocation = originalResult['words_result'][0]['location']
            text = originalResult['words_result'][0]['words']

            image = cv2.imread(picLocation)
            first_point = (textLocation['left'], textLocation['top'])
            last_point = (textLocation['left'] + textLocation['width'] + 3, textLocation['top'] + textLocation['height'])
            cv2.rectangle(image, first_point, last_point, (0, 255, 0), 2)
            cv2.imwrite(picLocation, image)

            return (picLocation, text)
        return (picLocation, None)
    return (None, None)


if __name__ == '__main__':
    print(main())
