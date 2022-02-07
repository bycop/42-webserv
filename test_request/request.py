# from requests import Request, Session
import requests
from io import BytesIO
# pload = {'username':'Olivia','password':'123'}
# data = "BLABLA"
data = "7\r\n" + "Mozilla\r\n" + \
       "9\r\n" + "Developer\r\n" + \
       "7\r\n" + "Network\r\n" + \
       "0\r\n" + "\r\n"
# headers = {}
headers = {'Transfer-Encoding': 'chunked'}
r = requests.post('http://localhost:8080', data=data, headers=headers)

print(r.text)
# s = Session()
# req = Request('POST', "http://localhost:8080", data=data)
# prepped = req.prepare()
# del prepped.headers['content-length']
# response = s.send(BytesIO(prepped))