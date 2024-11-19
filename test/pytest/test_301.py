import test_responses

import requests

def test_Moved_Permanently():
    RETURN = "/redirect"
    w_res = requests.get(test_responses.WEBSERV_URL + RETURN)
    n_res = requests.get(test_responses.NGINX_URL + RETURN)
    assert w_res.status_code == n_res.status_code 
    assert w_res.headers["Content-Length"] == n_res.headers["Content-Length"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert w_res.content == n_res.content