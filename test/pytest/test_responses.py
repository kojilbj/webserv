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
        test_100.Continue_response()
        test_200.OK_response()
        test_204.No_Content_response()
        test_301.Moved_Permanently()
        test_400.Bad_request_response()
        test_403.Forbidden_response()
        test_404.Not_Found_response()
        test_405.Not_allowed_response()
        # test_406.Not_Acceptable_response()
        test_408.Request_timeout_response()
        # test_409.Conflict_response()
        test_413.Request_entity_too_large_response()
        test_414.Request_line_too_long_response()
        # test_415.Unsupported_media_type_response()
        test_417.Expectation_failed_response()
        # test_500.Internal_server_error_response()
        test_501.Not_implemented_response()
        # test_502.Bad_gateway_response()
        # test_505.Http_version_not_supported_response()
