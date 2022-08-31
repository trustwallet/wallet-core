package main

import "dev-console/native"

func main() {
	s := native.TWStringCreateWithGoString("Hey")
	defer native.TWStringDelete(s)
	println(native.TWStringGoString(s))
}
