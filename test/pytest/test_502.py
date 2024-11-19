import test_responses

import requests

# 502 Bad Gateway

def bad_php():
    BAD = "/php/bad.php"
    w_res = requests.get(test_responses.WEBSERV_URL + BAD)
    assert w_res.status_code == requests.codes.bad_gateway
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"502 Bad Gateway" in w_res.content

def test_Bad_gateway_response():
    bad_php()