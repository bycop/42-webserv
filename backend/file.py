import cgi, base, os
form = cgi.FieldStorage()

upload_files = form['file']
content = ''
for file in upload_files:
    filename = file.filename
    if file.filename:
        open('upload_file/' + filename, 'wb').write(file.file.read())
        content += '<p>The file ' + filename + ' was uploaded successfully</p>'
    else:
        content += '<p>The file ' + filename + ' has a error</p>'

content += "<a href=\"/pages/file.html\"> Upload a another file</a>"
base.print_base(content)
