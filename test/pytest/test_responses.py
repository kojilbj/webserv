import test_100
import test_200
import test_204
import test_301
import test_400
import test_403
import test_404
import test_405
import test_408
import test_413
import test_414
import test_417
import test_501
import test_502

WEBSERV_URL = "http://172.17.0.2:8080"
NGINX_URL = "http://172.17.0.2:8000"

ROOT = "/home/kisobe/webserv/test/"

def test_responses():
    for i in range(2):
        if i == 1:
            # update port
            global WEBSERV_URL
            WEBSERV_URL = "http://172.17.0.2:8081"
            global NGINX_URL
            NGINX_URL = "http://172.17.0.2:8001"
        test_100.test_Continue_response()
        test_200.test_OK_response()
        test_204.test_No_Content_response()
        test_301.test_Moved_Permanently()
        test_400.test_Bad_request_response()
        test_403.test_Forbidden_response()
        test_404.test_Not_Found_response()
        test_405.test_Not_allowed_response()
        # test_406.test_Not_Acceptable_response()
        test_408.test_Request_timeout_response()
        # test_409.test_Conflict_response()
        test_413.test_Request_entity_too_large_response()
        test_414.test_Request_line_too_long_response()
        # test_415.test_Unsupported_media_type_response()
        test_417.test_Expectation_failed_response()
        # test_500.test_Internal_server_error_response()
        test_501.test_Not_implemented_response()
        test_502.test_Bad_gateway_response()
        # test_505.test_Http_version_not_supported_response()
