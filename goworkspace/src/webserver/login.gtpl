<html>
<head>
<title></title>
</head>
<body>
  <form action="http://127.0.0.1:9090/login?username=wong" method="post">
    username: <input type="text" name="username">
    password: <input type="password" name="password">
    <input type="hidden" name="token" value="{{.}}">
    <input type="submit" value="login">
  </form>
</body>
</html>
