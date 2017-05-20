package main

import (
  "strconv"
  "crypto/md5"
  "io"
  "time"
  "fmt"
  "html/template"
  //"text/template"
  "net/http"
  "strings"
  "log"
)

func sayHi(w http.ResponseWriter, r *http.Request) {
  r.ParseForm()
  fmt.Println(r.Form)
  fmt.Println("path",r.URL.Path)
  fmt.Println("scheme",r.URL.Scheme)
  fmt.Println(r.Form["url_long"])
  for k,v := range r.Form {
    fmt.Println("key:",k)
    fmt.Println("val:",strings.Join(v,""))
  }
  fmt.Fprintf(w,"hi man")
}

func login(w http.ResponseWriter, r *http.Request) {
  fmt.Println("method:",r.Method)
  if r.Method=="GET" {
    crutime:=time.Now().Unix()
    h:=md5.New()
    io.WriteString(h,strconv.FormatInt(crutime,10))
    token:=fmt.Sprintf("%x",h.Sum(nil))
    fmt.Println(token)

    t,_:=template.ParseFiles("login.gtpl")
    //t.Execute(w,token)
    t.Execute(w,nil)
  } else {
    r.ParseForm()
    token:=r.Form.Get("token")
    if token != "" {
      fmt.Println("token:",token)
    } else {
      fmt.Println("token not found error")
    }
    fmt.Println("username:",r.Form["username"])
    fmt.Println("password:",r.Form["password"])
    //fmt.Fprintf(w,r.Form["username"][0])
    template.HTMLEscape(w,[]byte(r.Form["username"][0]))
  }
}

func xss(w http.ResponseWriter, r *http.Request) {
  t,_:=template.New("foo").Parse(`{{define "T"}}hello,{{.}}!{{end}}`)
  //t.ExecuteTemplate(w,"T",template.HTML("<script>alert('you have been pwned')</script>")) 
  t.ExecuteTemplate(w,"T","<script>alert('you have been pwned')</script>") 
}

func main() {
  http.HandleFunc("/",sayHi)
  http.HandleFunc("/login",login)
  http.HandleFunc("/xss",xss)
  err := http.ListenAndServe(":9090",nil)
  if err != nil {
    log.Fatal("ListerAndServe: ",err)
  }
}

