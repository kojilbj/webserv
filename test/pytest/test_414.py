import test_responses

import requests

def test_Request_line_too_long_response():
    LONG_STR = "x" * 10000
    w_res = requests.get(test_responses.WEBSERV_URL + "/" + LONG_STR)
    n_res = requests.get(test_responses.NGINX_URL + "/" + LONG_STR)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"414 Request-Line Too Long" in w_res.content