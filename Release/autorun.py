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
department=None
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
	global cln,token,user_name,department
	try:
		cln=rpc.RpcClient(server_address,9090)
		b_ok,res=cln.login(user_name,cry_password)
		if not b_ok:
			msgbox(res)
			return 0
		token,user_name,department=res
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
	fn = __main__.stack__[0]
	import xlrd
	book = xlrd.open_workbook(fn)
	st = book.sheet_by_index(0)
	r0=4 if '耕保' in department else 1 
	for r in range(r0,st.nrows):
		data=[str(x) for x in st.row_values(r)]
		add_new_piece(data)

def new_piece_from_stack():
	pos_end=__main__.stack__.index(0)
	add_new_piece(__main__.stack__[:pos_end])

def get_title():
	return '部门：%s  姓名：%s'%(department,user_name)

def fill_data(st,pieces):
	#find a line have title
	_r=1
	for r in range(1,10):
		if st.get_text(r,1):
			_r=r
			break
	#find the 1st blank line as r0.
	L=len(pieces[0])
	r0=0
	for r in range(_r+1,20):
		r0=r
		for c in range(1,L+1):
			if st.get_text(r,c):
				r0=0
				continue
		if r0:
			break
	#fill from the 1st blank line.
	for r,pc in enumerate(pieces,r0):
		for c,x in enumerate(pc,1):
			st.set_text(r,c,x)


def export_xls():
	fn=__main__.stack__[0]
	dic=cln.get_export_data(token)
	if not dic:
		return
	import office
	import os
	xls=office.Excel(0)
	bk2=xls.new()
	st2=bk2.sheets[0]
	for x in ['耕保科','利用科','地籍科','不动产']:
		if x in dic:
			pcs=dic[x]
			bk1=xls.open(os.path.join(os.getcwd(),x+'.template'))
			st1=bk1.sheets[0]
			st1.copy_before(st2)
			stnew=bk2.sheets[-4]
			fill_data(stnew,pcs)
			stnew.name=x
	for st in bk2.sheets[-3:]:
		st.raw.Delete()
	bk2.saveas(fn)
