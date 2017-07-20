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

users={'keyuan1':['kezhang1',encrypt('123'),'耕保科'],
'kezhang1':['lingdao1',encrypt('123'),'耕保科'],
'lingdao1':['caiwu',encrypt('123'),'耕保科'],
'caiwu':['',encrypt('123'),'耕保科'],
}

pieces=dict()

table_heads={'耕保科':['批次号','省批复文号','地块','报批亩数','报批公顷数','农用地小计','耕地','建设用地','未利用地','社保资金','土地补偿费','地上附着物','青苗费','占补平衡指标','被征地村居','批复日期']}

def get_table_head(token):
	name=decrypt(token)
	if name not in users:
		return []
	department=users[name][2]
	return table_heads.get(department,[])

def login(un,pw):
	if un not in users:
		return 0,'用户不存在'
	if users[un][1]!=pw:
		return 0,'密码不正确'
	return 1,encrypt(un)#as token

def submit_piece(piece):
	pass

svr=rpc.RpcSvr('0.0.0.0',9090)
svr.reg_fun(login)
svr.reg_fun(get_table_head)
svr.reg_fun(submit_piece)
svr.run()