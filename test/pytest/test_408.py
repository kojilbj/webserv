import test_responses

import requests

# 408 Request Timeout

def invalid_content_length():
    headers = {"Content-Length" : "100"}
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers)
    assert w_res.status_code == requests.codes.timeout
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"408 Request Timeout" in w_res.content

def Request_timeout_response():
    invalid_content_length()