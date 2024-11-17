import requests
import test_responses

# 100 Continue

def continue_when_get_request():
    headers = {"Expect": "100-continue"}
    w_res = requests.get(test_responses.WEBSERV_URL, headers=headers)
    n_res = requests.get(test_responses.NGINX_URL, headers=headers)
    assert w_res.status_code == n_res.status_code 
    assert w_res.content == n_res.content

def Continue_response():
    continue_when_get_request()
