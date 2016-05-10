import redis

def script_load(script):
	sha=[None]
	def call(conn, keys=[], args=[], force_eval=False):
		if not force_eval:
			if not sha[0]:
				sha[0]=conn.execute_command("SCRIPT","LOAD",script,parse="LOAD")
		try:
			return conn.execute_command("EVALSHA", sha[0], len(keys), *(keys+args))
		except redis.exceptions.ResponseError as msg:
			if not msg.args[0].startswith("NOSCRIPT"):
				raise
		return conn.execute_command("EVAL",script,len(keys),*(keys+args))
	return call

def redis_conn():
	addr = {'host':'127.0.0.1', 'port':6379}
	return redis.Redis(**addr)

def check_hkey(conn, key):
	ret = conn.execute_command("HGETALL", key)
	print ret

def main():
	f = open('./hash_json_set.lua','rb')
	try:
		script = f.read()
		hash_json_set = script_load(script)
		
		conn = redis_conn()
		
		keys = ['info']
		args = ['1','{"a":1}']
		ret = hash_json_set(conn, keys, args)
		# print ret
		
		args = ['1','{"b":2}']
		ret = hash_json_set(conn, keys, args)
		# print ret

		args = ['2','{"a":11}']
		ret = hash_json_set(conn, keys, args)
		# print ret

		check_hkey(conn, 'info')

		args = ['1','{"a":3}']
		ret = hash_json_set(conn, keys, args)
		# print ret

		check_hkey(conn, 'info')
	finally:
		f.close()

if __name__ == '__main__':
	main()
