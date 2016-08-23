package main

//#include "p.h"
import "C" 
import "fmt"
import "unsafe"

//export Gp
func Gp(a int) {
	fmt.Printf("go: %d\n", a)
}

func main() {
    str := "hello world"
    
    cstr := C.CString(str)
    C.p(cstr)
    C.free(unsafe.Pointer(cstr))
}

