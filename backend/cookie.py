import cgi
form = cgi.FieldStorage()

if "nickname" not in form or form["nickname"].value == '':
    content = "<p>" + \
              "Error : Sorry your information in the form is incorrect." + "<br>" + \
              "Please try again." + \
              "</p>"
else:
    content = "<p>" + \
              "Hello " + form["nickname"].value + "." + \
              "</p>"

content += "<a href=\"/pages/cookie_test.html\">Go to cookie_test</a>"
fd = open("backend/base.html")
html_content = fd.read()
html_content = html_content.replace("content", content, 1)
print("Content-Type: text/html\nSet-Cookie: nickname=" + form["nickname"].value + "; Max-Age=86400; Path=/\n")
print(html_content)
