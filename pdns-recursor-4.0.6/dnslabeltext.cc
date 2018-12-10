
#line 1 "dnslabeltext.rl"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include "dnsname.hh"
#include "namespaces.hh"

namespace {
void appendSplit(vector<string>& ret, string& segment, char c)
{
  if(segment.size()>254) {
    ret.push_back(segment);
    segment.clear();
  }
  segment.append(1, c);
}

}

vector<string> segmentDNSText(const string& input )
{
  // cerr<<"segmentDNSText("<<input<<")"<<endl; 

#line 28 "dnslabeltext.cc"
static const char _dnstext_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 0, 1, 
	2, 4, 5
};

static const char _dnstext_key_offsets[] = {
	0, 0, 1, 3, 5, 7, 9, 11, 
	15
};

static const unsigned char _dnstext_trans_keys[] = {
	34u, 34u, 92u, 48u, 57u, 48u, 57u, 48u, 
	57u, 34u, 92u, 32u, 34u, 9u, 13u, 34u, 
	0
};

static const char _dnstext_single_lengths[] = {
	0, 1, 2, 0, 0, 0, 2, 2, 
	1
};

static const char _dnstext_range_lengths[] = {
	0, 0, 0, 1, 1, 1, 0, 1, 
	0
};

static const char _dnstext_index_offsets[] = {
	0, 0, 2, 5, 7, 9, 11, 14, 
	18
};

static const char _dnstext_trans_targs[] = {
	2, 0, 7, 3, 2, 4, 2, 5, 
	0, 6, 0, 7, 3, 2, 8, 2, 
	8, 0, 2, 0, 0
};

static const char _dnstext_trans_actions[] = {
	3, 0, 0, 0, 11, 7, 5, 7, 
	0, 7, 0, 9, 9, 16, 0, 13, 
	0, 0, 13, 0, 0
};

static const char _dnstext_eof_actions[] = {
	0, 0, 0, 0, 0, 0, 0, 1, 
	1
};

static const int dnstext_start = 1;
static const int dnstext_first_final = 7;
static const int dnstext_error = 0;

static const int dnstext_en_main = 1;


#line 28 "dnslabeltext.rl"

	(void)dnstext_error;  // silence warnings
	(void)dnstext_en_main;
        const char *p = input.c_str(), *pe = input.c_str() + input.length();
        const char* eof = pe;
        int cs;
        char val = 0;

        string segment;
        vector<string> ret;

        
#line 98 "dnslabeltext.cc"
	{
	cs = dnstext_start;
	}

#line 103 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dnstext_trans_keys + _dnstext_key_offsets[cs];
	_trans = _dnstext_index_offsets[cs];

	_klen = _dnstext_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dnstext_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _dnstext_trans_targs[_trans];

	if ( _dnstext_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dnstext_actions + _dnstext_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 40 "dnslabeltext.rl"
	{ 
                        ret.push_back(segment);
                        segment.clear();
                }
	break;
	case 1:
#line 44 "dnslabeltext.rl"
	{ 
                        segment.clear();
                }
	break;
	case 2:
#line 48 "dnslabeltext.rl"
	{
                  char c = *p;
                  appendSplit(ret, segment, c);
                }
	break;
	case 3:
#line 52 "dnslabeltext.rl"
	{
                  char c = *p;
                  val *= 10;
                  val += c-'0';
                  
                }
	break;
	case 4:
#line 58 "dnslabeltext.rl"
	{
                  appendSplit(ret, segment, val);
                  val=0;
                }
	break;
	case 5:
#line 63 "dnslabeltext.rl"
	{
                  appendSplit(ret, segment, *(p));
                }
	break;
#line 218 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dnstext_actions + _dnstext_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
#line 40 "dnslabeltext.rl"
	{ 
                        ret.push_back(segment);
                        segment.clear();
                }
	break;
#line 241 "dnslabeltext.cc"
		}
	}
	}

	_out: {}
	}

#line 76 "dnslabeltext.rl"


        if ( cs < dnstext_first_final ) {
                throw runtime_error("Unable to parse DNS TXT '"+input+"'");
        }

        return ret;
};


DNSName::string_t segmentDNSNameRaw(const char* realinput)
{

#line 263 "dnslabeltext.cc"
static const char _dnsnameraw_actions[] = {
	0, 1, 0, 1, 1, 1, 2, 1, 
	3, 1, 4, 1, 5, 2, 1, 5, 
	2, 4, 0, 2, 4, 5
};

static const char _dnsnameraw_key_offsets[] = {
	0, 0, 2, 4, 6, 8, 10, 12
};

static const unsigned char _dnsnameraw_trans_keys[] = {
	46u, 92u, 48u, 57u, 48u, 57u, 48u, 57u, 
	46u, 92u, 46u, 92u, 46u, 92u, 0
};

static const char _dnsnameraw_single_lengths[] = {
	0, 2, 0, 0, 0, 2, 2, 2
};

static const char _dnsnameraw_range_lengths[] = {
	0, 0, 1, 1, 1, 0, 0, 0
};

static const char _dnsnameraw_index_offsets[] = {
	0, 0, 3, 5, 7, 9, 12, 15
};

static const char _dnsnameraw_trans_targs[] = {
	0, 2, 5, 3, 5, 4, 0, 7, 
	0, 6, 2, 5, 0, 2, 5, 6, 
	2, 5, 0
};

static const char _dnsnameraw_trans_actions[] = {
	0, 3, 13, 7, 5, 7, 0, 7, 
	0, 1, 0, 11, 0, 3, 13, 16, 
	9, 19, 0
};

static const char _dnsnameraw_eof_actions[] = {
	0, 0, 0, 0, 0, 1, 0, 16
};

static const int dnsnameraw_start = 1;
static const int dnsnameraw_first_final = 5;
static const int dnsnameraw_error = 0;

static const int dnsnameraw_en_main = 1;


#line 92 "dnslabeltext.rl"

	(void)dnsnameraw_error;  // silence warnings
	(void)dnsnameraw_en_main;

        DNSName::string_t ret;

        if(!*realinput || *realinput == '.') {
          ret.append(1, (char)0);
          return ret;
        }

        unsigned int inputlen=strlen(realinput);
        ret.reserve(inputlen+1);

        const char *p = realinput, *pe = realinput + inputlen;
        const char* eof = pe;
        int cs;
        char val = 0;
        char labellen=0;
        unsigned int lenpos=0;
        
#line 336 "dnslabeltext.cc"
	{
	cs = dnsnameraw_start;
	}

#line 341 "dnslabeltext.cc"
	{
	int _klen;
	unsigned int _trans;
	const char *_acts;
	unsigned int _nacts;
	const unsigned char *_keys;

	if ( p == pe )
		goto _test_eof;
	if ( cs == 0 )
		goto _out;
_resume:
	_keys = _dnsnameraw_trans_keys + _dnsnameraw_key_offsets[cs];
	_trans = _dnsnameraw_index_offsets[cs];

	_klen = _dnsnameraw_single_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + _klen - 1;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + ((_upper-_lower) >> 1);
			if ( (*p) < *_mid )
				_upper = _mid - 1;
			else if ( (*p) > *_mid )
				_lower = _mid + 1;
			else {
				_trans += (unsigned int)(_mid - _keys);
				goto _match;
			}
		}
		_keys += _klen;
		_trans += _klen;
	}

	_klen = _dnsnameraw_range_lengths[cs];
	if ( _klen > 0 ) {
		const unsigned char *_lower = _keys;
		const unsigned char *_mid;
		const unsigned char *_upper = _keys + (_klen<<1) - 2;
		while (1) {
			if ( _upper < _lower )
				break;

			_mid = _lower + (((_upper-_lower) >> 1) & ~1);
			if ( (*p) < _mid[0] )
				_upper = _mid - 2;
			else if ( (*p) > _mid[1] )
				_lower = _mid + 2;
			else {
				_trans += (unsigned int)((_mid - _keys)>>1);
				goto _match;
			}
		}
		_trans += _klen;
	}

_match:
	cs = _dnsnameraw_trans_targs[_trans];

	if ( _dnsnameraw_trans_actions[_trans] == 0 )
		goto _again;

	_acts = _dnsnameraw_actions + _dnsnameraw_trans_actions[_trans];
	_nacts = (unsigned int) *_acts++;
	while ( _nacts-- > 0 )
	{
		switch ( *_acts++ )
		{
	case 0:
#line 113 "dnslabeltext.rl"
	{ 
                        if (labellen < 0 || labellen > 63) {
                          throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': invalid label length "+std::to_string(labellen));
                        }
                        ret[lenpos]=labellen;
                        labellen=0;
                }
	break;
	case 1:
#line 120 "dnslabeltext.rl"
	{ 
                        lenpos=ret.size();
                        ret.append(1, (char)0);
                        labellen=0;
                }
	break;
	case 2:
#line 126 "dnslabeltext.rl"
	{
                  char c = *p;
                  ret.append(1, c);
                  labellen++;
                }
	break;
	case 3:
#line 131 "dnslabeltext.rl"
	{
                  char c = *p;
                  val *= 10;
                  val += c-'0';
                }
	break;
	case 4:
#line 136 "dnslabeltext.rl"
	{
                  ret.append(1, val);
                  labellen++;
                  val=0;
                }
	break;
	case 5:
#line 142 "dnslabeltext.rl"
	{
                  ret.append(1, *(p));
                  labellen++;
                }
	break;
#line 463 "dnslabeltext.cc"
		}
	}

_again:
	if ( cs == 0 )
		goto _out;
	if ( ++p != pe )
		goto _resume;
	_test_eof: {}
	if ( p == eof )
	{
	const char *__acts = _dnsnameraw_actions + _dnsnameraw_eof_actions[cs];
	unsigned int __nacts = (unsigned int) *__acts++;
	while ( __nacts-- > 0 ) {
		switch ( *__acts++ ) {
	case 0:
#line 113 "dnslabeltext.rl"
	{ 
                        if (labellen < 0 || labellen > 63) {
                          throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': invalid label length "+std::to_string(labellen));
                        }
                        ret[lenpos]=labellen;
                        labellen=0;
                }
	break;
	case 4:
#line 136 "dnslabeltext.rl"
	{
                  ret.append(1, val);
                  labellen++;
                  val=0;
                }
	break;
#line 497 "dnslabeltext.cc"
		}
	}
	}

	_out: {}
	}

#line 163 "dnslabeltext.rl"


        if ( cs < dnsnameraw_first_final ) {
                throw runtime_error("Unable to parse DNS name '"+string(realinput)+"': cs="+std::to_string(cs));
        }
        ret.append(1, (char)0);
        return ret;
};



#if 0
int main()
{
	//char blah[]="\"blah\" \"bleh\" \"bloeh\\\"bleh\" \"\\97enzo\"";
  char blah[]="\"v=spf1 ip4:67.106.74.128/25 ip4:63.138.42.224/28 ip4:65.204.46.224/27 \\013\\010ip4:66.104.217.176/28 \\013\\010ip4:209.48.147.0/27 ~all\"";
  //char blah[]="\"abc \\097\\098 def\"";
  printf("Input: '%s'\n", blah);
	vector<string> res=dnstext(blah);
  cerr<<res.size()<<" segments"<<endl;
  cerr<<res[0]<<endl;
}
#endif
