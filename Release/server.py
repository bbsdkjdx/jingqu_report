import rpc

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

def get_table_head(token):
	name=decrypt(token)
	if name not in g_users:
		return []
	department=g_users[name][2]
	return table_heads.get(department,[])

def login(un,pw):
	if un not in g_users:
		return 0,'用户不存在'
	if g_users[un][1]!=pw:
		return 0,'密码不正确'
	return 1,(encrypt(un),un,g_users[un][2])#encrypt un as token

def upload_piece(piece):
	g_pieces[piece[0]]=piece
	return 1

def submit_piece(name,_id):
	if _id not in g_pieces or name not in g_users:
		return 0
	piece=g_pieces[_id]
	ldr=g_users[name][0]
	piece[1]=name#from
	piece[2].append(ldr)#path
	g_pieces[_id]=piece
	return 1

def dismiss_piece(name,_id):
	if _id not in g_pieces:
		return 0
	pc=g_pieces[_id]
	pth=pc[2]
	if len(pth)<2 or pth[-1]!=name:
		return 0
	pc[1]=name
	pc[2].pop(-1)
	g_pieces[_id]=pc
	return 1

def delete_piece(name,_id):
	try:
		pc=g_pieces[_id]
		pth=pc[2]
		if pth[0]==name:
			g_pieces.pop(_id)
			return 1
		return 0
	except:
		return 0

def refresh(name):
	return [x for x in g_pieces.values() if x[2][-1]==name]

def get_r0_base0(name):
	try:
		dep=g_users[name][2]
		if dep=='耕保科':
			return 4
		return 1
	except:
		return -1


svr=rpc.RpcSvr('0.0.0.0',9090)
svr.reg_fun(login)
svr.reg_fun(get_r0_base0)
svr.reg_fun(upload_piece)
svr.reg_fun(get_table_head)
svr.reg_fun(submit_piece)
svr.reg_fun(refresh)
svr.reg_fun(dismiss_piece)
svr.reg_fun(delete_piece)
svr.run(0)
