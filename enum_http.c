#include "enum_http.h"

char *http_to_str(http_error_code_t code) {
  switch (code) {
  case HTTP_BAD_REQUEST:
    return "400 Bad Request";
    break;
  case HTTP_UNAUTHORIZED:
    return "401 Unauthorized";
    break;
  case HTTP_NOT_FOUND:
    return "404 Not Found";
    break;
  case HTTP_TEAPOT:
    return "418 I AM A TEAPOT!";
    break;
  case HTTP_INTERNAL_SERVER_ERROR:
    return "500 Internal Server Error";
    break;
  default:
    return "Unknown HTTP status code";
    break;
  }
}
