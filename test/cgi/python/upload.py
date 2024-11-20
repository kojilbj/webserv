import cgi
import os

print("Content-Type: test/html")
print()
form = cgi.FieldStorage()
fileitem = form["file"]
if fileitem.file:
    basename = os.path.basename(fileitem.filename)
    directory = os.environ["UPLOAD_DIR"]
    if not directory:
        directory = "/home/kisobe/webserv/test/uploaded_files/"
    with os.open(directory + basename, O_WRONLY | O_CREAT) as f:
        f.write(fileitem.file.read())
    print("file upload is valid, uploaded at " + dir + basename)
else:
    print("file upload is invalid")
