'''
http (POST) server template
@author: zd
'''
import urlparse
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler


class Server(BaseHTTPRequestHandler):
	
    def do_GET(self):
        query_string = urllib.parse.unquote(self.path.split('?', 1)[1])  # path/arg1=123&arg2=456
        args = dict(urllib.parse.parse_qsl(query_string))       # {'arg1': '123', 'arg2': '456'}
        ...
	
    def do_POST(self):
        query_string = self.rfile.read(int(self.headers['Content-Length']))
        args = dict(urlparse.parse_qsl(query_string))
        value = args['name']
		print 'request param:', 'name=', value 

        response = "OK"
        self.send_response(200)
        self.send_header("Content-type", "text")
        response = str(response).decode('unicode-escape').encode('utf-8')
        self.send_header('response', 
                         response
                         )
        self.end_headers()
	
# RequestHandler 的实例是每次接到请求new一个，因此其依赖的变量等可以使用全局的
        
        
# start:
addr = ('', PORT_NUMBER)
server = HTTPServer(addr, Server)
print 'Started server on port ' , PORT_NUMBER
server.serve_forever()
