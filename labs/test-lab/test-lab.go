package main

import (
	"fmt"
	"os"
)

func main() {
	args := len(os.Args)
	if args < 2 {
		fmt.Println("No name provided, pls try again")
	} else {
		name := os.Args[1]
		for i := 2; i < args; i++ {
			name += " "+os.Args[i]
		}
		fmt.Printf("Hello %v Welcome to the GO \n", name)
	}

}
