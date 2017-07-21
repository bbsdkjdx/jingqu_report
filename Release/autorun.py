import ctypes
import json
import rpc
import __main__
import socket
import time
socket.setdefaulttimeout(1)
msgbox = lambda s: ctypes.windll.user32.MessageBoxW(ctypes.windll.user32.GetForegroundWindow(), str(s), '', 0)

cln=None
user_name=None
cry_password=None
server_address=None
token=None

##################################################
# [id,from,path,info]
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

def add_new_piece(data):
	id=str(time.time())
	time.sleep(0.001)#make sure no id is equal.
	piece=[id,user_name,[user_name],data]
	try:
		cln.upload_piece(piece)
		grid_append_piece(piece)
	except Exception as exp:
		msgbox(str(exp))

def get_piece_status(frm,pth):
	if frm not in pth:
		return '被驳回'
	if len(pth)==1:
		return '本地添加'
	return '待审核'

def grid_append_piece(piece):
	# [id,from,path,info]
	_id,frm,pth,info=piece
	cnt=__main__.exe_fun__['get_item_count']()
	__main__.exe_fun__['insert_item'](cnt,_id)
	__main__.exe_fun__['set_item_text'](cnt,1,frm)
	__main__.exe_fun__['set_item_text'](cnt,2,get_piece_status(frm,pth))
	for n,x in enumerate(info,3): 
		__main__.exe_fun__['set_item_text'](cnt,n,x)

def submit_piece(_id):
	cln.submit_piece(user_name,_id)

def dismiss_piece(_id):
	cln.dismiss_piece(user_name,_id)

def delete_piece(_id):
	cln.delete_piece(user_name,_id)

def refresh():
	pcs=cln.refresh(user_name)
	__main__.exe_fun__['delete_all_items']()
	for pc in pcs:
		grid_append_piece(pc)

def load_excel():
	fn=__main__.stack__[0]
	import xlrd
	book = xlrd.open_workbook(fn)
	st=book.sheet_by_index(0)
	for r in range(4,st.nrows):
		data=[str(x) for x in st.row_values(r)]
		add_new_piece(data)

