package main

import (
	"dev-console/native"
	"fmt"
)

func main() {
	s := native.NewTWString("Hey")
	defer s.Delete()
	println(s.String())
	fmt.Printf("%d\n", s.Size())
}
