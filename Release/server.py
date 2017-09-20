import rpc
import json
import time
####### common userd function ###############################################
DB_FILE='pieces.acc_db'

class writer():
	def __init__(self):
		pass
	def write(self,s):
		pass
import sys
sys.stderr=writer()
#################################################
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

def save_pieces():
	global g_pieces
	json.dump(g_pieces,open('pieces.dat','w'))

def save_one_piece(pc):
	with open(DB_FILE,'a') as f:
		f.write(json.dumps(pc)+'\n')

def load_pieces():
	global g_pieces
	cnt=0
	print('正在加载数据库...')
	try:
		with open(DB_FILE,'r') as f:
			for ln in f:
				cnt+=1
				obj=json.loads(ln)
				if type(obj) is str:
					if obj in g_pieces:
						g_pieces.pop(obj)
				else:
					g_pieces[obj[0]]=obj
	except:
		load_pieces_old_db()
	print('操作记录%d条，实有数据%d条。'%(cnt,len(g_pieces)))
	if cnt>len(g_pieces)*2 or cnt==0:#need compact or load old db.
		compact_db()

def compact_db():
	print('正在压缩数据库...')
	open(DB_FILE,'w')
	with open(DB_FILE,'a') as f:
		for pc in g_pieces.values():
			f.write(json.dumps(pc)+'\n')
	print('数据库压缩完成。')

def load_pieces_old_db():
	global g_pieces
	try:
		g_pieces=json.load(open('pieces.dat','r'))
		print('导入旧版数据库记录%d条。'%(len(g_pieces)))
	except:
		pass
##############################################

g_users={'gb1':['gb2',encrypt('123'),'耕保科'],
'gb2':['gb3',encrypt('123'),'耕保科'],
'gb3':['cw',encrypt('123'),'耕保科'],
'ly1':['ly2',encrypt('123'),'利用科'],
'ly2':['ly3',encrypt('123'),'利用科'],
'ly3':['cw',encrypt('123'),'利用科'],
'dj1':['dj2',encrypt('123'),'地籍科'],
'dj2':['dj3',encrypt('123'),'地籍科'],
'dj3':['cw',encrypt('123'),'地籍科'],
'bdc1':['bdc2',encrypt('123'),'不动产'],
'bdc2':['bdc3',encrypt('123'),'不动产'],
'bdc3':['cw',encrypt('123'),'不动产'],
'cw':['',encrypt('123'),'财务科'],
}

g_pieces=dict()
table_heads=dict()
table_heads['耕保科']=['批次号','省批复文号','座落位置','报批亩数','报批公顷数','农用地小计','耕地','建设用地','未利用地','社保资金','土地补偿费','地上附着物','青苗费','占补平衡指标','被征地村居','批复日期','图件信息','影像资料','备注']
table_heads['利用科']=['受让方*','受让方法定代表人','土地位置*','土地面积（单位：平方米）','出让时间','出让总金额（单位：万元）','用途','年限','备注']
table_heads['地籍科']=['出（转）让方*','出（转）让方法定代表人','受让方*','受让方法定代表人','土地位置*','土地面积（单位：平方米）','出（转）让时间','出（转）让总金额（单位：万元）','备注']
table_heads['不动产']=['序号','座落','土地权利人','不动产证书号','使用权类型','用途','面积（㎡）','宗地编码','变更日期','备注']

svr=rpc.RpcSvr('0.0.0.0',9090)
#%%
def get_table_head(token):
	name=decrypt(token)
	if name not in g_users:
		return []
	department=g_users[name][2]
	return table_heads.get(department,[])
svr.reg_fun(get_table_head)
#%%
def login(un,pw):
	if un not in g_users:
		return 0,'用户不存在'
	if g_users[un][1]!=pw:
		return 0,'密码不正确'
	return 1,(encrypt(un),un,g_users[un][2])#encrypt un as token
svr.reg_fun(login)
#%%
def upload_piece(piece):
	g_pieces[piece[0]]=piece
	save_one_piece(piece)
	return 1
svr.reg_fun(upload_piece)
#%%
def submit_piece(name,_id):#do not change id
	if _id not in g_pieces or name not in g_users:
		return 0
	piece=g_pieces[_id]
	ldr=g_users[name][0]
	if not ldr:#财务无上级，不能提交。
		return 0
	piece[1]=name#from
	piece[2].append(ldr)#path
	g_pieces[_id]=piece
	save_one_piece(piece)
	return 1
svr.reg_fun(submit_piece)
#%%
def dismiss_piece(name,_id):#do not change id
	if _id not in g_pieces:
		return 0
	pc=g_pieces[_id]
	pth=pc[2]
	if len(pth)<2 or pth[-1]!=name:
		return 0
	pc[1]=name
	pc[2].pop(-1)
	g_pieces[_id]=pc
	save_one_piece(pc)
	return 1
svr.reg_fun(dismiss_piece)
#%%
def delete_piece(name,_id):#do not change id
	try:
		pc=g_pieces[_id]
		pth=pc[2]
		if pth[0]==name:#common delete
			g_pieces.pop(_id)
			save_one_piece(_id)
			return 1
		if g_users[name][2]=='财务科':
			#cw delete and save history.
			with open('history.acc_db','a') as f:
				f.write(json.dumps(pc)+'\n')
			g_pieces.pop(_id)
			save_one_piece(_id)
			return 1
		return 0
	except:
		return 0
svr.reg_fun(delete_piece)
#%%
def refresh(name):#do not change id
	return [x for x in g_pieces.values() if x[2][-1]==name]
svr.reg_fun(refresh)
#%%

def get_export_data(token,b_history,t1,t2):
	name=decrypt(token)
	if not b_history:
		pcs=[x for x in g_pieces.values() if x[2][-1]==name]
		ret=dict()
		for pc in pcs:
			dep=g_users[pc[1]][-1]
			ret.setdefault(dep,[]).append(pc)
		return ret
	else:
		time1=time.strptime(t1,'%Y年%m月%d日')
		time1=time.mktime(time1)
		time2=time.strptime(t2,'%Y年%m月%d日')
		time2=time.mktime(time2)
		if time1>time2:
			time1,time2=time2,time1
		ret=dict()
		with open('history.acc_db','r') as f:
			for ln in f:
				pc=json.loads(ln)
				if time1<float(pc[0])<time2+3600*24:
					dep=g_users[pc[1]][-1]
					ret.setdefault(dep,[]).append(pc)
		return ret
svr.reg_fun(get_export_data)

load_pieces()
print('数据服务V1.0.0.2正在运行中...')
svr.run(1)
