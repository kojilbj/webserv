import test_responses

import requests

def Request_entity_too_large_response():
    LONG_STR = "x" * 5000000
    w_res = requests.get(test_responses.WEBSERV_URL, data=LONG_STR)
    n_res = requests.get(test_responses.NGINX_URL, data= LONG_STR)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"413 Request Entity Too Large" in w_res.content