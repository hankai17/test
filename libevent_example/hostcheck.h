#ifndef HEADER_CURL_HOSTCHECK_H
#define HEADER_CURL_HOSTCHECK_H
#define CURL_HOST_NOMATCH 0
#define CURL_HOST_MATCH   1
int Curl_cert_hostcheck(const char *match_pattern, const char *hostname);

#endif /* HEADER_CURL_HOSTCHECK_H */

