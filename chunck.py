import requests
data = "7\r\n" + "Mozilla\r\n" + \
       "11\r\n" + "Developer\r\n\r\n" + \
       "7\r\n" + "Network\r\n" + \
       "0\r\n" + "\r\n"
headers = {'Transfer-Encoding': 'chunked'}
r = requests.post('http://localhost:8080', data=data, headers=headers)
print(r.text)