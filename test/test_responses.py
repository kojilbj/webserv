import requests

def normal_request():
    r = requests.get("http://172.17.0.2")
    assert r.status_code == requests.codes.ok

def test_responses():
    normal_request()
