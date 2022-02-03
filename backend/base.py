def print_base(content):
    fd = open("backend/base.html")
    html_content = fd.read()
    html_content = html_content.replace("content", content, 1)
    print(html_content)
