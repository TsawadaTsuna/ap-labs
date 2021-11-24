package main

import (
	//"io"
	"log"
	"net"
	"os"
	"fmt"
	"strings"
	"bufio"
)


func main() {
	fmt.Println("")
	argv := os.Args
	argc := len(argv)
	if argc < 2 {
		fmt.Printf("No time zones passed\n")
		return
	}
	done := make(chan int)
	for i := 1; i < argc; i++ {

		conn, err := net.Dial("tcp", strings.Split(argv[i],"=")[1])
		defer conn.Close()
		if err != nil {
			log.Fatal(err)
		}
		
		go func(city string, conn net.Conn) {
			for {
				status, erro := bufio.NewReader(conn).ReadString('\n')
				if erro != nil{
					log.Fatal(erro)
				}
				fmt.Printf("\r%s : %s",city,status)
			}
			//io.Copy(os.Stdout, conn)
			//fmt.Println("")
		}(strings.Split(argv[i],"=")[0],conn)
		
	}
	
	x := 1
	x = <-done // wait for background goroutine to finish
	log.Println("Channel Closed with value: ", x)
	//close(done)
	
}