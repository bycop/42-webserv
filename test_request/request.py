import requests

# pload = {'username':'Olivia','password':'123'}
data = "7\r\n" + "Mozilla\r\n" + \
       "9\r\n" + "Developer\r\n" + \
       "7\r\n" + "Network\r\n" + \
       "0\r\n" + "\r\n"
headers = {'Transfer-Encoding': 'chunked'}
r = requests.post('http://localhost:8080', data=data, headers=headers)

# def gen():
#     yield 'hi'
#     yield 'there'
#
# r = requests.post('http://localhost:8080', data=gen())
print(r.text)
