// Copyright © 2016 Alan A. A. Donovan & Brian W. Kernighan.
// License: https://creativecommons.org/licenses/by-nc-sa/4.0/

// See page 156.

// Package geometry defines simple types for plane geometry.
//!+point
package main

import (
	"fmt"
	"math"
	"math/rand"
	"os"
	"strconv"
)

type Point struct{ x, y float64 }

// traditional function

func (p Point) X() float64 {
	return p.x
}

func (p Point) Y() float64 {
	return p.y
}

func Distance(p, q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

// same thing, but as a method of the Point type
func (p Point) Distance(q Point) float64 {
	return math.Hypot(q.x-p.x, q.y-p.y)
}

//!-point

//!+path

// A Path is a journey connecting the points with straight lines.
type Path []Point

// Distance returns the distance traveled along the path.
func (path Path) Distance() float64 {

	sum := 0.0
	for i := range path {
		if i > 0 {
			r := path[i-1].Distance(path[i])
			sum += r
			fmt.Printf("%.2f + ", math.Floor(r*100)/100)
		}
	}
	return sum
}

// Given three colinear points p, q, r, the function checks if
// point q lies on line segment 'pr'
func OnSegment(p, q, r Point) bool {
	if p.x <= math.Max(p.x, r.x) && q.x >= math.Min(p.x, r.x) && q.y <= math.Max(p.y, r.y) && q.y >= math.Min(p.y, r.y) {
		return true
	}
	return false
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
func Orientation(p, q, r Point) int {
	val := (q.y-p.y)*(r.x-q.x) - (q.x-p.x)*(r.y-q.y)
	if val == 0 {
		return 0
	} else if val > 0 {
		return 1
	} else {
		return 2
	}
}

// Function that returns true if line segment 'p1q1'
// and 'p2q2' intersect.
func DoIntersect(p1, q1, p2, q2 Point) bool {
	o1 := Orientation(p1, q1, p2)
	o2 := Orientation(p1, q1, q2)
	o3 := Orientation(p2, q2, p1)
	o4 := Orientation(p2, q2, q1)
	if o1 != o2 && o3 != o4 {
		return true
	}
	if o1 == 0 && OnSegment(p1, p2, q1) {
		return true
	}
	if o2 == 0 && OnSegment(p1, q2, q1) {
		return true
	}
	if o3 == 0 && OnSegment(p2, p1, q2) {
		return true
	}
	if o4 == 0 && OnSegment(p2, q1, q2) {
		return true
	}
	return false
}

func main() {
	argc := len(os.Args)
	if argc < 2 {
		fmt.Println("Error. Wronh number of arguments.")
		fmt.Println("go run geometry.go <number_of_sides>")
	} else {
		n, err := strconv.Atoi(os.Args[1])
		var min, max float64
		min = -100
		max = 100
		var path Path
		if err == nil {
			path = make([]Point, n)
			path[0] = Point{min + rand.Float64()*(max-min), min + rand.Float64()*(max-min)}
			path[1] = Point{min + rand.Float64()*(max-min), min + rand.Float64()*(max-min)}
			for i := 2; i < n; i++ {
				path[i] = Point{min + rand.Float64()*(max-min), min + rand.Float64()*(max-min)}
				if i == 2 {
					for OnSegment(path[0], path[2], path[1]) {
						path[i] = Point{min + rand.Float64()*(max-min), min + rand.Float64()*(max-min)}
					}
				} else {
					intersect := false
					j := 0
					for j < i-2 {
						inter := DoIntersect(path[i], path[i-1], path[j], path[j+1])
						if inter {
							intersect = inter
							j = i
						} else {
							j++
						}
					}
					for intersect {
						path[i] = Point{min + rand.Float64()*(max-min), min + rand.Float64()*(max-min)}
						intersect = false
						j = 0
						for j < i-2 {
							inter := DoIntersect(path[i], path[i-1], path[j], path[j+1])
							if inter {
								intersect = inter
								j = i
							} else {
								j++
							}
						}
					}
				}
			}
			//Make the print
			fmt.Printf(" - Generating a [%d] sides figure\n", n)
			fmt.Println(" - Figure's vertices")
			for p := range path {
				fmt.Printf("   - (%3.1f,%3.1f)\n", path[p].x, path[p].y)
			}
			fmt.Println(" - Figure's Perimeter")
			fmt.Print("   - ")
			last := path[len(path)-1].Distance(path[0])
			res := path.Distance() + last
			fmt.Printf("%.2f = ", math.Floor(last*100)/100)
			fmt.Printf("%.2f\n", math.Floor(res*100)/100)
		} else {
			fmt.Println("Error argument must be a number")
		}
	}
	
}

//!-path
