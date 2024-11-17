import test_100
import test_200
import test_204
import test_403
import test_404

WEBSERV_URL = "http://127.0.0.1:8080"
NGINX_URL = "http://127.0.0.1:8000"

ROOT = "/home/kisobe/webserv/test/"

def test_responses():
    test_100.Continue_request()
    test_200.OK_request()
    test_204.No_Content_request()
    test_403.Forbidden_request()
    test_404.Not_Found_request()
