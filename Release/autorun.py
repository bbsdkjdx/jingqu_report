import ctypes
import json
import rpc
import __main__
import socket
import time
socket.setdefaulttimeout(1)
msgbox = lambda s: ctypes.windll.user32.MessageBoxW(ctypes.windll.user32.GetForegroundWindow(), s, '', 0)

cln=None
user_name=None
cry_password=None
server_address=None
token=None

##################################################
# [id,from,mod,view,info]
##################################################
####### common userd function ###############################################


def encrypt(s):
    import base64
    s = s.encode()
    s = base64.a85encode(base64.b85encode(s))
    return s.decode()

def decrypt(s):
    import base64
    s = s.encode()
    s = base64.b85decode(base64.a85decode(s))
    return s.decode()
######## login info buffer ###################################################
def get_login_info():
	try:
		info = json.load(open('login.json', 'r'))
		u, p, s = info
		p = decrypt(p)
	except:
		u, p, s = '', '', ''
	__main__.stack__[0] = u
	__main__.stack__[1] = p
	__main__.stack__[2] = s

def set_login_info():
	global user_name,cry_password,server_address
	info=[0,0,0]
	info[0]=__main__.stack__[0]
	info[1]=encrypt(__main__.stack__[1])
	info[2]=__main__.stack__[2]
	user_name,cry_password,server_address=info
	json.dump(info, open('login.json', 'w'))

def login():
	global cln,token
	try:
		cln=rpc.RpcClient(server_address,9090)
		b_ok,res=cln.login(user_name,cry_password)
		if not b_ok:
			msgbox(res)
			return 0
		token=res
		return 1
	except:
		msgbox('无法连接到服务器')
		return 0
###########################################################

def get_table_head():
	heads=cln.get_table_head(token)
	L=len(heads)
	__main__.stack__[:L]=heads
	return L

def add_data(data):
	id=time.time()
	time.sleep(0.001)#make sure no id is equal.
	piece=[id,user_name,user_name,user_name,data]
