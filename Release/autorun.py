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

class CGlobal(object):
	pass
g_=CGlobal()
		
g_.cln=None
g_.user_name=None
g_.cry_password=None
g_.server_address=None
g_.token=None
g_.department=None
g_.tid=None

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
	info=[0,0,0]
	info[0]=__main__.stack__[0]
	info[1]=encrypt(__main__.stack__[1])
	info[2]=__main__.stack__[2]
	g_.user_name,g_.cry_password,g_.server_address=info
	json.dump(info, open('login.json', 'w'))

def login():
	try:
		g_.cln=rpc.RpcClient(g_.server_address,9090)
		b_ok,res=g_.cln.login(g_.user_name,g_.cry_password)
		if not b_ok:
			msgbox(res)
			return 0
		g_.token,g_.user_name,g_.department=res
		return 1
	except:
		msgbox('无法连接到服务器')
		return 0
###########################################################


def fill_table_combo():
	ids=g_.cln.get_tables_id_name(g_.token)
	for n,(_id,name) in enumerate(ids):
		__main__.exe_fun__['insert_combo_data'](n,name,_id)
	return ids

def switch_table():
	g_.tid=__main__.stack__[0]
	th=g_.cln.get_table_head(g_.tid)
	__main__.exe_fun__['delete_all_columns']()
	insert_column=__main__.exe_fun__['insert_column']
	insert_column(0,'流水号',0)
	insert_column(1,'数据来源',100)
	insert_column(2,'数据状态',100)
	for n,x in enumerate(th,3):
		insert_column(n,x,len(x)*20)


def get_table_head():
	heads=g_.cln.get_table_head(g_.tid)
	L=len(heads)
	__main__.stack__[:L]=heads
	return L

def add_new_piece(data):
	id=str(time.time())
	time.sleep(0.001)#make sure no id is equal.
	piece=[id,g_.user_name,[g_.user_name],g_.tid,data]
	try:
		g_.cln.upload_piece(piece)
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
	g_.cln.submit_piece(g_.user_name,_id)

def dismiss_piece(_id):
	g_.cln.dismiss_piece(g_.user_name,_id)

def delete_piece(_id):
	g_.cln.delete_piece(g_.user_name,_id)

def refresh():
	pcs=g_.cln.refresh(g_.user_name,g_.tid)
	__main__.exe_fun__['delete_all_items']()
	for pc in pcs:
		grid_append_piece(pc)
	return pcs

def load_excel():
	fn = __main__.stack__[0]
	import xlrd
	book = xlrd.open_workbook(fn)
	st = book.sheet_by_index(0)
	for r in range(10,st.nrows):
		data=[int(x) if type(x) is float and int(x)==x else x for x in st.row_values(r)]
		data=[str(x) for x in data]
		add_new_piece(data)

def new_piece_from_stack():
	pos_end=__main__.stack__.index(0)
	add_new_piece(__main__.stack__[:pos_end])

def get_title():
	return '部门：%s  姓名：%s'%(g_.department,g_.user_name)

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
	li=g_.cln.get_export_data(g_.token,b_history,t1,t2)
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
	newdat=g_.cln.download_export_template(crc)
	if newdat:
		open(fn,'wb').write(newdat.data)
		return 1
	return 0

def get_import_template():
	dat=g_.cln.download_import_template(g_.tid)
	if dat:
		fn=win32tools.select_file(1,'Excel 03\0*.xls\0')
		if not fn:
			return
		if '.xls' not in fn:
			fn+='.xls'
		open(fn,'wb').write(dat.data)
	msgbox('当前表格的模板已下载，请编辑并导入。')

def GridRange(st,r0,c0,r1,c1):
	s=chr(c0+64)+str(r0)+':'+chr(c1+64)+str(r1)
	rgn=st.raw.Range(s)
	for x in [7,8,9,10]:
		bd=rgn.Borders(x)
		bd.LineStyle=1
		bd.Weight=4
	for x in [11,12]:
		bd=rgn.Borders(x)
		bd.LineStyle=1
		bd.Weight=2