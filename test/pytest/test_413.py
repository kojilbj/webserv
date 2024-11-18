import test_responses

import requests

def Request_entity_too_large_response():
    headers1 = {"Host": "webserv"}
    headers2 = {"Host": "nginx"}
    STR = "a" * 2000
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers1, data=STR)
    n_res = requests.get(test_responses.NGINX_URL, headers=headers2, data=STR)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"413 Request Entity Too Large" in w_res.content