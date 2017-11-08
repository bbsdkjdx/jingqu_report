import ctypes
import json
import rpc
import __main__
import socket
import time
import os
import binascii
import win32tools
exe_dir=os.getcwd()
socket.setdefaulttimeout(1)
msgbox = lambda s: ctypes.windll.user32.MessageBoxW(ctypes.windll.user32.GetForegroundWindow(), str(s), '', 0)

cln=None
user_name=None
cry_password=None
server_address=None
token=None
department=None
tid=None

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

def cache_login_info():
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


def fill_table_combo():
	ids=cln.get_tables_id_name(token)
	for n,(_id,name) in enumerate(ids):
		__main__.exe_fun__['insert_combo_data'](n,name,_id)
	return ids

def switch_table():
	global tid
	tid=__main__.stack__[0]
	th=cln.get_table_head(tid)
	__main__.exe_fun__['delete_all_columns']()
	insert_column=__main__.exe_fun__['insert_column']
	insert_column(0,'流水号',0)
	insert_column(1,'数据来源',100)
	insert_column(2,'数据状态',100)
	for n,x in enumerate(th,3):
		insert_column(n,x,len(x)*20)


def get_table_head():
	heads=cln.get_table_head(tid)
	L=len(heads)
	__main__.stack__[:L]=heads
	return L

def add_new_piece(data):
	id=str(time.time())
	time.sleep(0.001)#make sure no id is equal.
	piece=[id,user_name,[user_name],tid,data]
	try:
		cln.upload_piece(piece)
	except Exception as exp:
		msgbox(str(exp))

def get_piece_status(frm,pth):
	if frm not in pth:
		return '被驳回'
	if len(pth)==1:
		return '本地添加'
	return '待审核'

def grid_append_piece(piece):
	_id,frm,pth,_tid,info=piece
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
	pcs=cln.refresh(user_name,tid)
	__main__.exe_fun__['delete_all_items']()
	for pc in pcs:
		grid_append_piece(pc)
	return pcs

def load_excel():
	fn = __main__.stack__[0]
	import xlrd
	book = xlrd.open_workbook(fn)
	st = book.sheet_by_index(0)
	for r in range(11,st.nrows):
		data=[str(x) for x in st.row_values(r)]
		add_new_piece(data)

def new_piece_from_stack():
	pos_end=__main__.stack__.index(0)
	add_new_piece(__main__.stack__[:pos_end])

def get_title():
	return '部门：%s  姓名：%s'%(department,user_name)

def fill_data(st,r0,c0,pieces):
	#fill from the 1st blank line.
	for r,pc in enumerate(pieces,r0):
		ttp=time.gmtime(float(pc[0]))
		stime=time.strftime('%Y.%m.%d',ttp)
		pc[-1].append(stime)
		for c,x in enumerate(pc[-1],c0):
			st.set_text(r,c,x)


def export_xls(b_history):
	import office
	update_template()
	t1,t2=__main__.stack__[:2]
	li=cln.get_export_data(token,b_history,t1,t2)
	if not li:
		__main__.msgbox('没有需要导出的数据！')
		return
	fn=win32tools.select_file(0,'Excel 03\0*.xls\0')
	if not fn:
		return
	if '.xls' not in fn:
		fn+='.xls'
	if os.path.isfile(fn):
		os.remove(fn)
	dic=dict()
	for x in li:
		tid=x[3]
		if tid in dic:
			dic[tid].append(x)
		else:
			dic[tid]=[x]
	xls=office.Excel(1)
	bk2=xls.new()#for save
	st2=bk2.sheets[0]
	bk1=xls.open(os.path.join(exe_dir,'template.xls'))
	st0=bk1.sheets[0]#for read r0,c0
	for _id in dic:
		pcs=dic[_id]
		r0=int(st0.get_text(_id,1))
		c0=int(st0.get_text(_id,2))
		st1=bk1.sheets[_id]
		name=st1.get_text(1,1)
		st1.copy_before(st2)
		stnew=bk2.sheets[-4]
		fill_data(stnew,r0,c0,pcs)
		stnew.name=name
	for st in bk2.sheets[-3:]:
		st.raw.Delete()
	bk2.saveas(fn)
	bk1.close()
	__main__.msgbox('导出完成！')

def update_template():
	fn=os.path.join(exe_dir,'template.xls')
	try:
		with open(fn,'rb') as f:
			dat=f.read()
	except:
		dat=b''
	crc=str(binascii.crc32(dat))
	newdat=cln.download_export_template(crc)
	if newdat:
		open(fn,'wb').write(newdat.data)
		return 1
	return 0

def get_import_template():
	dat=cln.download_import_template(tid)
	if dat:
		fn=win32tools.select_file(1,'Excel 03\0*.xls\0')
		if not fn:
			return
		if '.xls' not in fn:
			fn+='.xls'
		open(fn,'wb').write(dat.data)
	msgbox('当前表格的模板已下载，请编辑并导入。')