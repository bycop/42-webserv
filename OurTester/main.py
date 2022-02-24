import requests
from time import sleep
from tqdm import tqdm
# pload = {'username':'Olivia','password':'123'}
data = "7\r\n" + "Mozilla\r\n" + \
       "9\r\n" + "Developer\r\n" + \
       "7\r\n" + "Network\r\n" + \
       "0\r\n" + "\r\n"
def make_request(method, url, printbool, expected, data=None):
	if (printbool):
		print("Testing " + method + " on " + url)
	if method == 'GET':
		r = requests.get(url)
	elif method == 'POST':
		r = requests.post(url, data=data)
	elif method == 'DELETE':
		r = requests.delete(url)
	elif method == 'PUT':
		r = requests.put(url, data=data)
	elif method == 'HEAD':
		r = requests.head(url)
	if printbool:
		print(r.status_code)
	if r.status_code != expected:
		return False
	return True

# headers = {'Transfer-Encoding': 'chunked'}

testlist = [
	# Miscelaneous tests
	['PUT', 'http://localhost:8080/pages', 0, 501],
	['HEAD', 'http://localhost:8080/pages/1', 0, 501],
	['GET', 'http://localhost:8080/pages/1', 0, 404],
	['POST', 'http://localhost:8080/pages/1', 0, 405],
	#	Delete Tests
	['DELETE', 'http://localhost:8080/pages/txtfiles/test.txt', 0, 200],
	['DELETE', 'http://localhost:8080/pages/txtfiles/test.txt', 0, 404],
	['DELETE', 'http://localhost:8080/', 0, 405],
	['DELETE', 'http://localhost:8080/FileNotExist', 0, 405],
	['DELETE', 'http://localhost:8080/pages/txtfiles/FolderNotExist', 0, 404]

];

output = ""
count = 1

for test in tqdm(testlist):
	if not make_request(test[0], test[1], test[2], test[3]):
		output += "\nTest " + str(count) + " failed"
	count += 1
	sleep(0.1)

print(output)
		