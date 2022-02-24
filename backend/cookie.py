import cgi, base

form = cgi.FieldStorage()

infoHeader = None
if "nickname" not in form or form["nickname"].value == '':
    content = "<p>" + \
              "Error : Sorry your information in the form is incorrect." + "<br>" + \
              "Please try again." + \
              "</p>"
else:
    content = "<p>" + \
              "Hello " + form["nickname"].value + "." + \
              "</p>"
    infoHeader = "Set-Cookie: nickname=" + form["nickname"].value + "; Max-Age=86400; Path=/\n"

content += "<a href=\"/pages/cookie_test.html\">Go to cookie_test</a>"
base.print_base(content, infoHeader)
