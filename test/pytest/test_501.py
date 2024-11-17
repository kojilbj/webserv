import test_responses

import requests

def Not_implemented_response():
    headers = {"Transfer-Encoding": "identity"}
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers)
    n_res = requests.get(test_responses.NGINX_URL, headers=headers)
    assert w_res.status_code == n_res.status_code
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert b"501 Not Implemented" in w_res.content