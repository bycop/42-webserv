import requests
import config
from time import sleep
from tqdm import tqdm
import socket
from http.client import HTTPResponse

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
		print('Error! Code: {c}, Message, {m}'.format(
			c=type(e).__name__, m=str(e)))
		return False
	return True


def send_request(request_header, expected):
	if config.PRINT_REQUESTS:
		print("Testing " + request_header + " on " +
			  config.SERVER_ADDRESS + ":" + str(config.SERVER_PORT))

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

def send_chunk(url, data, headers, expected):
	if config.PRINT_REQUESTS:
		print("Testing chunk on " +
			  config.SERVER_ADDRESS + ":" + str(config.SERVER_PORT))

	r = requests.post(url, data=data, headers=headers)
	if config.PRINT_REQUESTS:
		print(r.status_code)
	if r.status_code != expected:
		return False
	return True

# headers = {'Transfer-Encoding': 'chunked'}

basic_tests = [
	# Miscelaneous tests
	['PUT', 'http://localhost:8080/pages', 501],  # 1
	['HEAD', 'http://localhost:8080/pages/1', 501],  # 2

	# GET tests
	['GET', 'http://localhost:8080/', 200],  # 3
	# ['GET', 'http://localhost:8080/missing_rights/', 403],  # 4
	['GET', 'http://localhost:8080/Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin gravida vulputate metus. Fusce vitae justo ex. Mauris eget neque vitae justo laoreet facilisis. Nunc blandit sit amet mi sed posuere. Nunc ac tortor sagittis, faucibus enim eu, eleifend magna. Ut fringilla eros non lorem aliquet, sed pulvinar augue tempus. Sed vitae purus luctus, porttitor libero mattis, blandit velit. Interdum et malesuada fames ac ante ipsum primis in faucibus. Etiam elit ipsum, iaculis sed nulla non, pretium venenatis tortor. Nunc auctor viverra suscipit. Donec commodo venenatis arcu, quis vestibulum orci sodales tempus. Vestibulum porta facilisis elit at luctus.In eu mollis leo, at fermentum velit. Suspendisse bibendum at elit nec condimentum. Phasellus et felis porttitor, convallis orci eu, sollicitudin leo. Nulla facilisi. Proin aliquet magna risus, sit amet rhoncus metus bibendum eu. Morbi venenatis dapibus massa, vitae aliquet orci fermentum vel. Etiam non dapibus felis. Etiam lobortis non sem eu vulputate. Vestibulum vitae risus felis. Vestibulum blandit commodo tellus, nec tempus lacus laoreet a.Curabitur enim ante, efficitur et mauris et, fermentum mattis lacus. Quisque suscipit rutrum placerat. Vivamus euismod lorem nec felis tincidunt, id faucibus augue varius. Vivamus sapien metus, venenatis at purus ut, maximus interdum sapien. Integer est purus, luctus non pretium eu, euismod et massa. Cras at odio ultricies, molestie massa sed, ultricies dui. Interdum et malesuada fames ac ante ipsum primis in faucibus. Mauris vulputate elit vitae eros sodales tincidunt.Morbi rutrum sem nisi, in pellentesque lectus suscipit vel. Nullam lacinia eget elit ut elementum. Morbi in nunc pulvinar, scelerisque nisl sit amet, aliquam ligula. Donec condimentum sagittis nulla eu auctor. Donec vel metus eu ipsum ultricies lacinia. Sed tincidunt, magna vitae tempor vehicula, purus ipsum mattis nisi, quis eleifend mi neque vitae quam. Duis magna lacus, tincidunt sed rhoncus sed, finibus in tortor. Maecenas ultrices eros eget ligula tempus elementum. Phasellus interdum tellus sed posuere aliquet. Vestibulum congue vestibulum leo vel pretium. Aenean nulla sem, porta sit amet est vitae, lobortis maximus magna. Pellentesque consequat aliquet diam luctus pellentesque.Maecenas tempus suscipit lacus eu aliquet. Vestibulum placerat massa lectus, et viverra dui porttitor id. Maecenas ac suscipit nisl, quis iaculis urna. Suspendisse potenti. Sed eleifend eleifend aliquam. Ut semper urna quis lectus consequat, eu auctor nisl convallis. Pellentesque dolor leo, iaculis ut tellus eu, rutrum elementum diam. Nulla sit amet ligula id orci efficitur accumsan mollis ac est. Phasellus hendrerit nulla tellus, sit amet iaculis sapien sodales at. Cras libero dui, ornare sit amet odio non, efficitur ultricies dui. Etiam risus eros, ornare ut eros sit amet, consequat bibendum ante. Nunc et pharetra justo. Quisque lobortis purus tincidunt ultricies porttitor. Maecenas odio eros, finibus eu tempor et, hendrerit in lorem. Integer non risus eros. Donec lectus ex, bibendum eu accumsan nec, efficitur non urna.In ac tortor accumsan, interdum ligula id, aliquet odio. Curabitur egestas metus vitae diam congue iaculis. Morbi faucibus urna ac varius viverra. Morbi imperdiet id velit et molestie. Fusce non semper enim, ut finibus purus. Ut quis augue ut magna congue imperdiet a a quam. Ut varius ligula nisi, eget porttitor dui congue vel. Nam in quam enim. Pellentesque at congue odio. Aenean urna nibh, hendrerit et posuere id, malesuada ac est. Mauris vel sem sit amet ex cursus interdum ac ut diam. Donec tristique bibendum porta.Phasellus mauris lectus, egestas ac finibus vitae, ullamcorper sed ex. Aliquam lacinia malesuada accumsan. Aenean suscipit egestas arcu sit amet pharetra. Nullam a tempor augue. Donec lobortis nibh et lobortis bibendum. Pellentesque elementum eleifend posuere. Vivamus convallis gravida interdum. Mauris venenatis placerat elit, ac pulvinar tellus consectetur vel. Nam id vulputate massa, sed molestie leo. Sed at euismod nisl, ac viverra mauris. Sed semper, turpis a pellentesque aliquam, nisl tellus gravida augue, et mattis est nisl et justo. Phasellus nunc augue, luctus eleifend facilisis quis, malesuada eu dui. Fusce venenatis et erat at dictum. Maecenas sed faucibus nisl, a ornare nibh. Nulla facilisi.Integer imperdiet odio orci, et maximus mauris convallis non. Nulla porta quis sapien rhoncus ornare. Interdum et malesuada fames ac ante ipsum primis in faucibus. Sed non tincidunt metus. Cras rutrum sollicitudin nisi, id mollis velit consequat vel. Donec odio magna, placerat consectetur lorem sit amet, ultricies tristique dolor. Maecenas suscipit elit eget risus lobortis egestas.Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nulla efficitur mi vel augue dignissim pretium. Pellentesque quis elit in dui sollicitudin rhoncus pellentesque a dui. Praesent sed porttitor purus. Praesent quis aliquet ex. Ut tincidunt, augue a dapibus aliquam, quam ex interdum sem, sed dictum leo nisi in libero. Lorem ipsum dolor sit amet, consectetur adipiscing elit.Integer commodo porttitor cursus. In varius nulla iaculis porttitor porta. Donec ac erat lorem. Phasellus dui diam, aliquam vel porttitor eu, tincidunt eget augue. Nulla venenatis ex vitae tellus consectetur, id tincidunt purus pharetra. Etiam augue dolor, pulvinar a dictum quis, euismod at justo. Suspendisse ultrices metus vel imperdiet fermentum. Praesent ac mollis lectus.Phasellus auctor sit amet elit ac fringilla. Nulla eget ornare ex, eu faucibus lectus. Duis dapibus sit amet metus et tincidunt. Nullam consectetur lacinia magna id mattis. Etiam id tempor diam, eget hendrerit tortor. Donec nibh libero, fermentum ut feugiat non, auctor at nisi. Curabitur sagittis massa id metus sodales condimentum. Suspendisse quis velit nisl.Curabitur ultrices augue non facilisis molestie. Etiam quis velit felis. Nam ac egestas mauris. Pellentesque orci quam, porta non iaculis sed, hendrerit non leo. Ut porta, tortor sit amet tempor viverra, dui eros lacinia nunc, vel molestie urna ligula sed velit. Nulla eu odio ac est ornare pharetra at in turpis. Nulla imperdiet lobortis libero eu scelerisque. Nulla quis blandit urna. Nulla purus odio, accumsan nec elit sollicitudin, eleifend consequat nibh. Maecenas euismod arcu tortor, sollicitudin volutpat nulla tincidunt tempus.Sed metus est, auctor in auctor vel, semper quis ipsum. Fusce nec condimentum sapien. Donec suscipit erat eget ligula laoreet congue. Etiam efficitur auctor lacinia. Pellentesque lacinia nunc sit amet tincidunt pretium. Donec cursus justo non nunc blandit maximus. Nunc vel lectus nec dolor varius eleifend vitae vel lacus.Suspendisse potenti. Nunc leo leo, pellentesque sed velit vitae, cursus imperdiet nibh. Quisque sit amet fringilla enim. Suspendisse tempor tortor sed dui volutpat, ac rhoncus erat egestas. Nunc eget volutpat purus. Vestibulum vehicula tincidunt augue id dapibus. Curabitur eget aliquam lorem. Morbi porta, elit nec condimentum faucibus, nulla velit pretium nisi, quis consectetur mauris leo non diam. Nunc dapibus metus vel nibh semper, at placerat erat porta. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Aliquam tortor magna, sollicitudin quis iaculis eu, varius eu augue. Ut vehicula venenatis ipsum sed auctor. Phasellus viverra dignissim finibus. Vivamus eget ex et elit dictum elementum. Nam vitae consectetur purus, non pharetra metus. Ut ut est ultrices, efficitur enim vitae, volutpat lacus.Pellentesque gravida libero enim. Cras eu nulla nisi. Nulla maximus fringilla mauris, sit amet posuere elit aliquam ut. Donec id risus tortor. Curabitur tempor euismod erat, nec congue ipsum porttitor nec. Donec elementum cursus consectetur. Quisque posuere vitae mi a pharetra.Maecenas at massa non felis pretium tincidunt. Aenean vitae magna eu lacus congue tempor quis et nunc. In sed mi ut felis mattis aliquam vel sit amet sem. Mauris gravida consequat sem vel viverra. Ut vel quam placerat sapien placerat fringilla. Nullam molestie quam ac nisi gravida, ac scelerisque risus vestibulum. Donec fringilla augue eros, eget facilisis tortor blandit at. Sed sollicitudin odio a velit ultricies, non pulvinar est lacinia. Donec rutrum at ipsum vel fringilla. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Phasellus vitae dui auctor, dictum est at, volutpat nulla. Sed vel ante leo.Praesent venenatis, elit eu tincidunt luctus, turpis justo facilisis libero, in dictum mi neque vel orci. Sed lobortis leo quis congue aliquet. Vivamus ac dui sed nulla ultricies imperdiet quis ut ex. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Nam a arcu at metus elementum vulputate sed at arcu. Vestibulum nulla odio, placerat et sem nec, blandit porttitor lectus. Duis metus velit, condimentum sit amet erat cursus, sagittis vulputate mi. Cras eu finibus quam. In pharetra nunc in lacus tristique pellentesque. Mauris tincidunt lobortis enim, ac molestie tortor tincidunt quis. Sed sit amet enim vulputate, euismod magna ut, blandit metus. Sed sed turpis justo. Pellentesque tincidunt diam at porta tincidunt. Cras a risus aliquet tellus luctus sollicitudin ac nec enim. Curabitur non rhoncus ipsum, et interdum purus.Mauris at nunc in dolor aliquam sodales. Quisque commodo, risus id faucibus tempus, mi nibh egestas nibh, et molestie turpis purus in diam. Maecenas egestas interdum turpis, a convallis quam dapibus ac. Ut nisl nunc, eleifend at metus id, facilisis mattis ante. Donec varius tellus vitae tincidunt suscipit. Proin congue ultricies quam hendrerit mattis. Aliquam ac porttitor odio, sit amet lobortis est. Phasellus tempor elementum ligula, vitae ultrices felis auctor eget. Pellentesque ac lacus ac dui vestibulum lacinia sed sed diam. Donec efficitur leo nec orci aliquam, nec eleifend orci efficitur. Sed sit amet pulvinar augue. Etiam imperdiet ut odio nullam.', 414],  # 5
	['GET', 'http://localhost:8080/file with space.html', 200], # 6

	# DELETE tests
	['DELETE', 'http://localhost:8080/post/post.html', 405],  # 7
	['DELETE', 'http://localhost:8080/delete/delete2.html', 404],  # 8
	['DELETE', 'http://localhost:8080/post-delete/post-delete2.html', 404],  # 9

	# POST tests
	['POST', 'http://localhost:8080/post-delete/post-delete.html', 200],  # 10
	['POST', 'http://localhost:8080/post/post.html', 200],  # 11
	['POST', 'http://localhost:8080/delete/delete.html', 405],  # 12
]

complete_tests = [
	["\0", 400], # 13
	["GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 200], # 14
	["GET / HTTP/1.2\r\nHost: localhost:8080\r\n\r\n", 505], # 15
	["HEAD / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 501], # 16
	["GET / HTTP/1.1\r\nHost: localhost:8080\r\nbouboubou\r\n", 200], # 17
	["GET  /  HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 400], # 18
	["GET {} HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 400], # 19
	["GET / HTTP/1.1\r\nHost: test1212\r\n\r\n", 400], # 20
	["BLABLA / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n", 501], # 21
	["GET / HTTP/1.1\r\nHost : localhost:8080\r\n\r\n", 400] # 22
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

print("Chunk test...") # 20
if send_chunk('http://localhost:8080/post', "7\r\n" + "Mozilla\r\n" + "B\r\n" + "Developer\r\n\r\n" + "7\r\n" + "Network\r\n" + "0\r\n" + "\r\n", {'Transfer-Encoding': 'chunked'}, 200):
	output += "\nTest " + str(count) + " failed"
count += 1
print("\033[91m" + output + "\033[0m" if output !=
	  "" else "\033[92m" + "All tests passed" + "\033[0m")

#print("Stress test...")
#for test in tqdm(range(600)):
#	make_request('GET', 'http://localhost:8080/', 200)
