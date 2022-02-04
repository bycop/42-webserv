def print_base(content):
    fd = open("backend/base.html")
    html_content = fd.read()
    html_content = html_content.replace("content", content, 1)
    print("Content-Type: text/html\n")
    print(html_content)
