// Clock Server is a concurrent TCP server that periodically writes the time.
package main

import (
	"io"
	"log"
	"net"
	"time"
	"os"
	"fmt"
)

func handleConn(c net.Conn) {
	defer c.Close()
	for {
		_, err := io.WriteString(c, time.Now().Format("15:04:05\n"))
		if err != nil {
			return // e.g., client disconnected
		}
		time.Sleep(1 * time.Second)
	}
}

func main() {
	argv := os.Args
	argc := len(argv)
	if argc < 3{
		fmt.Printf("Missing arguments.\nUse it as go run serverClock.go -port \"port number\"\n")
		return
	}
	if argv[1] != "-port" {
		fmt.Printf("Missing -port flag\n")
		return
	}
	host := "localhost:"+argv[2]
	listener, err := net.Listen("tcp", host)
	if err != nil {
		log.Fatal(err)
	}
	for {
		conn, err := listener.Accept()
		if err != nil {
			log.Print(err) // e.g., connection aborted
			continue
		}
		go handleConn(conn) // handle connections concurrently
	}
}