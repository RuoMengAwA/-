import requests
import random
import json
from requests_toolbelt.multipart.encoder import MultipartEncoder

AIP_HOME = 'https://tmonit.akasaki.space/api'  # 初始头文件端


class Service():
    def __init__(self, enID):
        '''
        param: enID: encrypted vehicle id
        '''
        self.enID = enID

    def newevent(self, ch, image):
        url = AIP_HOME + '/events/new'

        if ch == 'smoke':
            ch = 'ch1'
            level = 1
        elif ch == 'drink':
            ch = 'ch2'
            level = 2
        elif ch == 'phone':
            ch = 'ch3'
            level = 3
        elif ch == 'fatigue':
            ch = 'ch4'
            level = 4

        data = {
            'vehicleIdEncrypted':self.enID,
            'description': '{ac:'+str(random.random()*10000//100/100)+',tem:'+str(17)+',hum:'+str(random.randint(55,60)+random.randint(0,10)*0.1)+',hea:'+str(random.randint(63,66))+',blo:'+str(99+random.randint(0,10)*0.1)+',bls:'+str(4+random.randint(0,10)*0.1)+',blph:'+str(random.randint(110,120)+random.randint(0,10)*0.1)+',blpl:'+str(random.randint(70,80)+random.randint(0,10)*0.1)+',sdnn:'+str(random.randint(110,120))+'}',
            'dangerousLevel': level
        }
        string = json.dumps(data)
        r = requests.post(
            url,
            data=string,
            headers={"Content-Type": "application/json"},
        )

        if r.status_code == 200:
            print('new event success')
            enventID = r.json()['id']
            self.uploadimage(enventID, image)
        else:
            print('new event fail')

    def uploadimage(self, eventID, image):
        '''
        param: eventID: event id
        param: image: image file binary
        '''
        url = AIP_HOME + '/events' + '/' + eventID + '/image'

        binfile = open(image, 'rb')
        headers = {}
        data = MultipartEncoder(
            fields={
                'formFile': ("1.jpg", binfile, "image/jpg"),
                'encryptedVehicleId': self.enID,
            },
            boundary='----WebKitFormBoundary7MA4YWxkTrZu0gW')
        headers['Content-Type'] = data.content_type
        r = requests.put(url, data=data, headers=headers)
        print(r)
        if r.status_code == 200:
            print('upload image success')


if __name__ == '__main__':
    service = Service(
        'j1wbLNFholppgePX/VpOryVMYDgfNA1Xf+f2Bn9sSpyKiU7ztJGD4B8AjYa1d7OG3IV4L96XNwl4uTn6u52ZkLfHdXXjGl40/HJgBSDsykkASQPiEeynmssfUrfoW5Yl/VYcBFUdKupbaLoBm30f4e3KzeBQg8y0dQuuRDD2SsE='
    )
    service.newevent('smoke', './picture/01.jpg')
