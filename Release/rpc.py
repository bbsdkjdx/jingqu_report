class RpcSvr(object):
	"""docstring for RpcSvr"""
	def __init__(self, ip=None,port=8000):
		super(RpcSvr, self).__init__()
		from xmlrpc import server
		if not ip:
			import socket
			ip=socket.gethostbyname_ex(socket.gethostname())[-1][-1]
		self._svr=server.SimpleXMLRPCServer((ip,port))
		self._svr.register_introspection_functions()
		self._thd=None
	def reg_fun(self,fun):
		self._svr.register_function(fun)
	def run(self,block=True):
		if self._thd:
			return
		if block:
			self._svr.serve_forever()
		else:
			import threading
			self._thd=threading.Thread(target=self._svr.serve_forever)
			self._thd.start()
			

from xmlrpc import client as  _client
class RpcClient(_client.ServerProxy):
	"""docstring for RpcClient"""
	def __init__(self, ip,port):
		s=r'http://%s:%d/'%(ip,port)
		super(RpcClient, self).__init__(s)
	@property
	def members(self):
		return self.system.listMethods()
