import test_responses

import requests

def do_request(URI):
    w_res = requests.get(test_responses.WEBSERV_URL + URI)
    n_res = requests.get(test_responses.NGINX_URL + URI)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert w_res.headers["Content-Type"] == n_res.headers["Content-Type"]

def test1():
    URI = "/abc"
    do_request(URI)
    
def test2():
    URI = "/abc/"
    do_request(URI)

def test3():
    URI = "/abc/def"
    do_request(URI)

def test4():
    URI = "/abc/def"
    do_request(URI)

def test5():
    URI = "/abc/def/index.html"
    do_request(URI)

def test6():
    URI = "/////////////////////////////////////////////////////////////////////////"
    do_request(URI)

def test7():
    URI = "/index.htm"
    do_request(URI)

def test8():
    URI = "/uploaded_images/"
    do_request(URI)

def test9():
    URI = "/uploaded_images/index.html"
    do_request(URI)

def test10():
    URI = "/uploaded_images/penguin.png"
    w_res = requests.get(test_responses.WEBSERV_URL + URI)
    n_res = requests.get(test_responses.NGINX_URL + URI)
    assert w_res.status_code == n_res.status_code 
    assert "webserv/1.0" in w_res.headers["Server"]
    assert w_res.headers["Date"]
    assert "image/png" == w_res.headers["Content-Type"]

def test11():
    URI = "/uploaded_images//"
    do_request(URI)

def test12():
    URI = "/!#$%&'()0="
    do_request(URI)

def test13():
    URI = "/!#$%&'()0=/"
    do_request(URI)

def test14():
    URI = "/!\"#$%&'()0=~||{}_?*``+>/!#$%&'()0="
    do_request(URI)

def test15():
    URI = "/!\"#$%&'()0=~||{}_?*``+>/!#$%&'()0=/"
    do_request(URI)

def test16():
    URI = "/%"
    do_request(URI)

def test17():
    URI = "/%a"
    do_request(URI)

def test18():
    URI = "/\r\n\r\n"
    do_request(URI)

def test19():
    URI = "/ abc"
    do_request(URI)

def test20():
    URI = "/%2"
    do_request(URI)

def test_various_url():
    test1()
    test2()
    test3()
    test4()
    test5()
    test6()
    test7()
    test8()
    test9()
    test10()
    test11()
    test12()
    test13()
    test14()
    test15()
    test16()
    test17()
    test18()
    test19()
    test20()