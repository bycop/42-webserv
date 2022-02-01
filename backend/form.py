import cgi
import base
form = cgi.FieldStorage()

if "name" not in form or "email" not in form or "message" not in form or \
        form["name"].value == '' or form["email"].value == '' or form["message"].value == '':
    content = "<p>" + \
              "Error : Sorry your information in the form is incorrect." + "<br>" + \
              "Please try again." + \
              "</p>"
else:
    content = "<p>" + \
              "Hello " + form["name"].value + "." + "<br>" + \
              "Your email is : " + form["email"].value + "." + "<br>" + \
              "And your message is here : " + form["message"].value + "." + "<br>" + \
              "</p>"

content += "<a href=\"/pages/form.html\">Go to form</a>"
base.print_base(content)
