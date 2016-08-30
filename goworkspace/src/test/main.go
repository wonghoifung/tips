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

    //////////////

    done := make(chan struct{})

    data := []chan int{
    	make(chan int, 3),
    }

    go func() {
    	defer close(done)
    	for i:=0; i<10; i++ {
    		select {
    		case data[len(data)-1] <- i:
    		default:
    			data = append(data, make(chan int, 3))
    		}
    	}
    }()

    <- done

    fmt.Printf("len(data): %d\n", len(data)) // 4
    for i:=0; i<len(data); i++ {
    	c := data[i]
    	close(c)
    	fmt.Printf("---> i:%d\n", i)
    	for x := range c {
    		fmt.Println(x)
    	}
    }
}

