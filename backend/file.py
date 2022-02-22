import cgi, base, os

def upload_file(file):
    filename = file.filename
    extension_file = os.path.splitext(filename)[1]
    extension_file = extension_file[1:]
    accept_extensions = ["jpeg", "png", "jpg", "gif", "svg", "bmp"]

    if file.filename and (extension_file in accept_extensions):
        open(filename, 'wb').write(file.file.read())
        return '<p>The file ' + filename + ' was uploaded successfully</p>'
    else:
        return '<p>The file ' + filename + ' has a error</p>'


form = cgi.FieldStorage()
if "file" not in form:
    content = "<p>Error upload file. Try again</p>"
else:
    upload_files = form['file']
    content = ''
    check_list = type(upload_files) is list
    # if not os.path.exists("upload_file"):
    #     os.mkdir("upload_file")

    if check_list:
        for file in upload_files:
            content += upload_file(file)
    else:
        content = upload_file(upload_files)

content += "<a href=\"/pages/file.html\"> Upload a another file</a>"
base.print_base(content)
