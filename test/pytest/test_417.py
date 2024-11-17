import test_responses

import requests

def Expectation_failed_response():
    headers = {"Expect": "Please Continue, please ... T_T"}
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers)
    n_res = requests.get(test_responses.NGINX_URL, headers=headers)
    assert w_res.status_code == requests.codes.expectation_failed
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]
    assert b"417 Expectation Failed" in w_res.content