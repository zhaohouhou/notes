'''
http (POST) server template
@author: zd
'''
import urlparse
from BaseHTTPServer import HTTPServer, BaseHTTPRequestHandler


class Server(BaseHTTPRequestHandler):
    

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
        
        
# start:
addr = ('', PORT_NUMBER)
server = HTTPServer(addr, Server)
print 'Started server on port ' , PORT_NUMBER
server.serve_forever()