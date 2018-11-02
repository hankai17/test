#include "util_cgi.h"
#include "make_log.h"

int query_parse_key_value(const char *query, const char *key, char *value, int *value_len_p)
{
  char *temp = NULL;
  char *end = NULL;
  int value_len = 0;

  temp = strstr((char*)query, key);
  if (temp == NULL) {
    return -1;
  }

  temp += strlen(key);//=
  temp++;//value

  end = temp;
  while('\0' != *end && '#' != *end && '&' != *end) {
    end++;
  }

  value_len = end - temp;
  strncpy(value, temp, value_len);
  value[value_len] ='\0';
  if (value_len_p != NULL) {
    *value_len_p = value_len;
  }
  return 0;
}
