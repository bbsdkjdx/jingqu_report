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

g_users={'keyuan1':['kezhang1',encrypt('123'),'耕保科'],
'kezhang1':['lingdao1',encrypt('123'),'耕保科'],
'lingdao1':['caiwu',encrypt('123'),'耕保科'],
'caiwu':['',encrypt('123'),'耕保科'],
}

g_pieces=dict()

table_heads={'耕保科':['批次号','省批复文号','地块','报批亩数','报批公顷数','农用地小计','耕地','建设用地','未利用地','社保资金','土地补偿费','地上附着物','青苗费','占补平衡指标','被征地村居','批复日期']}

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
	return 1,encrypt(un)#as token

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
	

svr=rpc.RpcSvr('0.0.0.0',9090)
svr.reg_fun(login)
svr.reg_fun(upload_piece)
svr.reg_fun(get_table_head)
svr.reg_fun(submit_piece)
svr.reg_fun(refresh)
svr.reg_fun(dismiss_piece)
svr.reg_fun(delete_piece)
svr.run(0)