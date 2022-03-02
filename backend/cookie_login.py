import cgi
import base
import os
form = cgi.FieldStorage()

content = ""
cookie = ""
if "username=" in os.environ['HTTP_COOKIE']:
    content = "<p> You're already logged in! </p>"
else:
    if "username" not in form or "password" not in form or \
            form["username"].value == '' or form["password"].value == '':
        content = "<p>" + \
                  "Error : Sorry your information are incorrect." + "<br>" + \
                  "Please try again." + \
                  "</p>"
    else:
        file1 = open(os.environ['DOCUMENT_ROOT'] + "backend/.htpasswd", 'r')
        Lines = file1.readlines()
        count = 0
        for line in Lines:
            line = line.replace("\n", "")
            if line.split(":")[0] == form["username"].value and line.split(":")[1] == form["password"].value:
                content = "<p>You're now logged</p>"
                cookie = "Set-Cookie: username=" + form["username"].value + "; Max-Age=60; Path=/\n"
                break
            count += 1

        if count == len(Lines):
            content = "<p>Incorrect password</p>"
content += "<a href=\"/pages/cookie_login.html\">Go back to form</a>"
base.print_base(content, cookie)
