package main

import (
	"fmt"
	"os"
	"strconv"
)

// adds/subtracts/multiplies all values that are in the *values array.
// nValues is the number of values you're reading from the array
// operator will indicate if it's an addition (1), subtraction (2) or
// multiplication (3)
func calc(operator int, values []int) int {
	sum := values[0]
	if operator == 1 {
		for i := 1; i < len(values); i++ {
			sum += values[i]
		}
	} else if operator == 2 {
		for i := 1; i < len(values); i++ {
			sum -= values[i]
		}
	} else if operator == 3 {
		for i := 1; i < len(values); i++ {
			sum *= values[i]
		}
	} else {
		return -1
	}
	return sum
}

func main() {
	argv := os.Args
	argc := len(argv)
	if argc < 3 {
		fmt.Printf("you need to pass at least 3 arguments:\n")
		fmt.Print("./calculator <operator> <numbers separated by space>\n")
	} else {
		values := make([]int, argc-2)
		for i := 0; i < argc-2; i++ {
			n, err := strconv.Atoi(argv[i+2])
			if err != nil {
				fmt.Printf("Conversion error.\n")
				fmt.Printf("pass only positive integers\n")
				return
			} else {
				values[i] = n
			}
		}
		var result int
		if argv[1] == "add" {
			result = calc(1, values)
		} else if argv[1] == "sub" {
			result = calc(2, values)
		} else if argv[1] == "mult" {
			result = calc(3, values)
		} else {
			fmt.Printf("wrong operator. Must be add, sub or mult\n")
			return
		}
		fmt.Printf("result: %d\n", result)
	}

}
