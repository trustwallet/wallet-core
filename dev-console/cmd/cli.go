package main

import "dev-console/native"

func main() {
	s := native.NewTWString("Hey")
	defer s.Delete()
	println(s.String())
}
