import os


def print_header_html(info_header=None):
    print("Content-Type: text/html")
    if (info_header != None):
        print(info_header)
    print("")


def print_base(content, info_header=None):
    fd = open(os.environ['DOCUMENT_ROOT'] + "backend/base.html")
    html_content = fd.read()
    html_content = html_content.replace("content", content, 1)
    print_header_html(info_header)
    print(html_content)
