import sys
from io import BytesIO
import uuid
import httplib
import ssl

class Part(object):

    """Represent a part in a multipart messsage"""
    def __init__(self, name, contentType, data, paramName=None):
        super(Part, self).__init__()
        self.name = name
        self.paramName = paramName
        self.contentType = contentType
        self.data = data

    def encode(self):
        body = BytesIO()

        if self.paramName:
            body.write('Content-Disposition: form-data; name="%s"; paramName="%s"\r\n' % (self.name, self.paramName))
        else:
            body.write('Content-Disposition: form-data; name="%s"\r\n' % (self.name,))

        body.write("Content-Type: %s\r\n" % (self.contentType,))
        body.write("\r\n")
        body.write(self.data)

        return body.getvalue()

class Request(object):
    """A handy class for creating a request"""
    def __init__(self):    
        super(Request, self).__init__()
        self.parameters = []

    def add_json_parameter(self, name, paramName, data):
        self.parameters.append(Part(name=name, paramName=paramName, contentType="application/json; charset=utf-8", data=data))

    def add_audio_parameter(self, name, paramName, data):
        self.parameters.append(Part(name=name, paramName=paramName, contentType="audio/x-wav;codec=pcm;bit=16;rate=16000", data=data))

    def encode(self):
        boundary = uuid.uuid4().hex
        body = BytesIO()
        
        for parameter in self.parameters:
            body.write("--%s\r\n" % (boundary,))
            body.write(parameter.encode())
            body.write("\r\n")

        body.write("--%s--\r\n" % (boundary,))
	#print body.getvalue()
        return body.getvalue(), boundary

	
host = "goertek-ncs-engusa-ssl.nuancemobility.net"
port = 443


uri = "/NmspServlet/"

RequestData = """{
        "appKey":"9acf33fb7520598c5729de7eeaad3141b7fed01fd89f227ff12a826761749e42b3ee02e35a4defc99d6d71a4d072b017c43085bf80cc507430a308ce5d6a93eb",
	"appId":"GOERTEK_EVAL_20150601",
	"uId":"1234567890123456789012345678901234567890",
	"inCodec":"PCM_16_16K",
	"outCodec":"PCM_16_16K",
	"cmdName":"DRAGON_NLU_ASR_CMD",
#	"cmdName":"NVC_ASR_CMD",
	"appName":"ddpasrproxy",
	"appVersion":"1.0",
	"language":"eng-USA",
	"carrier":"carrier",
	"deviceModel":"UNKNOWN",
	"cmdDict": {
		"dictation_type":"DTV",
#		"dictation_type":"Dictation",
		"dictation_language":"eng-USA",
		"application_name":"ddpasrproxy",
		"organization_id":"organization name",
		"application_session_id":"1234567890",
		"utterance_number":"5",
		"ui_language":"en",
		"application_state_id":""
	}
}"""

REQUEST_INFO = """{
	"start": 0,
	"end": 0,
	"text": "text present on the device screen",
#        "appserver_data" : {
#        }
}"""
    	
import pdb 
#pdb.set_trace()

request = Request()
request.add_json_parameter("RequestData", None, RequestData)
request.add_json_parameter("DictParameter", "REQUEST_INFO", REQUEST_INFO)

# Change if you'd like to pass in a PCM file to test with
#inputFile = open((sys.argv[1]), "rb")
inputFile = open("TomCruise.pcm", "rb")

while True:
	audioChunk = inputFile.read(1000000)
	if audioChunk != '':
		request.add_audio_parameter("ConcludingAudioParameter", "AUDIO_INFO", audioChunk)
	else:
		break
		
body, boundary = request.encode()

# If needed, connect unverified.
create_unverified = getattr(ssl, "_create_unverified_context", None)
if create_unverified == None:
    h = httplib.HTTPSConnection(host, port)
else:
    h = httplib.HTTPSConnection(host, port, context=ssl._create_unverified_context())

h.set_debuglevel(0)
headers = {
	"Content-Type": "multipart/form-data; boundary=%s" % (boundary,),
	"Connection": "Keep-Alive",	
}
h.request('POST', uri, body, headers)
res = h.getresponse()
print res.read() 
h.close()
