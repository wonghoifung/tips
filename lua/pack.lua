module(..., package.seeall)

ver = "0.1 alpha"

function sayhello(  )
	print('hello')
end

_G.sayhelloG = sayhello