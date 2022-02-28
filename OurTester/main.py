import requests
import config
from time import sleep
from tqdm import tqdm
import socket
from http.client import HTTPResponse


# data = "7\r\n" + "Mozilla\r\n" + \
#        "9\r\n" + "Developer\r\n" + \
#        "7\r\n" + "Network\r\n" + \
#        "0\r\n" + "\r\n"
def make_request(method, url, expected, data=None):
    r = 0
    try:
        if config.PRINT_REQUESTS:
            print("Testing " + method + " on " + url)
        if method == 'GET':
            r = requests.get(url, timeout=config.REQUEST_TIMEOUT)
        elif method == 'POST':
            r = requests.post(url, data=data, timeout=config.REQUEST_TIMEOUT)
        elif method == 'DELETE':
            r = requests.delete(url, timeout=config.REQUEST_TIMEOUT)
        elif method == 'PUT':
            r = requests.put(url, data=data, timeout=config.REQUEST_TIMEOUT)
        elif method == 'HEAD':
            r = requests.head(url, timeout=config.REQUEST_TIMEOUT)
        if config.PRINT_REQUESTS:
            print(r.status_code)
        if r.status_code != expected:
            return False
    except requests.exceptions.RequestException as e:
        print('Error! Code: {c}, Message, {m}'.format(c = type(e).__name__, m = str(e)))
        return False
    return True

def send_request(request_header, expected):
    if config.PRINT_REQUESTS:
        print("Testing " + request_header + " on " + config.SERVER_ADDRESS + ":" + str(config.SERVER_PORT))

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((config.SERVER_ADDRESS, config.SERVER_PORT))
    client.send(request_header.encode())
    # read and parse http response
    http_response = HTTPResponse(client)
    http_response.begin()
    if config.PRINT_REQUESTS:
        print(http_response.status)
    if http_response.status != expected:
        return False
    return True


# headers = {'Transfer-Encoding': 'chunked'}

basic_tests = [
    # Miscelaneous tests
    ['PUT', 'http://localhost:8080/pages', 501],  # 1
    ['HEAD', 'http://localhost:8080/pages/1', 501],  # 2

    # GET tests
    ['GET', 'http://localhost:8080/', 200],  # 3
    ['GET', 'http://localhost:8080/missing_rights/', 403],  # 4

    # DELETE tests
    ['DELETE', 'http://localhost:8080/post/post.html', 405],  # 5
    ['DELETE', 'http://localhost:8080/delete/delete2.html', 404],  # 6
    ['DELETE', 'http://localhost:8080/post-delete/post-delete2.html', 404],  # 7

    # POST tests
    ['POST', 'http://localhost:8080/post-delete/post-delete.html', 200],  # 8
    ['POST', 'http://localhost:8080/post/post.html', 200],  # 9
    ['POST', 'http://localhost:8080/delete/delete.html', 405],  # 10
]

complete_tests = [
    ["\0", 400],
    ["GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 200]
]

output = ""
count = 1

print("Starting basic tests...")
for test in tqdm(basic_tests):
    if not make_request(test[0], test[1], test[2]):
        output += "\nTest " + str(count) + " failed"
    count += 1
    sleep(0.1)

print("Starting complete tests...")
for test in tqdm(complete_tests):
    if not send_request(test[0], test[1]):
        output += "\nTest " + str(count) + " failed"
    count += 1
    sleep(0.1)

print("\033[91m" + output + "\033[0m" if output != "" else "\033[92m" + "All tests passed" + "\033[0m")
