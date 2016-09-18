package main

import (
    "log"
    "sync"
    "time"
    "math/rand"
    "runtime"
)

func consumer(id int) {
    defer consumer_wg.Done()

    for item := range resultingChannel {
        log.Printf("consumer %d get %d", id, item)
    }
}

func producer(id int) {
    defer producer_wg.Done()

    success := rand.Float32() > 0.1
    if success {
        var num = rand.Int() % 10
        log.Printf("producer %d push %d\n", id, num)
        resultingChannel <- num
    }
}

var resultingChannel = make(chan int)
var producer_wg sync.WaitGroup
var consumer_wg sync.WaitGroup

func main() {
    rand.Seed(time.Now().Unix())

    for c := 0; c < runtime.NumCPU(); c++ {
        producer_wg.Add(1)  
        go producer(c)
    }

    for c := 0; c < runtime.NumCPU(); c++ {
        consumer_wg.Add(1)
        go consumer(c)
    }

    producer_wg.Wait()

    close(resultingChannel)

    consumer_wg.Wait()
}

